#pragma once
#include "Scene.h"
#include <GL/glew.h>

class TriangleScene : public Scene
{
	public :

		TriangleScene();
		void render() override;

	protected :

		GLuint vao;
		GLuint vbo;

		GLuint vertexShader;
		GLuint fragmentShader;

		GLuint shaderProgram;
};