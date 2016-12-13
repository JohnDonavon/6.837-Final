#include <iostream>
#include "clothsystem.h"
#include "camera.h"
#include "vertexrecorder.h"

 // your system should at least contain 8x8 particles.
const int W = 8;
const int H = 8;

ClothSystem::ClothSystem()
{
    // TODO 5. Initialize m_vVecState with cloth particles.
    constructClothParticles(W,H,{1,1});
    addStructSprings();
    addStructSpringVariables(.5,20);
    addShearSprings();
    addShearSpringVariables(.5,20);
    addFlexSprings();
    addFlexSpringVariables(.5,20);
}

void ClothSystem::constructClothParticles(int width, int height, std::vector<float> bottomLeftOfCloth){
    float xSkew = bottomLeftOfCloth[0];
    float ySkew = bottomLeftOfCloth[1];
    for(int i=0; i<width; ++i){
        for(int j=0; j<height; ++j){
            m_vVecState.push_back(Vector3f((i+xSkew)/2, (j+ySkew)/2, 0));
            m_vVecState.push_back(Vector3f(0,0,0));
            structSprings.push_back({});
            shearSprings.push_back({});
            flexSprings.push_back({});
        }
    }
}

void ClothSystem::addStructSprings(){
    for(int i=0; i<W; ++i){
        for(int j=0; j<H; ++j){
            int particleNum = H*i + j;
            if(j!=H-1){
                structSprings[particleNum].push_back(particleNum+1);
                structSprings[particleNum+1].push_back(particleNum);
            }
            if(i!=W-1) {
                structSprings[particleNum].push_back(particleNum + W);
                structSprings[particleNum+W].push_back(particleNum);
            }
        }
    }
    //Printing purposes
//    std::cout << "Struct springs: " << std::endl;
//    for(unsigned i=0; i<structSprings.size(); ++i){
//        std::cout << "particle "<<i << ":";
//        for(unsigned j=0; j<structSprings[i].size(); ++j){
//            std::cout << structSprings[i][j] << " ";
//        }
//        std::cout << std::endl;
//    }
}

void ClothSystem::addStructSpringVariables(float restLength, float k){
    for(unsigned i=0; i< structSprings.size(); ++i){
        structSprings[i].push_back(restLength);
        structSprings[i].push_back(k);
    }
}

void ClothSystem::addShearSprings(){
    for(int i=0; i<W; ++i){
        for(int j=0; j<H; ++j){
            int particleNum = H*i + j;
            if(i==W-1){
                continue;
            }
            if(j!=0){
                shearSprings[particleNum].push_back(particleNum+W-1);
                shearSprings[particleNum+W-1].push_back(particleNum);
            }
            if(j!=H-1){
                shearSprings[particleNum].push_back(particleNum+W+1);
                shearSprings[particleNum+W+1].push_back(particleNum);
            }
        }
    }
    //Printing purposes
//    std::cout << "Shear springs: " << std::endl;
//    for(unsigned i=0; i<shearSprings.size(); ++i){
//        std::cout << "particle "<<i << ":";
//        for(unsigned j=0; j<shearSprings[i].size(); ++j){
//            std::cout << shearSprings[i][j] << " ";
//        }
//        std::cout << std::endl;
//    }
}

void ClothSystem::addShearSpringVariables(float restLength, float k){
    for(unsigned i=0; i< shearSprings.size(); ++i){
        shearSprings[i].push_back(restLength);
        shearSprings[i].push_back(k);
    }
}

void ClothSystem::addFlexSprings(){
    for(int i=0; i<W; ++i){
        for(int j=0; j<H; ++j){
            int particleNum = H*i + j;
            if(j<H-2){
                flexSprings[particleNum].push_back(particleNum+2);
                flexSprings[particleNum+2].push_back(particleNum);
            }
            if(i<W-2){
                flexSprings[particleNum].push_back(particleNum + H*2);
                flexSprings[particleNum+H*2].push_back(particleNum);
            }
        }
    }
    //Printing purposes
//    std::cout << "Flex springs: " << std::endl;
//    for(unsigned i=0; i<flexSprings.size(); ++i){
//        std::cout << "particle "<<i << ":";
//        for(unsigned j=0; j<flexSprings[i].size(); ++j){
//            std::cout << flexSprings[i][j] << " ";
//        }
//        std::cout << std::endl;
//    }
}

void ClothSystem::addFlexSpringVariables(float restLength, float k){
    for(unsigned i=0; i< flexSprings.size(); ++i){
        flexSprings[i].push_back(restLength);
        flexSprings[i].push_back(k);
    }
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

std::vector<Vector3f> ClothSystem::evalF(std::vector<Vector3f> state)
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
void ClothSystem::detectAndReflect() {}

void ClothSystem::draw(GLProgram& gl)
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

