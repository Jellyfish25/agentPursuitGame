#include "ofApp.h"
#include "Player.h"
#include "Agent.h"


//--------------------------------------------------------------
void ofApp::setup() {
	guiPanel.setup();

	//player toggles/sliders
	guiPanel.add(playerSpeed.setup("Player Speed", 10.0, 0.5, 20.0));		// speed slider (floating point)
	guiPanel.add(playerScale.setup("Scale", 0.5, 0.5, 2.0));			// scale slider (single float value, uniform scale)
	guiPanel.add(playerEnergy.setup("Energy Level", 10, 1, 100));
	guiPanel.add(showPlayerSprite.setup("show player sprite", true));			// show sprite (button toggle)
	guiPanel.add(showParticleSprite.setup("show particle sprite", true));
	guiPanel.add(bHeading.setup("show heading vector", false)); // show heading vector (button toggle)
	guiPanel.add(particleSpeed.setup("Particle Speed", 5.0, 1.0, 15.0));
	guiPanel.add(shootDelay.setup("Shoot Delay", 0.5, 0.1, 2.5));
	//enemy toggles/sliders
	guiPanel.add(dampingSlider.setup("Agent Damping", 0.99, 0.5, 1.0));
	guiPanel.add(enemySpeedSlider.setup("Agent Speed", 2.0, 1.0, 20.0));
	guiPanel.add(agentScale.setup("Agent Scale", 0.5, 0.5, 2.0));
	guiPanel.add(totalAgents.setup("Total Agents", 1, 1, 20));
	guiPanel.add(rateSlider.setup("Spawn Rate", 1.0, 0.5, 5.0));
	guiPanel.add(lifeSpanSlider.setup("Life Span", 5.0, 0.0, 20.0));
	guiPanel.add(showAgentSprite.setup("show agent sprite", true));
	guiPanel.add(levelOne.setup("Level One", false));
	guiPanel.add(levelTwo.setup("Level Two", false));
	guiPanel.add(levelThree.setup("Level Three", false));

	playerObj = Player(playerEnergy, glm::vec3(-50, 50, 0), glm::vec3(50, 50, 0), glm::vec3(0, -50, 0), playerEnergy);
	playerObj.pos = glm::vec3(ofGetWindowWidth() / 2.0 - 100, ofGetWindowHeight() / 2.0 - 100, 0);
	ofResetElapsedTimeCounter();

	if (!menuScreenImage.load("images/menuScreen.png")) {
		cout << "Can't load menu screen image" << endl;
		exit();
	}

	if (!backgroundImage.load("images/backgroundImage.png")) {
		cout << "Can't load background image" << endl;
		exit();
	}

	if (!gameOverImage.load("images/endScreen.png")) {
		cout << "Can't load game over image" << endl;
		exit();
	}

	//load font
	if (!myfont.load("abril.ttf", 28)) {
		cout << "Can't load font" << endl;
		exit();
	}

	//load audio
	if (!movementAudio.load("fishFlop.mp3")) {
		cout << "Can't load movement audio" << endl;
		exit();
	}
	movementAudio.setVolume(0.5);

	if (!shootAudio.load("shootAudio.mp3")) {
		cout << "Can't load shoot audio" << endl;
		exit();
	}
	shootAudio.setMultiPlay(true);
	shootAudio.setVolume(0.5);
	shootAudio.setSpeed(1.5);

	if (!hitAudio.load("hitAudio.mp3")) {
		cout << "Can't load hit audio" << endl;
		exit();
	}
	hitAudio.setMultiPlay(true);
	hitAudio.setVolume(0.5);

	if (!enemyHitAudio.load("enemyHit.mp3")) {
		cout << "Can't load enemy hit audio" << endl;
		exit();
	}
	enemyHitAudio.setMultiPlay(true);
	enemyHitAudio.setVolume(0.5);

	// initialize agent emitter
	agentEmitter = new AgentEmitter();
	agentEmitter->pos = glm::vec3(ofGetWindowWidth() / 2.0, ofGetWindowHeight() / 2.0, 0);
	agentEmitter->drawable = false;
	agentEmitter->playerRef = &playerObj;

	// initialize particleEmitter
	bulletEmitter = new BulletEmitter();
	bulletEmitter->pos = playerObj.pos;
	bulletEmitter->drawable = false;
	bulletEmitter->playerRef = &playerObj;

	explosionEmitter = new ExplosionEmitter();
	explosionEmitter->playerRef = &playerObj;
	explosionEmitter->drawable = false;

	flopStart = ofGetElapsedTimeMillis();
	shootStart = ofGetElapsedTimeMillis();
}

//--------------------------------------------------------------
void ofApp::update() {
	if (levelOne) {
		playerSpeed = 10.0;
		playerScale = 0.5;
		playerEnergy = 10;
		particleSpeed = 5.0;
		shootDelay = 0.25;
		enemySpeedSlider = 5.0;
		agentScale = 0.5;
		totalAgents = 1;
		rateSlider = 1.0;
		lifeSpanSlider = 5.0;
		dampingSlider = 0.99;
	}
	if (levelTwo) {
		playerSpeed = 12.0;
		playerScale = 0.5;
		playerEnergy = 15;
		particleSpeed = 5.0;
		shootDelay = 0.5;
		enemySpeedSlider = 9.0;
		agentScale = 0.5;
		totalAgents = 1;
		rateSlider = 0.85;
		lifeSpanSlider = 7;
		dampingSlider = 0.99;
	}
	if (levelThree) {
		playerSpeed = 12.0;
		playerScale = 0.5;
		playerEnergy = 30.0;
		particleSpeed = 15.0;
		shootDelay = 0.25;
		enemySpeedSlider = 9.0;
		agentScale = 0.5;
		totalAgents = 2;
		rateSlider = 0.6;
		lifeSpanSlider = 10.0;
		dampingSlider = 0.99;
	}

	// Prevent player interaction until game started
	if (!startGame || endGame) {
		return;
	}

	// update agent emitter
	agentEmitter->nAgent = totalAgents;
	agentEmitter->setRate(rateSlider);
	agentEmitter->setLifespan(lifeSpanSlider);
	agentEmitter->scaleValue = agentScale;
	agentEmitter->dampingValue = dampingSlider;
	agentEmitter->speed = enemySpeedSlider;
	agentEmitter->update();

	bulletEmitter->rotation = playerObj.rotation;
	bulletEmitter->speed = particleSpeed;
	bulletEmitter->update();

	explosionEmitter->showSprite = showPlayerSprite;
	explosionEmitter->update();

	// check if the bullet collides with agent
	for (int i = 0; i < bulletEmitter->sys->sprites.size(); i++) {
		Sprite b = bulletEmitter->sys->sprites[i];

		if (b.insideScreen(b.pos)) {
			for (int j = 0; j < agentEmitter->sys->sprites.size(); j++) {
				Sprite a = agentEmitter->sys->sprites[j];

				/*
				if (a.inside(b.pos)) {
					agentEmitter->sys->remove(j);
					bulletEmitter->sys->remove(i);
					playerObj.nEnergy++;
					i--;
					break;
				}
				*/
				int distance = glm::distance(a.pos, b.pos);
				int contactDistanceW = 0;
				int contactDistanceH = 0;

				if (a.bShowImage) {
					contactDistanceW = (a.scaleValue * (a.spriteImage.getWidth() / 2)) + (b.scaleValue * (b.spriteImage.getWidth() / 2)) - 50;
					contactDistanceH = (a.scaleValue * (a.spriteImage.getHeight() / 2)) + (b.scaleValue * (b.spriteImage.getHeight() / 2)) - 50;
				}
				else {
					contactDistanceW = (a.scaleValue * 50) + (b.scaleValue * 50);
					contactDistanceH = (a.scaleValue * 50) + (b.scaleValue * 50);
				}

				if (distance < contactDistanceW || distance < contactDistanceH) {
					agentEmitter->sys->remove(j);
					bulletEmitter->sys->remove(i);
					playerObj.nEnergy++;
					i--;
					enemyHitAudio.play();
					break;
				}
			}
		}
		else {
			bulletEmitter->sys->remove(i);
			i--;
		}
	}

	// check if agent collides with player
	for (int i = 0; i < agentEmitter->sys->sprites.size(); i++) {
		Sprite s = agentEmitter->sys->sprites[i];

		/*
		if (playerObj.inside(s.pos)) {
			playerObj.nEnergy -= 1;
			agentEmitter->sys->remove(i);
			i--;
		}
		*/

		int distance = glm::distance(s.pos, playerObj.pos);
		int contactDistanceW = 0;
		int contactDistanceH = 0;

		if (s.bShowImage) {
			contactDistanceW = (s.scaleValue * (s.spriteImage.getWidth() / 2)) + (playerObj.scaleValue * (playerObj.spriteImage.getWidth() / 2)) - 50;
			contactDistanceH = (s.scaleValue * (s.spriteImage.getHeight() / 2)) + (playerObj.scaleValue * (playerObj.spriteImage.getHeight() / 2)) - 50;
		}
		else {
			contactDistanceW = (s.scaleValue * 50) + (playerObj.scaleValue * 50);
			contactDistanceH = (s.scaleValue * 50) + (playerObj.scaleValue * 50);
		}

		if (distance < contactDistanceW || distance < contactDistanceH) {
			playerObj.nEnergy -= 1;
			agentEmitter->sys->remove(i);
			i--;
			explosionEmitter->spawnSprite();
			hitAudio.play();
		}

	}

	// display the sprites (both for player and agents)
	playerObj.bShowImage = showPlayerSprite;
	agentEmitter->showSprite = showAgentSprite;
	explosionEmitter->showSprite = showAgentSprite;
	bulletEmitter->showSprite = showParticleSprite;

	// update player object
	playerObj.nEnergy += playerEnergy - playerObj.maxEnergy;
	playerObj.maxEnergy = playerEnergy;
	playerObj.scaleValue = playerScale; // update the scale value of the player object

	// rotate the player left and right (keys: left & right)
	if (keyMap[OF_KEY_LEFT]) {
		playerObj.rotation -= playerSpeed;
	}
	else if (keyMap[OF_KEY_RIGHT]) {
		playerObj.rotation += playerSpeed;
	}

	playerObj.acceleration = glm::vec3(0, 0, 0);
	playerObj.integrate();
	// move the player up and down (keys: up & down)
	if (keyMap[OF_KEY_UP]) {
		glm::vec3 newPos = playerObj.pos + (float)playerSpeed * playerObj.heading();
		if (playerObj.insideScreen(newPos)) {
			playerObj.pos = newPos;
			//playerObj.acceleration = (playerObj.heading() * 50 * (float)playerSpeed);
			//playerObj.integrate();
			flopEnd = ofGetElapsedTimeMillis();
			if ((flopEnd - flopStart) / 1000 > 0.1) {
				playerObj.spriteImage.mirror(false, true);
				flopStart = flopEnd;

				if (!movementAudio.isPlaying()) {
					movementAudio.play();
				}
			}
		}
	}
	else if (keyMap[OF_KEY_DOWN]) {
		glm::vec3 newPos = playerObj.pos - (float)playerSpeed * playerObj.heading();
		if (playerObj.insideScreen(newPos)) {
			playerObj.pos = newPos;

			flopEnd = ofGetElapsedTimeMillis();
			if ((flopEnd - flopStart) / 1000 > 0.1) {
				playerObj.spriteImage.mirror(false, true);
				flopStart = flopEnd;

				if (!movementAudio.isPlaying()) {
					movementAudio.play();
				}
			}
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	// If the game did not start, draw the menu screen image
	if (!startGame) {
		ofSetColor(ofColor::white);
		menuScreenImage.resize(ofGetWindowWidth(), ofGetWindowHeight());
		menuScreenImage.draw(0, 0);
	}
	else if (!endGame) {
		ofSetColor(ofColor::white);
		backgroundImage.resize(ofGetWindowWidth(), ofGetWindowHeight());
		backgroundImage.draw(0, 0);

		if (bHeading) {
			ofSetColor(ofColor::red);
			playerObj.drawHeading();
		}

		playerObj.draw();
		agentEmitter->draw();
		bulletEmitter->draw();
		explosionEmitter->draw();
		ofSetColor(ofColor::black);

		string str;
		str = "current energy: " + std::to_string(playerObj.nEnergy) + "\n";
		str += "max energy: " + std::to_string(playerObj.maxEnergy) + "\n";
		ofDrawBitmapString(str, ofGetWindowWidth() / 2 - 100, 15);

		str = "Elapsed time: " + std::to_string(playerObj.age()) + "\n";
		str += "Frame Rate: " + std::to_string(ofGetFrameRate());
		ofDrawBitmapString(str, ofGetWindowWidth() - 185, 15);

		if (playerObj.nEnergy <= 0) {
			endGame = true;
			bShoot = false;
			agentEmitter->stop();
			bulletEmitter->stop();
			playerObj.surviveDuration = playerObj.age();
			movementAudio.stop();
			shootAudio.stop();
		}
	}
	else {
		ofSetColor(ofColor::white);
		gameOverImage.resize(ofGetWindowWidth(), ofGetWindowHeight());
		gameOverImage.draw(0, 0);
		string playerTime = "					GAME OVER!\n";
		playerTime += "TOTAL TIME SURVIVED: " + std::to_string(playerObj.surviveDuration) + "s" + "\n";
		playerTime += "PRESS Q TO GO BACK TO MENU";

		myfont.drawString(playerTime, ofGetWindowWidth() / 2 - 300, (ofGetWindowHeight() / 2) + 50);
		//ofDrawBitmapString(playerTime, ofGetWindowWidth() / 2 - 180, ofGetWindowHeight() / 2 - 180);
	}

	if (!bHide) {
		guiPanel.draw();
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	keyMap[key] = true;

	// hide the gui panel when pressing 'h' (toggle)
	if (keyMap['h'] && !bHide) {
		bHide = true;
	}
	else if (keyMap['h'] && bHide) {
		bHide = false;
	}

	// if the user presses spacebar and the game hasn't started, start game
	if (keyMap[' '] && !startGame) {
		playerObj.birthtime = ofGetElapsedTimeMillis();
		startGame = true;
		agentEmitter->start();
	}

	// if the user presses 'q', return to menu
	if (keyMap['q'] && startGame && endGame) {
		startGame = false;
		endGame = false;
		setup();
	}

	if (!startGame || endGame) {
		return;
	}

	//press c to shoot
	if (keyMap['c'] && !bShoot) {
		// cout << "bullet should start" << endl;
		shootEnd = ofGetElapsedTimeMillis();
		if ((shootEnd - shootStart) / 1000 >= shootDelay) {
			bulletEmitter->spawnSprite();
			shootAudio.play();
			bShoot = true;
			shootStart = shootEnd;
		}
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
	keyMap[key] = false;

	if (!keyMap['c']) {
		bShoot = false;
	}
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) { // task 2
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {
}