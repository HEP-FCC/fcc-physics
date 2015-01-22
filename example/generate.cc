// Data model
#include "datamodel/EventInfo.h"
#include "datamodel/EventInfoCollection.h"
#include "datamodel/MCParticle.h"
#include "datamodel/MCParticleCollection.h"
#include "datamodel/LorentzVector.h"

// STL
#include <iostream>
#include <vector>

// albers specific includes
#include "albers/EventStore.h"
#include "albers/Registry.h"
#include "albers/Writer.h"

#include "Pythia8/Pythia.h"
#include "Pythia8Plugins/HepMC2.h"

int main(){
  std::cout<<"start processing"<<std::endl;

  albers::Registry   registry;
  albers::EventStore store(&registry);
  albers::Writer     writer("example.root", &registry);


  EventInfoCollection& evinfocoll = store.create<EventInfoCollection>("EventInfo");
  MCParticleCollection& pcoll = store.create<MCParticleCollection>("MCParticle");

  writer.registerForWrite<EventInfoCollection>("EventInfo");

  // collections from the dummy generator
  writer.registerForWrite<MCParticleCollection>("MCParticle");

  unsigned nevents=10;

  // Generator. Process selection. LHC initialization. Histogram.
  Pythia8::Pythia pythia;
  pythia.readString("Beams:idA = 11");
  pythia.readString("Beams:idB = -11");
  pythia.readString("Beams:eCM = 91.");
  pythia.readString("WeakSingleBoson:all = on");
  pythia.readString("WeakZ0:gmZmode = 0");
  pythia.readString("PhaseSpace:pTHatMin = 20.");
  pythia.init();

  // Interface for conversion from Pythia8::Event to HepMC event.
  HepMC::Pythia8ToHepMC ToHepMC;
  
  for(unsigned iev=0; iev<nevents; ++iev) {
    // fill event information
    EventInfoCollection* evinfocoll = nullptr;
    // here, asking the store for the collection.
    // could also just reuse the reference obtained at the time of the creation
    // of the collection
    store.get("EventInfo", evinfocoll);
    if(evinfocoll==nullptr) {
      std::cerr<<"collection EventInfo does not exist!"<<std::endl;
      return 1;
    }
    EventInfoHandle& evinfo = evinfocoll->create();
    evinfo.mod().Number = iev;

    if (!pythia.next()) continue;
    HepMC::GenEvent* hepmcevt = new HepMC::GenEvent();
    ToHepMC.fill_next_event( pythia, hepmcevt );

    std::cout<<"Nvtx = "<<hepmcevt->vertices_size()<<std::endl;
    for ( HepMC::GenEvent::vertex_iterator v = hepmcevt->vertices_begin();
	  v != hepmcevt->vertices_end(); ++v ) {
      (*v)->print();
    }

    std::cout<<"Nptc = "<<hepmcevt->particles_size()<<std::endl;
    for ( HepMC::GenEvent::particle_iterator ip = hepmcevt->particles_begin();
	  ip != hepmcevt->particles_end(); ++ip ) {
      HepMC::GenParticle* hepmcptc = *ip; 
      MCParticleHandle& ptc = pcoll.create();
      BareParticle& core = ptc.mod().Core; 
      core.Type = hepmcptc->pdg_id();
      core.Status = hepmcptc->status();
      core.P4.Pt = hepmcptc->momentum().perp();
      core.P4.Eta = hepmcptc->momentum().eta();
      core.P4.Phi = hepmcptc->momentum().phi();
      core.P4.Mass = hepmcptc->momentum().m();
    }

    writer.writeEvent();
    store.next();
  }

  writer.finish();

  return 0;
}
