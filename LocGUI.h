#ifndef LOCALITY_GUI_H
#define LOCALITY_GUI_H

#include <inttypes.h>
#include "SDL2-Utility/graphicsutils.h"

struct SysData;

void Blitable_sr(struct SysData* sys);

typedef struct pressable_l{
	struct Blitable normal;
	struct Blitable hover;
	struct Blitable press;
	struct Blitable* active;
	void (*action)(void*);
	uint32_t w;
	uint32_t h;
}pressable_l;

void pressable_l_init(pressable_l* pres, void act(void*), const char* normal, const char* hover, const char* press, uint32_t w, uint32_t h);

void pressable_su(struct SysData* sys);
void pressable_sr(struct SysData* sys);

#endif
