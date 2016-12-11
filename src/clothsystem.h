#ifndef CLOTHSYSTEM_H
#define CLOTHSYSTEM_H

#include <vector>

#include "particlesystem.h"

class ClothSystem : public ParticleSystem
{
    ///ADD MORE FUNCTION AND FIELDS HERE
public:
    ClothSystem();

    // evalF is called by the integrator at least once per time step
    std::vector<Vector3f> evalF(std::vector<Vector3f> state) override;

    // draw is called once per frame
    void draw(GLProgram& ctx);

    void constructClothParticles(int width, int height, std::vector<float> bottomLeftOfCloth);

    //Adds structural springs dependent on the W and H variables defined.
    void addStructSprings();
    //Adds Shear springs dependent on the W and H variables defined.
    void addShearSprings();
    //Adds Flex springs dependent on the W and H variables defined.
    void addFlexSprings();

    void addStructSpringVariables(float restLength, float k);
    void addShearSpringVariables(float restLength, float k);
    void addFlexSpringVariables(float restLength, float k);

    //See pendulumsystem.h for explanation of how these vectors work.
    std::vector<std::vector<float>> structSprings;
    std::vector<std::vector<float>> shearSprings;
    std::vector<std::vector<float>> flexSprings;
    // inherits
    // std::vector<Vector3f> m_vVecState;
};


#endif
