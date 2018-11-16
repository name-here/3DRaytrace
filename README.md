# 3DRaycast
The goal of this project is to create a 3D raycaster that can render various objects with reflections, refraction, and shadows.  In other words, it should be able to generate an image of a scene containing a variety of objects made of "materials" with different properties.  Unfortunately, most of these goals have not yet been achieved.

### Progress
Currently, the program is able to render flat shaded spheres and planes with simplified direct lighting, and reflective planes.  Unfortunately, all planes are restricted to being aligned with one of the three axes.  A TODO list with features to add, bugs to fix, and things to finish can be found in "[TODO.md](http://github.com/name-here/3DRaycast/blob/master/TODO.md)".  

### Compiling
To compile, simply ```cd``` into directory containing the project files and run ```make```.  This will create the executable "3DRaycast".  To start the program, just run the executable with ```./3DRaycast```.

Note: You must have the development version of SDL2 (download [here](http://www.libsdl.org/download-2.0.php))installed to compile the program, and either the development or runtime version installed to run it.  As a temporary solution for Mac, the executable must be in either /Library/Frameworks (needed for compiling) or ~/Library/Frameworks, ```$(PKGFLAGS)``` on line 8 must be deleted, and ``` `sdl2-config --cflags` ``` on line 2 must be replaced with ```-I/Library/Frameworks/SDL2.framework/Headers -F/Library/Frameworks -framework SDL2```.