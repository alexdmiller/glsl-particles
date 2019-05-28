#pragma once

#include "ofMain.h"
#include "swapBuffer.h"
#include "ofxGui.h"

class ofApp : public ofBaseApp {

public:
  void setup();
  void update();
  void draw();

  void keyPressed(int key);
  void keyReleased(int key);
  void mouseMoved(int x, int y );
  void mouseDragged(int x, int y, int button);
  void mousePressed(int x, int y, int button);
  void mouseReleased(int x, int y, int button);
  void mouseEntered(int x, int y);
  void mouseExited(int x, int y);
  void windowResized(int w, int h);
  void dragEvent(ofDragInfo dragInfo);
  void gotMessage(ofMessage msg);

  int NUM_PARTICLES = 100;
  
  ofParameter<float> repelThreshold = 0.001;
  ofParameter<float> repelStrength = 0.001;
  ofParameter<float> attractionThreshold = 0.002;
  ofParameter<float> attractionStrength = 0.0001;
  ofParameter<float> friction = 0.8;
  
  ofShader updateShader;
  ofShader renderShader;
  ofShader trailShader;
  ofVboMesh mesh;
  bool showTextures = true;
  
  swapBuffer buffer;
  swapBuffer trailMap;
  
  ofFbo particleLayer;

  ofxPanel gui;
};
