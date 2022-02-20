#include "LocGUI.h"
#include "SDL2-Utility/graphicsutils.h"
#include "SDL2-Utility/inpututils.h"
#include "SDL2-Utility/mathutils.h"
#include "Entity-Component-System/ecs.h"

void pressable_l_init(pressable_l* pres, void act(void*), const char* normal, const char* hover, const char* press, uint32_t w, uint32_t h){
	BlitableInitF(&pres->normal, normal, w, h);
	BlitableInitF(&pres->hover, hover, w, h);
	BlitableInitF(&pres->press, press, w, h);
	pres->normal.center.x = 0;
	pres->normal.center.y = 0;
	pres->hover.center.x = 0;
	pres->hover.center.y = 0;
	pres->press.center.x = 0;
	pres->press.center.y = 0;
	pres->active = &pres->normal;
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
	press->active = &press->normal;
	if (pointInRectV2(mp, area)){
		press->active = &press->hover;
		if (mousePressed(1)){
			press->active = &press->press;
			press->action(NULL);
		}
	}
}

void pressable_sr(SysData* sys){
	v2* pos = componentArg(sys, 0);
	pressable_l* press = componentArg(sys, 1);
	renderBlitableV2(press->active, *pos);
}

void Blitable_sr(SysData* sys){
	v2* pos = componentArg(sys, 0);
	Blitable* active = componentArg(sys, 1);
	renderBlitableV2(active, *pos);
}
