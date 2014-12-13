// Data model
#include "datamodel/EventInfo.h"
#include "datamodel/EventInfoCollection.h"
#include "datamodel/Particle.h"
#include "datamodel/ParticleCollection.h"

// ROOT
#include "TLorentzVector.h"
#include "TBranch.h"
#include "TFile.h"
#include "TTree.h"
#include "TSystem.h"

// STL
#include <iostream>
#include <vector>

// albers specific includes
#include "albers/EventStore.h"
#include "albers/Registry.h"
#include "albers/Writer.h"


void processEvent(unsigned iEvent, albers::EventStore& store, albers::Writer& writer) {
  if(iEvent % 1000 == 0)
    std::cout<<"processing event "<<iEvent<<std::endl;

  // fill event information
  EventInfoCollection* evinfocoll = nullptr;
  store.get("EventInfo", evinfocoll);
  if(evinfocoll==nullptr) {
    std::cerr<<"collection EventInfo does not exist!"<<std::endl;
    return;
  }
  EventInfoHandle& evinfo = evinfocoll->create();
  evinfo.mod().Number = iEvent;

  // and now for the writing
  // TODO: do that at a different time w/o coll pointer
  // COLIN: calling writeEvent should not be left up to the user.
  writer.writeEvent();
  store.next();

  return;
}


int main(){
  // gSystem->Load("libDataModelExample.so");

  std::cout<<"start processing"<<std::endl;

  albers::Registry   registry;
  albers::EventStore store(&registry);
  albers::Writer     writer("example.root", &registry);

  unsigned nevents=10000;

  EventInfoCollection& evinfocoll = store.create<EventInfoCollection>("EventInfo");
  writer.registerForWrite<EventInfoCollection>("EventInfo");

  for(unsigned i=0; i<nevents; ++i) {
    processEvent(i, store, writer);
  }

  writer.finish();
}
