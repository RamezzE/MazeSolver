CPP_DIR     := src/cpp
HPP_DIR     := src/hpp
OBJ_DIR     := obj
BIN_DIR     := bin

ifeq ($(OS),Windows_NT)
	SFML_DIR    := SFML
endif

MAIN_CPP    := main.cpp
MAIN_OBJ    := $(OBJ_DIR)/main.o

CPP_FILES   := $(wildcard $(CPP_DIR)/*.cpp)
OBJ_FILES   := $(CPP_FILES:$(CPP_DIR)/%.cpp=$(OBJ_DIR)/%.o)

DEFINITON_FILE := $(wildcard $(HPP_DIR)/Definitions.hpp)

CC          := g++
CFLAGS      := -std=c++17
CFLAGS-D    := -std=c++17 -g -gdwarf-2 -fno-omit-frame-pointer


ifeq ($(OS),Windows_NT)
    # Windows-specific settings
    CPPFLAGS    := -I"$(SFML_DIR)\include"
    LDFLAGS     := -L"$(SFML_DIR)\lib"
    LDLIBS      := -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -mwindows
    LDLIBS-D    := -lsfml-graphics-d -lsfml-window-d -lsfml-system-d -lsfml-audio-d
    TARGET      := "MazeSolver.exe"
    RM          := del
    SLASH       := \\
    CP          := xcopy /s /i
    DLLS_DIR    := $(SFML_DIR)\bin
    MKDIR       := mkdir
else
    # Linux-specific settings
    CPPFLAGS    := -I/usr/include/SFML
    LDFLAGS     := -L/usr/lib
    TARGET      := "MazeSolver"
    LDLIBS      := -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
    LDLIBS-D    := -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
    RM          := rm -f
    SLASH       := /
    CP          := cp -r
    MKDIR       := mkdir -p
endif
ONY: all clean debug

all: build run

debug: CFLAGS := $(CFLAGS-D)

.PHbug: LDLIBS := $(LDLIBS-D)
debug: build

$(OBJ_FILES): $(OBJ_DIR)/%.o: $(CPP_DIR)/%.cpp $(HPP_DIR)/%.hpp $(DEFINITON_FILE)
	$(CC) -c $< -o $@ $(CFLAGS) $(CPPFLAGS)

$(MAIN_OBJ): $(MAIN_CPP)
	$(CC) -c $< -o $@ $(CFLAGS) $(CPPFLAGS)

build: $(OBJ_DIR) $(BIN_DIR) link symlink-assets copy-dlls

rebuild: clean build
rebuild-debug: clean debug

$(OBJ_DIR):
	@test -d "$(OBJ_DIR)" || $(MKDIR) $(OBJ_DIR)

$(BIN_DIR):
	@test -d "$(BIN_DIR)" || $(MKDIR) $(BIN_DIR)

link: $(OBJ_FILES) $(MAIN_OBJ)
	$(CC) $^ -o $(BIN_DIR)/$(TARGET) $(LDFLAGS) $(LDLIBS)

symlink-assets: $(BIN_DIR)

symlink-assets: $(BIN_DIR)
ifeq ($(OS),Windows_NT)
	@if not exist $(subst /,\,$(BIN_DIR)\assets) @powershell -Command "New-Item -ItemType Junction -Path '$(subst /,\,$(BIN_DIR)\assets)' -Target '$(subst /,\,$(CURDIR)\assets)'"
else
	@if [ ! -e $(BIN_DIR)/assets ]; then ln -s $(CURDIR)/assets $(BIN_DIR)/assets; fi
endif



copy-dlls: $(BIN_DIR)
ifeq ($(OS),Windows_NT)
	@for %%I in ($(SFML_DIR)\bin\*.dll) do \
		if not exist $(BIN_DIR)\%%~nxI $(CP) "%%I" $(BIN_DIR)
endif


run:
ifeq ($(OS),Windows_NT)
	cd $(BIN_DIR) && $(TARGET)
else
	cd $(BIN_DIR) && ./$(TARGET)
endif

clean:
	$(RM) $(OBJ_DIR)$(SLASH)*.o
	$(RM) $(BIN_DIR)$(SLASH)$(TARGET)