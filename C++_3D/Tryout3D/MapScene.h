#pragma once
#include "Scene.h"
#include "Camera.h"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "Minimap.h"

class MapScene :
    public Scene
{
public:
	MapScene();
	void render() override;
	void Update(float dt, Camera *cam) override;

protected:
	Minimap*minimap;
	GLuint vao;
	GLuint vbo;
	GLuint ebo;
	GLuint texture;
	GLuint uniModel;
	GLuint uniView;
	GLuint uniProj;

	GLfloat* vertices;

	glm::mat4 modelMatrix;
	glm::mat4 viewMatrix;
	glm::mat4 projMatrix;

	GLuint vertexShader;
	GLuint fragmentShader;

	GLuint shaderProgram;

	unsigned int verticesWidth, verticesHeight;
};

