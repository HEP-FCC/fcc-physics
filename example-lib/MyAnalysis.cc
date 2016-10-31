#include "MyAnalysis.h"

// podio specific includes
#include "podio/EventStore.h"
#include "podio/ROOTReader.h"

#include "datamodel/EventInfoCollection.h"
#include "datamodel/ParticleCollection.h"

// Utility functions
#include "utilities/VectorUtils.h"
#include "utilities/ParticleUtils.h"

#include "TLorentzVector.h"

MyAnalysis::MyAnalysis() :
  m_h_ptc_pdgid("h_ptc_pdgid","particle pdgid", 500, -250, 250) {}


void MyAnalysis::loop(const char* filename) {
  m_h_ptc_pdgid.Reset();
  auto reader = podio::ROOTReader();
  auto store = podio::EventStore();
  try {
    reader.openFile(filename);
  }
  catch(std::runtime_error& err) {
    std::cerr << err.what() << ". Quitting." << std::endl;
    exit(1);
  }
  store.setReader(&reader);

  bool verbose = true;

  // unsigned nEvents = 5;
  unsigned nEvents = reader.getEntries();
  for (unsigned i=0; i<nEvents; ++i) {
    if (i%1000==0) {
      std::cout << "reading event " << i<<std::endl;
    }
    if (i>2) {
      verbose = false;
    }
    processEvent(store, verbose, reader);
    store.clear();
    reader.endOfEvent();
  }
  return;
}

void MyAnalysis::processEvent(podio::EventStore& store, bool verbose,
                  podio::ROOTReader& reader) {

  // read event information
  const fcc::EventInfoCollection* evinfocoll(nullptr);
  bool evinfo_available = store.get("EventInfo", evinfocoll);
  if (evinfo_available) {
    auto evinfo = evinfocoll->at(0);

    if (verbose) {
      std::cout << "event number " << evinfo.number() << std::endl;
    }
  }

  // read particles
  const fcc::ParticleCollection* ptcs(nullptr);
  bool particles_available = store.get("GenParticle", ptcs);
  if (particles_available) {
    if(verbose) {
      std::cout << "particle collection:" << std::endl;
    }
    for(const auto& ptc : *ptcs){
      if(verbose) {
        std::cout << "\t" << ptc << std::endl;
      }
      m_h_ptc_pdgid.Fill(ptc.pdgId());
    }
  }
}
