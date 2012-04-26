DEFINES = -g
LIBS = $(shell sdl-config --libs)
CXXFLAGS = $(shell sdl-config --cflags) --std=c99
CC = gcc
INC =
LD = gcc
LDFLAGS = $(LIBS)
SRC = pukis_pc.c
OBJ = $(SRC:.c=.o)
EXE = pukiscontrol


All: $(SRC) $(EXE)

$(EXE): $(OBJ)
	$(CC) -o $@ $(OBJ) $(LDFLAGS) 

.c.o:
	$(CC) -c $< $(CXXFLAGS) -o $@


clean:
	rm -f $(EXE) *.o

