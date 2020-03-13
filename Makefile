CC = gcc
DBG = no
EXEC = csg
SRC = src/
INCLUDE = include/

ifeq ($(DBG),yes)
  CFLAGS = -I./$(INCLUDE) -g -Wpointer-arith -Wall -ansi
else
  CFLAGS = -I./$(INCLUDE) -O2 -ansi -DNDEBUG
endif

LDFLAGS = -lm -lGL -lGLU -lglut

all: $(EXEC) clean

csg.o: types.o point_cloud.o tree.o parser.o

point_cloud.o: types.o

shape.o: types.o point_cloud.o

tree.o: types.o point_cloud.o shape.o

parser.o: tree.o

$(EXEC): types.o point_cloud.o shape.o tree.o parser.o csg.o

%.o: $(SRC)%.c
	$(CC) $(CFLAGS) -c $< -o $@

%: %.o
	$(CC) $^ $(LDFLAGS) -o $@

clean: 
	@rm -f *.o

mrproper: clean
	@rm -f *.o $(EXEC)
	
