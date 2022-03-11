#include "LocBaseSystems.h"
#include "Locality.h"

#include "SDL2-Utility/graphicsutils.h"
#include "SDL2-Utility/mathutils.h"
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
	pos->x += forces->x;
	pos->y += forces->y;
}
