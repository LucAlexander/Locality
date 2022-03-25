# Locality
![](https://github.com/LucAlexander/Locality/blob/master/Locality.png)

Locality is a (proof of concept) general purpose software development tool built to optimize program locality using a modified Entity (Data) Component System for data management. This custom software architecture promotes a separation between the programs data and logic systems in a data oriented approach to software development. 
## ( Please note that this Project is heavily work in progress )
### Modularity
This project was build with the idea in mind that you could swap out the SDL media layer with your own, and replace the relevant functions with your own preferred functionality, while this may be a daughnting task at first, it is possible, you may just have to recompile this project from source yourself to get it to work, but it is entirely open source so use it at will.
# Documentation

## Usage
### Setup

Please note that all code was written and tested on GNU-C 11, however this project is written with compatibility in mind, and you should be able to use this for any C or C++ project, just keep in mind that you may want to keep within the GNU standard versions.

### Entry Point Files
When you create a new project, you should be provided with two default files, you can add your own to this directory and they will be included in the projects compilation by default. The first file you are provided is `project.c` which serves as an entry point fiel for your project. This file generates as:

```
#include <stdio.h>
#include "Locality.h"
#include "LocGUI.h"

void project_config(Project_config* config){
	
}

void project_systems(){
	
}

void project(){
	printf("Hello World\n");
}

```

`project()` is the setup function for all custom behavior of any specific project. This is where you create and position the initial state of your program. This runs immediately after `project_systems()`.
`project_systems()` is where any user defined logic, destructor, or rendering systems go. These can be based on both user defined components and Locality defined components. This runs after all Locality defined components are registered. The user must also register all systems here to their appropriate program activity states.
`project_config(Project_config* config)` is where any configuration code can be put to edit the default Locality configuration settings. This is where the window size, title, and tick update time can be edited.

In addition to this file, you are provided a file `projectComponents.h` which contains a pre processor definition of two lists, `PROJECT_COMPONENTS_H` which takes the enumerated ID of whatever component you add, and `PROJECT_COMPONENT_SIZES` which takes the size in bytes of each component you add, the order of these two lists must be consistent.

## Data Components
When you create a new data component type, you must register it through these lists. 
```
typedef struct v2{
	float x;
	float y;
}v2;

typedef struct image{
	const char* source;
	uint32_t width;
	uint32_t height;
}image;

```

In `projectComponents.h`:
```
#ifndef PROJECT_COMPONENTS_H
#define PROJECT_COMPONENTS_H \
	POINT_C,\
	SPRITE_C

#define PROJECT_COMPONENT_SIZES \
	sizeof(v2),\
	sizeof(image)
```

This component can now be used in accordance with the data management system.

## Systems
The way systems are created, activated, customized and destroyed follows as it is described in the Entity Component System Documentation. This project serves as a complete implementation of s system manager. In Locality, systems must be registered to some target program state which denotes at which stage in this program the system should activate. Other functionalities such as flagged entity purging are entirely implemented and it should be assumed that they work as written and intended.

### Program States
The available program states are defines in the `PROGRAM_STATE` enumerator as
```
typedef enum PROGRAM_STATE{
	LOCALITY_STATE_INIT,
	LOCALITY_STATE_DEINIT,
	LOCALITY_STATE_UPDATE_PRE,
	LOCALITY_STATE_UPDATE,
	LOCALITY_STATE_UPDATE_POST,
	LOCALITY_STATE_FREE_DATA,
	LOCALITY_STATE_RENDER,
	LOCALITY_STATE_RENDER_ABSOLUTE
}PROGRAM_STATE;
```

- LOCALITY_STATE_INIT runs at the very beginning of the program, and is meant as an initialization state
- LOCALITY_STATE_DEINIT runs at the very end of the program, and is meant as a memory cleanup state
- LOCALITY_STATE_UPDATE_PRE runs at the beginning of the logical update segment of the program, this runs every logical update tick
- LOCALITY_STATE_UPDATE is the primary logical update state of the program, this runs every logical update every tick
- LOCALITY_STATE_UPDATE_POST runs at the end of the logical update segment of the program, this runs every logical update tick
- LOCALITY_STATE_FREE_DATA runs after all logical updates, and deals with freeing all marked memory 
- LOCALITY_STATE_RENDER runs when the program is rendering all changes to the screen. Rendering in this state occurs relative to the windows view port.
- LOCALITY_STATE_RENDER_ABSOLUTE runs at the end of the programs render state. Rendering in this state occurs with absolute positioning to the window.


### Registering Systems
Systems are registered to your project with `Project_registerSystem(struct System* sys, PROGRAM_STATE mode);`. Suppose you have a set of systems `pressUpdate` and `pressRender` defined as
```
System pressUpdate = SystemInit(pressabe_su, 2, POSITION_C, PRESSABLE_C);
System pressRender = SystemInit(pressabe_sr, 2, POSITION_C, PRESSABLE_C);
System pressFree = SystemInit(pressable_f, 1, PRESSABLE_C);
```

which respectively update and render, and free relevant data  associated with a clickable component to a data entity. `pressUpdate` needs to be registered with `LOCALITY_STATE_UPDATE`. This is simple enough, as all we need to do is call
```
Project_registerSystem(&pressUpdate, LOCALITY_STATE_UPDATE);
```

`pressRender` needs to be registered with `LOCALITY_STATE_RENDER_ABSOLUTE` because we want it to render with absolute positioning so it remains at the same position on the screen no matter what state the system view port is in.
```
Project_registerSystem(&pressRender, LOCALITY_STATE_RENDER_ABSOLUTE);
```

finally `pressFree` must be reigistered with `LOCALITY_STATE_FREE_DATA`, note that registering a system in this state automatically applies a magnet for deactivated entities, so the system will only run on entity data which is marked for deletion with `void markForPurge(uint32_t eid);`.
```
Project_registerSystem(&pressFree, LOCALITY_STATE_FREE_DATA);
```

Note the naming convention for the internal system functions, `_su` for logical update system, `_sr` for logical rendering system, and `_f` for any system which frees marked memory.

## Resources
There are build in directory references to non code resource files. Anything from fonts to images. These are defined in the preprocessor in the following manner
```
#define IMAGE_FILE "resources/images/"
#define FONT_FILE "resources/fonts/"

```
This helps simplify the reference process, if you mean to reference an image file located at `"resources/images/box.png"`, you may simply reference it as `IMAGE_FILE "box.png"`.

## Built in Utilities
The base components and systems are divided up into a series of modules. Systems listed here will usually run on their own, and the components are available for you to use.

### Excplicit Base Components
- `behavior_l` is a behavior component for explicit custom behavior. It contains a function pointer which takes a `uint32_t` entity id. It is not reccommended that you overuse this component, as its operation is naturally slower.
- `repeater_l` is a functional component for repeated action among some time frame.

### Base Systems
- `void Blitable_sr(struct SysData* sys);`
This system requires a `POSITION_C` component and a `BLITABLE_C` component. It blits the image to the screen at the given position.
- `void Blitable_f(struct SysData* sys);`
This system requires only a `BLITABLE_C`, of which it then frees any relevant data.

- `void forces_su(struct SysData* sys);`
This system requires a `POSITION_C` component and a `FORCES_C` component. Both are `v2` struct types. This System applies forces' x to position's x, and forces' y to position's y.

- User available function `void forces_applyForce(v2* left, float len, float dir);`
This function applies forces to an existing `v2` force.
- User available function `void forces_applyForceV2(v2* left, v2 force);`
This function is the Same as the previous one, but with `v2` input for the added force.

- User available function `void behavior_l_init(struct behavior_l* s, void(*f)(uint32_t));`
- User available function `void behavior_su(struct SysData* sys);`
- User available function `void repeater_l_init(struct repeater_l* l, void (*f)(void*), uint32_t ms, uint32_t n);` where ms is the interval in milliseconds, and n is the times to repeat per interval.
- User available function `void repeater_setMaxTriggers(struct repeater_l* l, uint32_t max);` to set the maximum number of trigger intervals which can elapse before the component entity self destructs.
- User available function `void repeater_setMaxTime(struct repeater_l* l, uint32_t max);` to set the maximum time for the component entity to exist.
- `void repeater_su(struct SysData* sys);` which updates the repeater and triggers it when time intervals elapse.

### GUI Utilities
**Component** `struct pressable_l`, **ID:** `PRESSABLE_C`

The pressable component denotes a region in which the user can click, which activates some provided function. This component can be given three image sources for idle, hovering and being pressed.

The action can be set as
```
void f(void*){}

...


pressable_l a;
a.action = f;
```

The argument for this action can be passed as a `PRESSABLE_ARC_C`, the type is `void*`.

- User available function `pressable_l_init(pressable* pres, void act(void*), const char* normal, const char* hover, const char* press, uint32_t w, uint32_t h);`
- System `void pressable_su(struct SysData* sys);`
- System `void pressable_sr(struct SysData* sys);`
- System `void pressable_f(struct SysData* sys);`

Both systems require a position to function as well.

**Component** `struct text_l` **ID:** `TEXT_C`

The text component denotes some text to be blitted to the screen. 

- User available function `void text_l_init(text_l* txt, const char* message, uint8_t r, uint8_t g, uint8_t b, uint8_t a);`
- User available function `void text_l_setColor(text_t* txt, uint8_t r, uint8_t g, uint8_t b, uint8_t a);`
- System `void text_sr(struct SysData* sys);`

The render system requires a `POSITION_C` component as well.
