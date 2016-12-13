#include "timestepper.h"
#include <iostream>

#include <cstdio>

void ForwardEuler::takeStep(ParticleSystem* particleSystem, float stepSize)
{
    particleSystem->detectAndReflect();
    std::vector < Vector3f > newStates;
    std::vector < Vector3f > curStates = particleSystem->getState();
    std::vector< Vector3f > derivStates = particleSystem->evalF(curStates);
    for(unsigned i=0;i<derivStates.size(); ++i){
        newStates.push_back(curStates[i] + stepSize*derivStates[i]);
    }
    particleSystem->setState(newStates);

}

void Trapezoidal::takeStep(ParticleSystem* particleSystem, float stepSize)
{
    particleSystem->detectAndReflect();
    //Final states that we'll be building up.
    std::vector <Vector3f> newStates;
    //The current condition of the states.
    std::vector <Vector3f> curStates = particleSystem->getState();
    //The derivative of the current states. This is f_0
    std::vector <Vector3f> f_0 = particleSystem->evalF(curStates);
    //List of states after Euler step
    std::vector <Vector3f> firstStepStates;

    //Go through the states in the f_0 and do an Euler step.
    for(unsigned i=0;i<f_0.size(); ++i){
        firstStepStates.push_back(curStates[i] + stepSize*f_0[i]);
    }

    //Derivative of state we're in after Euler step
    std::vector <Vector3f> f_1 = particleSystem->evalF(firstStepStates);

    //Build up final states using trapezoidal rule.
    for(unsigned i=0;i<curStates.size();++i){
        newStates.push_back(curStates[i] + (stepSize/2)*(f_0[i] + f_1[i]));
    }
    //Update the state of the system.
    particleSystem->setState(newStates);
}


void RK4::takeStep(ParticleSystem* particleSystem, float stepSize)
{
    particleSystem->detectAndReflect();
    std::vector<Vector3f> newStates;
    std::vector<Vector3f> curStates = particleSystem->getState();

    std::vector<Vector3f> k_1 = particleSystem->evalF(curStates);


    std::vector<Vector3f> k_2Prep;
    for(unsigned i=0; i<curStates.size(); ++i){
        k_2Prep.push_back(curStates[i] + (stepSize/2)*k_1[i]);
    }
    std::vector<Vector3f> k_2 = particleSystem->evalF(k_2Prep);

    std::vector<Vector3f> k_3Prep;
    for(unsigned i=0; i<curStates.size(); ++i){
        k_3Prep.push_back(curStates[i] + (stepSize/2)*k_2[i]);
    }
    std::vector<Vector3f> k_3 = particleSystem->evalF(k_3Prep);

    std::vector<Vector3f> k_4Prep;
    for(unsigned i=0; i<curStates.size(); ++i){
        k_4Prep.push_back(curStates[i] + stepSize*k_3[i]);
    }
    std::vector<Vector3f> k_4 = particleSystem->evalF(k_4Prep);

    for(unsigned i=0; i<curStates.size(); ++i){
        newStates.push_back(curStates[i] + (stepSize/6)*(k_1[i] + 2*k_2[i] + 2*k_3[i] + k_4[i]));
    }

    particleSystem->setState(newStates);
}

