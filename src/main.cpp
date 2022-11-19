#include "ofMain.h"
#include "ofApp.h"
/*
	Lab 3: Player Movement
	@Author: Dimitar Dimitrov
	@Version: 9/27/2022
*/

//========================================================================
int main( ){
	ofSetupOpenGL(1240,768,OF_WINDOW);			// <-------- setup the GL context

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(new ofApp());

}
