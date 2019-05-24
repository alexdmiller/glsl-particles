#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
  ofBackground(0, 0, 0);
  
  particleData.allocate(NUM_PARTICLES, 1);
  
  ofPixels pix;
  
  pix.allocate(NUM_PARTICLES, 1, 4);
  // put some stuff in the pixels
  for (int i = 0; i < NUM_PARTICLES; i++) {
    pix.setColor(i,0,ofColor(ofRandom(255), ofRandom(255), ofRandom(255)));
  }
  
  particleData.getTexture().loadData(pix);
  
  mesh.setMode(OF_PRIMITIVE_POINTS);
  
  for (int i = 0; i < NUM_PARTICLES; i++) {
    mesh.addVertex(ofPoint(i, 0));
  }
  
  shader.load("render.vert", "render.frag");
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
  shader.begin();
  shader.setUniform1f("time", ofGetElapsedTimef());
  shader.setUniform2f("resolution", ofGetWidth(), ofGetHeight());
  shader.setUniformTexture("particleData", particleData, 0);
  
  mesh.disableColors();
  ofSetColor(255, 255, 255);
  glPointSize(3);
  mesh.draw();
  shader.end();
  
  particleData.draw(0, 0);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
