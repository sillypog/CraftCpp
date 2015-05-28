/*
 * main.cpp
 *
 *  Created on: May 14, 2014
 *      Author: sillypog
 */
#define GLEW_STATIC
#define GLFW_INCLUDE_GL3  /* don't drag in legacy GL headers. */
#define GLFW_NO_GLU       /* don't drag in the old GLU lib - unless you must. */

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <png.h>

#include <glm/gtc/constants.hpp> // Wanted to use this for two_pi
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <cmath>	// Needed for sin
#include <fstream>	// Needed to read files
#include <iostream>	// Needed to write to console
#include <limits>	// Needed to get lowest float value
#include <sstream>	// Needed for stringstream, to hold file contents as string buffer
#include <string>	// C++ strings

using namespace std;

// Create something to render
// Vertexes are X, Y, Z, R, G, B, texX, texY
// Images are read upside down so the top of the shape has the bottom of the image
float repeatX = 1.0f;
float repeatY = 1.0f;
float vertices[] = {
	-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, // 0
	 0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, // 1
	 0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, // 2
//	 0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, // 2
	-0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, // 3
//	-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, // 0

	-0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, // 4
	 0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, // 5
	 0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, // 6
//	 0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, // 6
	-0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, // 7
//	-0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, // 4

	-0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, // 8
	-0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, // 9
	-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, // 10
//	-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, // 10
//	-0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, // 4
//	-0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, // 8

	 0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, // 11
//	 0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, // 2
	 0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, // 12
//	 0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, // 12
	 0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, // 13
//	 0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, // 11

//	-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, // 10
	 0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, // 14
//	 0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, // 5
//	 0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, // 5
//	-0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, // 4
//	-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, // 10

//	-0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, // 3
//	 0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, // 2
//	 0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, // 11
//	 0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, // 11
	-0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, // 15
//	-0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, // 3

	// Floor
	-1.0f, -1.0f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, // 16
	 1.0f, -1.0f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, // 17
	 1.0f,  1.0f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, // 18
//	 1.0f,  1.0f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, // 18
	-1.0f,  1.0f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, // 19
//	-1.0f, -1.0f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f  // 16
};

float quadVertices[] = {
    -0.5f,  0.5f, 0.0f, 1.0f,
     0.5f,  0.5f, 1.0f, 1.0f,
     0.5f, -0.5f, 1.0f, 0.0f,

     0.5f, -0.5f, 1.0f, 0.0f,
    -0.5f, -0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.0f, 1.0f
};

GLuint elements[] = {
    0,  1,  2,  2,  3,  0,
    4,  5,  6,  6,  7,  4,
    8,  9, 10, 10,  4,  8,
    11, 2, 12, 12, 13, 11,
    10, 14, 5,  5,  4, 10,
    3,  2, 11, 11, 15,  3,

    // Floor
    16, 17, 18, 18, 19, 16
};

GLubyte *kittenImage;

int frames = 0;

void error_callback(int error, const char* description){
	cout << "There was an error: " << error << ": " << description << endl;
}

void loadShader(string filename, string &shader){
	cout << "Loading file: " << filename << endl;

	ifstream data(filename);
	if (data.bad() || !data.is_open()) {
	    cerr << "Unable to open " << filename << endl;
	    exit(EXIT_FAILURE);
	}

	stringstream buffer;
	buffer << data.rdbuf();
	shader = buffer.str();

	data.close();
}

void checkShaderStatus(GLuint shader, string shaderName){
	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status == GL_TRUE){
		cout << shaderName << " compiled successfully" << endl;
	} else {
		cout << shaderName << " compilation failed" << endl;
		char buffer[512];
		glGetShaderInfoLog(shader, 512, NULL, buffer);
		cout << buffer;
	}
}

GLuint buildShaderProgram(string vertexShaderFilename, string fragmentShaderFilename){
    string vertexShaderFile;
    string fragmentShaderFile;

    loadShader(vertexShaderFilename, vertexShaderFile);
    loadShader(fragmentShaderFilename, fragmentShaderFile);

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    const char *vertexShader_cStr = vertexShaderFile.c_str(); // Have to convert to c pointer array to pass to function
    glShaderSource(vertexShader, 1, &vertexShader_cStr, NULL);
    glCompileShader(vertexShader);

    checkShaderStatus(vertexShader, "vertexShader");

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    const char *fragmentShader_cStr = fragmentShaderFile.c_str();
    glShaderSource(fragmentShader, 1, &fragmentShader_cStr, NULL);
    glCompileShader(fragmentShader);

    checkShaderStatus(fragmentShader, "fragmentShader");

    // Combine shaders
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    //glBindFragDataLocation(shaderProgram, 0, "outColor"); // Not needed as 0 is default buffer
    glLinkProgram(shaderProgram);   // Saves changes

    glDeleteShader(fragmentShader);
    glDeleteShader(vertexShader);

    return shaderProgram;
}

void checkFramebufferStatus(){
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

    string not_complete = "Framebuffer is not complete: ";

    switch(status){
        case GL_FRAMEBUFFER_COMPLETE:
            cout << "GL_FRAMEBUFFER_COMPLETE" << endl;
            break;
        case GL_FRAMEBUFFER_UNDEFINED:
            cout << not_complete << "GL_FRAMEBUFFER_UNDEFINED" << endl;
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
            cout << not_complete << "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT" << endl;
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
            cout << not_complete << "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT" << endl;
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
            cout << not_complete << "GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER" << endl;
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
            cout << not_complete << "GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER" << endl;
            break;
        case GL_FRAMEBUFFER_UNSUPPORTED:
            cout << not_complete << "GL_FRAMEBUFFER_UNSUPPORTED" << endl;
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
            cout << not_complete << "GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE" << endl;
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
            cout << not_complete << "GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS" << endl;
            break;
        default:
            cout << "Framebuffer status unknown: " << status << endl;
    }
}

bool loadPngImage(char *name, int &outWidth, int &outHeight, bool &outHasAlpha, GLubyte **outData) {
    png_structp png_ptr;
    png_infop info_ptr;
    unsigned int sig_read = 0;
    int color_type, interlace_type;
    FILE *fp;

    if ((fp = fopen(name, "rb")) == NULL)
        return false;

    /* Create and initialize the png_struct
     * with the desired error handler
     * functions.  If you want to use the
     * default stderr and longjump method,
     * you can supply NULL for the last
     * three parameters.  We also supply the
     * the compiler header file version, so
     * that we know if the application
     * was compiled with a compatible version
     * of the library.  REQUIRED
     */
    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING,
                                     NULL, NULL, NULL);

    if (png_ptr == NULL) {
        fclose(fp);
        return false;
    }

    /* Allocate/initialize the memory
     * for image information.  REQUIRED. */
    info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == NULL) {
        fclose(fp);
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return false;
    }

    /* Set error handling if you are
     * using the setjmp/longjmp method
     * (this is the normal method of
     * doing things with libpng).
     * REQUIRED unless you  set up
     * your own error handlers in
     * the png_create_read_struct()
     * earlier.
     */
    if (setjmp(png_jmpbuf(png_ptr))) {
        /* Free all of the memory associated
         * with the png_ptr and info_ptr */
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        fclose(fp);
        /* If we get here, we had a
         * problem reading the file */
        return false;
    }

    /* Set up the output control if
     * you are using standard C streams */
    png_init_io(png_ptr, fp);

    /* If we have already
     * read some of the signature */
    png_set_sig_bytes(png_ptr, sig_read);

    /*
     * If you have enough memory to read
     * in the entire image at once, and
     * you need to specify only
     * transforms that can be controlled
     * with one of the PNG_TRANSFORM_*
     * bits (this presently excludes
     * dithering, filling, setting
     * background, and doing gamma
     * adjustment), then you can read the
     * entire image (including pixels)
     * into the info structure with this
     * call
     *
     * PNG_TRANSFORM_STRIP_16 |
     * PNG_TRANSFORM_PACKING  forces 8 bit
     * PNG_TRANSFORM_EXPAND forces to
     *  expand a palette into RGB
     */
    png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_STRIP_16 | PNG_TRANSFORM_PACKING | PNG_TRANSFORM_EXPAND, NULL);

    png_uint_32 width, height;
    int bit_depth;
    png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type,
                 &interlace_type, NULL, NULL);
    outWidth = width;
    outHeight = height;
    outHasAlpha = (color_type & PNG_COLOR_MASK_ALPHA);

    unsigned int row_bytes = png_get_rowbytes(png_ptr, info_ptr);
    *outData = (unsigned char*) malloc(row_bytes * outHeight);

    png_bytepp row_pointers = png_get_rows(png_ptr, info_ptr);

    for (int i = 0; i < outHeight; i++) {
        // note that png is ordered top to
        // bottom, but OpenGL expect it bottom to top
        // so the order or swapped
        memcpy(*outData+(row_bytes * (outHeight-1-i)), row_pointers[i], row_bytes);
    }

    /* Clean up after the read,
     * and free any memory allocated */
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

    /* Close the file */
    fclose(fp);

    /* That's it */
    return true;
}

void defineVertexAttribute(GLuint shaderProgram, string attributeName, int attributeLength, int stride, int offset){
    GLint attribute = glGetAttribLocation(shaderProgram, attributeName.c_str());
    glVertexAttribPointer(attribute, attributeLength, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(offset * sizeof(float)));
    glEnableVertexAttribArray(attribute);
}

int main() {
	cout << "Hello World from C++!" << endl;

    cout << "Sizeof GLuint is " << sizeof(GLuint) << endl;
    cout << "Sizeof elements is " << sizeof(elements) << endl;

	glfwSetErrorCallback(error_callback);

	if (!glfwInit()){
    	exit(EXIT_FAILURE);
    }

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* window = glfwCreateWindow(640, 480, "My Title", NULL, NULL);
    if (!window){
    	glfwTerminate();
    	exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);

    glfwSwapInterval(0);

    // Setup GLEW to handle modern OpenGL functions
    glewExperimental = GL_TRUE;
    glewInit();

    printf("OpenGL version supported by this platform (%s): \n", glGetString(GL_VERSION));

    // Load texture image
    int widthKitten, heightKitten;
    bool hasAlphaKitten;
    char filename[] = "textures/sample.png";
    bool success = loadPngImage(filename, widthKitten, heightKitten, hasAlphaKitten, &kittenImage);
    printf("Outcome of loading %s: %s \n ", filename, success ? "success" : "fail");
    if (success){
    	cout << filename << ": " << widthKitten << "x" << heightKitten << ", hasAlpha: " << hasAlphaKitten << endl;
    }

    // Prepare things for rendering
    GLuint vao, vaoQuad;
	glGenVertexArrays(1, &vao);
    glGenVertexArrays(1, &vaoQuad);

    GLuint vbo, vboQuad;	// Vertex buffer object
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &vboQuad);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, vboQuad);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

    // Textures
    GLuint textures[1];
    glGenTextures(1, textures);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textures[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, hasAlphaKitten ? GL_RGBA : GL_RGB, widthKitten, heightKitten, 0, hasAlphaKitten ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, kittenImage);

    // Create shaders from files
    GLuint shaderProgram = buildShaderProgram("shaders/vertex_shader.glsl", "shaders/fragment_shader.glsl");
    GLuint quadProgram = buildShaderProgram("shaders/quad_vertex_shader.glsl", "shaders/quad_fragment_shader.glsl");
    glUseProgram(shaderProgram);    // Only one can be used at a time

    // Once vertex array object is created, define how our vertex data is passed in
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    defineVertexAttribute(shaderProgram, "position", 3, 8, 0);
    defineVertexAttribute(shaderProgram, "color",    3, 8, 3);
    defineVertexAttribute(shaderProgram, "texcoord", 2, 8, 6);

    glBindVertexArray(vaoQuad);
    glBindBuffer(GL_ARRAY_BUFFER, vboQuad);
    defineVertexAttribute(quadProgram, "position", 2, 4, 0);
    defineVertexAttribute(quadProgram, "texcoord", 2, 4, 2);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);


    glUniform1i(glGetUniformLocation(shaderProgram, "texKitten"), 0);
    GLint elapsed = glGetUniformLocation(shaderProgram, "elapsed");
    glUniform1f(elapsed, 0.0f);

    // Create a transformation
    GLint uniModel = glGetUniformLocation(shaderProgram, "model");

    // Create a view, which is the camera
    glm::mat4 view = glm::lookAt(
    		glm::vec3(2.0f, 2.0f, 1.2f), // Camera position
    		glm::vec3(0.0f, 0.0f, 0.0f), // Point to focus on
    		glm::vec3(0.0f, 0.0f, 1.0f) // Define the "up" axis
    );
    GLint uniView = glGetUniformLocation(shaderProgram, "view");
    glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(view));

    // Create a perspective projection
    glm::mat4 proj = glm::perspective(glm::radians(45.0f), 640.0f / 480.0f, 1.0f, 10.0f);
    GLint uniProj = glGetUniformLocation(shaderProgram, "proj");
    glUniformMatrix4fv(uniProj, 1, GL_FALSE, glm::value_ptr(proj));

    GLint uniColor = glGetUniformLocation(shaderProgram, "overrideColor");

    // Make an element buffer to reuse vertices
    GLuint ebo;
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);

    float startTime = glfwGetTime();

    constexpr int VERTICES_IN_FACE = 6;
    constexpr int VERTICES_IN_CUBE = VERTICES_IN_FACE * 6;

    while(!glfwWindowShouldClose(window)){
        // Set up for drawing 3D elements
        glBindVertexArray(vao);
        glUseProgram(shaderProgram);
        glEnable(GL_DEPTH_TEST);

    	// Keep running
    	// Clear the screen to white
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClearDepth(1.0f); // This is the default anyway
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		float time = glfwGetTime();
		glUniform1f(elapsed, time);

        // Reset the model matrix each frame
        glm::mat4 model;

        // Apply rotation
        model = glm::rotate(model, time * 1.0f, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));

        // Cube - don't bother drawing the face that's on the floor
        glDrawElements(GL_TRIANGLES, VERTICES_IN_CUBE - (1 * VERTICES_IN_FACE), GL_UNSIGNED_INT, (void*)(1* VERTICES_IN_FACE * sizeof(GLuint)));

        // Floor
        glEnable(GL_STENCIL_TEST);  // Discard any reflection that is outside floor
        glStencilFunc(GL_ALWAYS, 1, 0xFF);  // Set any stencil to 1
        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
        glStencilMask(0xFF);    // Write to stencil buffer
        glDepthMask(GL_FALSE);
        glClear(GL_STENCIL_BUFFER_BIT); // Clear stencil buffer - can't do this until GL_STENCIL_TEST enabled
        glDrawElements(GL_TRIANGLES, VERTICES_IN_FACE, GL_UNSIGNED_INT, (void*)(VERTICES_IN_CUBE * sizeof(GLuint)));

		// Cube reflection - don't bother drawing the face that's on the floor
		model = glm::scale(
            glm::translate(model, glm::vec3(0, 0, -1)),
            glm::vec3(1, 1, -1)
        );
        glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));
        glStencilFunc(GL_EQUAL, 1, 0xFF);   // Pass test if stencil is 1 (set by floor)
        glStencilMask(0x00);    // Don't write to stencil buffer
        glDepthMask(GL_TRUE);
        glUniform3f(uniColor, 0.7f, 0.7f, 0.7f); // Darken the reflection
        glDrawElements(GL_TRIANGLES, VERTICES_IN_CUBE - (1 * VERTICES_IN_FACE), GL_UNSIGNED_INT, (void*)(1* VERTICES_IN_FACE * sizeof(GLuint)));

        // Reset for next draw
        glUniform3f(uniColor, 1.0f, 1.0f, 1.0f);
        glDisable(GL_STENCIL_TEST); // Turn off before next cube draw
        glDisable(GL_DEPTH_TEST);


        // Setup to draw 2D element
        glBindVertexArray(vaoQuad);
        glUseProgram(quadProgram);

        // Draw 2D element
        glDrawArrays(GL_TRIANGLES, 0, 6);

    	glfwSwapBuffers(window);
    	glfwPollEvents();

    	frames++;
    }

    // Calculate average frames per second
    float endTime = glfwGetTime();
    float fps = frames/(endTime - startTime);
    float mspf = ((endTime - startTime) * 1000) / frames;
    cout << "FPS: " << fps << " over " << (endTime-startTime) << " seconds." << endl;
    cout << "MSPF: " << mspf << " over " << frames << " frames." << endl;

    // Cleanup
    glDeleteProgram(shaderProgram);

	glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);

	glDeleteVertexArrays(1, &vao);

	// Cleanup GLFW
    glfwDestroyWindow(window);

    glfwTerminate();
    return 0;
}




