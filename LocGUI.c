#include "LocGUI.h"
#include "Locality.h"
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
	uint32_t entity = entityArg(sys);
	v2* pos = componentArg(sys, 0);
	pressable_l* press = componentArg(sys, 1);
	v2 mp = mousePos();
	v4 area = {
		pos->x, pos->y,
		press->w, press->h
	};
	press->active = &press->normal;
	if (pointInRectV2(mp, area)){
		pressable_hovered(entity, press);
	}
}

void pressable_hovered(uint32_t entity, pressable_l* press){
	void* arg = NULL;
	if (containsComponent(entity, PRESSABLE_ARG_C)){
		arg = getComponent(entity, PRESSABLE_ARG_C);
	}
	press->active = &press->hover;
	if (mousePressed(1)){
		press->action(arg);
	}
	if (mouseHeld(1)){
		press->active = &press->press;
	}
}

void pressable_sr(SysData* sys){
	v2* pos = componentArg(sys, 0);
	pressable_l* press = componentArg(sys, 1);
	renderBlitableV2(press->active, *pos);
}

void pressable_destroy(uint32_t eid, uint32_t cid){
	pressable_l* trigger = getComponent(eid, cid);
	BlitableFree(&(trigger->normal));
	BlitableFree(&(trigger->hover));
	BlitableFree(&(trigger->press));
	markForPurge(eid);
}

void text_l_init(text_l* txt, const char* message, uint8_t r, uint8_t g, uint8_t b, uint8_t a){
	txt->content = message;
	txt->r = r;
	txt->g = g;
	txt->b = b;
	txt->a = a;
}

void text_l_setColor(text_l* txt, uint8_t r, uint8_t g, uint8_t b, uint8_t a){
	txt->r = r;
	txt->g = g;
	txt->b = b;
	txt->a = a;
}

void text_sr(SysData* sys){
	v2* pos = componentArg(sys, 0);
	text_l* txt = componentArg(sys, 1);
	drawTextV2C(*pos, txt->content, txt->r, txt->b, txt->b, txt->a);
}
