#pragma once
#include "Emitter.h"
#include "Agent.h"
#include "Particle.h"

class ExplosionEmitter : public Emitter {
public:
	void moveSprite(Sprite* sprite) {
		sprite->rotation += sin(ofGetElapsedTimef()) * ofDegToRad(45);
		sprite->pos += (sprite->heading() * speed);
	}

	// virtual function to spawn sprite (can be overloaded)
	void spawnSprite() {
		for (int i = 0; i < particleAmount; i++) {
			Particle particle = Particle(glm::vec3(-50, 50, 0), glm::vec3(50, 50, 0), glm::vec3(0, -50, 0));
			if (haveChildImage) {
				cout << "image was set" << endl;
				particle.setImage(childImage);
			}

			particle.lifespan = 2.0;
			particle.bShowImage = showSprite;
			glm::vec3 centerPos = playerRef->pos;
			particle.pos = glm::vec3(centerPos.x + rand() % 25, centerPos.y + rand() % 25, 1);
			particle.rotation = rand() % 360;
			particle.scaleValue = playerRef->scaleValue;
			particle.birthtime = ofGetElapsedTimeMillis();

			sys->add(particle);
		}
	}

	int speed = 1;
	bool showSprite;
	int particleAmount = 25;
	Player* playerRef = NULL;
};