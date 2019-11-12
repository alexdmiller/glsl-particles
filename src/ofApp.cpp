#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
  ofBackground(0);
  
  sim.setup();
  
  gui.setup();
  gui.add(sim.feedback.set("feedback", 0.98, 0.9, 1));
  gui.add(sim.radius.set("radius", 1, 0, 10));
  gui.add(sim.speed.set("speed", 2, 0, 10));
  gui.add(sim.sensorAngle.set("sensor angle", 1, 0, PI * 2));
  gui.add(sim.sensorDistance.set("sensor distance", 20, 0, 100));
  gui.add(sim.rotateIncrement.set("rotate increment", 0.1, 0, 0.2));
  gui.add(sim.waves.set("waves", 0., -4, 4));
  
  ofBoxPrimitive tmpBox;
  tmpBox.set(
             sim.getOutputLayer()->getWidth() / instances,
             sim.getOutputLayer()->getWidth() / instances,
             1.0);
  box = tmpBox.getMesh();
  
  instanceShader.load("instanced.vert", "instanced.frag");
}

//--------------------------------------------------------------
void ofApp::update() {
  sim.update();
}

//--------------------------------------------------------------
void ofApp::draw() {
//  sim.draw();
  
  ofEnableDepthTest();
  
  cam.enableOrtho();
  cam.begin();
  
  ofScale(2);

  ofPushMatrix();
  ofTranslate(-sim.getOutputLayer()->getWidth() / 2, -sim.getOutputLayer()->getHeight() / 2, 0);

  instanceShader.begin();
  instanceShader.setUniform1i("instanceGridSize", instances);
  instanceShader.setUniform2i("slimeTextureSize", sim.getOutputLayer()->getWidth(), sim.getOutputLayer()->getHeight());
  instanceShader.setUniformTexture("tex0", sim.getOutputLayer()->getTexture(), 0);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);

  box.drawInstanced(OF_MESH_FILL, instances*instances);

  glDisable(GL_CULL_FACE);
  ofPopMatrix();
  cam.end();
  instanceShader.end();

//  drawLayer.draw(0, 0);

  ofDisableDepthTest();

  gui.draw();
  ofDrawBitmapString(ofGetFrameRate(), 20, ofGetHeight());
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
