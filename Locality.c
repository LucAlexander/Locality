#include <stdio.h>

#include "Entity-Component-System/ecs.h"
#include "SDL2-Utility/inpututils.h"
#include "SDL2-Utility/mathutils.h"

#include "Locality.h"
#include "LocGUI.h"
#include "LocBaseSystems.h"

VECTOR_SOURCE(vSystem, System)

static Project_state state;

void Project_stateInit(){
	state.run = 1;
	state.updateList_pre = vSystemInit();
	state.updateList = vSystemInit();
	state.updateList_post = vSystemInit();
	state.renderList = vSystemInit();
	state.renderList_abs = vSystemInit();
	state.programState = LOCALITY_STATE_INIT;
	state.baseTime = 0;
	state.tick = 0;
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
		case LOCALITY_STATE_RENDER_ABSOLUTE:
			vSystemPushBack(&(state.renderList_abs), *sys);
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
	vSystemFree(&(state.renderList_abs));
}

void Solution_setup(){
	ecsInit(6, sizeof(v2), sizeof(Blitable), sizeof(pressable_l), sizeof(void*), sizeof(text_l), sizeof(v2));
	graphicsInit(WINDOW_WIDTH, WINDOW_HEIGHT, "Locality Project");
	inputInit();
	loadFont(FONT_FILE "arcade.TTF", "default");
	setFont("default");
	view v = {
		32, 32,
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

void Solution_tick(){
	projectTickUpdate();
	if (projectTick()){
		projectTickReset();
		SolutionLogic_update_pre();
		SolutionLogic_update();
		SolutionLogic_update_post();
		purgeDeactivatedData();
		newInputFrame();
		renderClear();
		renderSetColor(0, 0, 0, 255);
		SolutionLogic_render();
		SolutionLogic_render_abs();
		renderFlip();
	}
}

int main(int argc, char** argv){
	Project_stateInit();
	Solution_setup();
	SolutionLogic_init();
	projectTickReset();
	while(state.run){
		while (SDL_PollEvent(&(state.event))){
			Solution_inputEvents();
		}
		Solution_tick();
	}
	SolutionLogic_deinit();
	Solution_exit();
	Project_stateExit();
	return 0;
}

void SolutionLogic_init(){
	state.programState = LOCALITY_STATE_INIT;
	
	// Define and initialize systems
	System forcesUpdate = SystemInit(forces_su, 2, POSITION_C, FORCES_C);
	
	System pressUpdate = SystemInit(pressable_su, 2, POSITION_C, PRESSABLE_C);
	System pressRender = SystemInit(pressable_sr, 2, POSITION_C, PRESSABLE_C);
	SystemAddFilter(&pressRender, RENDER_RELATIVE);

	System blitRender = SystemInit(Blitable_sr, 2, POSITION_C, BLITABLE_C);
	SystemAddFilter(&blitRender, RENDER_ABSOLUTE);

	System blitRenderAbs = SystemInit(Blitable_sr, 2, POSITION_C, BLITABLE_C);
	SystemAddFilter(&blitRenderAbs, RENDER_RELATIVE);

	System textRender = SystemInit(text_sr, 2, POSITION_C, TEXT_C);
	SystemAddFilter(&textRender, RENDER_ABSOLUTE);

	System textRenderAbs = SystemInit(text_sr, 2, POSITION_C, TEXT_C);
	SystemAddFilter(&textRenderAbs, RENDER_RELATIVE);

	// Register systems to states
	Project_registerSystem(&forcesUpdate, LOCALITY_STATE_UPDATE);
	Project_registerSystem(&pressUpdate, LOCALITY_STATE_UPDATE);
	Project_registerSystem(&blitRender, LOCALITY_STATE_RENDER);
	Project_registerSystem(&textRender, LOCALITY_STATE_RENDER);
	Project_registerSystem(&pressRender, LOCALITY_STATE_RENDER_ABSOLUTE);
	Project_registerSystem(&blitRenderAbs, LOCALITY_STATE_RENDER_ABSOLUTE);
	Project_registerSystem(&textRenderAbs, LOCALITY_STATE_RENDER_ABSOLUTE);

	// Launch project specific code
	project();
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

void SolutionLogic_render_abs(){
	state.programState = LOCALITY_STATE_RENDER_ABSOLUTE;
	view world = renderGetView();
	renderSetViewAbsolute();
	vSystemActivate(&(state.renderList_abs));
	renderSetView(world);
}

// time

void projectTickUpdate(){
	state.tick += (clock() - state.baseTime);
}

uint8_t projectTick(){
	return (((double)state.tick/(double)CLOCKS_PER_SEC)*1000) >= TICK_TIME;
}

void projectTickReset(){
	state.baseTime = clock();
	state.tick = 0;
}

