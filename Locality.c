#include <stdio.h>
#include <inttypes.h>

#include "Entity-Component-System/ecs.h"
#include "SDL2-Utility/graphicsutils.h"
#include "SDL2-Utility/inpututils.h"
#include "SDL2-Utility/mathutils.h"

#include "Locality.h"

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 360

int main(int argc, char** argv){
	ecsInit(1, sizeof(v2));
	graphicsInit(WINDOW_WIDTH, WINDOW_HEIGHT, "Locality Project");
	inputInit();
	view v = {
		0, 0,
		0, 0,
		WINDOW_WIDTH, WINDOW_HEIGHT
	};
	renderSetView(v);
	SDL_Event event;
	uint8_t run = 1;
	while(run){
		if (SDL_PollEvent(&event)){
			switch(event.type){
				case SDL_QUIT:
					run = 0;
				break;
				case SDL_KEYDOWN:
					if (event.key.repeat == 0){
						keyDownEvent(event);
					}
				break;
				case SDL_KEYUP:
					keyUpEvent(event);
				break;
				case SDL_MOUSEBUTTONDOWN:
					if (!mouseHeld(event.button.button)){
						mouseDownEvent(event);
					}
				break;
				case SDL_MOUSEBUTTONUP:
					mouseUpEvent(event);
				break;
				case SDL_MOUSEWHEEL:
					mouseScrollEvent(event.wheel.y);
				break;
				case SDL_MOUSEMOTION:
					mouseMoveEvent(event.motion.x, event.motion.y);
				break;
			}
		}
		newInputFrame();
		renderClear();
		renderSetColor(0, 0, 0, 255);
		renderFlip();
	}
	graphicsClose();
	freeEcs();
	return 0;
}
