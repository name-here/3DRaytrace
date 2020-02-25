# 3DRaytrace
The goal of this project is to create a 3D raytracer that can render various objects with reflections, refraction, and shadows.  In other words, it should be able to generate an image of a scene containing a variety of objects made of "materials" with different properties.

### Progress
Currently, the program is able to render flat shaded and reflective spheres and planes with direct lighting and some reflection effects.  For now, all planes are confined to being aligned with one of the three axes.  A TODO list with features to add, bugs to fix, and things to finish can be found in [TODO.md](http://github.com/name-here/3DRaytrace/blob/master/TODO.md).  

### Problems Solved
- Calculating all of the rays for a camera with position and rotation
- Finding the intersection between a 3D ray and planes or spheres
- Calculating surface normals, and calculating a reflected ray based on that normal
- Determining how much light from a light source will actually reach the camera (distance, angle), and the beginnings of caluclating the brightness of reflections
- Color mixing

### How to Use
Once built, just run the executable or load up the webpage.  Click on the window to start controlling the camera.  In this mode, move the mouse to look around, and use the W, S, A, and D keys to move the camera forward, backward, left, and right respectively.  Additionally, the SHIFT keys will move the camera vertically down, and the SPACE key will move the camera vertically up.  Also, clicking in control mode will save a screenshot as a .bmp if the SAVE_IMAGE_ON_CLICK flag is set, or print the framerate to the console otherwise.  Press the ESCAPE (ESC) key to exit camera control mode.  For the web build, you must press ESCAPE twice to fully exit camera control mode.  Hold down the Q key to render in full resolution (much slower)

### Notes
- Currently, there may be bugs if there are more objects loaded than the max int value, as each object has an ID, which is stored as an int.

### Compiling

#### Web
To compile for web on either Linux or Mac, you must first install Emscripten, instructions for which can be found [here](https://emscripten.org/docs/getting_started/downloads.html#sdk-download-and-install).  Once you've done that, just ```cd``` into the directory containing the project files and run ```make jsbin/asm.js```.  If you just want a viewable webpage to look at it without setting up any html, run ```make jsbin/3DRaytrace.html```.  Unfortunately, for some web security reasons, in most browsers, the page can only be viewed when running from a server.  Emscripten's website has some information on that [here](https://emscripten.org/docs/getting_started/Tutorial.html#generating-html).

#### Linux
To compile for linux, simply ```cd``` into directory containing the project files and run ```make bin/3DRaytrace```.  This will create the executable "3DRaytrace".  To start the program, just run the executable with ```./bin/3DRaytrace```.

Note: You must have the development version of SDL2(download [here](http://www.libsdl.org/download-2.0.php)) installed to compile the program, and either the development or runtime version installed to run it.  Also, I have only been testing command line builds on a Mac.

#### Mac OS
For Mac OS, compile using the Xcode project in the App folder.

#### Windows
I have not tested compiling for Windows yet, so you're on your own.