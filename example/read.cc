#include "datamodel/ParticleCollection.h"
#include "datamodel/EventInfoCollection.h"
#include "datamodel/JetCollection.h"
#include "datamodel/JetParticleAssociationCollection.h"

// Utility functions
#include "utilities/JetUtils.h"
#include "utilities/VectorUtils.h"
#include "utilities/ParticleUtils.h"

// ROOT
#include "TBranch.h"
#include "TFile.h"
#include "TTree.h"
#include "TROOT.h"
#include "TLorentzVector.h"

// STL
#include <vector>
#include <iostream>

// podio specific includes
#include "podio/EventStore.h"
#include "podio/ROOTReader.h"

void processEvent(podio::EventStore& store, bool verbose,
                  podio::ROOTReader& reader) {

  // read event information
  const fcc::EventInfoCollection* evinfocoll(nullptr);
  bool evinfo_available = store.get("EventInfo", evinfocoll);
  if(evinfo_available) {
    auto evinfo = evinfocoll->at(0);

    if(verbose)
      std::cout << "event number " << evinfo.Number() << std::endl;
  }

  // read particles
  const fcc::ParticleCollection* ptcs(nullptr);
  bool particles_available = store.get("GenParticle", ptcs);
  if (particles_available){
    std::vector<fcc::Particle> muons;
    // there is probably a smarter way to get a vector from collection?

    if(verbose)
      std::cout << "particle collection:" << std::endl;
    for(const auto& ptc : *ptcs){
      if(verbose)
        std::cout<<"\t"<<ptc<<std::endl;
      if( ptc.Core().Type == 4 ) {
        muons.push_back(ptc);
      }
    }
  }
}

int main(){
  auto reader = podio::ROOTReader();
  auto store = podio::EventStore();
  try {
    reader.openFile("example.root");
  }
  catch(std::runtime_error& err) {
    std::cerr<<err.what()<<". Quitting."<<std::endl;
    exit(1);
  }
  store.setReader(&reader);

  bool verbose = true;

  // unsigned nEvents = 5;
  unsigned nEvents = reader.getEntries();
  for(unsigned i=0; i<nEvents; ++i) {
    if(i%1000==0) {
      std::cout<<"reading event "<<i<<std::endl;
    }
    if(i>10) {
      verbose = false;
    }
    processEvent(store, verbose, reader);
    store.clear();
    reader.endOfEvent();
  }
  return 0;
}
