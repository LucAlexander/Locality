#ifndef LOCALITY_MAIN_H
#define LOCALITY_MAIN_H

#include <time.h>
#include <inttypes.h>

#include "LocBaseComponents.h"

#include "SDL2-Utility/graphicsutils.h"
#include "DataContainers/src/vector/vector.h"

#define IMAGE_FILE "resources/images/"
#define FONT_FILE "resources/fonts/"

#define PROJECT_TITLE_LENGTH 64

struct System;

VECTOR(vSystem, struct System)

void vSystemActivate(vSystem* vec);

typedef enum PROGRAM_STATE{
	LOCALITY_STATE_INIT,
	LOCALITY_STATE_DEINIT,
	LOCALITY_STATE_UPDATE_PRE,
	LOCALITY_STATE_UPDATE,
	LOCALITY_STATE_UPDATE_POST,
	LOCALITY_STATE_FREE_DATA,
	LOCALITY_STATE_RENDER,
	LOCALITY_STATE_RENDER_ABSOLUTE
}PROGRAM_STATE;

typedef struct Project_config{
	uint32_t window_w;
	uint32_t window_h;
	char window_title[PROJECT_TITLE_LENGTH];
	uint32_t ticks_per_second;
	uint32_t tick_time;
}Project_config;

uint32_t getWindowW();
uint32_t getWindowH();

typedef struct Project_state{
	uint8_t run;
	vSystem updateList_pre;
	vSystem updateList;
	vSystem updateList_post;
	vSystem freeData;
	vSystem renderList;
	vSystem renderList_abs;
	PROGRAM_STATE programState;
	SDL_Event event;
	uint32_t baseTime;
	uint32_t tick;
}Project_state;

void Project_configInit();
void Project_configRead();
void Project_configParse(char* line, uint32_t len);
void Project_configSetVariable(char* variable, char* value);

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

void SolutionLogic_freeData();

void SolutionLogic_render();
void SolutionLogic_render_abs();

void project_systems();
void project();

// time
void projectTickUpdate();
uint8_t projectTick();
void projectTickReset();
uint32_t getFrameTime();

#endif
