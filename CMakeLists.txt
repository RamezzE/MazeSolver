cmake_minimum_required(VERSION 3.27)
project(MazeSolver LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(SOURCE_DIR ${CMAKE_SOURCE_DIR})

set(DESTINATION_DIR ${CMAKE_BINARY_DIR}/bin)

set(FOLDERS_TO_LINK assets/fonts assets/imgs "Exported Maze Images")

if (WIN32)
    foreach(FOLDER ${FOLDERS_TO_LINK})
        execute_process(
                COMMAND ${CMAKE_COMMAND} -E copy_directory ${SOURCE_DIR}/${FOLDER} ${DESTINATION_DIR}/${FOLDER}
                COMMAND ${CMAKE_COMMAND} -E env cmd /c mklink /J ${DESTINATION_DIR}/${FOLDER} ${SOURCE_DIR}/${FOLDER}
                COMMENT "Creating directory junction to ${FOLDER} folder"
        )
    endforeach()
else()
    # For non-Windows platforms, use symbolic links
    foreach(FOLDER ${FOLDERS_TO_LINK})
        execute_process(
                COMMAND ${CMAKE_COMMAND} -E create_symlink ${SOURCE_DIR}/${FOLDER} ${DESTINATION_DIR}/${FOLDER}
                COMMENT "Creating symbolic link to ${FOLDER} folder"
        )
    endforeach()
endif()

option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

include(FetchContent)
FetchContent_Declare(SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)

add_executable(MazeSolver
        main.cpp

        src/cpp/Button.cpp

        src/cpp/CheckBox.cpp

        src/cpp/ExportImageScreen.cpp

        src/cpp/Game.cpp

        src/cpp/GameScreen.cpp

        src/cpp/Maze.cpp

        src/cpp/Slider.cpp

        src/cpp/TextBox.cpp

        src/cpp/SpriteButton.cpp

        src/cpp/Tile.cpp
        
)
target_link_libraries(MazeSolver PRIVATE sfml-graphics sfml-audio)
target_compile_features(MazeSolver PRIVATE cxx_std_17)

if (WIN32)
    # Copy OpenAL DLL
    add_custom_command(
            TARGET MazeSolver
            COMMENT "Copy OpenAL DLL"
            PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:MazeSolver>
            VERBATIM)
endif ()

install(TARGETS MazeSolver)
