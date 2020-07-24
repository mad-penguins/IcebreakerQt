# Icebreaker (Qt)

:ice_cube::ship:Qt is an API wrapper for Antarctica :antarctica: implemented in C++ with Qt framework.
You can use it for building your own client for Antarctica. (You can also help us to make a binding for another another language, there's a lot of instructions how to bind C++ code to Python, Golang, etc.)

Our wiki contains the best [reference](https://github.com/mad-penguins/IcebreakerQt/wiki/) for this project (only because it's the only one).

Our [official client](https://github.com/mad-penguins/Antarctica) uses this wrapper as a git submodule, so this project is officially maintainable.

## Build
Icebreaker is build with Qt framework and based on CMake build system.
So, you need to install cmake binary, C++ compiler with C++17 standard support and Qt5 runtime and development packages.

##### Needed packages
1. `git`
2. `cmake`
3. `clang` or `g++`(Debian-based) / `gcc-g++`(openSUSE)
4.  Qt Packages (or just install Qt from official website and specify it to CMake with `-DCMAKE_PREFIX_PATH` flag)
    - Ubuntu
        - `libqt5core5a`
        - `libqt5widgets5`
        - `libqt5network5`
    - openSUSE
        - `libQt5Core-devel`
        - `libQt5Widgets-devel`
        - `libQt5Network-devel`

##### Using in your project & build process:
1. `$ cd into your project`
2. ```bash
   $ git submodule add https://github.com/mad-penguins/IcebreakerQt.git
   $ git add -A
   $ git commit -m "Add Icebreaker as a submodule"
   ```
3. Make your CMakeLists.txt in project root look like this:
   ```cmake
   cmake_minimum_required(VERSION 3.13)
   project(yourProjectName)

   set(CMAKE_CXX_STANDARD 17)  # C++17 standard is strongly required

   set(SOURCE_FILES main.cpp ...)
   include_directories(${CMAKE_CURRENT_SOURCE_DIR})

   find_package(Qt5Core REQUIRED)
   find_package(Qt5Widgets REQUIRED)
   find_package(Qt5Network REQUIRED)

   add_subdirectory(api)  # include Icebreaker into your project

   set(CMAKE_AUTOMOC ON)
   set(CMAKE_AUTORCC ON)

   add_executable(yourProjectName ${SOURCE_FILES} resources/resources.qrc)
   target_link_libraries(yourProjectName icebreaker Qt5::Core Qt5::Widgets Qt5::Network)  # link the Icebreaker to your binary 
   ```
4. `$ mkdir build && cd build`
5. `$ cmake .. && make` (you can specify number of cores used for compilation with flag `-j`, e.g. `-j 4`)

##### Troubleshooting
If step 2 won't  work for you, you can clone API wrapper repository manually and put it to the `api` directory:

`$ git clone https://github.com/mad-penguins/IcebreakerQt && mv IcebreakerQt/ api/`


Happy coding and we hope you will help make Linux :penguin: user experience much more friendly :smile:
