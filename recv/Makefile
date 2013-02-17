DEFINES = -g
LIBS = -lusb -lusb-1.0
CXXFLAGS = -lusb
CC = gcc
INC =
LD = gcc
LDFLAGS = $(LIBS)
SRC = recv.c
OBJ = $(SRC:.c=.o)
EXE = recv

All: $(SRC) $(EXE)

$(EXE): $(OBJ)
	$(CC) -o $@ $(OBJ) $(LDFLAGS) 

.c.o:
	$(CC) -c $< $(CXXFLAGS) -o $@


clean:
	rm -f $(EXE) *.o

