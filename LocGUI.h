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

void pressable_destroy(uint32_t eid, uint32_t cid);

typedef struct text_l{
	const char*  content;
	uint8_t r;
	uint8_t g;
	uint8_t b;
	uint8_t a;
}text_l;

void text_l_init(text_l* txt, const char* message, uint8_t r, uint8_t g, uint8_t b, uint8_t a);

void text_l_setColor(text_l* txt, uint8_t r, uint8_t g, uint8_t b, uint8_t a);

void text_sr(struct SysData* sys);

#endif
