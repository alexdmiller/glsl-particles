#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){
  ofGLFWWindowSettings settings;
  settings.glVersionMajor = 3;
  settings.glVersionMinor = 3;
  settings.setSize(1500, 1000);
//  settings.set
  ofCreateWindow(settings);
  
//  ofSetFullscreen(true);
  // this kicks off the running of my app
  ofRunApp(new ofApp());

}
