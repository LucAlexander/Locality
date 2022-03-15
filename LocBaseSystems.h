#ifndef LOCALITY_BASE_SYSTEMS_H
#define LOCALITY_BASE_SYSTEMS_H

#include "SDL2-Utility/mathutils.h"

struct SysData;
struct behavior_l;
struct repeater_l;

void Blitable_sr(struct SysData* sys);
void Blitable_f(struct SysData* sys);

void forces_su(struct SysData* sys);
void forces_applyForce(v2* left, float len, float dir);
void forces_applyForceV2(v2* left, v2 force);

void behavior_l_init(struct behavior_l* s, void(*f)(uint32_t));
void behavior_su(struct SysData* sys);

void repeater_l_init(struct repeater_l* l, void (*f)(void*), uint32_t ms, uint32_t n);
void repeater_trigger(struct SysData* sys, struct repeater_l* l);
void repeater_setMaxTriggers(struct repeater_l* l, uint32_t max);
void repeater_setMaxTime(struct repeater_l* l, uint32_t max);
void repeater_su(struct SysData* sys);

#endif
