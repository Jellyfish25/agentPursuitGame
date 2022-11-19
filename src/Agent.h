#pragma once
#include "Sprite.h"

class Agent : public Sprite {
public:
	Agent() {}
	Agent(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3) {
		verts.push_back(p1);
		verts.push_back(p2);
		verts.push_back(p3);

		if (!enemyImage.load("images/knife.png")) {
			cout << "Can't load image" << endl;
			ofExit();
		}

		spriteImage = enemyImage;
		spriteImage.resize(enemyImage.getWidth() / 3, enemyImage.getHeight() / 3);
		color = ofColor::red;
		birthtime = ofGetElapsedTimeMillis();
	}

	ofImage enemyImage;
};