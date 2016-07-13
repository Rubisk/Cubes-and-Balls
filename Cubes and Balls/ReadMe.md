# Cubes and Balls Project Overview

This file contains a summary of what you will find in each of the files that
make up this application.

The projects recommended build system is MSVC 2015, and support files for building are included.

Cubes and Balls.vcxproj

    This is the MSVC main project file, which will be used to build the application.

Cubes and Balls.vcxproj.filters

    MSVC project filter file. Read the MSVC documentation on project files to learn more:
    https://msdn.microsoft.com/en-us/library/2208a1f2.aspx

Cubes and Balls.cpp

    This is the application entry point.

src/

    This contains all the actual code running the game. There's a world state used to store the current state of the game,
    a renderer rendering world states, a physics engine updating the world state, and some support classes interacting
    with the user or file system.
    
res/

    Contains all assets for the game, such as textures/models etc.
