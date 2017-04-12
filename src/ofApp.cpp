#include "ofApp.h"

//image
ofImage zWord;
float zScale;

agent Agents[MAX_AGENTS];
agent Agents2[MAX_AGENTS];
int curAgent = 0;

//--------------------------------------------------------------
// relative point list  (starting at origin)
ofPoint zBot = ofPoint(0, 0);  // start
ofPoint zRi = ofPoint(150, 90); // go up and right
ofPoint zLe = ofPoint(-150, -150);  // go up and left
ofPoint zEnd = ofPoint(150, 90); // go down and left
ofPoint zEnd1 = ofPoint(5, 3);  // go down and right   it's a circle!
ofPoint gPoints[] = {zBot, zRi, zLe, zEnd,zEnd1};
//ofPoint gPoints[] = {zBot, zRi};

simpleGesture gesture;

void ofApp::setup(){
    
    ofSetFrameRate(60);
    ofSetVerticalSync(true);
	ofSetLogLevel(OF_LOG_VERBOSE);
    
	leap.open();
	cam.setOrientation(ofPoint(-20,0, 0));
    
//    ofTranslate(ofPoint(-20, 0, 0));
//    ofSetWindowPosition(1024,968);
  
    
	glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    
    gesture.setup(gPoints, 4, 20000);
    
    //agent
    for (curAgent; curAgent < MAX_AGENTS; curAgent++) {  // only enable if we disable mouse gravity
        Agents[curAgent].start(ofRandom(ofGetWidth()),ofRandom(ofGetHeight()));
    }
    
    for (curAgent; curAgent < MAX_AGENTS; curAgent++) {  // only enable if we disable mouse gravity
        Agents2[curAgent].start(ofRandom(ofGetWidth()),ofRandom(ofGetHeight()));
    }
    
    //image
    zScale = 1;
}


//--------------------------------------------------------------
void ofApp::update(){
	fingersFound.clear();
    
    //agent
    for(int i = 0; i < simpleHands.size(); i++){
        ofPoint handPos    = simpleHands[i].handPos;
    for (int i=0; i < MAX_AGENTS; i++) {
        Agents[i].update(Agents,handPos);
        }
    }

    //agent
//    ofPoint Ram    = ofPoint(ofRandom(ofGetWidth()),ofRandom(ofGetHeight()),800);
        for (int i=0; i < MAX_AGENTS; i++) {
            Agents2[i].update(Agents,ofPoint(-20,0,0));
    }

    
    simpleHands = leap.getSimpleHands();
    
    if( leap.isFrameNew() && simpleHands.size() ){
        
        leap.setMappingX(-230, 230, -ofGetWidth()/2, ofGetWidth()/2);
		leap.setMappingY(90, 490, -ofGetHeight()/2, ofGetHeight()/2);
        leap.setMappingZ(-150, 150, -200, 200);
        
        fingerType fingerTypes[] = {THUMB, INDEX, MIDDLE, RING, PINKY};
        
    }

	leap.markFrameAsOld();
}


// define your gesture
void simpleGesture::setup(ofPoint *points, int maxIndex, int msTout){
    gPts = points;
    maxIdx = maxIndex;
    timeout = msTout;
    curIdx = 1;
}

// Give a moving input point.  Returns true if gesture matched.
bool simpleGesture::track (ofPoint iPt) {
    bool xMatch = false, yMatch = false;
    int timePerPoint = timeout / maxIdx;
    
    int currentT = ofGetElapsedTimeMillis();
    if (currentT > stepStartTime + timePerPoint) {
        curIdx = 1;
        lastPoint = iPt;
        stepStartTime = currentT;
    }
    
    // where to go next abs screen coords
    ofPoint absNext = lastPoint + gPoints[curIdx];
    
    if (gPoints[curIdx-1].x == gPoints[curIdx].x) {  // if nothing has changed, match.
        xMatch = true;
    }
    
    if (gPoints[curIdx-1].y == gPoints[curIdx].y) {  // if nothing has changed, match.
        yMatch = true;
    }
    
    // find out what direction we are heading in
    if (gPoints[curIdx-1].x < gPoints[curIdx].x) {  // going right
        if (absNext.x < iPt.x)			// have we exceeded that x?
            xMatch = true;
    } else {					// going left
        if (absNext.x > iPt.x)
            xMatch = true;
    }
    
    if (gPoints[curIdx-1].y < gPoints[curIdx].y) {  // going down
        if (absNext.y < iPt.y) 			// have we exceeded that y?
            yMatch = true;
    } else {					// going up
        if (absNext.y > iPt.y)
            yMatch = true;
    }
    
    // if we have x & y
    if (xMatch && yMatch) {
        curIdx++;
        lastPoint = iPt;
        stepStartTime = currentT;
        
        if (curIdx == maxIdx) {  // if we just matched our last point
            // cout << "Matched" << endl;  // debug
            curIdx = 1; // start from beginning
            return(true);
        } 
    }
    
    // cout << iPt.x << " " << iPt.y << " " << curIdx << endl;  // debug
    return (false);
}

//--------------------------------------------------------------
void ofApp::draw(){
//    ofBackgroundGradient(ofColor(90, 90, 90), ofColor(30, 30, 30),  OF_GRADIENT_BAR);
    
    ofBackground(0);
    
    //agent
    int alpha = 150;
    
    ofColor aqua(252, 252, 255, alpha);
    ofColor purple(198, 0, 205, alpha);
    ofColor inbetween = aqua.getLerped(purple, ofRandom(1.0));  // linear interpolation between colors color
    
    for (int i=0; i < MAX_AGENTS; i++) {
        ofSetColor(inbetween);
        Agents2[i].draw(ofPoint(-20,0,0));
    }


	
	ofSetColor(200);
//	ofDrawBitmapString("ofxLeapMotion - Example App\nLeap Connected? " + ofToString(leap.isConnected()), 20, 20);
   
	cam.begin();
    

    
	ofPushMatrix();
    ofRotate(90, 0, 0, 1);
    ofSetColor(20);
//    ofDrawGridPlane(800, 20, false);
	ofPopMatrix();
    
    
    fingerType fingerTypes[] = {THUMB, INDEX, MIDDLE, RING, PINKY};
    
    for(int i = 0; i < simpleHands.size(); i++){
        bool isLeft        = simpleHands[i].isLeft;
        ofPoint handPos    = simpleHands[i].handPos;
        ofPoint handNormal = simpleHands[i].handNormal;
        
       // ofSetColor(255,20);
       // ofDrawSphere(handPos.x-10, handPos.y-10, handPos.z-20, 90);
        
        //agent
        int alpha = 150;
        
        ofColor aqua(252, 252, 255, alpha);
        ofColor purple(250, 200, 4, alpha);
        ofColor inbetween = aqua.getLerped(purple, ofRandom(1.0));  // linear interpolation between colors color
        
        for (int i=0; i < MAX_AGENTS; i++) {
            ofSetColor(inbetween);
            Agents[i].draw(handPos);
        }
        
        
//        ofDrawCircle(handPos.x, handPos.y, 90);
//        ofTranslate(ofGetWidth()/2, ofGetHeight()/2);
        
//        ofDrawArrow(handPos, handPos + 100*handNormal);
        
        // Give simple gesture more data and get return value
        bool gDone = gesture.track(simpleHands[0].fingers[INDEX].tip);  // send index finger tip
        
        if (gDone)
        {
              count ++;
              cout << "----------------------GOT IT----------------------" << endl;
              cout << count<< endl;
//            ofBackground(0);
        }

        
        for (int f=0; f<5; f++) {
            ofPoint mcp = simpleHands[i].fingers[ fingerTypes[f] ].mcp;  // metacarpal
            ofPoint pip = simpleHands[i].fingers[ fingerTypes[f] ].pip;  // proximal
            ofPoint dip = simpleHands[i].fingers[ fingerTypes[f] ].dip;  // distal
            ofPoint tip = simpleHands[i].fingers[ fingerTypes[f] ].tip;  // fingertip
            
//            ofSetColor(0, 255, 0);
//            ofDrawSphere(mcp.x, mcp.y, mcp.z, 12);
//            ofDrawSphere(pip.x, pip.y, pip.z, 12);
//            ofDrawSphere(dip.x, dip.y, dip.z, 12);
           // ofDrawSphere(tip.x, tip.y, tip.z, 12);
            
//            ofSetColor(255, 0, 0);
//            ofSetLineWidth(20);
//            ofDrawLine(mcp, pip);
//            ofDrawLine(pip.x, pip.y, pip.z, dip.x, dip.y, dip.z);
//            ofDrawLine(dip.x, dip.y, dip.z, tip.x, tip.y, tip.z);

//        
        for (int f=0; f<5; f++) {
            ofPoint mcp = simpleHands[i].fingers[ fingerTypes[f] ].mcp;  // metacarpal
            ofPoint pip = simpleHands[i].fingers[ fingerTypes[f] ].pip;  // proximal
            ofPoint dip = simpleHands[i].fingers[ fingerTypes[f] ].dip;  // distal
            ofPoint tip = simpleHands[i].fingers[ fingerTypes[f] ].tip;  // fingertip
            
            
            ofSetColor(255,20);
            ofDrawSphere(tip.x, tip.y, tip.z, 12);
            ofDrawSphere(mcp.x, mcp.y, mcp.z, 12);
            ofDrawSphere(pip.x, pip.y, pip.z, 12);
            ofDrawSphere(dip.x, dip.y, dip.z, 12);
            
//            ofDrawCircle(tip.x, tip.y, 12);
//            ofDrawCircle(mcp.x, mcp.y, 12);
//            ofDrawCircle(pip.x, pip.y, 12);
//            ofDrawCircle(dip.x, dip.y, 12);

            
            
            
            ofSetColor(0);
            ofSetLineWidth(100);
            ofDrawLine(mcp.x, mcp.y, mcp.z, pip.x, pip.y, pip.z);
            ofDrawLine(pip.x, pip.y, pip.z, dip.x, dip.y, dip.z);
            ofDrawLine(dip.x, dip.y, dip.z, tip.x, tip.y, tip.z);
//            ofBeginShape();
//            ofVertex(mcp.x,mcp.y);
//          
//            ofVertex(mcp.x+24,mcp.y);
//            ofVertex(pip.x+24,pip.y);
//              ofVertex(pip.x,pip.y);
//            ofEndShape();
            
//            ofSetColor(100);
//            ofDrawCylinder(tip.x, tip.y,tip.z, 12, 40);
//        }
        }
        }
    }
    
    if(count > 0) {
        zWord.load("z.png");
        ofSetColor(255,255,255);
        zWord.draw(0-zWord.getWidth()/2,0-zWord.getHeight()/2,-50 );
        ofScale(zScale/10, zScale/10,zScale/10);
    }
	cam.end();
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
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
    
}

//--------------------------------------------------------------
void ofApp::exit(){
    // let's close down Leap and kill the controller
    leap.close();
}
