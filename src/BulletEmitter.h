#pragma once
#include "Emitter.h"
#include "Bullet.h"
#include "Player.h"

class BulletEmitter : public Emitter {
public:
	void moveSprite(Sprite* sprite) {
		sprite->bShowImage = showSprite;
		sprite->pos += (sprite->heading() * speed);
		sprite->scaleValue = playerRef->scaleValue;
	}

	// virtual function to spawn sprite (can be overloaded)
	void spawnSprite() {
		Bullet bullet = Bullet(glm::vec3(-50, 50, 0), glm::vec3(50, 50, 0), glm::vec3(0, -50, 0));
		if (haveChildImage) {
			bullet.setImage(childImage);
		}

		glm::vec3 bulletPosition = glm::vec3(playerRef->getMatrix() * glm::vec4(playerRef->verts[2], 1.0));
		if (playerRef->bShowImage) {
			bulletPosition = glm::vec3(playerRef->getMatrix() * glm::vec4(glm::vec3(-20, -20 - playerRef->spriteImage.getHeight() / 2, 0), 1.0));
		}

		bullet.pos = bulletPosition;
		bullet.velocity = velocity;
		bullet.lifespan = -1;

		bullet.rotation = playerRef->rotation;
		bullet.scaleValue = playerRef->scaleValue;

		sys->add(bullet);
	}

	float speed = 1.0;
	float scale = 1.0;
	//int delay = 1.0;
	bool showSprite;
	Player* playerRef = NULL;
};