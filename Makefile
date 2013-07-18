DEBUG=-ggdb

build:
	@case `uname` in \
		CYGWIN*) \
			echo "PLATFORM: Cygwin"; \
			$(MAKE) all OPENGL_LIB="-lglut32 -lglu32 -lopengl32" EXE_SUFFIX=".exe";; \
		Linux*) \
			echo "PLATFORM: Linux"; \
			$(MAKE) all OPENGL_LIB="-lGL -lGLU -lglut" EXE_SUFFIX="" ;; \
		*) \
			echo "PLATFORM: UNKNOWN. ABORT." ;; \
	esac

all: flight imgtest objects pong

flight: \
		flight.o \
		Vector3.o \
		Quaternion.o \
		Transform.o \
		Camera.o \
		Image.o \
		Image_BMP.o \
		ImageLoader.o \
		FPSCounter.o
	$(CXX) $(DEBUG) -o $@ $^ $(OPENGL_LIB) -lpthread -lstdc++ -L/usr/X11R6/lib -lXi -lXmu
	#strip $@$(EXE_SUFFIX)

imgtest: imgtest.o Image.o Image_BMP.o ImageLoader.o
	$(CXX) $(DEBUG) -o $@ $^ $(OPENGL_LIB) -lstdc++ -L/usr/X11R6/lib -lXi -lXmu
	#strip $@$(EXE_SUFFIX)

objects: objects.o Image.o Image_BMP.o ImageLoader.o Camera.o Vector3.o Quaternion.o Transform.o
	$(CXX) $(DEBUG) -o $@ $^ $(OPENGL_LIB) -lstdc++ -L/usr/X11R6/lib -lXi -lXmu
	#strip $@$(EXE_SUFFIX)

pong: pong.o
	$(CXX) $(DEBUG) -o $@ $^ $(OPENGL_LIB) -lpthread -lstdc++ -L/usr/X11R6/lib -lXi -lXmu
	#strip $@$(EXE_SUFFIX)

Image_BMP.o: Image_BMP.h Image_BMP.cpp Image.h

clean:
	find . -maxdepth 1 -name "*.o" -exec rm {} \;
	find . -maxdepth 1 -name "*~" -exec rm {} \;
	find . -maxdepth 1 -name "*.exe" -exec rm {} \;
	find . -maxdepth 1 -name "*.stackdump" -exec rm {} \;
	find . -maxdepth 1 -name "flight" -exec rm {} \;
	find . -maxdepth 1 -name "imgtest" -exec rm {} \;
	find . -maxdepth 1 -name "objects" -exec rm {} \;
	find . -maxdepth 1 -name "pong" -exec rm {} \;

.cpp.o:
	$(CXX) $(DEBUG) -pedantic -Wall -o $@ -c $<

