OFILES=main.o GLSL_helper.o MStackHelp.o BBObject.o BBMatObject.o BBCamera.o BBPlayerObject.o BBLight.o BBAvoidObject.o BBCollectObject.o
CFILES=main.cpp GLSL_helper.cpp MStackHelp.cpp BBObject.cpp BBMatObject.cpp BBCamera.cpp BBPlayerObject.cpp BBLight.cpp BBAvoidObject.cpp BBCollectObject.cpp
FRAMEWORKS=-lglfw -framework Cocoa -framework OpenGL -framework IOKit

all: $(OFILES)
	g++ $(OFILES) $(FRAMEWORKS) -o Program01
	
main.o: main.cpp
	g++ -c main.cpp
	
GLSL_helper.o: GLSL_helper.cpp
	g++ -c GLSL_helper.cpp

MStackHelp.o: MStackHelp.cpp
	g++ -c MStackHelp.cpp
	
BBObject.o: BBObject.cpp
	g++ -c BBObject.cpp
	
BBMatObject.o: BBMatObject.cpp
	g++ -c BBMatObject.cpp
	
BBCamera.o: BBCamera.cpp
	g++ -c BBCamera.cpp
	
BBPlayerObject.o: BBPlayerObject.cpp
	g++ -c BBPlayerObject.cpp
	
BBAvoidObject.o: BBAvoidObject.cpp
	g++ -c BBAvoidObject.cpp
	
BBCollectObject.o: BBCollectObject.cpp
	g++ -c BBCollectObject.cpp
	
BBLight.o: BBLight.cpp
	g++ -c BBLight.cpp