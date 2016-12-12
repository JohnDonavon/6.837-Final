#ifndef A3_COLLISIONSYSTEM_H
#define A3_COLLISIONSYSTEM_H

#include <vector>

#include "particlesystem.h"

class CollisionSystem : public ParticleSystem
{
    ///ADD MORE FUNCTION AND FIELDS HERE
public:
    CollisionSystem();

    // evalF is called by the integrator at least once per time step
    std::vector<Vector3f> evalF(std::vector<Vector3f> state) override;

    // draw is called once per frame
    void draw(GLProgram& ctx);

    void constructCollisionParticles(int width, int height, std::vector<float> bottomLeftOfCloth);

    // inherits
    // std::vector<Vector3f> m_vVecState;
};




#endif //A3_COLLISIONSYSTEM_H
