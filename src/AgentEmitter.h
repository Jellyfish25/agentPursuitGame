#pragma once
#include "Emitter.h"
#include "Agent.h"

class AgentEmitter : public Emitter {
public:
	void moveSprite(Sprite* sprite) {
		sprite->bShowImage = showSprite;
		glm::vec3 direction = glm::normalize(playerRef->pos - sprite->pos);
		float angle = glm::dot(sprite->heading(), direction);
		float dirAngle = glm::dot(glm::vec3(-sprite->heading().y, sprite->heading().x, sprite->heading().z), direction);

		if (dirAngle < 0) {
			sprite->rotation -= (glm::acos(angle) * speed);
		}
		else {
			sprite->rotation += (glm::acos(angle) * speed);
		}

		//update 100 to increase/decrease acceleration
		sprite->acceleration = (sprite->heading() * (speed * 100));
		sprite->integrate();
		sprite->scaleValue = scaleValue;
	}

	// virtual function to spawn sprite (can be overloaded)
	void spawnSprite() {
		for (int i = 0; i < nAgent; i++) {
			Agent agent = Agent(glm::vec3(-50, 50, 0), glm::vec3(50, 50, 0), glm::vec3(0, -50, 0));
			if (haveChildImage) {
				agent.setImage(childImage);
			}
			agent.velocity = velocity;
			agent.lifespan = lifespan;
			//agent.pos = glm::vec3(ofRandom(playerRef->pos.x, ofGetWindowWidth()), ofRandom(playerRef->pos.y, ofGetWindowHeight()), 0);
			agent.pos = glm::vec3(rand() % ofGetWindowWidth() - 20, rand() % ofGetWindowHeight() - 20, 0);
			agent.rotation = rand() % 360;
			agent.scaleValue = scaleValue;
			agent.damping = dampingValue;
			//agent.pos = pos;
			agent.birthtime = ofGetElapsedTimeMillis();
			sys->add(agent);
		}
	}

	int speed = 1;
	bool showSprite;
	int nAgent;
	float dampingValue;
	Player* playerRef = NULL;
};