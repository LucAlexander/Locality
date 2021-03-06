#include <stdio.h>

#include "Entity-Component-System/ecs.h"
#include "SDL2-Utility/inpututils.h"
#include "SDL2-Utility/mathutils.h"

#include "Locality.h"
#include "LocGUI.h"
#include "LocBaseSystems.h"

VECTOR_SOURCE(vSystem, System)

static Project_state state;
static Project_config config;

void Project_configInit(){
	config.window_w = 640;
	config.window_h = 360;
	strcpy(config.window_title,"Locality Project");
	config.ticks_per_second = 60;
	project_config(&config);
	config.tick_time = 1000/config.ticks_per_second;
}

uint32_t getWindowW(){
	return config.window_w;
}

uint32_t getWindowH(){
	return config.window_h;
}

void Project_stateInit(){
	state.run = 1;
	state.updateList_pre = vSystemInit();
	state.updateList = vSystemInit();
	state.updateList_post = vSystemInit();
	state.freeData = vSystemInit();
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
		case LOCALITY_STATE_FREE_DATA:
			SystemAddMagnet(sys, ENTITY_DEACTIVATE);
			vSystemPushBack(&(state.freeData), *sys);
		return;
		case LOCALITY_STATE_RENDER:
			SystemAddFilter(sys, RENDER_ABSOLUTE);
			vSystemPushBack(&(state.renderList), *sys);
		return;
		case LOCALITY_STATE_RENDER_ABSOLUTE:
			SystemAddFilter(sys, RENDER_RELATIVE);
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
	vSystemFree(&(state.freeData));
	vSystemFree(&(state.renderList));
	vSystemFree(&(state.renderList_abs));
}

void Solution_setup(){
	ecsInit(LOCALITY_COMPONENT_COUNT,
		sizeof(v2),
		sizeof(Blitable),
		sizeof(pressable_l),
		sizeof(void*),
		sizeof(text_l),
		sizeof(v2),
		sizeof(behavior_l),
		sizeof(repeater_l),
		PROJECT_COMPONENT_SIZES
	);
	graphicsInit(config.window_w, config.window_h, config.window_title);
	inputInit();
	loadFont(FONT_FILE "arcade.TTF", "default");
	setFont("default");
	view v = {
		0, 0,
		0, 0,
		config.window_w, config.window_h
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
		SolutionLogic_update_pre();
		SolutionLogic_update();
		SolutionLogic_update_post();
		SolutionLogic_freeData();
		purgeDeactivatedData();
		newInputFrame();
		renderClear();
		renderSetColor(0, 0, 0, 255);
		SolutionLogic_render();
		SolutionLogic_render_abs();
		renderFlip();
		projectTickReset();
	}
}

int main(int argc, char** argv){
	Project_configInit();
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
	System pressFree = SystemInit(pressable_f, 1, PRESSABLE_C);

	System blitRender = SystemInit(Blitable_sr, 2, POSITION_C, BLITABLE_C);
	System blitRenderAbs = SystemInit(Blitable_sr, 2, POSITION_C, BLITABLE_C);
	System blitFree = SystemInit(Blitable_f, 1, BLITABLE_C);

	System textRender = SystemInit(text_sr, 2, POSITION_C, TEXT_C);
	System textRenderAbs = SystemInit(text_sr, 2, POSITION_C, TEXT_C);

	System behaviorUpdate = SystemInit(behavior_su, 1, BEHAVIOR_C);
	System repeaterUpdate = SystemInit(repeater_su, 1, REPEATER_C);

	// Register systems to states
	Project_registerSystem(&behaviorUpdate, LOCALITY_STATE_UPDATE);
	Project_registerSystem(&repeaterUpdate, LOCALITY_STATE_UPDATE);
	Project_registerSystem(&forcesUpdate, LOCALITY_STATE_UPDATE);
	Project_registerSystem(&pressUpdate, LOCALITY_STATE_UPDATE);
	Project_registerSystem(&blitRender, LOCALITY_STATE_RENDER);
	Project_registerSystem(&textRender, LOCALITY_STATE_RENDER);
	Project_registerSystem(&pressRender, LOCALITY_STATE_RENDER_ABSOLUTE);
	Project_registerSystem(&blitRenderAbs, LOCALITY_STATE_RENDER_ABSOLUTE);
	Project_registerSystem(&textRenderAbs, LOCALITY_STATE_RENDER_ABSOLUTE);
	Project_registerSystem(&blitFree, LOCALITY_STATE_FREE_DATA);
	Project_registerSystem(&pressFree, LOCALITY_STATE_FREE_DATA);

	// Launch project specific code
	project_systems();
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

void SolutionLogic_freeData(){
	state.programState = LOCALITY_STATE_FREE_DATA;
	vSystemActivate(&(state.freeData));
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
	state.tick = (SDL_GetTicks() - state.baseTime);
}

uint8_t projectTick(){
	return state.tick >= config.tick_time;
}

void projectTickReset(){
	state.baseTime = SDL_GetTicks();
	state.tick = 0;
}

uint32_t getFrameTime(){
	return state.tick;
}
