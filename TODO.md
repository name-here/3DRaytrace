TODO:

## |𝔻𝕠𝕟𝕖𝕟𝕖𝕤𝕤|

# |Maybe Do|
-Add and implement/use "ray.length". 
	
-Separate alpha from color object (not needed for some uses).                      
	
# |StilToDo|

[classes.cpp](classes.cpp) line 182: add position to ball shadow casting(??).
	
-Find the normal of a triangle when it is created.
	
-Finish triangle.cast.
	
-Make triangle.cast work for shadows.
	
-Find the required vectors for the camera when it is created.
	
-Properly clean up World objects on exit.
	
-Make shadows more realistic (add indirect lighting, integrate into normal cast).
	
-Fix issue of no shadows on line of x=0.


# |_-_-_-_-|
	
-Optimize getting rays for camera (maybe don't use sine/cosine).
	
- 
	
-
	

# |PartDone|
-Add shadows for plane.cast.                                                         
	
-Add reflections to all objects.
	
-Redefine operators and such to work with points (and rays?)  (Object overloading).
	
-Avoid drawing spheres that are behind the camera.

	
# |  Done  |
	
-Finish camera.rotate and camera.getRay to make them actually work with rotation.
	
-Fix color issues caused by non-linear association of RGB values and brightness.
	
-Make use of Color objects universal.
	
-Fix shadows!
	
-Make use of Point object universal.
	
-Restructure so that world object contains all objects to be cast.
	
-Figure out mouseX and mouseY polarity.
	
-Make the plane cast actually get what axis it is on from the variable.
