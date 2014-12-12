#ifdef __CINT__

#include <vector>
#include "albers/CollectionBase.h"

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;
#pragma link C++ nestedclasses;

#pragma link C++ class LorentzVector+;
#pragma link C++ class std::vector<LorentzVector>+;
#pragma link C++ class EventInfo+;
#pragma link C++ class std::vector<EventInfo>+;
#pragma link C++ class EventInfoHandle+;
#pragma link C++ class EventInfoCollection+;
#pragma link C++ class Particle+;
#pragma link C++ class std::vector<Particle>+;
#pragma link C++ class ParticleHandle+;
#pragma link C++ class ParticleCollection+;


#pragma link C++ class albers::CollectionBase+;
#pragma link C++ class albers::Registry+;

#pragma link C++ class PyEventStore+;

#endif
