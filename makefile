all: debug release

rund: debug
	./$(DEBUG_TARGET)

runr: release
	./$(RELEASE_TARGET)

debug: cleand
	$(CC) $(INCLUDES) $(DEBUG_FLAGS) -o $(DEBUG_TARGET) src/main.cpp

release: cleanr
	$(CC) $(INCLUDES) $(RELEASE_FLAGS) -o $(RELEASE_TARGET) src/main.cpp


clean: cleand cleanr

cleand:
	$(RM) $(DEBUG_TARGET)

cleanr:
	$(RM) $(RELEASE_TARGET)

CC = g++

DEBUG_TARGET   = bin/debug/qi
RELEASE_TARGET = bin/release/qi

DEBUG_FLAGS = -ggdb3 -Wextra -Wno-pointer-arith -pthread -std=c++17
RELEASE_FLAGS = -Wextra -Wno-pointer-arith -pthread -std=c++17 -Ofast

INCLUDES = -I./includes/