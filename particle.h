#pragma once
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/common.hpp>
#include <math.h>

#include "shader.h"
#include <random>  

struct Particle {
	glm::vec2 position;
	glm::vec2 direction;
	GLfloat speed = 0.2f;
	GLfloat scale = 100.0f;
	GLfloat lifetime = 5.3f;
	GLfloat rotate_angle;

	Particle(glm::vec2 pos, glm::vec2 dir);
	void update(float dt);
	bool done() const;
};


class ParticleGenerator {
public:
	ParticleGenerator(Shader shader, GLint texture, glm::mat4 proj);
	void create(glm::vec2 pos, GLint number);
	void update(GLfloat dt);
	void draw();
private:
	std::vector<Particle> particles;
	Shader shader;
	GLint textureId;
	GLuint VAO;
	glm::mat4 projection;
};

