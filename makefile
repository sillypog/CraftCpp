CFLAGS=-Wall -Wextra -std=c++1y
INCLUDE_PATHS = -I/Users/sillypog/OpenGL/glfw/include -I/Users/sillypog/OpenGL/glew/glew-1.10.0-built/include -I/Users/sillypog/OpenGL/glm
LIBRARY_PATHS = -L/Users/sillypog/OpenGL/glfw/build/src -L/Users/sillypog/OpenGL/glew/glew-1.10.0-built/lib
LINKER_FLAGS = -lglew -lglfw3 -lpng -framework Cocoa -framework OpenGL -framework IOKit -framework CoreVideo
#DEFINES = -DGLM_MESSAGES

all:
	g++ -v $(CFLAGS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(LINKER_FLAGS) $(DEFINES) src/main.cpp -o GLFWWindowCpp
