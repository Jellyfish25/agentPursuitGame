#pragma once
#include "ofMain.h"

class Shape {
public:
	Shape() {}
	virtual void draw() {}
	virtual bool inside(glm::vec3 p) {
		return true;
	}
	bool insideScreen(glm::vec3 updatedPosition) {
		return updatedPosition.x > 0 && updatedPosition.y > 0 && updatedPosition.x < ofGetWindowWidth() && updatedPosition.y < ofGetWindowHeight();
	}
	glm::mat4 getMatrix() {
		glm::mat4 trans = glm::translate(glm::mat4(1.0), glm::vec3(pos));
		glm::mat4 rot = glm::rotate(glm::mat4(1.0), glm::radians(rotation), glm::vec3(0, 0, 1));
		glm::mat4 scale = glm::scale(glm::mat4(1.0), glm::vec3(scaleValue, scaleValue, 1.0));
		return (trans * rot * scale); // note: scale -> rotate -> translate
	}
	glm::vec3 pos;
	float rotation = 0.0;    // degrees 
	float scaleValue = 1.0;
	ofColor color = ofColor::blue;
	vector<glm::vec3> verts;
};

class TriangleShape : public Shape {
public:
	TriangleShape() {}
	TriangleShape(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3) {
		verts.push_back(p1);
		verts.push_back(p2);
		verts.push_back(p3);
	}
	void draw();
	bool inside(glm::vec3 p) {
		glm::vec4 p4 = glm::vec4(p, 1.0); 		// convert the clicked coordinates (vec3) to vec4
		p4 = glm::inverse(getMatrix()) * p4;	// multiply by the inverse
		glm::vec3 convertedP = glm::vec3(p4); 	// convert vec4 back to vec3
		return inside(convertedP, verts[0], verts[1], verts[2]);
	}
	bool inside(glm::vec3 p, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3) {
		glm::vec3 v1 = glm::normalize(p1 - p);
		glm::vec3 v2 = glm::normalize(p2 - p);
		glm::vec3 v3 = glm::normalize(p3 - p);
		float a1 = glm::orientedAngle(v1, v2, glm::vec3(0, 0, 1));
		float a2 = glm::orientedAngle(v2, v3, glm::vec3(0, 0, 1));
		float a3 = glm::orientedAngle(v3, v1, glm::vec3(0, 0, 1));
		if (a1 < 0 && a2 < 0 && a3 < 0) return true;
		else return false;
	}
};

class Sprite : public TriangleShape {
public:
	Sprite() {}
	Sprite(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3) {
		verts.push_back(p1);
		verts.push_back(p2);
		verts.push_back(p3);
	}
	void draw();

	bool inside(glm::vec3 p) {
		if (!bShowImage) {
			return TriangleShape::inside(p);
		}

		glm::vec3 s = glm::inverse(getMatrix()) * glm::vec4(p, 1);
		int w = spriteImage.getWidth();
		int h = spriteImage.getHeight();
		if (s.x > -w / 2 && s.x < w / 2 && s.y > -h / 2 && s.y < h / 2) {
			int x = s.x + w / 2;
			int y = s.y + h / 2;
			ofColor color = spriteImage.getColor(x, y);
			return (color.a != 0);   // check if color is opaque (not the transparent background)
		}
		else {
			return false;
		}
	}

	glm::vec3 heading() {
		float dirX = sin(ofDegToRad(this->rotation));
		float dirY = cos(ofDegToRad(this->rotation)) * -1;
		glm::vec3 direction = glm::normalize(glm::vec3(dirX, dirY, 0));
		return direction;
	}

	void drawHeading() {
		glm::vec3 direction = heading();
		glm::vec3 origin = this->pos;
		glm::vec3 line = origin + (HEADING_LENGTH * this->scaleValue) * direction;
		ofDrawLine(origin, line);
	}

	void setImage(ofImage image) {
		spriteImage = image;
	}

	float age() {
		return (ofGetElapsedTimeMillis() - birthtime) / 1000;
	}

	void integrate() {
		pos += velocity / ofGetFrameRate();
		velocity += acceleration / ofGetFrameRate();
		velocity *= damping;
		//cout << velocity.x << " " << velocity.y << endl;
		//cout << acceleration.x << " " << acceleration.y << endl;
		//cout << pos.x << " " << pos.y << endl;
	}

	// Physics data
	glm::vec3 velocity = glm::vec3(0, 0, 0);
	glm::vec3 acceleration = glm::vec3(0, 0, 0);
	glm::vec3 turbulence = glm::vec3(0, 0, 0);

	float damping = 0.99;
	ofImage spriteImage;
	bool bShowImage = false;
	int HEADING_LENGTH = 75;
	float birthtime = 0;
	float lifespan = -1;
};