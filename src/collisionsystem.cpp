#include <iostream>
#include "collisionsystem.h"
#include "camera.h"
#include "vertexrecorder.h"

CollisionSystem::CollisionSystem()
{
    // TODO 5. Initialize m_vVecState with particles.

}

std::vector <Vector3f> extract2(std::vector <Vector3f>state, bool pos = true){
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

std::vector<Vector3f> CollisionSystem::evalF(std::vector<Vector3f> state)
{
    std::vector<Vector3f> f(state.size());
    std::vector<Vector3f> positions = extract2(state);
    std::vector<Vector3f> velocities = extract2(state,false);
    /// TODO 5. implement evalF
    for(unsigned i=0; i<positions.size(); ++i){
        Vector3f curVelocity = velocities[i];
        Vector3f gravVect = 9.8*Vector3f(0,-1,0);
        Vector3f dragVect = -.5*curVelocity;
        Vector3f springVect = Vector3f(0,0,0); //init a vect to add to.

        std::vector<float> structSpringDetails = structSprings[i];
        std::vector<float> shearSpringDetails = shearSprings[i];
        std::vector<float> flexSpringDetails = flexSprings[i];
        std::vector<float> noSpringCheck = {};


        if(structSpringDetails != noSpringCheck) {
            float springConst = structSpringDetails[structSpringDetails.size() - 1];
            float springRestLength = structSpringDetails[structSpringDetails.size() - 2];
            for (unsigned j = 0; j < structSpringDetails.size() - 2; ++j) {
                Vector3f d = positions[i] - positions[structSpringDetails[j]];
                Vector3f springForce = -springConst * (d.abs() - springRestLength) * (d / d.abs());
                springVect += springForce;
            }
        }

        if(shearSpringDetails != noSpringCheck) {
            float springConst = shearSpringDetails[shearSpringDetails.size() - 1];
            float springRestLength = shearSpringDetails[shearSpringDetails.size() - 2];
            for (unsigned j = 0; j < shearSpringDetails.size() - 2; ++j) {
                Vector3f d = positions[i] - positions[shearSpringDetails[j]];
                Vector3f springForce = -springConst * (d.abs() - springRestLength) * (d / d.abs());
                springVect += springForce;
            }
        }

        if(flexSpringDetails != noSpringCheck) {
            float springConst = flexSpringDetails[flexSpringDetails.size() - 1];
            float springRestLength = flexSpringDetails[flexSpringDetails.size() - 2];
            for (unsigned j = 0; j < flexSpringDetails.size() - 2; ++j) {
                Vector3f d = positions[i] - positions[flexSpringDetails[j]];
                Vector3f springForce = -springConst * (d.abs() - springRestLength) * (d / d.abs());
                springVect += springForce;
            }
        }

        Vector3f forceSum = gravVect + dragVect + springVect;
        f[2*i]+=curVelocity;
        if(i%H!=H-1){
            f[2*i+1]+=forceSum;
        }
        else{
            f[2*i+1]+=Vector3f(0,0,0);
        }
    }
    return f;

}


void CollisionSystem::draw(GLProgram& gl)
{
    //TODO 5: render the system

    const Vector3f CLOTH_COLOR(0.9f, 0.9f, 0.9f);
    gl.updateMaterial(CLOTH_COLOR);
    std::vector<Vector3f> positions = extract2(m_vVecState);

    for(unsigned i=0;i<positions.size();++i){
        Vector3f pos = positions[i];
        gl.updateModelMatrix(Matrix4f::translation(pos));
        drawSphere(0.04f, 8, 8);
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
    gl.disableLighting();
    gl.updateModelMatrix(Matrix4f::identity()); // update uniforms after mode change
    VertexRecorder rec;
    for(unsigned i=0; i<structSprings.size();++i){
        for(unsigned j=0; j<structSprings[i].size()-2;++j){
            gl.updateModelMatrix(Matrix4f::identity());
            rec.record(positions[i], CLOTH_COLOR);
            rec.record(positions[structSprings[i][j]], CLOTH_COLOR);
        }
    }
    glLineWidth(3.0f);
    rec.draw(GL_LINES);

    gl.enableLighting(); // reset to default lighting model

}
