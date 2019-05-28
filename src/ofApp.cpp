#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
  renderShader.load("render.vert", "render.frag");
  updateShader.load("update.vert", "update.frag");

  ofBackground(0, 0, 0);
  
  buffer.allocate(NUM_PARTICLES, NUM_PARTICLES);
  
  float* particlePosns = new float[NUM_PARTICLES * NUM_PARTICLES * 4];
  for (unsigned y = 0; y < NUM_PARTICLES; ++y) {
    for (unsigned x = 0; x < NUM_PARTICLES; ++x) {
      unsigned idx = y * NUM_PARTICLES + x;
      particlePosns[idx * 4] =     ofRandom(1);
      particlePosns[idx * 4 + 1] = ofRandom(1);
      particlePosns[idx * 4 + 2] = 0.f;
      particlePosns[idx * 4 + 3] = 0.f;
    }
  }
  
  buffer.src->getTexture(0).bind();
  glTexSubImage2D(GL_TEXTURE_RECTANGLE, 0, 0, 0, NUM_PARTICLES, NUM_PARTICLES, GL_RGBA, GL_FLOAT, particlePosns);
  buffer.src->getTexture(0).unbind();
  
  float* velocities = new float[NUM_PARTICLES * NUM_PARTICLES * 4];
  for (unsigned y = 0; y < NUM_PARTICLES; ++y) {
    for (unsigned x = 0; x < NUM_PARTICLES; ++x) {
      unsigned idx = y * NUM_PARTICLES + x;
      velocities[idx * 4] =     ofRandom(0.01) + 0.5 - 0.005;
      velocities[idx * 4 + 1] = ofRandom(0.01) + 0.5 - 0.005;
      velocities[idx * 4 + 2] = 0.f;
      velocities[idx * 4 + 3] = 0.f;
    }
  }
  
  buffer.src->getTexture(1).bind();
  glTexSubImage2D(GL_TEXTURE_RECTANGLE, 0, 0, 0, NUM_PARTICLES, NUM_PARTICLES, GL_RGBA, GL_FLOAT, velocities);
  buffer.src->getTexture(1).unbind();
  
  
//  particleData.getTexture().loadData(pix);
  mesh.setMode(OF_PRIMITIVE_POINTS);
  
  for (int x = 0; x < NUM_PARTICLES; x++) {
    for (int y = 0; y < NUM_PARTICLES; y++) {
      mesh.addVertex(ofPoint(x, y));
    }
  }
  
  gui.setup();
  gui.add(repelThreshold.set("repel thresh", 0.001, 0, 0.05));
  gui.add(repelStrength.set("repel strength", 0.000001, 0, 0.00001));
  gui.add(attractionThreshold.set("attraction thresh", 0.001, 0, 0.05));
  gui.add(attractionStrength.set("attraction strength", 0.000001, 0, 0.00001));
  gui.add(friction.set("friction", 0.8, 0, 1));
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw() {
  // TODO: figure out if this is drawing anything?
  // maybe switch to a rect so you can actually see the pixels
  
  
  buffer.dst->begin();

//  glPushAttrib(GL_ENABLE_BIT);
  // we set up no camera model and ignore the modelview and projection matrices
  // in the vertex shader, we make a viewport large enough to ensure the shader
  // is executed for each pixel
//  glViewport(0, 0, buffer.dst->getWidth(), buffer.dst->getHeight());
//  glDisable(GL_BLEND);
  ofSetColor(255);
  buffer.dst->activateAllDrawBuffers();

  ofClear(0, 0, 0, 0);
  updateShader.begin();
  updateShader.setUniformTexture("positionData", buffer.src->getTexture(0), 0);
  updateShader.setUniformTexture("velocityData", buffer.src->getTexture(1), 1);
  updateShader.setUniform1i("numParticles", NUM_PARTICLES);
  
  updateShader.setUniform1f("repelThreshold", repelThreshold);
  updateShader.setUniform1f("repelStrength", repelStrength);
  updateShader.setUniform1f("attractionThreshold", attractionThreshold);
  updateShader.setUniform1f("attractionStrength", attractionStrength);
  updateShader.setUniform1f("friction", friction);

  ofDrawRectangle(0, 0, buffer.src->getWidth(), buffer.src->getHeight());
  updateShader.end();
  
//  glPopAttrib();

  buffer.dst->end();

  renderShader.begin();
  renderShader.setUniform1f("time", ofGetElapsedTimef());
  renderShader.setUniform2f("resolution", ofGetWidth(), ofGetHeight());
  renderShader.setUniformTexture("particleData", buffer.dst->getTexture(0), 0);
  
  mesh.disableColors();
  ofSetColor(255, 255, 255, 50);
  glPointSize(2);
  mesh.draw();
  renderShader.end();
  
  if (showTextures) {
    buffer.dst->getTexture(0).draw(0, 0);
    buffer.dst->getTexture(1).draw(NUM_PARTICLES, 0);
  }

  buffer.swap();

//  particleData.draw(0, 0);
  gui.draw();
  
  ofDrawBitmapString(ofGetFrameRate(), 20, ofGetHeight());
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
  showTextures = !showTextures;
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
