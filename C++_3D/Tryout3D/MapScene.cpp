#include "MapScene.h"
#include "Settings.h"
#include <glm\gtc\type_ptr.hpp>
#include "bmp.h"
#include <iostream>

#define GLSL(shader) "#version 150 core\n" #shader

MapScene::MapScene() : Scene()
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);

	unsigned int w, h;
	unsigned char *map = BMP::loadBMP_raw("../heights.bmp", w, h, false);

	verticesWidth = w;
	verticesHeight = h;
	unsigned int size = w * h;
	//unsigned int w = 4, h = 4;

	vertices = new GLfloat[size * 8];


	for (unsigned  int y = 0; y < h; y++)
	{
		for (unsigned int x = 0; x < w; x++)
		{
			unsigned int idxPixel = (y * w + x);
			
			//Positions
			vertices[idxPixel * 8 + 0] = (GLfloat)(x - w*0.5f)*0.1f;
			vertices[idxPixel * 8 + 1] = (GLfloat)map[idxPixel * 3] * 0.1f;
			vertices[idxPixel * 8 + 2] = (GLfloat)(y - h * 0.5f) * 0.1f;
			//Color
			vertices[idxPixel * 8 + 3] = 1; 
			vertices[idxPixel * 8 + 4] = 1;
			vertices[idxPixel * 8 + 5] = 1;
			//UV
			vertices[idxPixel * 8 + 6] = x / (float)(w-1);
			vertices[idxPixel * 8 + 7] = y / (float)(h-1);					
		}
	}	

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * size * 8, vertices, GL_STATIC_DRAW);

	//Triangles
	glGenBuffers(1, &ebo);
	GLuint* triangles = new GLuint[size * 6]; // A quad for each vertex

	for (unsigned int y = 0; y < h-1; y++)
	{
		for (unsigned int x = 0; x < w-1; x++)
		{
			unsigned int idxPixel = y * w + x;

			unsigned int A = idxPixel;
			unsigned int B = A + 1;
			unsigned int C = A + w + 1;
			unsigned int D = A + w;



			triangles[idxPixel * 6 + 0] = A;
			triangles[idxPixel * 6 + 1] = B; 
			triangles[idxPixel * 6 + 2] = C;
			triangles[idxPixel * 6 + 3] = C;
			triangles[idxPixel * 6 + 4] = D;
			triangles[idxPixel * 6 + 5] = A;
		}
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * size * 6, triangles, GL_STATIC_DRAW);

	//---------------------------VERTEX SHADER --------------------------------
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
			outUv = uv;
			outputColor = color;
			gl_Position = proj *view* model* vec4(position, 1.0);
		}
	);
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);

	// -------------------------PIXEL SHADER----------------------------------------
	const GLchar* fragmentSource = GLSL(
		in vec3 outputColor;
		in vec2 outUv;

		out vec4 outColor;
		uniform sampler2D myTexture;
		void main()
		{			
			outColor =vec4(texture(myTexture, outUv).rgb,1) * vec4(outputColor, 1.0);
		}
	);

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
	unsigned int wT, hT;
	texture = BMP::loadBMP_texture("../top.bmp", wT, hT ,false);
	glUniform1i(glGetUniformLocation(shaderProgram, "myTexture"), 0);

	//MATRIX
	uniModel = glGetUniformLocation(shaderProgram, "model");
	modelMatrix = glm::mat4(1); // identity matrix
	glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	uniView = glGetUniformLocation(shaderProgram, "view");
	viewMatrix = glm::mat4(1); // identity matrix
	glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(viewMatrix));

	uniProj = glGetUniformLocation(shaderProgram, "proj");
	projMatrix = glm::perspective((double)glm::radians(45.0f), (double)Settings::GetScreenWidth() / (double)Settings::GetScreenHeight(), 0.1, (double)10000);
	glUniformMatrix4fv(uniProj, 1, GL_FALSE, glm::value_ptr(projMatrix));

	minimap = new Minimap();
}


void MapScene::render()
{
	glBindVertexArray(vao);
	glUseProgram(shaderProgram);	
	
	glActiveTexture(GL_TEXTURE0);
	glUniform1i(glGetUniformLocation(shaderProgram, "myTexture"), 0);

	glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(modelMatrix));
	glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(viewMatrix));
	glUniformMatrix4fv(uniProj, 1, GL_FALSE, glm::value_ptr(projMatrix));

	glDrawElements(GL_TRIANGLES, 1024*1024*6, GL_UNSIGNED_INT, 0);
	glEnable(GL_DEPTH_TEST);

	glBindVertexArray(0);
	
	minimap->render();
}

void MapScene::Update(float dt, Camera *cam) {
	int x = (int)cam->Position.x * 10 + verticesWidth * 0.5f;
	int y = (int)cam->Position.z * 10 + verticesHeight * 0.5f;
	int idx = (y * verticesWidth + x) * 8 + 1;
	if(idx >= 0 && idx <= verticesWidth * verticesHeight * 8 && x >=0 && x<= verticesWidth && y>=0 && y<= verticesHeight)
		cam->Position.y = vertices[idx] + 2;
	viewMatrix = cam->GetViewMatrix();
	glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(viewMatrix));

	modelMatrix = glm::rotate(modelMatrix, (float)glm::radians(0.00), glm::vec3(0, 1, 0));
	glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	projMatrix = glm::perspective((double)glm::radians(45.0f), (double)Settings::GetScreenWidth() / (double)Settings::GetScreenHeight(), 0.1, (double)10000);
	glUniformMatrix4fv(uniProj, 1, GL_FALSE, glm::value_ptr(projMatrix));
	minimap->Update(dt, cam);
	minimap->UpdatePlayerPos(sf::Vector2f((cam->Position.x*10.0f+verticesWidth*0.5f)/1024.0f, (cam->Position.z*10 + verticesHeight*0.5f) / 1024.0f));
	//minimap->UpdatePlayerPos(sf::Vector2f((cam->Position.x + 50.0f) / 100.0f, (cam->Position.z + 50.0f) / 100.0f));
	//std::cout << (cam->Position.x + 50.0f) / 100.0f << " " << (cam->Position.z + 50.0f) / 100.0f << std::endl;
	std::cout << (cam->Position.x * 10.0f + verticesWidth * 0.5f) / 1024.0f << " " << (cam->Position.z * 10 + verticesHeight*0.5f) / 1024.0f << std::endl;

}