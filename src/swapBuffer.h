#ifndef OFXSWAPBUFFER
#define OFXSWAPBUFFER

class swapBuffer {
public:
  virtual void allocate( int _width, int _height, int _internalformat = GL_RGBA){
    ofFbo::Settings settings;
    settings.width = _width;
    settings.height = _height;
    settings.numColorbuffers = 2;
    settings.internalformat = GL_RGBA32F;
    settings.textureTarget = GL_TEXTURE_RECTANGLE;
    settings.minFilter = GL_NEAREST;
    settings.maxFilter = GL_NEAREST;
    settings.wrapModeHorizontal = GL_CLAMP;
    settings.wrapModeVertical = GL_CLAMP;
    
    for(int i = 0; i < 2; i++) {
      FBOs[i].allocate(settings);
    }
    
    clear();
    
    // Set everything to 0
    flag = 0;
    swap();
    flag = 0;
  }
  
  virtual void allocate( ofFbo::Settings settings) {
    for(int i = 0; i < 2; i++){
      FBOs[i].allocate(settings);
    }
    
    
    clear();
    
    // Set everything to 0
    flag = 0;
    swap();
    flag = 0;
  }
  
  virtual void swap(){
    src = &(FBOs[(flag)%2]);
    dst = &(FBOs[++(flag)%2]);
  }
  
  virtual void clear(int _alpha = 255){
    for(int i = 0; i < 2; i++){
      FBOs[i].begin();
      ofClear(0,_alpha);
      FBOs[i].end();
    }
  }
  
  ofFbo& operator[]( int n ){ return FBOs[n];}
  
  ofFbo   *src;       // Source       ->  Ping
  ofFbo   *dst;       // Destination  ->  Pong
  
private:
  ofFbo   FBOs[2];    // Real addresses of ping/pong FBO´s
  int     flag;       // Integer for making a quick swap
};

#endif
