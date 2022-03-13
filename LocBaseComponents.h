#ifndef LOCALITY_BASE_COMPONENTS_H
#define LOCALITY_BASE_COMPONENTS_H

#include <inttypes.h>

typedef enum COMPONENT_ID{
	POSITION_C=0,
	BLITABLE_C=1,
	PRESSABLE_C=2,
	PRESSABLE_ARG_C=3,
	TEXT_C=4,
	FORCES_C=5,
	BEHAVIOR_C=6,
	REPEATER_C=7,
	REPEATER_ARG_C=8,
	LOCALITY_COMPONENT_COUNT
}COMPONENT_ID;

typedef enum LOCALITY_COMPONENT_SENTINELS{
	REPEATER_SENTINEL=-1
}LOCALITY_COMPONENT_SENTINELS;

typedef struct behavior_l{
	void (*action)(uint32_t entity);
}behavior_l;

typedef struct repeater_l{
	void (*action)(void*);
	uint32_t triggerInterval;
	int32_t triggerTime;
	uint32_t actionCount;
	int32_t maxTriggers;
	int32_t maxTime;
}repeater_l;

#endif
