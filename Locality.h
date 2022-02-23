#ifndef LOCALITY_MAIN_H
#define LOCALITY_MAIN_H

#include <inttypes.h>
#include "SDL2-Utility/graphicsutils.h"
#include "DataContainers/src/vector/vector.h"

#define IMAGE_FILE "resources/images/"
#define FONT_FILE "resources/fonts/"

typedef enum COMPONENT_ID{
	POSITION_C=0,
	BLITABLE_C=1,
	PRESSABLE_C=2,
	TEXT_C=3
}COMPONENT_ID;

struct System;

VECTOR(vSystem, struct System)

void vSystemActivate(vSystem* vec);

typedef enum PROGRAM_STATE{
	LOCALITY_STATE_INIT,
	LOCALITY_STATE_DEINIT,
	LOCALITY_STATE_UPDATE_PRE,
	LOCALITY_STATE_UPDATE,
	LOCALITY_STATE_UPDATE_POST,
	LOCALITY_STATE_RENDER
}PROGRAM_STATE;

typedef struct Project_state{
	uint8_t run;
	vSystem updateList_pre;
	vSystem updateList;
	vSystem updateList_post;
	vSystem renderList;
	PROGRAM_STATE programState;
	SDL_Event event;
}Project_state;

void Project_stateInit();
void Project_registerSystem(struct System* sys, PROGRAM_STATE mode);
void Project_freeSystems();
void Project_stateExit();

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
