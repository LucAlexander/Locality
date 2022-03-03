CC=gcc
CFLAGS=-lm
CDEBUGFLAGS=-Wall -g
CLIBS=-LEntity-Component-System -lEntityComponentSystem -LSDL2-Utility -lSoftUtils -lSDL2 -lSDL2main -lSDL2_ttf -lSDL2_image -LDataContainers -lDataContainers
CFILES=Locality.c Locality.h LocGUI.c LocGUI.h LocBaseSystems.c LocBaseSystems.h
OUT=lrun

compile:
	$(CC) $(CFILES) $(CLIBS) $(CFLAGS) -o $(OUT)

debug:
	$(CC) $(CFILES) $(CLIBS) $(CFLAGS) $(CDEBUGFLAGS) -o $(OUT)

clean:
	rm $(OUT)
