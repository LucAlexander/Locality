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
		Blitable* sprite = getComponent(entity, BLITABLE_C);
		BlitableFree(sprite);
		markForPurge(entity);
	}
}

void createCustomEntity(uint32_t n){
	uint32_t i;
	for (i = 0;i<n;++i){
		uint32_t entity = summon();
		v2 pos = {
			(rand() % (getWindowW() / 8) ) * 8,
			(rand() % (getWindowH() / 8) ) * 8
		};

		v2 forces = {0, 0};
		forces_applyForce(&forces, (rand()%2)+1, (rand()%90)*4);

		Blitable sprite;
		BlitableInitF(&sprite, IMAGE_FILE "box.png", 32, 32);

		behavior_l update;
		behavior_init(&update, beh);

		addComponent(entity, POSITION_C, &pos);
		addComponent(entity, BLITABLE_C, &sprite);
		addComponent(entity, FORCES_C, &forces);
		addComponent(entity, BEHAVIOR_C, &update);
		addEntityFlag(entity, RENDER_RELATIVE);
	}
}

void pressActionCustom(void* params){
	createCustomEntity(100);
}

void project(){
	renderSetSpriteScale(2, 2);
	uint32_t entity = summon();
	v2 pos = {45, 45};
	pressable_l pressComp;
	pressable_l_init(&pressComp, pressActionCustom,
		IMAGE_FILE "button.png",
		IMAGE_FILE "button_hover.png",
		IMAGE_FILE "button_press.png",
	64, 32);
	uint32_t arg = 16;
	text_l textNode;
	text_l_init(&textNode, "Press", 255, 255, 255, 255);
	addComponent(entity, POSITION_C, &pos);
	addComponent(entity, PRESSABLE_C, &pressComp);
	addComponent(entity, PRESSABLE_ARG_C, &arg);
	addComponent(entity, TEXT_C, &textNode);
	addEntityFlag(entity, RENDER_ABSOLUTE);
}
