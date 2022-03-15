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

void Blitable_f(struct SysData* sys){
	Blitable* blit = componentArg(sys, 0);
	BlitableFree(blit);
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

void behavior_l_init(behavior_l* s, void(*f)(uint32_t)){
	s->action = f;
}

void behavior_su(SysData* sys){
	behavior_l* s = componentArg(sys, 0);
	uint32_t entity = entityArg(sys);
	s->action(entity);
}

void repeater_l_init(repeater_l* l, void (*f)(void*), uint32_t ms, uint32_t n){
	l->action = f;
	l->triggerInterval = ms;
	l->triggerTime = ms;
	l->actionCount = n;
	l->maxTriggers = REPEATER_SENTINEL;
	l->maxTime = REPEATER_SENTINEL;
}

void repeater_setMaxTriggers(struct repeater_l* l, uint32_t max){
	l->maxTriggers = max;
}

void repeater_setMaxTime(struct repeater_l* l, uint32_t max){
	l->maxTime = max;
}

void repeater_trigger(SysData* sys, repeater_l* l){
	uint32_t entity = entityArg(sys);
	void* args = NULL;
	l->triggerTime = l->triggerInterval;
	if (l->maxTriggers==0){
		markForPurge(entity);
		return;
	}
	if (l->maxTriggers > 0){
		l->maxTriggers--;
	}
	if (containsComponent(entity, REPEATER_ARG_C)){
		args = getComponent(entity, REPEATER_ARG_C);
	}
	for (uint32_t i = 0;i<l->actionCount;++i){
		l->action(args);
	}
}

void repeater_su(SysData* sys){
	repeater_l* l = componentArg(sys, 0);
	l->triggerTime -= getFrameTime();
	if (l->maxTime > 0){
		l->maxTime -= getFrameTime();
		if (l->maxTime <= 0){
			uint32_t entity = entityArg(sys);
			markForPurge(entity);
			return;
		}
	}
	if (l->triggerTime <= 0){
		repeater_trigger(sys, l);
	}
}

