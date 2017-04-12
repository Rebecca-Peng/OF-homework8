#ifndef agent_hpp
#define agent_hpp
#include "ofMain.h"

#include <stdio.h>
#define MAX_AGENTS 200

class agent {
public:
    agent();
    void update(agent *arr,ofPoint P);
    void draw(ofPoint left);
    void start(float X, float Y);
    void calcGravityWell(ofPoint P);
    
    
private:
    float getRelAngle(ofVec2f neighbor);
    float calcAlignment(agent *arr, int max);
    ofVec2f calcGravity(agent *arr,int max);
    ofVec2f calcCohesion(agent *arr, int max);
    ofVec2f calcDispersion(agent *arr, int max);
    ofVec2f location;
    ofVec2f velDir; // velocity & direction for each step
    ofVec2f mouse;  //
    ofVec2f ball;
    
    int count;
    float gravity;
    
    bool active = false;	// if this agent is active
};



#endif /* agent_hpp */
