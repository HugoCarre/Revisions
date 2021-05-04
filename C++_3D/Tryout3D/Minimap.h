#pragma once
#include "Scene.h"
#include <SFML\System\Vector2.hpp>
class Minimap :
    public Scene
{
public:
	Minimap();
	void render() override;
	void Update(float dt, Camera* cam) override;
	void UpdatePlayerPos(sf::Vector2f pos);

protected:

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
	sf::Vector2f playerPos;

	unsigned int verticesWidth, verticesHeight;
};

