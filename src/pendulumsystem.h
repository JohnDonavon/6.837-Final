#ifndef PENDULUMSYSTEM_H
#define PENDULUMSYSTEM_H

#include <vector>

#include "particlesystem.h"

class PendulumSystem : public ParticleSystem
{
public:
    PendulumSystem();

    std::vector<Vector3f> evalF(std::vector<Vector3f> state) override;
    void draw(GLProgram&);

    // inherits 
    // std::vector<Vector3f> m_vVecState;

    //List of springs. springs[i] corresponds to a vect containing floats that correspond to other nodes that node i is connected to.
    //Last two values in each vect<float> are spring rest length followed by spring constant
    //INVARIANT! Whenever a particle is created a spring must also be added to this list. If there is no spring attached to said particle
    //Add {}.
    std::vector<std::vector<float>> springs;
};
#endif