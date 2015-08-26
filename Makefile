#includes

OPENCV_CFLAGS = $(shell pkg-config --cflags opencv)
INCLUDES = $(OPENCV_CFLAGS) -Iinclude/
#lib
OPENCV_LDFLAGS = $(shell pkg-config --libs opencv)
OPENGL_LDFLAGS= -lGL -lGLU -lglut
ARMADILLO_LDFLAGS = -larmadillo 
LIBS = $(OPENCV_LDFLAGS) $(OPENGL_LDFLAGS) $(ARMADILLO_LDFLAGS) -lpthread -lLeap



CFLAGS = -std=c++11 -g -Wall -DNDEBUG 
LDFLAGS =

CC = g++

SOURCE_FILES=\
	src/lib/IO.cpp\
	src/lib/PDM.cpp\
	src/lib/Patch.cpp\
	src/lib/CLM.cpp\
	src/lib/FDet.cpp\
	src/lib/PAW.cpp\
	src/lib/FCheck.cpp\
	src/lib/Tracker.cpp\
	src/lib/face_tracker.cpp\
	src/lib/faceTracker.cpp\
	src/lib/frameReader.cpp\
	src/lib/ggltools.cpp\
	src/lib/gpoint3d.cpp\
	src/lib/gmath.cpp\
	src/lib/gvector3d.cpp\
	src/lib/gmatrix3d.cpp\
	src/lib/splitstring.cpp\
	src/lib/camera.cpp\

	
ALL_OBJECTS=\
	$(patsubst %.cpp,%.o,$(SOURCE_FILES))
	
DEPENDENCY_FILES=\
	$(patsubst %.o,%.d,$(ALL_OBJECTS))
	
all: bin/face_tracker

%.o: %.cpp Makefile
	@# Make dependecy file
	$(CXX) -MM -MT $@ -MF $(patsubst %.cpp,%.d,$<) $(CFLAGS) $(INCLUDES) $<
	@# Compile
	$(CXX) $(CFLAGS) -c -o $@ $< $(INCLUDES)

-include $(DEPENDENCY_FILES)

bin/face_tracker: $(ALL_OBJECTS)	
	$(CXX) $(LDFLAGS)  -o $@ $(ALL_OBJECTS) $(LIBS)

.PHONY: clean
clean:
	rm -r src/lib/*.o && rm -r src/lib/*.d
