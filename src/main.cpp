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

#include <cmath>	// Needed for sin
#include <fstream>	// Needed to read files
#include <iostream>	// Needed to write to console
#include <sstream>	// Needed for stringstream, to hold file contents as string buffer
#include <string>	// C++ strings

using namespace std;

// Create something to render
// Vertexes are X, Y, R, G, B
float vertices[] = {
	 -0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // Top-left
	  0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 5.0f, 0.0f, // Top-right
	  0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 5.0f, 5.0f, // Bottom-right

//	  0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // Bottom-right Don't need to repeat
	 -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 5.0f // Bottom-left
//	 -0.5f,  0.5f, 1.0f, 0.0f, 0.0f  // Top-left Don't need to repeat
};

// Form a square from 2 triangles
GLuint elements[] = {
		0, 1, 2, // tl, tr, br
		2, 3, 0  // br, bl, tl
};

// Checkerboard
float pixels[] = {
		0.0f, 0.0f, 0.0f,     1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,     0.0f, 0.0f, 0.0f
};

string vertexShaderFile;
string fragmentShaderFile;

void error_callback(int error, const char* description){
	cout << "There was an error: " << description << endl;
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

    // Prepare things for rendering
    GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

    GLuint vbo;	// Vertex buffer object
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Textures
    GLuint tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 2, 2, 0, GL_RGB, GL_FLOAT, pixels);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

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




