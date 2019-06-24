CPP = g++
CPPFLAGS = -g -O3 -Wall -Wextra -pedantic -std=c++11 `sdl2-config --cflags`
PKGFLAGS = `sdl2-config --libs`
DEPS = point.h rays.h camera.h objects.h world.h
OBJ = point.o rays.o camera.o objects.o world.o main.o

3DRaytrace: $(OBJ)
	$(CPP) -o $@ $^ $(CPPFLAGS) $(PKGFLAGS)

%.o: %.cpp $(DEPS)
	$(CPP) -c -o $@ $< $(CPPFLAGS)

.PHONY: clean
clean:
	rm *.o 3DRaytrace