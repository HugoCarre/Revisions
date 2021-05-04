#include "Minimap.h"

#include "Settings.h"
#include <glm\gtc\type_ptr.hpp>
#include "bmp.h"
#include <iostream>

#define GLSL(shader) "#version 150 core\n" #shader

Minimap::Minimap() : Scene()
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);

	unsigned int w, h;
	w = 4;
	h = 4;
	verticesWidth = w;
	verticesHeight = h;
	unsigned int size = w * h;

	vertices = new GLfloat[size * 5];

	for (unsigned int y = 0; y < h; y++)
	{
		for (unsigned int x = 0; x < w; x++)
		{
			unsigned int idxPixel = (y * w + x);

			//Positions
			vertices[idxPixel * 5 + 0] = (GLfloat)(x)*0.2f -0.95f;
			vertices[idxPixel * 5 + 1] = (GLfloat)(y)*0.2f + 0.35f;
			vertices[idxPixel * 5 + 2] = 0;

			//UV
			vertices[idxPixel * 5 + 3] = x / (float)(w - 1);
			vertices[idxPixel * 5 + 4] = y / (float)(h - 1);
		}
	}

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * size * 5, vertices, GL_STATIC_DRAW);

	//Triangles
	glGenBuffers(1, &ebo);
	GLuint* triangles = new GLuint[size * 6]; // A quad for each vertex

	for (unsigned int y = 0; y < h - 1; y++)
	{
		for (unsigned int x = 0; x < w - 1; x++)
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
		in vec2 uv;

		out vec2 outUv;

		uniform mat4 proj;
		void main()
		{
			outUv = uv * vec2(1.02,1.02) - 0.01;
			gl_Position = vec4(position, 1.0);
		}
	);
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);

	// -------------------------PIXEL SHADER----------------------------------------
	const GLchar* fragmentSource = GLSL(
		in vec2 outUv;

		out vec4 outColor;
		uniform sampler2D myTexture;

		uniform vec2 playerPos;
		void main()
		{			
			if (outUv.x < 0.01 || outUv.x > 1-0.01 || outUv.y < 0.01 || outUv.y > 1-0.01) {
				outColor = vec4(0,0,0,1);
			}
			else {
				vec2 dir = vec2(outUv.x - playerPos.x, outUv.y - playerPos.y);
				float dist = sqrt(dir.x * dir.x + dir.y * dir.y);
				if (dist < 0.01) {
					outColor = vec4(1, 0, 0, 1);
				}
				else {
					outColor = vec4(texture(myTexture, outUv).rgb, 1);
				}
			}					
		}
	);

	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);

	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	glUseProgram(shaderProgram);

	GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
	glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);

	GLint textureAttrib = glGetAttribLocation(shaderProgram, "uv");
	glEnableVertexAttribArray(textureAttrib);
	glVertexAttribPointer(textureAttrib, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));

	//TEXTURE
	glActiveTexture(GL_TEXTURE0);
	unsigned int wT, hT;
	texture = BMP::loadBMP_texture("../top.bmp", wT, hT, false);
	glUniform1i(glGetUniformLocation(shaderProgram, "myTexture"), 0);

	//MINIMAP player
	glUniform2f(glGetUniformLocation(shaderProgram,"playerPos"), 2, 2);

}


void Minimap::render()
{
	glBindVertexArray(vao);
	glUseProgram(shaderProgram);	

	glActiveTexture(GL_TEXTURE0);
	glUniform1i(glGetUniformLocation(shaderProgram, "myTexture"), 0);
	glUniform2f(glGetUniformLocation(shaderProgram, "playerPos"), playerPos.x, playerPos.y);

	glDrawElements(GL_TRIANGLES, 4 * 4 * 6, GL_UNSIGNED_INT, 0);
	glEnable(GL_DEPTH_TEST);

	glBindVertexArray(0);
}

void Minimap::Update(float dt, Camera* cam) {
	/*UpdatePlayerPos(sf::Vector2f((cam->Position.x + 50.0f) / 100.0f, (cam->Position.z + 50.0f) / 100.0f));

	std::cout << (cam->Position.x + 50.0f) / 100.0f << " " << (cam->Position.z + 50.0f) / 100.0f << std::endl;*/
}

void Minimap::UpdatePlayerPos(sf::Vector2f pos) {
	playerPos = pos;
}