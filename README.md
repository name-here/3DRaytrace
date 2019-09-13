# 3DRaytrace
The goal of this project is to create a 3D raytracer that can render various objects with reflections, refraction, and shadows.  In other words, it should be able to generate an image of a scene containing a variety of objects made of "materials" with different properties.  Unfortunately, most of these goals have not yet been achieved.

### Progress
Currently, the program is able to render flat shaded and reflective spheres and planes with simplified direct lighting.  Unfortunately, all planes are restricted to being aligned with one of the three axes.  A TODO list with features to add, bugs to fix, and things to finish can be found in "[TODO.md](http://github.com/name-here/3DRaytrace/blob/master/TODO.md)".  

### Problems Solved
- Calculating all of the rays for a camera with position and rotation
- Finding the intersection between a 3D ray and planes or spheres
- Calculating surface normals, and calculating a reflected ray based on that normal
- Color mixing

### Notes
- Currently, there may be bugs if there are more objects loaded than the max int value, as each object has an ID, which is stored as an int.

### Compiling

#### Web
To compile for web on either Linux or Mac, you must first install Emscripten, instructions for which can be found [here](https://emscripten.org/docs/getting_started/downloads.html#sdk-download-and-install).  Once you've done that, just ```cd``` into the directory containing the project files and run ```make jsbin/asm.js```.  If you just want a viewable webpage to look at it without setting up any html, run ```make jsbin/3DRaytrace.html```.  Unfortunately, for some web security reasons, in most browsers, the page can only be viewed when running from a server.  Emscripten's website has some information on that [here](https://emscripten.org/docs/getting_started/Tutorial.html#generating-html).

#### Linux
To compile for linux, simply ```cd``` into directory containing the project files and run ```make bin/3DRaytrace```.  This will create the executable "3DRaytrace".  To start the program, just run the executable with ```./bin/3DRaytrace```.

Note: You must have the development version of SDL2(download [here](http://www.libsdl.org/download-2.0.php)) installed to compile the program, and either the development or runtime version installed to run it.  Also, I have been testing on a Mac.

#### Mac OS
For Mac OS, compile using the Xcode project in the App folder.

#### Windows
I have not tested compiling for Windows yet, so you're on your own.