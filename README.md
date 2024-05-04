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
## Prerequisites
- g++ 4.8 or above
- GNU Make
- SFML 2.5.1 or above [[www.sfml-dev.org](https://www.sfml-dev.org/download/sfml/2.5.1/)]

### Linux
- You can run these commands on linux to install g++, Make and SFML

```
$ sudo apt-get install g++
$ sudo apt-get install make
$ sudo apt-get install libsfml-dev
```

### Windows
- Install the proper SFML version from the website (MinGW version for GNU Make build or Visual C++ for MS Build)
- Unzip the SFML downloaded folder ( You can install 7zip to be able to unzip )

#### GNU Make
- Install GNU Make by installing [chocolatey](https://chocolatey.org/install#individual) package manager  and running this command from an elevated Command Prompt:
```
$ choco install make
```
- Install MinGW g++ [MinGW 32-bit](https://sourceforge.net/projects/mingw-w64/files/Toolchains%20targetting%20Win32/Personal%20Builds/mingw-builds/7.3.0/threads-posix/dwarf/i686-7.3.0-release-posix-dwarf-rt_v5-rev0.7z/download) or [MinGW 64-bit](https://sourceforge.net/projects/mingw-w64/files/Toolchains%20targetting%20Win64/Personal%20Builds/mingw-builds/7.3.0/threads-posix/seh/x86_64-7.3.0-release-posix-seh-rt_v5-rev0.7z/download) (The SFML version and the MinGW version have to match)

- After unzipping the MinGW file, add your path to mingw32/bin or mingw64/bin to PATH in the system environmental variables

- Edit the ```SFML_DIR``` in the Makefile to add your correct SFML folder or copy the folder to the project directory and name the folder 'SFML'

#### MS Build
- Visual Studio 15 or above
- VC++ 15 or above


## Building

### GNU Make (Method used)

- Download source code

- Open your terminal in your project directory where the Makefile exists

- Run Make in your terminal:

```
$ make
```

- List of make commands you can use
```
# Build and run in release mode
$ make

# Build and run in debug mode
$ make debug && make run

# Rebuild in release or debug mode
$ make rebuild

# or 
$ make rebuild-debug

# Clean the files
$ make clean

# Run the latest compiled build
$ make run
```

### MS Build

1. Add the `</path/to/sfml_folder/>/include` to your **additional C++ include directories**.
2. Add `</path/to/sfml_folder/>/lib` to your **additional library directories**.
3. Add the SFML libraries used to your **additional dependencies**

   eg:
   ``
   sfml-graphics.lib;sfml-system.lib;sfml-window.lib;
   ``

4. Add the dlls in your SFML path/bin along with your executable
  
## License

This project is licensed under the [MIT License](LICENSE).
