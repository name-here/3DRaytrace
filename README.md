# 3DRaycast
A 3D Raycasting Engine (fork of https://github.com/name-here/3DRaycast).


## TODO
* [] Work on [TODO](TODO.md).
* [] Find what other dependencies are necessary. (We have SDL2 on another mac, so why won't 3DRaycast compile there?)


## How to install
* Need SDL2 library. [Download either runtime or development SDL2](https://www.libsdl.org/download-2.0.php) and put it into `/Library/Frameworks`. (Installer says it will work in `~/Library/Frameworks`, too. We didn't have such a folder on our osX 10.13.6 "standard" non-admin account, so we made one.)


## How to compile and run:
* ```cd``` to code's directory, type ```make``` into terminal. Stand back!
 Using ```make``` unfortunately is giving us error ```sdl2-config: command not found``` followed by a (probably related) fatal error re header not found. (Here's the [Makefile](Makefile).) We're getting this error when SDL2.framework is in `/Library/Frameworks` and when SDL2.framework is instead in `~/Library/Frameworks`.
```
    g++ -c -o 3DRaycast.o 3DRaycast.cpp -g -Og -Wall -Wextra -pedantic -std=c++11 `sdl2-config --cflags`
    /bin/sh: sdl2-config: command not found
    3DRaycast.cpp:34:10: fatal error: 'SDL2/SDL.h' file not found
    #include <SDL2/SDL.h>
```
* ```./3DRaycast```


## References
* [SDL library](https://wiki.libsdl.org/) including ["how to compile"](https://wiki.libsdl.org/Installation). 
>Note: "Simple DirectMedia Layer is a cross-platform development library designed to provide low level access to audio, keyboard, mouse, joystick, and graphics hardware via OpenGL and Direct3D."
* [Tutorials for getting started with OpenSceneGraph](	http://www.openscenegraph.org/index.php/documentation/getting-started). 
>"Though these are OpenSceneGraph centric, the same exact concepts apply to	SDL... You can skim over the PlugIns stuff since SDLdoesn't have any PlugIns to worry about."
* Testing, see ["closed"](https://stackoverflow.com/questions/91384/unit-testing-for-c-code-tools-and-methodology) and and ["not constructive"](https://stackoverflow.com/questions/242926/comparison-of-c-unit-test-frameworks)
* Re-factoring (see Malcolm Fowler)
* [Markdown](https://daringfireball.net/projects/markdown/) text, 
[Github Flavored Markdown](http://github.github.com/gfm) 
and [live preview of gfm](http://jbt.github.io/markdown-editor).
Also see [Editing in Github in general](https://help.github.com/articles/about-writing-and-formatting-on-github/), 
and [how to create/highlight code blocks](https://help.github.com/articles/creating-and-highlighting-code-blocks/).


## Related Articles
* [desc](uri)
