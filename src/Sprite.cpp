#include "Sprite.h"

void TriangleShape::draw() {
	ofSetColor(color);
	ofFill();
	ofPushMatrix();
	ofMultMatrix(getMatrix());
	ofDrawTriangle(verts[0], verts[1], verts[2]);
	ofPopMatrix();
}

void Sprite::draw() {
	if (bShowImage) {
		ofSetColor(color);
		ofFill();
		ofPushMatrix();
		ofMultMatrix(getMatrix());
		ofSetColor(ofColor::white);
		spriteImage.draw(-spriteImage.getWidth() / 2, -spriteImage.getHeight() / 2);
		ofPopMatrix();
	}
	else {
		TriangleShape::draw();
	}
}