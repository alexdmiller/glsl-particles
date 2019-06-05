#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){
  ofGLFWWindowSettings settings;
  settings.glVersionMajor = 3;
  settings.glVersionMinor = 3;
  settings.setSize(1024, 1024);
//  settings.set
  ofCreateWindow(settings);
  // this kicks off the running of my app
  ofRunApp(new ofApp());

}
