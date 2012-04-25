DEFINES = -g
LIBS = $(shell sdl-config --libs)
CXXFLAGS = $(shell sdl-config --cflags)
CXX = gcc
INC =
LD = gcc
LDFLAGS = $(LIBS)
SRC = $(*.c)
OBJ = $(SRC:.c=.o)
EXE = pukis_pc


All: $(EXE)

atoll: $(SRC) $EXE
	$(CXX) $(CXXFLAGS) -o $@ $(LDFLAGS) $(OBJ)

%.o:%.cpp
	$(CXX) -c $< $(CXXFLAGS)


clean:
	rm -f $(EXE) *.o

