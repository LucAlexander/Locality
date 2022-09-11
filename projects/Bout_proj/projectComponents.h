//This is where your components go
#ifndef PROJECT_COMPONENTS_H

#include "cardComponents.h"

#define PROJECT_COMPONENTS_H \
	COMPONENT_NAME_C, \
	CARD_C, \
	CARD_DECK_C

#define PROJECT_COMPONENT_SIZES \
	sizeof(void*), \
	sizeof(card), \
	sizeof(Vu32)
#endif

