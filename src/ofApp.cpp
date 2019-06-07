#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
  font.load("InputMono-Bold", 100, true, true);

  renderShader.load("render.vert", "render.frag");
  updateShader.load("update.vert", "update.frag");
  trailShader.load("trail.vert", "trail.frag");
  finalRenderShader.load("finalrender.vert", "finalrender.frag");
  
  ofBackground(0, 0, 0);
  
  buffer.allocate(NUM_PARTICLES, NUM_PARTICLES);
  trailMap.allocate(ofGetWidth(), ofGetHeight());
  particleLayer.allocate(ofGetWidth(), ofGetHeight());
  drawLayer.allocate(ofGetWidth(), ofGetHeight());
  parameterLayer.allocate(ofGetWidth(), ofGetHeight());
  
  drawLayer.begin();
  ofClear(0);
  drawLayer.end();
  
  parameterLayer.begin();
  ofSetColor(100, 100, 20);
  ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
  parameterLayer.end();
  
  float* particlePosns = new float[NUM_PARTICLES * NUM_PARTICLES * 4];
  for (unsigned y = 0; y < NUM_PARTICLES; ++y) {
    for (unsigned x = 0; x < NUM_PARTICLES; ++x) {
      unsigned idx = y * NUM_PARTICLES + x;
      particlePosns[idx * 4] =     ofRandom(1);
      particlePosns[idx * 4 + 1] = ofRandom(1);
      particlePosns[idx * 4 + 2] = round(ofRandom(1));
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
      velocities[idx * 4] =     ofRandom(1);
      velocities[idx * 4 + 1] = 0.f;
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
  gui.add(center.set("center kernel weight", 0.7, 0, 1));
  gui.add(edge.set("edge kernel weight", 0.01, 0, 0.1));
  gui.add(speed.set("speed", 2, 0, 10));
  gui.add(sensorAngle.set("sensor angle", 1, 0, PI * 2));
  gui.add(sensorDistance.set("sensor distance", 20, 0, 100));
  gui.add(rotateIncrement.set("rotate increment", 0.1, 0, 0.2));
  gui.add(waves.set("waves", 0., -4, 4));
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw() {
  drawLayer.begin();
  ofSetColor(255, 0, 0);
  ofClear(0);
//  ofNoFill();
//  ofDrawCircle(ofGetWidth() / 2, ofGetHeight() / 2, 100);'
  ofRectangle rect = font.getStringBoundingBox("SPACEFILLER", 0, 0);
  font.drawString(currentString, ofGetWidth() / 2 - rect.width / 2, ofGetHeight() / 2);
  if (ofGetMousePressed()) {
    ofDrawCircle(ofGetMouseX(), ofGetMouseY(), 10);
  }
  drawLayer.end();
  
  buffer.dst->begin();

  glPushAttrib(GL_ENABLE_BIT);

  ofSetColor(255);
  buffer.dst->activateAllDrawBuffers();

  ofClear(0, 0, 0, 0);
  updateShader.begin();
  updateShader.setUniformTexture("positionData", buffer.src->getTexture(0), 0);
  updateShader.setUniformTexture("velocityData", buffer.src->getTexture(1), 1);
  updateShader.setUniformTexture("trailMap", trailMap.src->getTexture(), 2);
  updateShader.setUniformTexture("parameterMap", parameterLayer.getTexture(), 3);
  updateShader.setUniform1i("numParticles", NUM_PARTICLES);
  
  updateShader.setUniform1f("speed", speed);
  updateShader.setUniform1f("sensorAngle", sensorAngle);
  updateShader.setUniform1f("sensorDistance", sensorDistance);
  updateShader.setUniform1f("rotateIncrement", rotateIncrement);
  updateShader.setUniform1f("time", ofGetElapsedTimef());
  updateShader.setUniform2f("size", ofGetWidth(), ofGetHeight());
  updateShader.setUniform1f("waves", waves);
  
  ofDrawRectangle(0, 0, buffer.src->getWidth(), buffer.src->getHeight());
  updateShader.end();
  
  glPopAttrib();

  buffer.dst->end();
  
  particleLayer.begin();
  particleLayer.clearColorBuffer(ofFloatColor(0, 0, 0));

  renderShader.begin();
  renderShader.setUniform1f("time", ofGetElapsedTimef());
  renderShader.setUniform2f("resolution", ofGetWidth(), ofGetHeight());
  renderShader.setUniformTexture("particleData", buffer.dst->getTexture(0), 0);
  mesh.disableColors();
  ofSetColor(255, 255, 255, 50);
  glPointSize(1);
  mesh.draw();
  renderShader.end();
  
  particleLayer.end();
  
  buffer.swap();

  trailMap.dst->begin();
  trailShader.begin();
  trailShader.setUniformTexture("particleLayer", particleLayer.getTexture(), 0);
  trailShader.setUniformTexture("lastFrame", trailMap.src->getTexture(), 1);
  trailShader.setUniformTexture("drawLayer", drawLayer.getTexture(), 2);
  trailShader.setUniform1f("center", center);
  trailShader.setUniform1f("edge", edge);
  trailShader.setUniform2f("size", ofGetWidth(), ofGetHeight());
  ofDrawRectangle(0, 0, trailMap.dst->getWidth(), trailMap.dst->getHeight());
  trailShader.end();
  trailMap.dst->end();
  trailMap.swap();

  
  finalRenderShader.begin();
  finalRenderShader.setUniformTexture("image", particleLayer.getTexture(), 0);
  ofDrawRectangle(0, 0, trailMap.dst->getWidth(), trailMap.dst->getHeight());
  finalRenderShader.end();

//  drawLayer.draw(0, 0);
//
  if (showTextures) {
    buffer.dst->getTexture(0).draw(0, 0);
    buffer.dst->getTexture(1).draw(NUM_PARTICLES, 0);
  }
  
  gui.draw();
  
  ofDrawBitmapString(ofGetFrameRate(), 20, ofGetHeight());
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
  if (key == OF_KEY_BACKSPACE) {
    currentString = currentString.substr(0, currentString.size() - 2);
  }
  ofUTF8Append(currentString,  (char)key);
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
