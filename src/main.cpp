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

#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

// Create something to render
float vertices[] = {
	 0.0f,  0.5f, // Vertex 1 (X, Y)
	 0.5f, -0.5f, // Vertex 2 (X, Y)
	-0.5f, -0.5f  // Vertex 3 (X, Y)
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
    glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0); // position has 2 members of type float
    glEnableVertexAttribArray(posAttrib);

    // Set color through uniform to be passed to fragment shader
    GLint uniColor = glGetUniformLocation(shaderProgram, "triangleColor");
    glUniform3f(uniColor, 1.0f, 0.0f, 0.0f);



    while(!glfwWindowShouldClose(window)){
    	// Keep running
    	// Clear the screen to black
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		float time = glfwGetTime();
		glUniform3f(uniColor, (sin(time * 4.0f) + 1.0f) / 2.0f, 0.0f, 0.0f);

    	glDrawArrays(GL_TRIANGLES, 0, 3);

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




