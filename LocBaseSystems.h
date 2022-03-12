#ifndef LOCALITY_BASE_SYSTEMS_H
#define LOCALITY_BASE_SYSTEMS_H

#include "SDL2-Utility/mathutils.h"

struct SysData;
struct behavior_l;

void Blitable_sr(struct SysData* sys);

void forces_su(struct SysData* sys);
void forces_applyForce(v2* left, float len, float dir);
void forces_applyForceV2(v2* left, v2 force);

void behavior_init(struct behavior_l* s, void(*f)(uint32_t));
void behavior_su(struct SysData* sys);

#endif
