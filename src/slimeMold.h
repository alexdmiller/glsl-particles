//
//  slimeMold.hpp
//  slime-world
//
//  Created by Alex on 11/8/19.
//

#ifndef slimeMold_hpp
#define slimeMold_hpp

#include <stdio.h>
#include "ofxGui.h"
#include "ofMain.h"
#include "swapBuffer.h"

class slimeMold {
private:
  int NUM_PARTICLES = 400;
  int SIZE = 1000;
  
  ofShader updateShader;
  ofShader renderShader;
  ofShader trailShader;
  ofShader mixShader;
  ofShader blurShader;
  ofShader finalRenderShader;
  ofVboMesh mesh;
  bool showTextures = false;
  bool showGUI = true;
  
  swapBuffer buffer;
  swapBuffer trailMap;
  
  ofFbo particleLayer;
  ofFbo drawLayer;
  ofFbo parameterLayer;
  ofFbo outputLayer;
public:
//  ofParameter<float> center = 0.01;
//  ofParameter<float> edge = 0.01;
  
  ofParameter<float> feedback = 0.99;
  ofParameter<float> radius = 1;
  
  ofParameter<float> speed = 0.01;
  ofParameter<float> sensorDistance = 0.01;
  ofParameter<float> sensorAngle = 0.01;
  ofParameter<float> rotateIncrement = 0.001;
  ofParameter<float> waves = 0;
  
  void setup() {
    renderShader.load("render.vert", "render.frag");
    updateShader.load("update.vert", "update.frag");
    trailShader.load("trail.vert", "trail.frag");
    mixShader.load("trail.vert", "mix.frag");
    blurShader.load("trail.vert", "blur.frag");
    finalRenderShader.load("finalrender.vert", "finalrender.frag");
    
    buffer.allocate(NUM_PARTICLES, NUM_PARTICLES);
    trailMap.allocate(SIZE, SIZE);
    particleLayer.allocate(SIZE, SIZE);
    drawLayer.allocate(SIZE, SIZE);
    parameterLayer.allocate(SIZE, SIZE);
    outputLayer.allocate(SIZE, SIZE);
    
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
  }

  void update() {
    drawLayer.begin();
    // TODO: use the draw layer for anything?
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
    // glPointSize(1);
    mesh.draw();
    renderShader.end();
    
    particleLayer.end();
    
    buffer.swap();
    
    
    // first add particle layer to last frame of trail map
    trailMap.dst->begin();
    mixShader.begin();
    mixShader.setUniformTexture("layer1", particleLayer.getTexture(), 0);
    mixShader.setUniformTexture("layer2", trailMap.src->getTexture(), 1);
    mixShader.setUniform1f("level1", 1.0);
    mixShader.setUniform1f("level2", feedback.get());
    ofDrawRectangle(0, 0, trailMap.dst->getWidth(), trailMap.dst->getHeight());
    mixShader.end();
    trailMap.dst->end();
    trailMap.swap();
    
    // then apply blur horizontally
    trailMap.dst->begin();
    blurShader.begin();
    blurShader.setUniformTexture("u_texture", trailMap.src->getTexture(), 0);
    blurShader.setUniform1f("resolution", trailMap.src->getWidth());
    blurShader.setUniform1f("radius", radius.get());
    blurShader.setUniform2f("dir", 0, 1);
    ofDrawRectangle(0, 0, trailMap.dst->getWidth(), trailMap.dst->getHeight());
    blurShader.end();
    trailMap.dst->end();
    trailMap.swap();
    
    // and vertically
    trailMap.dst->begin();
    blurShader.begin();
    blurShader.setUniformTexture("u_texture", trailMap.src->getTexture(), 0);
    blurShader.setUniform1f("resolution", trailMap.src->getWidth());
    blurShader.setUniform1f("radius", radius.get());
    blurShader.setUniform2f("dir", 1, 0);
    ofDrawRectangle(0, 0, trailMap.dst->getWidth(), trailMap.dst->getHeight());
    blurShader.end();
    trailMap.dst->end();
    trailMap.swap();
    
//    trailMap.dst->begin();
//    trailShader.begin();
//    trailShader.setUniformTexture("particleLayer", particleLayer.getTexture(), 0);
//    trailShader.setUniformTexture("lastFrame", trailMap.src->getTexture(), 1);
//    //   trailShader.setUniformTexture("drawLayer", drawLayer.getTexture(), 2);
//    trailShader.setUniform1f("center", center);
//    trailShader.setUniform1f("edge", edge);
//    trailShader.setUniform2f("size", ofGetWidth(), ofGetHeight());
//    ofDrawRectangle(0, 0, trailMap.dst->getWidth(), trailMap.dst->getHeight());
//    trailShader.end();
//    trailMap.dst->end();
//    trailMap.swap();
    
    outputLayer.begin();
    ofBackground(0);
    finalRenderShader.begin();
    finalRenderShader.setUniformTexture("image", trailMap.dst->getTexture(), 0);
    ofDrawRectangle(0, 0, trailMap.dst->getWidth(), trailMap.dst->getHeight());
    finalRenderShader.end();
    
    ofNoFill();
    ofSetLineWidth(2);
    //  font.drawStringAsShapes("NEWS\nLETTER", SIZE / 2. - rect.width / 2, SIZE / 2.);
    
    outputLayer.end();
    
    if (showTextures) {
      buffer.dst->getTexture(0).draw(0, 0);
      buffer.dst->getTexture(1).draw(NUM_PARTICLES, 0);
    }
    
    
    //  ofPixels pixels;
    //  pixels.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);
    //  outputLayer.readToPixels(pixels);
    //  ofSaveImage(pixels, ofToString(ofGetFrameNum(), 4, '0') +".png");
    
  }
  
  void draw() {
    outputLayer.draw(0, 0);
  }
  
  ofFbo* getOutputLayer() {
    return &outputLayer;
  }
};

#endif /* slimeMold_hpp */
