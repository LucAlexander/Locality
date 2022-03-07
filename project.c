#include <stdio.h>
#include "Entity-Component-System/ecs.h"
#include "SDL2-Utility/inpututils.h"
#include "SDL2-Utility/mathutils.h"

#include "Locality.h"
#include "LocGUI.h"
#include "LocBaseSystems.h"

void createCustomEntity(uint32_t n){
	uint32_t i;
	for (i = 0;i<n;++i){
		uint32_t entity = summon();
		v2 pos;
		pos.x = (rand() % (WINDOW_WIDTH / 8) ) * 8;
		pos.y = (rand() % (WINDOW_HEIGHT / 8) ) * 8;
		v2 forces;
		forces.x = (rand() % 20)-10.0;
		forces.y = (rand() % 20)-10.0;
		forces.x /= 100.0;
		forces.y /= 100.0;
		if (forces.x == 0){
			forces.x += 0.1;
		}
		if (forces.y == 0){
			forces.y -= 0.1;
		}
		Blitable sprite;
		BlitableInitF(&sprite, IMAGE_FILE "box.png", 32, 32);
		addComponent(entity, POSITION_C, &pos);
		addComponent(entity, BLITABLE_C, &sprite);
		addComponent(entity, FORCES_C, &forces);
		addEntityFlag(entity, RENDER_RELATIVE);
	}
}

void pressActionCustom(void* params){
	createCustomEntity(6);
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
