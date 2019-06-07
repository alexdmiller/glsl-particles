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

  int NUM_PARTICLES = 500;
  
  ofParameter<float> center = 0.01;
  ofParameter<float> edge = 0.01;
  ofParameter<float> speed = 0.01;
  ofParameter<float> sensorDistance = 0.01;
  ofParameter<float> sensorAngle = 0.01;
  ofParameter<float> rotateIncrement = 0.001;
  ofParameter<float> waves = 0;
  
  ofShader updateShader;
  ofShader renderShader;
  ofShader trailShader;
  ofShader finalRenderShader;
  ofVboMesh mesh;
  bool showTextures = false;
  
  swapBuffer buffer;
  swapBuffer trailMap;
  
  ofFbo particleLayer;
  ofFbo drawLayer;
  ofFbo parameterLayer;
  
  ofxPanel gui;
  
  ofTrueTypeFont font;
  
  string currentString;
};
