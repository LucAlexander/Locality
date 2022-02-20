#include "LocGUI.h"
#include "SDL2-Utility/graphicsutils.h"
#include "SDL2-Utility/inpututils.h"
#include "SDL2-Utility/mathutils.h"
#include "Entity-Component-System/ecs.h"

void pressable_l_init(pressable_l* pres, void act(void*), Blitable* normal, Blitable* hover, Blitable* press, uint32_t w, uint32_t h){
	pres->normal = normal;
	pres->hover = hover;
	pres->press = press;
	pres->active = normal;
	pres->action = act;
	pres->w = w;
	pres->h = h;
}

void pressable_su(SysData* sys){
	v2* pos = componentArg(sys, 0);
	pressable_l* press = componentArg(sys, 1);
	v2 mp = mousePos();
	v4 area = {
		pos->x, pos->y,
		press->w, press->h
	};
	press->active = press->normal;
	if (pointInRectV2(mp, area)){
		press->active = press->hover;
		if (mousePressed(1)){
			press->active = press->press;
			press->action(NULL);
		}
	}
}

void Blitable_sr(SysData* sys){
	v2* pos = componentArg(sys, 0);
	Blitable* active = componentArg(sys, 1);
	renderBlitableV2(active, *pos);
}

void pressable_sr(SysData* sys){
	v2* pos = componentArg(sys, 0);
	pressable_l* press = componentArg(sys, 1);
	renderBlitableV2(press->active, *pos);
}
