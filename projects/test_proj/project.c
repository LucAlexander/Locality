#include <stdio.h>
#include "Entity-Component-System/ecs.h"
#include "SDL2-Utility/inpututils.h"
#include "SDL2-Utility/mathutils.h"

#include "Locality.h"
#include "LocGUI.h"
#include "LocBaseSystems.h"

void beh(uint32_t entity){
	v2* pos = getComponent(entity, POSITION_C);
	uint32_t ww = getWindowW();
	uint32_t hh = getWindowH();
	if (pos->x > ww || pos->x < 0 || pos->y > hh || pos->y < 0){
		markForPurge(entity);
	}
}

void summonRat(void* args){
	uint32_t entity = summon();
	v2* spawnpos = args;

	v2 pos = {spawnpos->x, spawnpos->y};

	v2 forces = {0, 0};
	forces_applyForce(&forces, ((rand()%2)+1)-((float)(rand()%10)/10.0), (rand()%90)*4);

	Blitable sprite;
	BlitableInitF(&sprite, IMAGE_FILE "box.png", 4, 4);

	behavior_l update;
	behavior_l_init(&update, beh);

	addComponent(entity, POSITION_C, &pos);
	addComponent(entity, BLITABLE_C, &sprite);
	addComponent(entity, FORCES_C, &forces);
	addComponent(entity, BEHAVIOR_C, &update);
	addEntityFlag(entity, RENDER_RELATIVE);
}

void summonRatSpawner(){
	uint32_t entity = summon();
	v2 pos = {
		(rand()%(getWindowW()/8))*8,
		(rand()%(getWindowW()/8))*8
	};

	repeater_l spawner;
	repeater_l_init(&spawner, summonRat, 50, 32);
	repeater_setMaxTime(&spawner, 2000);

	addComponent(entity, REPEATER_C, &spawner);
	addComponent(entity, REPEATER_ARG_C, &pos);
}

void pressActionCustom(void* params){
	uint32_t* n = params;
	for (uint32_t i = 0;i<*n;++i){
		summonRatSpawner();
	}
}

void summonButton(){
	uint32_t entity = summon();
	v2 pos = {45, 45};
	pressable_l pressComp;
	pressable_l_init(&pressComp, pressActionCustom,
		IMAGE_FILE "button.png",
		IMAGE_FILE "button_hover.png",
		IMAGE_FILE "button_press.png",
	64, 32);
	uint32_t arg = 1;
	text_l textNode;
	text_l_init(&textNode, "Press", 255, 255, 255, 255);
	addComponent(entity, POSITION_C, &pos);
	addComponent(entity, PRESSABLE_C, &pressComp);
	addComponent(entity, PRESSABLE_ARG_C, &arg);
	addComponent(entity, TEXT_C, &textNode);
	addEntityFlag(entity, RENDER_ABSOLUTE);
}

void project_config(Project_config* config){
	config->window_w = 1366;
	config->window_h = 768;
	config->ticks_per_second = 60;
	strcpy(config->window_title, "Test Project");
}

void project_systems(){
	
}

void project(){
	renderSetSpriteScale(2, 2);
	summonButton();
}

