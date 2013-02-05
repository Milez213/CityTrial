# Makefile by Mustafa Khafateh

CFLAGS=-Wall -g
LDFLAGS=-DGL_GLEXT_PROTOTYPES -lglut -lGL -lGLU -DCSL -lfreetype
IFLAGS=-I../

FT2=-I/usr/include/freetype2

# $@ - file name of target rule
# $^ - name of all prerequisits
# $< - name of first prerequisit

main:	main.o helper.o mesh_loader.o shader.o phongShader.o torus.o drawable.o \
		mesh.o texture.o sparks.o bunnie.o bunnies.o
		echo "Linking "$^
		g++  $(CFLAGS) $^ -o $@ $(FT2) $(LDFLAGS)

main.o:	main.cpp
		g++ $(IFLAGS) $(CFLAGS) $(LDFLAGS)  $< -c  $(FT2)

helper.o: GLSL_helper.cpp GLSL_helper.h
		echo "Compiling "$<
		g++ $(IFLAGS) $(CFLAGS) $(LDFLAGS) $< -o $@ -c

mesh_loader.o:	CMeshLoaderSimple.cpp CMeshLoaderSimple.h
		echo "Compiling "$<
		g++ $(CFLAGS) $(LDFLAGS) $< -o $@ -c

mesh.o:	mesh.cpp mesh.h
		echo "Compiling "$<
		g++ $(CFLAGS) $(LDFLAGS) $< -o $@ -c

shader.o:	Shader.cpp Shader.h
		echo "Compiling "$<
		g++ $(IFLAGS) $(CFLAGS) $(LDFLAGS) $< -o $@ -c

phongShader.o:	PhongShader.cpp PhongShader.h
		echo "Compiling "$<
		g++ $(IFLAGS) $(CFLAGS) $(LDFLAGS) $< -o $@ -c

torus.o:	vbotorus.cpp vbotorus.h
		echo "Compiling "$<
		g++ $(IFLAGS) $(CFLAGS) $(LDFLAGS) $< -o $@ -c

tfbparticles.o:	tfbparticles.cpp tfbparticles.h
		echo "Compiling "$<
		g++ $(IFLAGS) $(CFLAGS) $(LDFLAGS) $< -o $@ -c 

sparks.o:	sparks.cpp sparks.h
		echo "Compiling "$<
		g++ $(IFLAGS) $(CFLAGS) $(LDFLAGS) $< -o $@ -c 

bunnies.o:	Bunnies.cpp Bunnies.h
		echo "Compiling "$<
		g++ $(IFLAGS) $(CFLAGS) $(LDFLAGS) $< -o $@ -c 

bunnie.o:	Bunnie.cpp Bunnie.h
		echo "Compiling "$<
		g++ $(IFLAGS) $(CFLAGS) $(LDFLAGS) $< -o $@ -c 

planes.o:	PlaneManager.cpp PlaneManager.h
		echo "Compiling "$<
		g++ $(IFLAGS) $(CFLAGS) $(LDFLAGS) $< -o $@ -c 

plane.o:	Plane.cpp Plane.h
		echo "Compiling "$<
		g++ $(IFLAGS) $(CFLAGS) $(LDFLAGS) $< -o $@ -c 

drawable.o:	drawable.cpp drawable.h
		echo "Compiling "$<
		g++ $(IFLAGS) $(CFLAGS) $(LDFLAGS) $< -o $@ -c

texture.o:	LoadTexture.cpp LoadTexture.h
		echo "Compiling "$<
		g++ $(CFLAGS) $(LDFLAGS) $< -o $@ -c

clean:
		rm *.o

