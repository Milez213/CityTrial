
OFILES=main.o MStackHelp.o GLSL_helper.o Shader.o \
	ModelManager.o GameObject.o GameDrawableObject.o \
	GameKartObject.o GamePhysics.o GamePhysicsActor.o \
   	GameRamp.o GameUtilities.o GameUpgradeObject.o


# where to find .h files
IFLAGS=-I./glfw/include -I./physx/Include
CFLAGS=-DGL_GLEXT_PROTOTYPES -Wall -g -Wno-unknown-pragmas
PHYSXFLAGS=-g -D_DEBUG

# detect os and set flags accordingly
UNAME := $(shell uname)


# For linux. Uses local glfw for now
ifeq ($(UNAME), Linux)
	PHYSXLFLAG=-Lphysx/Lib/linux64
	PHYSXLIBS= \
 -lLowLevelCHECKED \
 -lLowLevelClothCHECKED \
 -lPhysX3CHECKED \
 -lPhysX3CharacterKinematicCHECKED \
 -lPhysX3CommonCHECKED \
 -lPhysX3CookingCHECKED \
 -lPhysX3ExtensionsCHECKED \
 -lPhysX3VehicleCHECKED \
 -lPhysXProfileSDKCHECKED \
 -lPhysXVisualDebuggerSDKCHECKED \
 -lPvdRuntimeCHECKED \
 -lPxTaskCHECKED \
 -lRepX3CHECKED \
 -lRepXUpgrader3CHECKED \
 -lSceneQueryCHECKED \
 -lSimulationControllerCHECKED \
 -lLowLevelCHECKED \
 -lLowLevelClothCHECKED \
 -lPhysX3CHECKED \
 -lPhysX3CharacterKinematicCHECKED \
 -lPhysX3CommonCHECKED \
 -lPhysX3CookingCHECKED \
 -lPhysX3ExtensionsCHECKED \
 -lPhysX3VehicleCHECKED \
 -lPhysXProfileSDKCHECKED \
 -lPhysXVisualDebuggerSDKCHECKED \
 -lPvdRuntimeCHECKED \
 -lPxTaskCHECKED \
 -lRepX3CHECKED \
 -lRepXUpgrader3CHECKED \
 -lSceneQueryCHECKED \
 -lSimulationControllerCHECKED

	LIB=./glfw/lib/x11/libglfw.a

	# -lXrandr for csl
	ifeq ($(shell uname -n), pack-nasa)
		XRANDR=
	else
		XRANDR=-lXrandr
	endif

LDFLAGS= $(LIB) -lXxf86vm -lXext  -lrt -lX11 -lGLU -lGL -pthread -lm $(XRANDR) \
$(PHYSXLFLAG) $(PHYSXLIBS)
endif


# For OS X
ifeq ($(UNAME), Darwin)

PHYSXLFLAG=-L./physx/Lib/osx64
PHYSXLIBS=\
-lLowLevelCHECKED \
-lLowLevelClothCHECKED \
-lPhysX3CHECKED \
-lPhysX3CharacterKinematicCHECKED \
-lPhysX3CommonCHECKED \
-lPhysX3CookingCHECKED \
-lPhysX3ExtensionsCHECKED \
-lPhysX3VehicleCHECKED \
-lPhysXProfileSDKCHECKED \
-lPhysXVisualDebuggerSDKCHECKED \
-lPvdRuntimeCHECKED \
-lPxTaskCHECKED \
-lRepX3CHECKED \
-lRepXUpgrader3CHECKED \
-lSceneQueryCHECKED \
-lSimulationControllerCHECKED

LDFLAGS=-lglfw -framework Cocoa -framework OpenGL -framework IOKit \
$(PHYSXLFLAG) $(PHYSXLIBS)

endif



all: build

build: $(OFILES)
	g++ $(CFLAGS) $(OFILES) -o kpp $(LDFLAGS)

%.o: %.cpp
	g++ -c $(CFLAGS) $(PHYSXFLAGS) $(IFLAGS) -o $@ $<

clean:
	rm -rf *.o kpp



# Comment this out
# that stuff is still in makefile2
