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

#include <cmath>	// Needed for sin
#include <fstream>	// Needed to read files
#include <iostream>	// Needed to write to console
#include <sstream>	// Needed for stringstream, to hold file contents as string buffer
#include <string>	// C++ strings

using namespace std;

// Create something to render
// Vertexes are X, Y, R, G, B, texX, texY
// Images are red upside down so the top of the shape has the bottom of the image
float repeatX = 1.0f;
float repeatY = 1.0f;
float vertices[] = {
	 -0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 0.0f, repeatY, // Top-left
	  0.5f,  0.5f, 0.0f, 1.0f, 0.0f, repeatX, repeatY, // Top-right
	  0.5f, -0.5f, 0.0f, 0.0f, 1.0f, repeatX, 0.0f, // Bottom-right

//	  0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // Bottom-right Don't need to repeat
	 -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f // Bottom-left
//	 -0.5f,  0.5f, 1.0f, 0.0f, 0.0f  // Top-left Don't need to repeat
};

// Form a square from 2 triangles
GLuint elements[] = {
		0, 1, 2, // tl, tr, br
		2, 3, 0  // br, bl, tl
};

string vertexShaderFile;
string fragmentShaderFile;

GLubyte *kittenImage;
GLubyte *puppyImage;

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

int main() {
	cout << "Hello World from C++!" << endl;

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

    // Setup GLEW to handle modern OpenGL functions
    glewExperimental = GL_TRUE;
    glewInit();
    // Test GLEW
    /*GLuint vertexBuffer;
    glGenBuffers(1, &vertexBuffer);
    printf("%u\n", vertexBuffer);*/

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

    int widthPuppy, heightPuppy;
    bool hasAlphaPuppy;
    strcpy(filename, "textures/sample2.png");	// c way of reassigning char array, eg `filename = "..."`
    success = loadPngImage(filename, widthPuppy, heightPuppy, hasAlphaPuppy, &puppyImage);
    printf("Outcome of loading %s: %s \n ", filename, success ? "success" : "fail");


    // Prepare things for rendering
    GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

    GLuint vbo;	// Vertex buffer object
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Textures
    GLuint textures[2];
    glGenTextures(2, textures);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textures[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, hasAlphaKitten ? GL_RGBA : GL_RGB, widthKitten, heightKitten, 0, hasAlphaKitten ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, kittenImage);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, textures[1]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, hasAlphaPuppy ? GL_RGBA : GL_RGB, widthPuppy, heightPuppy, 0, hasAlphaPuppy ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, puppyImage);

    loadShader("shaders/vertex_shader.glsl", vertexShaderFile);
    loadShader("shaders/fragment_shader.glsl", fragmentShaderFile);


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
    glLinkProgram(shaderProgram);	// Saves changes
    glUseProgram(shaderProgram);	// Only one can be used at a time

    // Once vertex array object is created, define how our vertex data is passed in
    GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
    glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 7*sizeof(float), 0); // position has 2 members of type float
    glEnableVertexAttribArray(posAttrib);

    GLint colAttrib = glGetAttribLocation(shaderProgram, "color");
    glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 7*sizeof(float), (void*)(2*sizeof(float)));
    glEnableVertexAttribArray(colAttrib);

    GLint texAttrib = glGetAttribLocation(shaderProgram, "texcoord");
    glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 7*sizeof(float), (void*)(5*sizeof(float)));
    glEnableVertexAttribArray(texAttrib);

    // Set color through uniform to be passed to fragment shader
    //GLint uniColor = glGetUniformLocation(shaderProgram, "triangleColor");
    //glUniform3f(uniColor, 1.0f, 0.0f, 0.0f);
    glUniform1i(glGetUniformLocation(shaderProgram, "texKitten"), 0);
    glUniform1i(glGetUniformLocation(shaderProgram, "texPuppy"), 1);


    // Make an element buffer to reuse vertices
    GLuint ebo;
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);



    while(!glfwWindowShouldClose(window)){
    	// Keep running
    	// Clear the screen to black
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//float time = glfwGetTime();
		//glUniform3f(uniColor, (sin(time * 4.0f) + 1.0f) / 2.0f, 0.0f, 0.0f);

    	glDrawElements(GL_TRIANGLES, sizeof(elements), GL_UNSIGNED_INT, 0);

    	glfwSwapBuffers(window);
    	glfwPollEvents();
    }

    // Cleanup
    glDeleteProgram(shaderProgram);
	glDeleteShader(fragmentShader);
	glDeleteShader(vertexShader);

	glDeleteBuffers(1, &vbo);

	glDeleteVertexArrays(1, &vao);

	// Cleanup GLFW
    glfwDestroyWindow(window);

    glfwTerminate();
    return EXIT_SUCCESS;
}




