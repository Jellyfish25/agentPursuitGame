#pragma once
#include "Sprite.h"

class Player : public Sprite {
public:
	Player() {}
	Player(int energy, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, int initialEnergy) {
		nEnergy = energy;
		verts.push_back(p1);
		verts.push_back(p2);
		verts.push_back(p3);

		if (!playerImage.load("images/fish.png")) {
			cout << "Can't load image" << endl;
			ofExit();
		}

		spriteImage = playerImage;
		spriteImage.resize(playerImage.getWidth() * 4, playerImage.getHeight() * 4);
		color = ofColor::blue;
		nEnergy = initialEnergy;
		maxEnergy = initialEnergy;
	}

	void updateEnergy(int amount) {
		nEnergy += amount;
	}

	int nEnergy = 1;
	int maxEnergy = 1;
	float surviveDuration = 0;
	ofImage playerImage;
};