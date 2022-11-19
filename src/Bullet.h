#pragma once
#include "Sprite.h"

class Bullet : public Sprite {
public:
	Bullet() {}
	Bullet(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3) {
		verts.push_back(p1);
		verts.push_back(p2);
		verts.push_back(p3);

		if (!ParticleImage.load("images/Bullet.png")) {
			cout << "Can't load image" << endl;
			ofExit();
		}

		spriteImage = ParticleImage;
		spriteImage.resize(ParticleImage.getWidth() / 5, ParticleImage.getHeight() / 5);
		color = ofColor::blue;
	}
	ofImage ParticleImage;
};