#ifndef CARD_COMPONENTS_H
#define CARD_COMPONENTS_H

#include "DataContainers/src/vector/vector.h"

#define VALUE_RANGE 13
#define SUIT_COUNT 4

#define LANE_COUNT 4
#define LANE_SIZE 2

#define HAND_SIZE 4

#define CARD_SENTINEL -1

typedef struct card{
	int8_t value, suit;
}card;

typedef enum PLAYING_ENTITIES{
	PLAYER=0,
	OPPONENT=1
}PLAYING_ENTITIES;

typedef enum LANE_STATE{
	LANE_EMPTY,
	LANE_ATTACK,
	LANE_PARRY,
	LANE_DOUBLE_PARRY,
	LANE_LUNGE
}LANE_STATES;

#endif
