# 3DRaycast TODO List(s)

### To Do:

<!-- - classes.cpp line 182: add position to ball shadow casting(??) -->
- Find the normal of a triangle when it is created
- Finish triangle.cast
- Make triangle.cast work for shadows
- Properly clean up World objects on exit
- Make shadows more realistic (add indirect lighting, integrate into normal cast)
- Fix issue of no shadows on line of x=0
- Optimize getting rays for camera (maybe don't use sine/cosine)
- Transfer reflections(maybe more) to World.cast()
- Solve Camera funcs(see start of draw()) not working without another func after it
- Make window resizable
- Make used CRay(s) not part of main file (maybe put into Camera class)
- Link SDL2 into application (see "macosx" [hg.libsdl.org/SDL/file/default/docs/](http://hg.libsdl.org/SDL/file/default/docs/))
- Optomize operator== on points if possible
- Move color setting into world.cast

### Partially Done:

- Avoid drawing spheres that are behind the camera
- Redefine operators and such to work with points(and rays?) (Operator overloading)
- Add reflections to all objects
- Add shadows for plane.cast
- Make new file for world and make camera part of world class

### Done:

- Make the plane cast actually get what axis it is on from the variable
- Figure out mouseX and mouseY polarity
- Restructure so that world object contains all objects to be cast
- Make use of Point object universal
- Fix shadows!
- Make use of Color objects universal
- Fix color issues caused by non-linear association of RGB values and brightness
- Finish camera.rotate and camera.getRay to make them actually work with rotation
- Find the required vectors for the camera when it is created
- Move this TODO list to a separate file

### Maybe Do:

- Separate alpha from color object (not needed for some uses of Color)
- Add and impliment/use ray.length


