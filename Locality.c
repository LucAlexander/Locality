#include <stdio.h>

#include "Entity-Component-System/ecs.h"
#include "SDL2-Utility/inpututils.h"
#include "SDL2-Utility/mathutils.h"

#include "Locality.h"

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 360

static Project_state state;

void Project_stateInit(){
	state.run = 1;
}

void Solution_setup(){
	ecsInit(1, sizeof(v2));
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
	if (!SDL_PollEvent(&(state.event))){
		return;
	}
	switch(state.event.type){
		case SDL_QUIT:
			state.run = 0;
		break;
		case SDL_KEYDOWN:
			if (state.event.key.repeat == 0){
				keyDownEvent(state.event);
			}
		break;
		case SDL_KEYUP:
			keyUpEvent(state.event);
		break;
		case SDL_MOUSEBUTTONDOWN:
			if (!mouseHeld(state.event.button.button)){
				mouseDownEvent(state.event);
			}
		break;
		case SDL_MOUSEBUTTONUP:
			mouseUpEvent(state.event);
		break;
		case SDL_MOUSEWHEEL:
			mouseScrollEvent(state.event.wheel.y);
		break;
		case SDL_MOUSEMOTION:
			mouseMoveEvent(state.event.motion.x, state.event.motion.y);
		break;
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
		Solution_inputEvents();
		newInputFrame();
		SolutionLogic_update_pre();
		SolutionLogic_update();
		SolutionLogic_update_post();
		renderClear();
		renderSetColor(0, 0, 0, 255);
		SolutionLogic_render();
		renderFlip();
	}
	SolutionLogic_deinit();
	Solution_exit();
	return 0;
}

void SolutionLogic_init(){
	// SOFTWARE_STATE_INITIALIZER
}

void SolutionLogic_deinit(){
	// SOFTWARE_STATE_DESTRUCTOR
}

void SolutionLogic_update_pre(){
	// SOFTWARE_STATE_PREUPDATER
}

void SolutionLogic_update(){
	// SOFTWARE_STATE_UPDATER
}

void SolutionLogic_update_post(){
	// SOFTWARE_STATE_POSTUPDATER
}

void SolutionLogic_render(){
	// SOFTWARE_STATE_RENDERER
}

