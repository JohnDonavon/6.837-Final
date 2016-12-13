#include <iostream>
#include "collisionsystem.h"
#include "camera.h"
#include "vertexrecorder.h"
const float radius = .2;
CollisionSystem::CollisionSystem()
{
    // TODO 5. Initialize m_vVecState with particles.
    //Particle 0
    m_vVecState.push_back(Vector3f(-2,0,0));
    m_vVecState.push_back(Vector3f(1,0,0));
    masses.push_back(1.0);
    radii.push_back(.2);
    //Particle 1
    m_vVecState.push_back(Vector3f(2,0,0));
    m_vVecState.push_back(Vector3f(-1,0,0));
    masses.push_back(1.0);
    radii.push_back(.2);
}

std::vector <Vector3f> plsExtract2(std::vector <Vector3f>state, bool pos = true){
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

std::vector<int> CollisionSystem::particlesCollided(int particleIndex, std::vector<Vector3f> positions){
    float radius = radii[particleIndex];
    std::vector<int> collidedParticles;
    for (int i=0; i<positions.size(); ++i){
        if (i == particleIndex){
            continue;
        }
        float dist = (positions[i] - positions[particleIndex]).abs();
        if (dist <= (radii[i] + radius)){
            std::cout << std::endl << "COLLISION!!!" << std::endl;
            collidedParticles.push_back(i);
        }
    }
    return collidedParticles;
}

std::vector<Vector3f> CollisionSystem::evalF(std::vector<Vector3f> state)
{
    std::vector<Vector3f> f(state.size());
    std::vector<Vector3f> positions = plsExtract2(state);
    std::vector<Vector3f> velocities = plsExtract2(state,false);
    /// TODO 5. implement evalF

    //draw vectors between current pos and every other
    //if length is less than 2r, then there is a collision
    for(unsigned i=0; i<positions.size(); ++i){
        std::vector<int> collidedParticles = particlesCollided(i, positions);
        Vector3f collision = Vector3f(0,0,0);
        if(!collidedParticles.empty()){
            for(int j : collidedParticles){
                Vector3f pos1 = positions[i];
                Vector3f pos2 = positions[j];
                Vector3f vel1 = velocities[i];
                Vector3f vel2 = velocities[j];
                float m1 = masses[i];
                float m2 = masses[j];

                Vector3f unitNormal = (pos1 - pos2).normalized();
                float vrel = Vector3f::dot((vel1 - vel2), unitNormal);
                collision+=vel1 - vrel*unitNormal;
            }
        }
        if(collision != Vector3f(0,0,0)){
            //collideForce.print();
            //m_vVecState[2*i] = positions[i];
            std::cout << "Particle " << i << std::endl;
            m_vVecState[2*i+1] = collision;
            f[2*i] += collision;
        }
        else{
            f[2*i] = m_vVecState[2*i+1];
        }
//        Vector3f curVelocity = m_vVecState[2*i+1];
//        //std::cout << "Vel: "; curVelocity.print();
//
//        f[2*i]+=-curVelocity;





        //f[2*i+1]+=collideForce;
    }
    std::cout << "State" << std::endl;
    for(Vector3f i : m_vVecState){
        i.print();
    }
//    std::cout << std::endl;
//    for(Vector3f i : f){
//        i.print();
//
//    }
//    std::cout << std::endl;
    return f;

}


void CollisionSystem::draw(GLProgram& gl)
{
    //TODO 5: render the system

//    const Vector3f CLOTH_COLOR(0.9f, 0.9f, 0.9f);
//    gl.updateMaterial(CLOTH_COLOR);
    const Vector3f PARTICLE_COLOR(0.4f, 0.7f, 1.0f);
    gl.updateMaterial(PARTICLE_COLOR);

    std::vector<Vector3f> positions = plsExtract2(m_vVecState);

    for(unsigned i=0;i<positions.size();++i){

        Vector3f pos = positions[i]; //YOUR PARTICLE POSITION
        gl.updateModelMatrix(Matrix4f::translation(pos));
        drawSphere(radii[i], 10, 10);
//        Vector3f pos = positions[i];
//        gl.updateModelMatrix(Matrix4f::translation(pos));
//        drawSphere(0.04f, 8, 8);
    }

    // EXAMPLE: This shows you how to render lines to debug the spring system.
    //
    //          You should replace this code.
    //
    //          Since lines don't have a clearly defined normal, we can't use
    //          a regular lighting model.
    //          GLprogram has a "color only" mode, where illumination
    //          is disabled, and you specify color directly as vertex attribute.
    //          Note: enableLighting/disableLighting invalidates uniforms,
    //          so you'll have to update the transformation/material parameters
    //          after a mode change.
//    gl.disableLighting();
//    gl.updateModelMatrix(Matrix4f::identity()); // update uniforms after mode change
//    VertexRecorder rec;
//    for(unsigned i=0; i<structSprings.size();++i){
//        for(unsigned j=0; j<structSprings[i].size()-2;++j){
//            gl.updateModelMatrix(Matrix4f::identity());
//            rec.record(positions[i], CLOTH_COLOR);
//            rec.record(positions[structSprings[i][j]], CLOTH_COLOR);
//        }
//    }
//    glLineWidth(3.0f);
//    rec.draw(GL_LINES);
//
//    gl.enableLighting(); // reset to default lighting model

}
