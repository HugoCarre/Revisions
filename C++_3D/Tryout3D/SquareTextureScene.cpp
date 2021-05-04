#include "SquareTextureScene.h"

#include <GL/glew.h>
#include <SFML/Graphics.hpp>
#include <stdlib.h>
#include "bmp.h"
#include "Settings.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>



#define GLSL(shader) "#version 150 core\n" #shader

SquareTextureScene::SquareTextureScene() : Scene()
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);

	GLfloat vertices[] = {
		// Position          //Color       //UV  
		-0.5f, 0.5f, 0.0f,   1,1,1,        0,1,
		0.5f, 0.5f, 0.0f,    1,1,1,        1,1,
		0.5f, -0.5f, 0.0f,   1,1,1,        1,0,
		-0.5f, -0.5f, 0.0f,  1,1,1,        0,0,
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
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(triangles), triangles, GL_STATIC_DRAW);

	// VERTEX
	/*float bw = 0.299 * color.x + 0.587 * color.y + 0.114 * color.z;
			outputColor = vec3(bw, bw, bw);
			*/
	const GLchar* vertexSource = GLSL(

		in vec3 position;
		in vec3 color;
		in vec2 uv;

		out vec3 outputColor;
		out vec2 outUv;

		uniform mat4 model;
		uniform mat4 view;
		uniform mat4 proj;
		void main()
		{			
			outUv =  uv;
			outputColor = color;
			gl_Position = proj * view * model * vec4(position, 1.0);
		}
	);


	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);

	const GLchar* fragmentSource = GLSL(
		in vec3 outputColor;
		in vec2 outUv;
		out vec4 outColor;
		uniform sampler2D myTexture;
		void main()
		{
			vec3 t = texture(myTexture, outUv).rgb;
			if (t.x >= .99) {
				discard;
			}
			else {
				outColor = vec4(t,1) * vec4(outputColor, 1.0);
			}
		}
	);
	//outColor = vec4(outputColor, 1.0);


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
	glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), 0);

	GLint colAttrib = glGetAttribLocation(shaderProgram, "color");
	glEnableVertexAttribArray(colAttrib);
	glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));

	GLint textureAttrib = glGetAttribLocation(shaderProgram, "uv");
	glEnableVertexAttribArray(textureAttrib);
	glVertexAttribPointer(textureAttrib, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));

	//TEXTURE
	glActiveTexture(GL_TEXTURE0);
	unsigned int w,h;
	texture = BMP::loadBMP_texture("../tree5.bmp",w,h,false);
	glUniform1i(glGetUniformLocation(shaderProgram, "myTexture"), 0);

	//MATRIX
	uniModel = glGetUniformLocation(shaderProgram,"model");
	modelMatrix = glm::mat4(1); // identity matrix
	glUniformMatrix4fv(uniModel,1, GL_FALSE, glm::value_ptr(modelMatrix));

	uniView = glGetUniformLocation(shaderProgram, "view");
	viewMatrix = glm::mat4(1); // identity matrix
	glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(viewMatrix));

	uniProj = glGetUniformLocation(shaderProgram, "proj");
	projMatrix = glm::perspective((double)glm::radians(45.0f), (double)1024 / (double)768, 0.1, (double)100);
	glUniformMatrix4fv(uniProj, 1, GL_FALSE, glm::value_ptr(projMatrix));
}


void SquareTextureScene::render()
{
	glBindVertexArray(vao);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	//glDrawArrays(GL_TRIANGLES, 0, 6);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);	

	glBindVertexArray(0);
}

void SquareTextureScene::Update(float dt, Camera *cam) {
		
	viewMatrix = cam->GetViewMatrix();
	glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(viewMatrix));

	modelMatrix = glm::rotate(modelMatrix, (float)glm::radians(0.01), glm::vec3(0, 1, 0));
	glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	projMatrix = glm::perspective((double)glm::radians(45.0f), (double)Settings::GetScreenWidth() / (double)Settings::GetScreenHeight(), 0.1, (double)100);
	glUniformMatrix4fv(uniProj, 1, GL_FALSE, glm::value_ptr(projMatrix));
}