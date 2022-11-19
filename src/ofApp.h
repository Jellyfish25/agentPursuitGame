#pragma once
#include "ofMain.h"
#include "ofxGui.h"
#include "Player.h"
#include "Agent.h"
#include "AgentEmitter.h"
#include "BulletEmitter.h"
#include "ExplosionEmitter.h"
#include "Emitter.h"


class ofApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();
	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);

	Player playerObj;
	float shootStart;
	float shootEnd;
	float flopStart;
	float flopEnd;
	float particleTimer;
	bool bShoot = false;
	map<int, bool> keyMap;
	ofSoundPlayer movementAudio;
	ofSoundPlayer shootAudio;
	ofSoundPlayer hitAudio;
	ofSoundPlayer enemyHitAudio;

private:
	bool bHide = false;
	bool startGame = false;
	bool endGame = false;

	// player menu sliders/toggles
	ofxFloatSlider playerSpeed = 1.0;
	ofxFloatSlider playerScale = 1.0;
	ofxFloatSlider particleSpeed = 1.0;
	ofxIntSlider playerEnergy;
	ofxToggle showPlayerSprite;
	ofxToggle showParticleSprite;
	ofxToggle bHeading;
	ofxFloatSlider dampingSlider;
	ofxButton levelOne;
	ofxButton levelTwo;
	ofxButton levelThree;
	// agent menu sliders/toggles
	ofxFloatSlider enemySpeedSlider = 1.0;
	ofxIntSlider totalAgents;
	ofxFloatSlider rateSlider;
	ofxFloatSlider lifeSpanSlider;
	ofxFloatSlider agentScale;
	ofxFloatSlider shootDelay;
	ofxToggle showAgentSprite;

	ofxPanel guiPanel;
	ofImage menuScreenImage;
	ofImage gameOverImage;
	ofImage backgroundImage;

	AgentEmitter* agentEmitter = NULL;
	BulletEmitter* bulletEmitter = NULL;
	ExplosionEmitter* explosionEmitter = NULL;
	ofTrueTypeFont myfont;
};