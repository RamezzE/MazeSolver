SRC_DIR := src
OBJ_DIR := obj

MAIN_CPP 	  := $(wildcard $(SRC_DIR)/main.cpp)
MAIN_OBJ := $(MAIN_CPP:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

CPP_FILES       := $(filter-out $(MAIN_CPP), $(wildcard $(SRC_DIR)/*.cpp))
OBJ_FILES     := $(CPP_FILES:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

DEFINITON_FILE := $(wildcard $(SRC_DIR)/Definitions.hpp)

CC = g++

#DEBUG FLAGS:
# CFLAGS       := -std=c++17 -g -gdwarf-2 -fno-omit-frame-pointer

#RELEASE FLAGS:
CFLAGS       := -std=c++17

# your SFML folder path goes here, for example: C:/Downloads/SFML-2.5.1
SFML_PATH := SFML

SFML_INCLUDE_PATH := "$(SFML_PATH)/include"
SFML_LIBRARY_PATH := "$(SFML_PATH)/lib"

CPPFLAGS := -I$(SFML_INCLUDE_PATH) -DSFML_STATIC

LDFLAGS := -L$(SFML_LIBRARY_PATH) -static-libgcc -static-libstdc++ -Wl,-Bstatic -lstdc++ -lpthread -Wl,-Bdynamic

#DEBUG FLAGS:
# LDLIBS := -lsfml-graphics-s-d -lsfml-window-s-d -lsfml-system-s-d -lopengl32 -lwinmm -lgdi32 -lsfml-main-d -lfreetype

#RELEASE FLAGS
LDLIBS := -lsfml-graphics-s -lsfml-window-s -lsfml-system-s -lopengl32 -lwinmm -lgdi32 -lsfml-main -lfreetype -mwindows

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