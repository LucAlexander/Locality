#ifndef LOCALITY_MAIN_H
#define LOCALITY_MAIN_H

#include <inttypes.h>
#include "SDL2-Utility/graphicsutils.h"


typedef enum COMPONENT_ID{
	POSITION_C=0
}COMPONENT_ID;

typedef struct Project_state{
	uint8_t run;
	SDL_Event event;
}Project_state;

void Project_stateInit();

void Solution_setup();
void Solution_inputEvents();
void Solution_exit();

// PROJECT LOGIC

void SolutionLogic_init();
void SolutionLogic_deinit();

void SolutionLogic_update_pre();
void SolutionLogic_update();
void SolutionLogic_update_post();

void SolutionLogic_render();

#endif
