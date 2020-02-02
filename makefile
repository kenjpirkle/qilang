all: debug release

debug:
	$(CC) $(INCLUDES) $(DEBUG_FLAGS) -o $(DEBUG_TARGET) src/main.cpp

release:
	$(CC) $(INCLUDES) $(RELEASE_FLAGS) -o $(RELEASE_TARGET) src/main.cpp

rund:
	./$(DEBUG_TARGET)

runr:
	./S(RELEASE_TARGET)

clean:
	$(RM) $(DEBUG_TARGET)
	$(RM) $(RELEASE_TARGET)

CC = g++

DEBUG_TARGET   = bin/debug/qi
RELEASE_TARGET = bin/release/qi

DEBUG_FLAGS = -ggdb3 -Wextra -pthread -std=c++2a
RELEASE_FLAGS = -Wextra -pthread -std=c++2a -Wno-switch -Wno-maybe-uninitialized -march=native -Ofast

INCLUDES = -I./header/