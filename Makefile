SRC_DIR := src
OBJ_DIR := obj

MAIN_CPP 	  := $(wildcard $(SRC_DIR)/main.cpp)
MAIN_OBJ := $(MAIN_CPP:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

CPP_FILES       := $(filter-out $(MAIN_CPP), $(wildcard $(SRC_DIR)/*.cpp))
OBJ_FILES     := $(CPP_FILES:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

DEFINITON_FILE := $(wildcard $(SRC_DIR)/Definitions.hpp)

CC = g++

CFLAGS       := -std=c++14 -g -gdwarf-2 -fno-omit-frame-pointer
# CFLAGS       := -std=c++14

CPPFLAGS  := -I"SFML\include" -DSFML_STATIC

LDFLAGS := -L"SFML\lib"

LDLIBS := -lsfml-graphics-s-d -lsfml-window-s-d -lsfml-system-s-d -lopengl32 -lwinmm -lgdi32 -lsfml-main-d -lfreetype
# LDLIBS := -lsfml-graphics-s -lsfml-window-s -lsfml-system-s -lopengl32 -lwinmm -lgdi32 -lsfml-main -lfreetype

TARGET := "myApp.exe"

.PHONY: all clean

all: build run

$(OBJ_FILES): $(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(SRC_DIR)/%.hpp $(DEFINITON_FILE) 
	$(CC) -c $< -o $@ $(CFLAGS) $(CPPFLAGS)

#handle main.cpp error (no .h file)
$(MAIN_OBJ): $(MAIN_CPP)
	$(CC) -c $< -o $@ $(CFLAGS) $(CPPFLAGS)

build: $(OBJ_DIR) link

$(OBJ_DIR):
	@test -d "$(OBJ_DIR)" || mkdir $(OBJ_DIR)

link: $(OBJ_FILES) $(MAIN_OBJ)
	$(CC) $^ -o $(TARGET) $(LDFLAGS) $(LDLIBS)

run:
	$(TARGET)

clean:
	del $(OBJ_DIR)\*.o
	del $(TARGET)