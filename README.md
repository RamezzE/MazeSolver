# Maze Solver

Maze generator & solver using backtracking

Written in C++ and SFML

![Maze Solver Preview (Custom) (2)](https://github.com/RamezzE/MazeSolver/assets/117018553/d085fe87-25e9-4ca6-a95d-1dbff72bda11)

## Table of Contents

- [Features](#features)
- [Building](#building)
- [License](#license)

## Features

1. [Maze Generation](#maze-generation)
2. [Maze Solving](#solving-the-maze)
3. [Finding all shortest paths](#shortest-paths)
4. [Resizing](#resize-the-grid)
5. [Adjusting simulation speed & wall thickness](#adjust-speed-and-wall-thickness)
6. [Changing start and end positions](#change-start-and-end)
7. [Manually editing the maze](#editing-the-maze)
8. [Exporting as an Image](#export-as-png)

#### Maze Generation
Randomly generate a maze with initially only 1 solution as many times as you want.

![Generate Maze](https://github.com/RamezzE/MazeSolver/assets/117018553/70736759-a81c-477e-91d0-2b75ae6226f6)

#### Solving the Maze

Solve the maze, backtracking at the first solution found.

![Solve Maze](https://github.com/RamezzE/MazeSolver/assets/117018553/20e7a1a7-7b2b-422a-a341-fda032ba3899)

#### Shortest Paths

Find all the shortest paths in the maze and view them all with side arrows.

![Find Shortest Paths](https://github.com/RamezzE/MazeSolver/assets/117018553/a47ff963-a086-4b5d-b6a3-a6c40b479f82)

#### Resize the Grid

Resize the grid and regenerate the maze.

![Resize Grid](https://github.com/RamezzE/MazeSolver/assets/117018553/862d611b-1f15-41ed-a588-4e21aa3b13e7)

#### Adjust Speed and Wall Thickness

Pause (speed to 0), slow down or speed up the simulation speed at any time.
Increase or decrease the wall thickness.

![Change Speed and Wall Thickness](https://github.com/RamezzE/MazeSolver/assets/117018553/184d2d6c-7ddc-4a03-b3a7-991b0c93e10e)

#### Change Start and End
Change start & end positions for the maze.

![Change Start and End](https://github.com/RamezzE/MazeSolver/assets/117018553/79a24c1b-83c1-46c4-9edf-a364f5026fd2)

#### Editing the maze

In edit mode:

Left mouse click toggles the visibility of the top wall of any cell.

Right mouse click does the same for the right wall

![Edit Maze](https://github.com/RamezzE/MazeSolver/assets/117018553/ba40b93f-bdd6-4b58-b91e-9309bff536b4)

#### Export as PNG
Export the maze as an image in PNG format.

![Mon Jun 19 190400 2023](https://github.com/RamezzE/MazeSolver/assets/117018553/a701b76e-c15c-40ef-99f9-9f6db6091461)
![Mon Jun 19 190427 2023](https://github.com/RamezzE/MazeSolver/assets/117018553/d02d0c28-9f72-4678-96fe-52060e51361f)
![x](https://github.com/RamezzE/MazeSolver/assets/117018553/965935ee-efa2-4729-9d4c-475bf89ab6f1)

## Building
1- [GNU Make](#gnu-make-method-used)

2- [MS Build](#ms-build-visual-studio)

### Prerequisites

- SFML 2.5.1 or above www.sfml-dev.org
- g++ 4.8 or above 

#### Windows

- You can download the latest version of MinGW g++ from this link (https://www.msys2.org/)

#### Linux

- You can run this command on linux to install SFML

```
$ sudo apt-get install libsfml-dev
```

### GNU Make (Method used)

- Make sure that the SFML version you have is compativle with MinGW (ex: GCC 13.1.0 MinGW version)

- Install GNU Make on your device (https://www.gnu.org/software/make/)

- Clone this repository on your device

- Edit the Makefile in the project directory to replace 'SFML_PATH' with your SFML folder path
```
# your SFML folder path goes here instead of SFML, for example: C:/Downloads/SFML-2.5.1
SFML_PATH := SFML
```

- Open your terminal in your project directory where the Makefile exists

- Run Make in your terminal:

```
$ make clean
$ make
```

- You may need to add the MinGW bin folder to your PATH variable in the system environment variables

### MS Build (Visual Studio)

- Add the `</path/to/sfml/>/include` to your **additional C++ include directories**.
- Add `</path/to/sfml/>/lib` to your **additional library directories**.
- Add the SFML libraries used to your **additional dependencies** 

    eg: 
     ``
     sfml-graphics-d.lib;sfml-system-d.lib;sfml-window-d.lib;
     ``
  
   You can remove the ``-d`` if you do not need to debug

- Add the dlls in your SFML path/bin along with your executable

#### To build SFML statically 
- Add the following to your **preprocesser definitons**
  
  ``SFML_STATIC``
  
- Add the following libraries to your **additional dependencies**

  ``
  sfml-graphics-s-d.lib sfml-window-s-d.lib sfml-system-s-d.lib opengl32.lib winmm.lib gdi32.lib sfml-main-d.lib freetype.lib
  ``

- You can remove the ``-d`` if you do not need to debug
  
## License

This project is licensed under the [MIT License](LICENSE).
