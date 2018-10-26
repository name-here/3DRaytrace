# 3DRaycast
The goal of this project is to create a 3D raycaster that can render various objects with reflections, refraction, and shadows.  In other words, it should be able to generate an image of a scene containing a variety of objects made of "materials" with different properties.  Unfortunately, most of these goals have not yet been achieved.

### Progress
Currently, the program is able to render flat shaded spheres and planes with simplified direct lighting, and reflective planes.  Unfortunately, all planes are restricted to being aligned with one of the three axes.  A TODO list with features to add, bugs to fix, and things to finish can be found at the beginning of 3DRaycast.cpp.  

### Compiling
To compile, simply ```cd``` into directory containing the project files and run ```make```.  This will create the executable "3DRaycast".  To start the program, just run ```./3DRaycast```.

Note: You must have SDL2 installed to compile and run the program.