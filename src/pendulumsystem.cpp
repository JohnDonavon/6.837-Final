#include "pendulumsystem.h"
#include <iostream>

#include <cassert>
#include "camera.h"
#include "vertexrecorder.h"

// TODO adjust to number of particles.
const int NUM_PARTICLES = 2;

PendulumSystem::PendulumSystem()
{

    // TODO 4.2 Add particles for simple pendulum
    // float f = rand_uniform(-0.5f, 0.5f);
    //Particle 0
    m_vVecState.push_back(Vector3f(0,0,0));
    m_vVecState.push_back(Vector3f(0,0,0));
    //Particle 1
    m_vVecState.push_back(Vector3f(0,-1,0));
    m_vVecState.push_back(Vector3f(-5,0,0));

    //Particle 0 (so index 0 of springs) is connected to particle 1. The spring const is .5, located at index -1
    springs.push_back({1,.5,5});

    //Particle 1 (so index 1 of springs) is connected to particle 0. The spring const is .5, located at index -1
    springs.push_back({0,.5,5});

    // TODO 4.3 Extend to multiple particles
    //Particle 2
    m_vVecState.push_back(Vector3f(-2,0,0));
    m_vVecState.push_back(Vector3f(0,0,0));
    springs.push_back({3,.5,20});
    //Particle 3
    m_vVecState.push_back(Vector3f(-2,-1,0));
    m_vVecState.push_back(Vector3f(-2,0,0));
    springs.push_back({2,4,.5,20});
    //Particle 4
    m_vVecState.push_back(Vector3f(-2,-2,0));
    m_vVecState.push_back(Vector3f(2,0,0));
    springs.push_back({3,5,.5,20});
    //Particle 5
    m_vVecState.push_back(Vector3f(-2,-3,-4));
    m_vVecState.push_back(Vector3f(-1,0,0));
    springs.push_back({4,.5,20});
}

//extract either positions or velocities from the state
std::vector <Vector3f> extract(std::vector <Vector3f>state, bool pos = true){
    std::vector <Vector3f> extracted;
    if(pos){
        for(unsigned i=0;i<state.size();i=i+2){
            extracted.push_back(state[i]);
        }
    }
    else{
        for(unsigned i=1;i<state.size();i=i+2){
            extracted.push_back((state[i]));
        }
    }
    return extracted;
}

std::vector<Vector3f> PendulumSystem::evalF(std::vector<Vector3f> state)
{
    std::vector<Vector3f> f(state.size());
    std::vector<Vector3f> positions = extract(state);
    std::vector<Vector3f> velocities = extract(state,false);
    // TODO 4.1: implement evalF
    //  - gravity
    //  - viscous drag
    //  - springs
    for(unsigned i=0; i<positions.size(); ++i){
        Vector3f curVelocity = velocities[i];
        Vector3f gravVect = 9.8*Vector3f(0,-1,0);
        Vector3f dragVect = -.5*curVelocity;
        Vector3f springVect = Vector3f(0,0,0); //init a vect to add to.

        std::vector<float> springDetails = springs[i];
        std::vector<float> noSpringCheck = {};
        //Conditional to check if particle actually has springs attached to it.
        //Read invariant of springs list in pendulumsystem.h
        if(springDetails != noSpringCheck) {
            float springConst = springDetails[springDetails.size() - 1];
            float springRestLength = springDetails[springDetails.size() - 2];
            for (unsigned j = 0; j < springDetails.size() - 2; ++j) {
                Vector3f d = positions[i] - positions[springDetails[j]];
                Vector3f springForce = -springConst * (d.abs() - springRestLength) * (d / d.abs());
                springVect += springForce;
            }
        }

        Vector3f forceSum = gravVect + dragVect + springVect;
        f[2*i]+=curVelocity;
        if(i!=0 and i!=2){
            f[2*i+1]+=forceSum;
        }
        else{
            f[2*i+1]+=Vector3f(0,0,0);
        }
    }
    return f;
}

// render the system (ie draw the particles)
void PendulumSystem::draw(GLProgram& gl)
{
    const Vector3f PENDULUM_COLOR(0.73f, 0.0f, 0.83f);
    gl.updateMaterial(PENDULUM_COLOR);

    // TODO 4.2, 4.3

    // example code. Replace with your own drawing code
    for(unsigned i=0;i<m_vVecState.size();i=i+2){
        Vector3f pos = m_vVecState[i];
        gl.updateModelMatrix(Matrix4f::translation(pos));
        drawSphere(0.075f, 10, 10);
    }

}
