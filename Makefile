CPP = g++
CPP_JS = emcc
CPPFLAGS = -g -O3 -Wall -Wextra -pedantic -std=c++11 -pthread
CPPFLAGS_JS = -s WASM=1 -s USE_SDL=2 -g -O3 -Wall -Wextra -pedantic -std=c++11 -pthread -DWASM
PKGFLAGS = `sdl2-config --cflags` `sdl2-config --libs`
PKGFLAGS_ALT = -ISDL2_DEV/SDL2.framework/Headers -FSDL2_DEV -framework SDL2  #SDL2 runtime (or development) framework must be present in /Library/Frameworks or [your user folder]/Library/Frameworks for executable to run (on mac)
DEPS = point.h camera.h color.h rays.h world.h objects.h
OBJ_NAMES = point.o camera.o color.o rays.o world.o objects.o main.o

OBJ = $(patsubst %,bin/%,$(OBJ_NAMES))
OBJ_JS = $(patsubst %,jsbin/%,$(OBJ_NAMES))

TARGETS = bin/3DRaytrace jsbin/asm.js

.PHONY: all
all: $(TARGETS)

jsbin/asm.js: $(OBJ_JS)
	@mkdir -p jsbin
	$(CPP_JS) -o $@ $^ $(CPPFLAGS_JS)

jsbin/3DRaytrace.html: $(OBJ_JS)
	@mkdir -p jsbin
	$(CPP_JS) -o $@ $^ $(CPPFLAGS_JS)

jsbin/%.o: %.cpp $(DEPS)
	@mkdir -p jsbin
	$(CPP_JS) -c -o $@ $< $(CPPFLAGS_JS)

bin/3DRaytrace: $(OBJ)
	@mkdir -p bin
	$(CPP) -o $@ $^ $(CPPFLAGS) $(PKGFLAGS)  #Change PKGFLAGS to PKGFLAGS_ALT to compile on a mac

bin/%.o: %.cpp $(DEPS)
	@mkdir -p bin
	$(CPP) -c -o $@ $< $(CPPFLAGS)

.PHONY: clean
clean:
	rm -rf bin jsbin