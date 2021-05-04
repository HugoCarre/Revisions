#pragma once
#include "Scene.h"
#include <GL/glew.h>

class GradiantTriangleScene : public Scene
{
public:

	GradiantTriangleScene();
	void render() override;

protected:

	GLuint vao;
	GLuint vbo;

	GLuint vertexShader;
	GLuint fragmentShader;

	GLuint shaderProgram;
};

