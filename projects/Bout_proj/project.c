//This is your projects entry point file
#include<stdio.h>
#include"Locality.h"
#include"LocGUI.h"

#include "Entity-Component-System/ecs.h"

#include "cardComponents.h"
#include "cardSystems.h"

void project_config(Project_config* config){
	// Configuration code
	config->window_w = 1366;
	config->window_h = 768;
	config->ticks_per_second = 60;
	strcpy(config->window_title, "Bout");
}

void project_systems(){
	// Initialize and register project systems
	
}

void project(){
	uint32_t i, k;
	Vu32 deck = Vu32Init();
	Vu32 playerHand = Vu32Init();
	Vu32 opponentHand = Vu32Init();
	for (i = 0;i<VALUE_RANGE;++i){
		for (k = 0;k<SUIT_COUNT;++k){
			uint32_t entity = summon();
			card n = {i, k};
			addComponent(entity, COMPONENT_NAME_C, &n);
			Vu32PushBack(&deck, entity);
		}
	}
	uint32_t deck_entity = summon();
	uint32_t player_hand_entity = summon();
	uint32_t opponent_hand_entity = summon();
	addComponent(deck_entity, CARD_DECK_C, &deck);
	addComponent(player_hand_entity, CARD_DECK_C, &playerHand);
	addComponent(opponent_hand_entity, CARD_DECK_C, &opponentHand);
	printf("setup complete\n");

}

