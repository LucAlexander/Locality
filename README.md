# Locality
Locality is a (proof of concept) general purpose software development tool built to optimize program locality using a modified Entity (Data) Component System for data management. This custom software architecture promotes a separation between the programs data and logic systems in a data oriented approach to software development. 
## ( Please note that this Project is heavily work in progress )
## Dependancies
### Data Containers
Clone [this repository](https://Github.com/LucAlexander/DataContainers) into your projects directory and use the make target `make build` to build a static library from the repositories source code.

This static library provides various common data containers such as Hash Maps, Queues etc.

### Entity Component System
Clone [this repository](https://Github.com/LucAlexander/Entity-Component-System) into your projects directory and use the make target `make build` to build a static library from the repositories source code. Note that this library also depends on the Data Containers dependancy. 

This static library provides the data management system.

### SDL2 Wrapper
Clone [this repository](https://Github.com/LucAlexander/SDL2-Utility) into your projects directory and use the make target `make build` to build a static library from the repositories source code, or alternatively use your own media utility of choice by reimplementing the locality source with separate graphical and input libraries.

Note that if you choose to use this repository, it also requires Data Components to work.

This static library provides implementation of a series of utilities necessary for the media capabilities of a software project. These include 
- Graphical Handler
- Font Wrapper
- Input Event System Handler
- Some File Utilities
- Basic Graphical Space Math Functions

If you wish to provide your own implementation in place of the SDL2 Wrapper utility, you must ensure to properly implement and replace all these components.

# Documentation

## Usage
### Setup
Assuming you have built the other three libraries and their containing directories are in your projects directory, you may use the provided make file and add your own source files to the `CFILES` variable. From there you may call the `compile` make target to compile your code into an executable, or alternatively the `debug` make target to compile your project with debug symbols for use in GDB, Valgrind, etc. 

Please note that all code was written and tested on GNU-C 11, however this project is written with compatibility in mind, and you should be able to use this for any C or C++ project, just keep in mind that you may want to keep within the GNU standard versions.

### Entry Point File
The Makefile has a variable `PROJFILE` which defines the entry point for your specific project. either create a file with the the current assigned value as the name ( the default should be  `project.c` ), or reassign the value of `PROJFILE` to whatever you want your entry point file to be. This entry point file is the link between all your personal code and Locality. You may put anything in this file and link it to anything else, the only requirement is that this file contains a definition of the function `void project();`
```
#include <stdio.h>
#include "Locality.h"
#include "LocGUI.h"

...

void project(){
	printf("Hello World\n");
}

...

```

This function is called during the initialization state of the progam, after all build in systems modules and components have been initialized and made ready for user code to interact with them.

## Data Components
The creation and usage of Data Components follows the documentation provided in the Entity Component System Documentation. Once a component has been created, it must be registered with the data management system in two ways. First you must update the variadic initialization function `ecsInit(uint32_t n, ...);`. The first argument represents the total number of component types which are to be registered, and the following arguments are the sizes in bytes of each individual component type. 
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

...

ecsInit(2, sizeof(v2), sizeof(image));
```

After this is done you must update the `COMPONENT_ID` enumerator with the new sequential id corrosponding with your new component type. Suppose you have implemented a component `struct text_content_l` as

```
typedef struct text_content_l{
	const char* content;
	uint8_t r, g, b, a;
}text_content_l;
```

You would then add an enumerator state to `COMPONENT_ID` as

```
typedef enum COMPONENT_ID{
	POSITION_C=0,
	BLITABLE_C=1,
	TEXT_CONTENT_C=2,	+
}COMPONENT_ID;
```

The value of the new enumerator state should follow natural sequential order. This id represents the bit position in a bit flag representing any given entities contents, so it must be unique. Also note the naming convention present. For a struct type component, use the suffix `_l` which stands for locality or locality component. For its enumerator reference use the suffix `_C` which stands for component.

This component can now be used in accordance with the data management system precicely how it is described in the Entity Component System documentation

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
	LOCALITY_STATE_RENDER,
	LOCALITY_STATE_RENDER_ABSOLUTE
}PROGRAM_STATE;
```

- LOCALITY_STATE_INIT runs at the very beginning of the program, and is meant as an initialization state
- LOCALITY_STATE_DEINIT runs at the very end of the program, and is meant as a memory cleanup state
- LOCALITY_STATE_UPDATE_PRE runs at the beginning of the logical update segment of the program, this runs every logical update tick
- LOCALITY_STATE_UPDATE is the primary logical update state of the program, this runs every logical update every tick
- LOCALITY_STATE_UPDATE_POST runs at the end of the logical update segment of the program, this runs every logical update tick
- LOCALITY_STATE_RENDER runs when the program is rendering all changes to the screen. Rendering in this state occurs relative to the windows view port.
- LOCALITY_STATE_RENDER_ABSOLUTE runs at the end of the programs render state. Rendering in this state occurs with absolute positioning to the window.

The rendering states implement usage of the `RENDER_RELATIVE` and `RENDER_ABSOLUTE` flags in the Entity Component System `ECS_DEFAULT_FLAGS` enumerator. Which means you can denote which one you want to render to with a system filter. All systems must be registered to one of these states.

### Registering Systems
Systems are registered to your project with `Project_registerSystem(struct System* sys, PROGRAM_STATE mode);`. Suppose you have a set of systems `pressUpdate` and `pressRender` defined as
```
System pressUpdate = SystemInit(pressabe_su, 2, POSITION_C, PRESSABLE_C);
System pressRender = SystemInit(pressabe_sr, 2, POSITION_C, PRESSABLE_C);
```

which respectively update and render data associated with a clickable component to a data entity. `pressUpdate` needs to be registered with `LOCALITY_STATE_UPDATE`. This is simple enough, as all we need to do is call
```
Project_registerSystem(&pressUpdate, LOCALITY_STATE_UPDATE);
```

`pressRender` needs to be registered with `LOCALITY_STATE_RENDER_ABSOLUTE` because we want it to render with absolute positioning so it remains at the same position on the screen no matter what state the system view port is in, however we dont want rendered in the standard `LOCALITY_STATE_RENDER` segment of the program to be possible. So before registering it with the project, we must add a filter for the other rendering state.
```
SystemAddFilter(&pressRender, RENDER_RELATIVE);

...

Project_registerSystem(&pressRender, LOCALITY_STATE_RENDER_ABSOLUTE);
```

Note the naming convention for the internal system functions, `_su` for logical update system and `_sr` for logical rendering system.

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
- `behavior_h` is a behavior component for explicit custom behavior. It contains a function pointer which takes a `uint32_t` entity id. It is not reccommended that you overuse this component, as its operation is naturally slower.

### Base Systems
- `void Blitable_sr(struct SysData* sys);`
This system requires a `POSITION_C` component and a `BLITABLE_C` component. It blits the image to the screen at the given position.

- `void forces_su(struct SysData* sys);`
This system requires a `POSITION_C` component and a `FORCES_C` component. Both are `v2` struct types. This System applies forces' x to position's x, and forces' y to position's y.

- User available function `void forces_applyForce(v2* left, float len, float dir);`
This function applies forces to an existing `v2` force.
- User available function `void forces_applyForceV2(v2* left, v2 force);`
This function is the Same as the previous one, but with `v2` input for the added force.

- User available function `void behavior_init(struct behavior_l* s, void(*f)(uint32_t));`
- User available function `void behavior_su(struct SysData* sys);`

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
- User available function`void pressable_destroy(uint32_t eid, uint32_t cid);`
- System `void pressable_su(struct SysData* sys);`
- System `void pressable_sr(struct SysData* sys);`

Both systems require a position to function as well.

**Component** `struct text_l` **ID:** `TEXT_C`

The text component denotes some text to be blitted to the screen. 

- User available function `void text_l_init(text_l* txt, const char* message, uint8_t r, uint8_t g, uint8_t b, uint8_t a);`
- User available function `void text_l_setColor(text_t* txt, uint8_t r, uint8_t g, uint8_t b, uint8_t a);`
- System `void text_sr(struct SysData* sys);`

The render system requires a `POSITION_C` component as well.
