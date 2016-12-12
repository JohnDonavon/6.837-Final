#ifndef COLLISIONSYSTEM_H
#define COLLISIONSYSTEM_H

#include <vector>

#include "particlesystem.h"

class CollisionSystem : public ParticleSystem
{
    ///ADD MORE FUNCTION AND FIELDS HERE
public:
    CollisionSystem();
    std::vector<float> masses;
    std::vector<float> radii;

    std::vector<int> particlesCollided(int particleIndex, std::vector<Vector3f> positions);
    // evalF is called by the integrator at least once per time step
    std::vector<Vector3f> evalF(std::vector<Vector3f> state) override;

    // draw is called once per frame
    void draw(GLProgram& ctx);


    // inherits
    // std::vector<Vector3f> m_vVecState;
};




#endif //COLLISIONSYSTEM_H
