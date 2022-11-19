#pragma once
#include "ofMain.h"
#include "Sprite.h"

class Particle : public Sprite {
public:
	Particle() {}
	Particle(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3) {
		verts.push_back(p1);
		verts.push_back(p2);
		verts.push_back(p3);

		std::stringstream imageRoute;
		imageRoute << "images/explosionImage" << (rand() % 5) + 1 << ".png";
		if (!ParticleImage.load(imageRoute.str())) {
			cout << "Can't load image" << endl;
			ofExit();
		}

		spriteImage = ParticleImage;
		spriteImage.resize(ParticleImage.getWidth() / 4, ParticleImage.getHeight() / 4);
		color = ofColor::red;
	}
	ofImage ParticleImage;
};
