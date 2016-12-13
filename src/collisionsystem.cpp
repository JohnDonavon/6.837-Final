#include <iostream>
#include "collisionsystem.h"
#include "camera.h"
#include "vertexrecorder.h"
const float radius = .2;
CollisionSystem::CollisionSystem()
{
    // TODO 5. Initialize m_vVecState with particles.
    //Particle 0
//    m_vVecState.push_back(Vector3f(-2,0,0));
//    m_vVecState.push_back(Vector3f(1,0,0));
//    masses.push_back(10.0);
//    radii.push_back(.3);
//    //Particle 1
//    m_vVecState.push_back(Vector3f(2,0,0));
//    m_vVecState.push_back(Vector3f(-2,0,0));
//    masses.push_back(1.0);
//    radii.push_back(.2);

    for (int i=0; i<5; i+=1){
        m_vVecState.push_back(Vector3f(rand()%5, rand()%5, rand()%5));
        m_vVecState.push_back(Vector3f(rand()%5, rand()%5, rand()%5));
        masses.push_back(rand() % 10);
        float radius = 1.0/(rand()%10+1);
        radii.push_back(radius);
    }

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

void CollisionSystem::detectAndReflect(){
    std::vector<Vector3f> positions = plsExtract2(m_vVecState);
    std::vector<Vector3f> velocities = plsExtract2(m_vVecState,false);
    for(unsigned i=0; i<positions.size(); ++i){
        float radiusI = radii[i];
        for(unsigned j=0; j<positions.size(); ++j){
            float radiusJ = radii[j];
            if(i==j){
                continue;
            }
            float dist = (positions[j] - positions[i]).abs();
            if(dist<=(radiusI+radiusJ)){
                Vector3f collision = Vector3f(0,0,0);
                Vector3f pos1 = positions[i];
                Vector3f pos2 = positions[j];
                Vector3f vel1 = velocities[i];
                Vector3f vel2 = velocities[j];
                float m1 = masses[i];
                float m2 = masses[j];

                Vector3f unitNormal = (pos1 - pos2).normalized();

                float v1n = Vector3f::dot(unitNormal, vel1);
                float v2n = Vector3f::dot(unitNormal, vel2);
                float newV = (v1n*(m1 - m2) + 2*m2*v2n)/(m1+m2);

                collision += newV * unitNormal;
                m_vVecState[2*i+1] = collision;
            }
        }
    }
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
        f[2*i] = m_vVecState[2*i+1];
    }
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
        float colorMult = masses[i]/2.0;
        Vector3f pos = positions[i]; //YOUR PARTICLE POSITION
        gl.updateModelMatrix(Matrix4f::translation(pos));
        const Vector3f PARTICLE_COLOR(0.4f, 0.7f, 1.0f);
        gl.updateMaterial(colorMult*PARTICLE_COLOR);
        drawSphere(radii[i], 10, 10);
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
