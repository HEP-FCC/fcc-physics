#include "datamodel/ParticleCollection.h"
#include "datamodel/EventInfoCollection.h"

// ROOT
#include "TBranch.h"
#include "TFile.h"
#include "TTree.h"
#include "TSystem.h"
#include "TROOT.h"
#include "TLorentzVector.h"

// STL
#include <vector>
#include <iostream>

// albers specific includes
#include "albers/EventStore.h"
#include "albers/Reader.h"
#include "albers/Registry.h"

void processEvent(albers::EventStore& store, bool verbose,
		  albers::Reader& reader) {

  // read event information
  EventInfoCollection* evinfocoll(nullptr);
  bool evinfo_available = store.get("EventInfo",evinfocoll);
  if(evinfo_available) {
    const EventInfoHandle& evinfoHandle = evinfocoll->get(0);
    const EventInfo& evinfo = evinfoHandle.read();
    if(verbose)
      std::cout << "event number " << evinfo.Number << std::endl;
    // COLIN avoid bug at first event
    if(evinfo.Number==0) {
      std::cerr<<"skipping bugged first event"<<std::endl;
      return;
    }
  }
}


int main(){
  // gSystem->Load("libDataModelExample.so");
  albers::Reader reader;
  albers::EventStore store(nullptr);
  store.setReader(&reader);
  reader.openFile("example.root");

  bool verbose = true;

  // unsigned nEvents = 5;
  unsigned nEvents = reader.getEntries();
  for(unsigned i=0; i<nEvents; ++i) {
    if(i%1000==0)
      std::cout<<"reading event "<<i<<std::endl;
    if(i>10)
      verbose = false;
    processEvent(store, verbose, reader);
    store.endOfEvent();
    reader.endOfEvent();
  }
  return 0;
}
