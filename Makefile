CC=gcc
CFLAGS=-lm
CDEBUGFLAGS=-Wall -g
CLIBS=-LDataContainers -lDataContainers -LSDL2-Utility -lSoftUtils -LEntity-Component-System -lEntityComponentSystem
CFILES=main.c
OUT=lrun
compile:
	$(CC) $(CFILES) $(CLIBS) $(CFLAGS) -o $(OUT)

debug:
	$(CC) $(CFILES) $(CLIBS) $(CFLAGS) $(CDEBUGFLAGS) -o $(OUT)
