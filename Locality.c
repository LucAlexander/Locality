#include <stdio.h>
#include "Entity-Component-System/ecs.h"
#include "SDL2-Utility/inpututils.h"
#include "SDL2-Utility/mathutils.h"

#include "Locality.h"
#include "LocGUI.h"

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 360

VECTOR_SOURCE(vSystem, System)

static Project_state state;

void Project_stateInit(){
	state.run = 1;
	state.updateList_pre = vSystemInit();
	state.updateList = vSystemInit();
	state.updateList_post = vSystemInit();
	state.renderList = vSystemInit();
	state.programState = LOCALITY_STATE_INIT;
}

void Project_registerSystem(System* sys, PROGRAM_STATE mode){
	switch(mode){
		case LOCALITY_STATE_UPDATE_PRE:
			vSystemPushBack(&(state.updateList_pre), *sys);
		return;
		default:
		case LOCALITY_STATE_UPDATE:
			vSystemPushBack(&(state.updateList), *sys);
		return;
		case LOCALITY_STATE_UPDATE_POST:
			vSystemPushBack(&(state.updateList_post), *sys);
		return;
		case LOCALITY_STATE_RENDER:
			vSystemPushBack(&(state.renderList), *sys);
		return;
	}
}

void vSystemActivate(vSystem* vec){
	uint32_t i;
	for (i = 0;i<vec->size;++i){
		SystemActivate(vSystemRef(vec, i));
	}
}

void Project_freeSystems(vSystem* systemList){
	uint32_t i;
	for (i = 0;i < systemList->size;++i){
		SystemFree(vSystemRef(systemList, i));
	}
	vSystemFree(systemList);
}

void Project_stateExit(){
	vSystemFree(&(state.updateList_pre));
	vSystemFree(&(state.updateList));
	vSystemFree(&(state.updateList_post));
	vSystemFree(&(state.renderList));
}

void Solution_setup(){
	ecsInit(3, sizeof(v2), sizeof(Blitable), sizeof(pressable_l)); 
	graphicsInit(WINDOW_WIDTH, WINDOW_HEIGHT, "Locality Project");
	inputInit();
	view v = {
		0, 0,
		0, 0,
		WINDOW_WIDTH, WINDOW_HEIGHT
	};
	renderSetView(v);
}

void Solution_inputEvents(){
	switch(state.event.type){
		case SDL_QUIT:
			state.run = 0;
		return;
		case SDL_KEYDOWN:
			if (state.event.key.repeat == 0){
				keyDownEvent(state.event);
			}
		return;
		case SDL_KEYUP:
			keyUpEvent(state.event);
		return;
		case SDL_MOUSEBUTTONDOWN:
			if (!mouseHeld(state.event.button.button)){
				mouseDownEvent(state.event);
			}
		return;
		case SDL_MOUSEBUTTONUP:
			mouseUpEvent(state.event);
		return;
		case SDL_MOUSEWHEEL:
			mouseScrollEvent(state.event.wheel.y);
		return;
		case SDL_MOUSEMOTION:
			mouseMoveEvent(state.event.motion.x, state.event.motion.y);
		return;
	}
}

void Solution_exit(){
	graphicsClose();
	freeEcs();
}

int main(int argc, char** argv){
	Project_stateInit();
	Solution_setup();
	SolutionLogic_init();
	while(state.run){
		while (SDL_PollEvent(&(state.event))){
			Solution_inputEvents();
		}
		SolutionLogic_update_pre();
		SolutionLogic_update();
		SolutionLogic_update_post();
		newInputFrame();
		renderClear();
		renderSetColor(0, 0, 0, 255);
		SolutionLogic_render();
		renderFlip();
	}
	SolutionLogic_deinit();
	Solution_exit();
	Project_stateExit();
	return 0;
}

//user space code

void pressActionCustom(void* params){
	v2 mp = mousePos();
	printf("\t%.0f %.0f\n", mp.x, mp.y);
}

void SolutionLogic_init(){
	state.programState = LOCALITY_STATE_INIT;
	// SOFTWARE_STATE_INITIALIZER
	
	// user code insert start
	
	uint32_t entity = summon();
	v2 pos = {45, 45};
	Blitable texNormal;
	BlitableInitF(&texNormal, IMAGE_FILE "button.png", 64, 32);
	texNormal.center.x = 0;
	texNormal.center.y = 0;
	Blitable texHover;
	BlitableInitF(&texHover, IMAGE_FILE "button_hover.png", 64, 32);
	texHover.center.x = 0;
	texHover.center.y = 0;
	Blitable texPress;
	BlitableInitF(&texPress, IMAGE_FILE "button_press.png", 64, 32);
	texPress.center.x = 0;
	texPress.center.y = 0;
	pressable_l pressComp;
	pressable_l_init(&pressComp, pressActionCustom, &texNormal, &texHover, &texPress, 64, 32);
	addComponent(entity, POSITION_C, &pos);
	addComponent(entity, BLITABLE_C, &texNormal);
	addComponent(entity, PRESSABLE_C, &pressComp);

	// user code insert end

	System pressUpdate = SystemInit(pressable_su, 2,
		POSITION_C,
		PRESSABLE_C
	);
	System pressRender = SystemInit(Blitable_sr, 2,
		POSITION_C,
		PRESSABLE_C
	); 
	System blitRender = SystemInit(Blitable_sr, 2, 
		POSITION_C,
		BLITABLE_C
	);
	Project_registerSystem(&pressUpdate, LOCALITY_STATE_UPDATE);
	Project_registerSystem(&blitRender, LOCALITY_STATE_RENDER);
	Project_registerSystem(&pressRender, LOCALITY_STATE_RENDER);
}

void SolutionLogic_deinit(){
	state.programState = LOCALITY_STATE_DEINIT;
	// SOFTWARE_STATE_DESTRUCTOR
}

void SolutionLogic_update_pre(){
	state.programState = LOCALITY_STATE_UPDATE_PRE;
	vSystemActivate(&(state.updateList_pre));
}

void SolutionLogic_update(){
	state.programState = LOCALITY_STATE_UPDATE;
	vSystemActivate(&(state.updateList));
}

void SolutionLogic_update_post(){
	state.programState = LOCALITY_STATE_UPDATE_POST;
	vSystemActivate(&(state.updateList_post));
}

void SolutionLogic_render(){
	state.programState = LOCALITY_STATE_RENDER;
	vSystemActivate(&(state.renderList));
}

