
OFILES=main.o MStackHelp.o GLSL_helper.o Shader.o \
	ModelManager.o GameObject.o GameDrawableObject.o \
	GameKartObject.o GameUpgradeObject.o GamePhysicalObject.o \
	GameRamp.o GameBuilding.o GameTerrain.o loadMap.o 

# where to find .h files
IFLAGS=-I./glfw/include 
CFLAGS=-DGL_GLEXT_PROTOTYPES -Wall -g

# detect os and set flags accordingly
UNAME := $(shell uname)


# For linux. Uses local glfw for now
ifeq ($(UNAME), Linux)
	LIB=./glfw/lib/x11/libglfw.a 
	LIB +=-L./lib

	# For csl (room 235)
	ifeq ($(findstring 235, $(shell uname -n)), 235)
		IFLAGS += -I./include/SDL_mixer
		XRANDR=-lXrandr
	else
		XRANDR=
	endif

    # uncomment to not compile with sound, and not use any real sound
	# CFLAGS += -DUSE_DUMMY_SOUND
	LIB += -lSDL_mixer
	IFLAGS += -I/usr/include/SDL


# uncomment to use TTF rendering
CFLAGS += -DMAIN_USE_TTF
LIB += -lfreetype
IFLAGS += -I/usr/include/freetype2


LDFLAGS= $(LIB) -lXxf86vm -lXext  -lrt -lX11 -lGLU -lGL -pthread -lm $(XRANDR)
endif


# For OS X
ifeq ($(UNAME), Darwin)

    # uncomment to not compile with sound, and not use any real sound
    #CFLAGS += -DUSE_DUMMY_SOUND
    CFLAGS += -DMAIN_USE_TTF

    LIB= -lfreetype
    IFLAGS += -I/usr/local/include/freetype2
    IFLAGS += -I/Library/Frameworks/SDL_mixer.framework/Headers \
	-I/Library/Frameworks/SDL.framework/Headers

    LDFLAGS= $(LIB) -lglfw -framework Cocoa -framework OpenGL -framework IOKit -framework SDL_mixer -framework SDL

endif



all: build

build: $(OFILES)
	g++ $(CFLAGS) $(OFILES) -o kpp $(LDFLAGS)

%.o: %.cpp
	g++ -c $(CFLAGS) $(IFLAGS) -o $@ $<

clean:
	rm -rf *.o kpp



# Comment this out
# that stuff is still in makefile2
