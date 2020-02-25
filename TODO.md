# 3DRaytrace TODO List(s)

### To Do:

<!-- - classes.cpp line 182: add position to ball shadow casting(??) -->
- Optimize getting rays for camera (maybe don't use sine/cosine)
- Optimize operator== on points if possible
- Optimize various functions in objects.cpp (cast functions) to use -=, +=, etc.
- Have all variables set by the object hit by the ray be set only for that object instead of being set by each successive object until it gets to the right one.
- Go through code and trim out unnecessary variables and stuff
- Make lists in World object private and create some other way of changing the properties of objects in them (add functions should return pointer/id)
- Find way of updating all relevant planeDist on window resize (difficult if multiple cameras)
- Add textured objects (texture system, textures maybe as part of generic Object?)
- Use square of distance to check closest to save sqrt() call
- Add multithreading (COMPLICATED)
- Don't include same inline functions in multiple files
- Add high DPI capability
- Move all global variables into main() or other functions
- Change Ray objects to store a point and a vector instead of two points (still usint Point objects for both?)
- Clean up includes (if necessary)
- Replace number/ID systems with pointers
- Replace simple square root color nonlinearization to something more correct (Account for screen gama or whatever, LIKELY COMPLICATED)
- Add complete list of dependencies to README
- Make Fresnel stuff actually physically accurate
- Fix having to press ESC twice to exit control on the web build
- Move (some) constants to a separate constants file
- Change CRay::finishCast(), CRay::lightColor, and CRay::addLight so that light can go over camera's max exposure and overexpose pixels (LIKELY COMPLICATED)
- Add HDR monitor support (MAYBE COMPLICATED)
- Go through entire code base and tweak formatting to be consistent
- Give objects textures
- Fix sparkles (random pixels or short lines of pixels lighting up) when looking up at bottom of reflective plane
- Use a specific variable to store intersection point on CRay instead of using its Ray
- Restructure all Object Cast() functions to be more efficient
- Optimize all calculations with Point objects (use special instructions that act on more than one number at once?)
- Make casting skip objects that are behind the direction of the cast and too small to possibly be intersected with (also exclude objects too far to the side?)
- Restructure so that only position and distance are calculated for each intersection with a particular ray, and then other properties are found after the closest intersection is known
- Change CRay's color property to us floatColor because it represents an ammount of light (which has no upper bound)
- Change doubles that should never be negative to be unsigned
- Add ambient light (eventually incl. ambient occlusion)
- Fix black line at the top of window with some pixel sizes (pixelSize in World::draw() )

### Partially Done:

- Properly clean up World objects on exit (I think I did it?)
- Optimize Plane::cast to not create new Point and Ray objects
- Fix issue of no shadows on line of x=0 (Maybe done?, probably not)
- Add cube object
- Optimize things like [variable] = Point() to set all of the values to 0 instead of constructing a new object, and replace similar constructions with Point.set() function  (also Camera.move)
- Change all instances of setting points by components to use Point.set() function
- Make used CRay(s) not part of main file (maybe put into Camera class)
- Make shadows more realistic (add indirect lighting, integrate into normal cast)
- Prevent casts from checking direct light from light sources behind object hit by ray (use set normal?)
- Optimize Triangle::cast(), especially with relation to calculations finding if and where ray hits plane (optimization could include precalculating more values/properties)
- Make more variables unsigned that should be

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
- Redefine operators and such to work with points(and rays?) (Operator overloading)
- Add shadows for plane.cast
- Transfer reflections(maybe more) to World.cast()
- Check if cstdint package is actually needed in world.cpp
- Avoid drawing spheres that are behind the camera
- Make window resizable
- Add reflections to all objects
- Link SDL2 into application (see "macosx" [hg.libsdl.org/SDL/file/default/docs/](http://hg.libsdl.org/SDL/file/default/docs/))  OR  add Xcode files to github
- Make new file for world and make camera part of world class
- Move Point object into separate file
- Fix black lines between some pixels when pixelSize > 1
- Add movement controls [for debug / testing]
- Solve Camera funcs(see start of draw()) not working without another func after it
- add INTERSECT_ERR functionality to planes
- Have planes return opposite normal vector for rays coming from back side (make planes double sided in terms of normals)
- Find the normal of a triangle when it is created
- Finish triangle.cast
- Make triangle.cast work for shadows
- Rewrite World::draw() and World::cast() to be less of a mess (try to make more readable, add comments)
- Rename or remove functions that are basically just wrappers for Point::dot()
- Fix frameRate showing too high when faster than minFrameTicks
- Make frameRate work if running at more than 1000fps
- Rename addColorAlpha in CRay::addColor to be more accurately descriptive

### Maybe Do:

- Separate alpha from color object (not needed for some uses of Color)
- Add and impliment/use ray.length
- Optimize cross and dot products
- Find way not to trust passed width and height values (bad values could allow for overflow and potential security vulnerability)
- Move Color to new file
- Move color setting into world.cast (??)
- make Vector object (separate from Point object?) to distinguish between uses
- Add player / first-person object(class) which does camera controls
- Add a Point2D class with most of the same functions as Point (rename Point to Point3D?)
- Use half-sphere version of brother's points on a sphere project (or at least the idea) to figure out how to randomly but evenly destribute directions to cast rays to calculate diffuse reflection and scattering
- Make objects first check normals (if applicable) to avoid checking objects that are not visible
- Move setting CRay color from its castColor back to a function in CRay rather than a function in World


