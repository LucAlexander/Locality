CC=gcc
CCWIN=x86_64-w64-mingw32-gcc
CFLAGS=-lm
CDEBUGFLAGS=-Wall -g
CLIBSWIN=-I/usr/include -L/usr/lib/x86_64-linux-gnu/installed-tests/SDL2
CLIBS=-LEntity-Component-System -lEntityComponentSystem -LSDL2-Utility -lSoftUtils -lSDL2 -lSDL2main -lSDL2_ttf -lSDL2_image -LDataContainers -lDataContainers
CFILES=Locality.c Locality.h LocGUI.c LocGUI.h LocBaseSystems.c LocBaseSystems.h LocBaseComponents.h
PROJFILE=project.c projectComponents.h 
OUT=lp

release:
	$(CC) $(PROJFILE) $(CFILES) $(CLIBS) $(CFLAGS) -o $(OUT)

release-windows:
	$(CCWIN) $(PROJFILE) $(CFILES) $(CLIBSWIN) $(CLIBS) $(CLFAGS) -o $(out).exe

debug:
	$(CC) $(PROJFILE) $(CFILES) $(CLIBS) $(CFLAGS) $(CDEBUGFLAGS) -o $(OUT)

clean:
	rm $(OUT)

