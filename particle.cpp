#include "particle.h"

Particle::Particle(glm::vec2 pos, glm::vec2 dir) : position(pos) {
	std::mt19937 rng(std::random_device{}());
	std::uniform_real_distribution<> dist(0, 2 * 3.14159265358);
	std::uniform_real_distribution<> dist2(-0.436332313, 0.436332313);
	rotate_angle = dist(rng);
	GLfloat deviation = dist2(rng);
	direction = glm::vec2(dir.x * cos(deviation) - dir.y * sin(deviation),
		dir.x * sin(deviation) + dir.y * cos(deviation));
}

void Particle::update(float dt) {
	if (!done()) {
		lifetime -= dt;
		position += direction * speed * pow(2.0f, lifetime);
		scale += 0.75f * (5.0f - lifetime);
	}
}

bool Particle::done() const {
	return lifetime <= 0.0f;
}

ParticleGenerator::ParticleGenerator(Shader shader, GLint texId, glm::mat4 proj) : shader(shader), textureId(texId) {
	GLuint VBO;
	GLfloat particle_quad[] = {
		// pos      // tex
		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,

		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f
	};
	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(this->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(particle_quad), particle_quad, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
	glBindVertexArray(0);

	this->shader = shader;
	this->shader.setInt("sprite", textureId);
	this->projection = proj;
}

void ParticleGenerator::create(glm::vec2 mouse_pos, GLint number) {
	auto fix_pos = glm::vec2(350.0f, 550.0f);
	while (number > 0) {
		this->particles.push_back(Particle(fix_pos, glm::normalize(mouse_pos - fix_pos)));
		number--;
	}
}

void ParticleGenerator::update(GLfloat dt) {
	// Update smoke particles
	for (Particle& particle : particles) {
		particle.update(dt);
	}

	// Remove done particles
	for (uint32_t i(0); i < particles.size();) {
		if (particles[i].done()) {
			std::swap(particles[i], particles.back());
			particles.pop_back();
		}
		else {
			++i;
		}
	}
}

void ParticleGenerator::draw() {
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureId);

	this->shader.use();
	this->shader.setMat4("projection", this->projection);

	for (Particle particle : particles) {
		auto model = glm::translate(glm::mat4(1.0f), glm::vec3(particle.position, 0.0f));
		model = glm::translate(model, glm::vec3(0.5f * particle.scale, 0.5f * particle.scale, 0.0f));
		model = glm::rotate(model, particle.rotate_angle, glm::vec3(0.0, 0.0, 1.0));
		model = glm::translate(model, glm::vec3(-0.5f * particle.scale, -0.5f * particle.scale, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f) * particle.scale);
		
		this->shader.setFloat("lifetime", particle.lifetime);
		this->shader.setMat4("model", model);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);
	}
}