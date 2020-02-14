all: debug release

rund: debug
	./$(DEBUG_TARGET)

runr: release
	./$(RELEASE_TARGET)

debug: cleand parser compile_context main
	$(CC) $(INCLUDES) $(DEBUG_FLAGS) -o $(DEBUG_TARGET) $(OBJS)

release: cleanr parser compile_context main
	$(CC) $(INCLUDES) $(RELEASE_FLAGS) -o $(RELEASE_TARGET) $(OBJS)


clean: cleand cleanr

cleand:
	$(RM) $(DEBUG_TARGET)

cleanr:
	$(RM) $(RELEASE_TARGET)

parser:
	$(CC) $(INCLUDES) $(DEBUG_FLAGS) -c src/parser.cpp -o $(OBJ_PATH)parser.o

compile_context:
	$(CC) $(INCLUDES) $(DEBUG_FLAGS) -c src/compile_context.cpp -o $(OBJ_PATH)compile_context.o

main:
	$(CC) $(INCLUDES) $(DEBUG_FLAGS) -c src/main.cpp -o $(OBJ_PATH)main.o

CC = g++

OBJ_PATH = obj/

DEBUG_TARGET   = bin/debug/qi
RELEASE_TARGET = bin/release/qi

DEBUG_FLAGS = -ggdb3 -Wextra -Wno-pointer-arith -pthread -std=c++17
RELEASE_FLAGS = -Wextra -Wno-pointer-arith -pthread -std=c++17 -Ofast

INCLUDES = -I./includes/
OBJS = $(OBJ_PATH)parser.o $(OBJ_PATH)compile_context.o $(OBJ_PATH)main.o