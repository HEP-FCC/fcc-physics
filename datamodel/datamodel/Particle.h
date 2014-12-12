#ifndef Particle_H 
#define Particle_H

// Reference to another particle
// author: C. Bernet, B. Hegner

#include "datamodel/LorentzVector.h"
#include "datamodel/ParticleHandle.h"


class Particle {
public:
  LorentzVector P4; //Four momentum. 
  int ID; //Particle type identification code 
  int Status; //Status code 
  ParticleHandle Mother; //Reference to another particle 

};

#endif