all: debug release

rund: $(DEBUG_TARGET)
	./$(DEBUG_TARGET)

runr: $(RELEASE_TARGET)
	./$(RELEASE_TARGET)

debug: cleand
	$(CC) $(INCLUDES) $(DEBUG_FLAGS) -o $(DEBUG_TARGET) $(SRC_FILES)

release: cleanr
	$(CC) $(INCLUDES) $(RELEASE_FLAGS) -o $(RELEASE_TARGET) $(SRC_FILES)


clean: cleand cleanr

cleand:
	$(RM) $(DEBUG_TARGET)

cleanr:
	$(RM) $(RELEASE_TARGET)

CC = g++

DEBUG_TARGET   = bin/debug/tests
RELEASE_TARGET = bin/release/tests

DEBUG_FLAGS = -ggdb3 -Wextra -Wno-pointer-arith -pthread -std=c++17
RELEASE_FLAGS = -Wextra -Wno-pointer-arith -pthread -std=c++17 -Ofast

INCLUDES = -I../includes/

SRC_DIR = src/
SRC_FILES = $(SRC_DIR)f_string_test.cpp $(SRC_DIR)mono_allocator_test.cpp $(SRC_DIR)bucket_array_test.cpp $(SRC_DIR)main.cpp