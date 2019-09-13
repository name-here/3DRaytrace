CPP = g++
CPP_JS = emcc
CPPFLAGS = -g -O3 -Wall -Wextra -pedantic -std=c++11 `sdl2-config --cflags`
CPPFLAGS_JS = -s WASM=1 -s USE_SDL=2 -g -O3 -Wall -Wextra -pedantic -std=c++11 -DWASM
PKGFLAGS = `sdl2-config --libs`
DEPS = point.h camera.h rays.h world.h objects.h
OBJ_NAMES = point.o camera.o rays.o world.o objects.o main.o

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
	$(CPP) -o $@ $^ $(CPPFLAGS) $(PKGFLAGS)

bin/%.o: %.cpp $(DEPS)
	@mkdir -p bin
	$(CPP) -c -o $@ $< $(CPPFLAGS)

.PHONY: clean
clean:
	rm -rf bin jsbin