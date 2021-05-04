#pragma once
#include "Scene.h"
#include "Camera.h"
#include <GL/glew.h>
#include <glm/glm.hpp>


class SquareTextureScene :
    public Scene
{
public:

	SquareTextureScene();
	void render() override;
	void Update(float dt, Camera *cam) override;

protected:

	GLuint vao;
	GLuint vbo;
	GLuint ebo;
	GLuint texture;
	GLuint uniModel;
	GLuint uniView;
	GLuint uniProj;

	glm::mat4 modelMatrix; 
	glm::mat4 viewMatrix; 
	glm::mat4 projMatrix; 

	GLuint vertexShader;
	GLuint fragmentShader;

	GLuint shaderProgram;
};

