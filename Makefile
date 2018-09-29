CXX := g++-4.9
CXXFLAGS := -std=c++11
GLFWLIBPATHS := glfw-3.2.1/src/  #glfw-3.2.1/deps
GLFWINCLUDES := glfw-3.2.1/include/ glm/
LIBS := -lglfw3 #-lglad
_FRAMEWORKS := Cocoa OpenGL IOKit CoreVideo
FRAMEWORKS := $(foreach fwrk, $(_FRAMEWORKS), $(addprefix -framework , $(fwrk)))

INCLUDES := $(foreach path,  $(GLFWINCLUDES), $(addprefix -I , $(path)))  $(FRAMEWORKS)
LIBPATHS := $(foreach path,  $(GLFWLIBPATHS), $(addprefix -L , $(path)))

glfwTest: openglPlayground.cpp 
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(LIBPATHS) $(LIBS) $(OBJECTS) $^ -o glfwTest

clean: 
	rm -f glfwTest
	rm -Rf *.dSYM
