#ifndef LOCALITY_MAIN_H
#define LOCALITY_MAIN_H

#include <time.h>
#include <inttypes.h>

#include "SDL2-Utility/graphicsutils.h"
#include "DataContainers/src/vector/vector.h"

#define IMAGE_FILE "resources/images/"
#define FONT_FILE "resources/fonts/"

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 360

#define WINDOW_TITLE "Loclity Project"

#define TICKS_PER_SECOND 60
#define TICK_TIME 1000/TICKS_PER_SECOND

typedef enum COMPONENT_ID{
	POSITION_C=0,
	BLITABLE_C=1,
	PRESSABLE_C=2,
	PRESSABLE_ARG_C=3,
	TEXT_C=4,
	FORCES_C=5
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
	LOCALITY_STATE_RENDER,
	LOCALITY_STATE_RENDER_ABSOLUTE
}PROGRAM_STATE;

typedef struct Project_state{
	uint8_t run;
	vSystem updateList_pre;
	vSystem updateList;
	vSystem updateList_post;
	vSystem renderList;
	vSystem renderList_abs;
	PROGRAM_STATE programState;
	SDL_Event event;
	clock_t baseTime;
	clock_t tick;
}Project_state;

void Project_stateInit();
void Project_registerSystem(struct System* sys, PROGRAM_STATE mode);
void Project_freeSystems();
void Project_stateExit();

void Solution_setup();
void Solution_inputEvents();
void Solution_tick();
void Solution_exit();

// PROJECT LOGIC

void SolutionLogic_init();
void SolutionLogic_deinit();

void SolutionLogic_update_pre();
void SolutionLogic_update();
void SolutionLogic_update_post();

void SolutionLogic_render();
void SolutionLogic_render_abs();

void project();

// time
void projectTickUpdate();
uint8_t projectTick();
void projectTickReset();

#endif
