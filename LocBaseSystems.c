#include "LocBaseSystems.h"
#include "LocBaseComponents.h"
#include "Locality.h"

#include "SDL2-Utility/graphicsutils.h"
#include "SDL2-Utility/inpututils.h"

#include "Entity-Component-System/ecs.h"

void Blitable_sr(SysData* sys){
	v2* pos = componentArg(sys, 0);
	Blitable* active = componentArg(sys, 1);
	renderBlitableV2(active, *pos);
}

void forces_su(SysData* sys){
	v2* pos = componentArg(sys, 0);
	v2* forces = componentArg(sys, 1);
	pos->x += forces->x * getFrameTime();
	pos->y += forces->y * getFrameTime();
}

void forces_applyForce(v2* left, float len, float dir){
	left->x += lenDirX(len, dir);
	left->y += lenDirY(len, dir);
}

void forces_applyForceV2(v2* left, v2 force){
	forces_applyForce(left, force.x, force.y);
}

void behavior_init(behavior_l* s, void(*f)(uint32_t)){
	s->action = f;
}

void behavior_su(SysData* sys){
	behavior_l* s = componentArg(sys, 0);
	uint32_t entity = entityArg(sys);
	s->action(entity);
}




