#include "SquareScene.h"

#include <GL/glew.h>
#include <stdlib.h>


#define GLSL(shader) "#version 150 core\n" #shader

SquareScene::SquareScene() : Scene()
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);

	GLfloat vertices[] = {
		// Position         //Color         
		-0.5f, 0.5f, 0.0f,  1,0,0,          
		0.5f, 0.5f, 0.0f,   0,1,0,          
		0.5f, -0.5f, 0.0f,  0,0,1,          
		-0.5f, -0.5f, 0.0f,  1,1,1,         
	};

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//Triangles
	glGenBuffers(1, &ebo);
	GLuint triangles[] = {
		0,1,2,
		2,3,0,
	};
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(triangles),triangles, GL_STATIC_DRAW);

	// VERTEX
	const GLchar* vertexSource = GLSL(

		in vec3 position;
	in vec3 color;
	out vec3 outputColor;
	void main()
	{
		/*float bw = 0.299 * color.x + 0.587 * color.y + 0.114 * color.z;
		outputColor = vec3(bw, bw, bw);*/
		outputColor = color;
		gl_Position = vec4(position, 1.0);
	}
	);


	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);

	const GLchar* fragmentSource = GLSL(
		in vec3 outputColor;
	out vec4 outColor;
	void main()
	{
		outColor = vec4(outputColor, 1.0);
	}
	);

	// FRAGMENT / PIXELS
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);

	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glBindFragDataLocation(shaderProgram, 0, "outColor");
	glLinkProgram(shaderProgram);
	glUseProgram(shaderProgram);

	GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
	glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), 0);

	GLint colAttrib = glGetAttribLocation(shaderProgram, "color");
	glEnableVertexAttribArray(colAttrib);
	glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
}


void SquareScene::render()
{
	glBindVertexArray(vao);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	//glDrawArrays(GL_TRIANGLES, 0, 6);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
}