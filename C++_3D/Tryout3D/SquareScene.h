#pragma once

#include "Scene.h"
#include <GL/glew.h>

class SquareScene : public Scene
{
public:

	SquareScene();
	void render() override;

protected:

	GLuint vao;
	GLuint vbo;
	GLuint ebo;

	GLuint vertexShader;
	GLuint fragmentShader;

	GLuint shaderProgram;
};

