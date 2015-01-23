// Data model
#include "datamodel/EventInfo.h"
#include "datamodel/EventInfoCollection.h"
#include "datamodel/MCParticle.h"
#include "datamodel/MCParticleCollection.h"
#include "datamodel/GenVertex.h"
#include "datamodel/GenVertexCollection.h"
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
  MCParticleCollection& pcoll = store.create<MCParticleCollection>("GenParticle");
  GenVertexCollection& vcoll = store.create<GenVertexCollection>("GenVertex");

  writer.registerForWrite<EventInfoCollection>("EventInfo");

  writer.registerForWrite<MCParticleCollection>("GenParticle");
  writer.registerForWrite<GenVertexCollection>("GenVertex");

  unsigned nevents=1000;

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
    EventInfoHandle evinfo = evinfocoll->create();
    evinfo.mod().Number = iev;

    if (!pythia.next()) continue;
    HepMC::GenEvent* hepmcevt = new HepMC::GenEvent();
    ToHepMC.fill_next_event( pythia, hepmcevt );

    //    std::cout<<"Nvtx = "<<hepmcevt->vertices_size()<<std::endl;
    typedef std::map<HepMC::GenVertex*, GenVertexHandle > VertexMap;
    VertexMap vtx_map;
    for ( HepMC::GenEvent::vertex_iterator iv = hepmcevt->vertices_begin();
	  iv != hepmcevt->vertices_end(); ++iv ) {
      //      (*iv)->print();
      // std::cout<<"hepmc vertex "<<(*iv)<<endl;
      const HepMC::FourVector& vpos = (*iv)->position();
      GenVertexHandle vtx = vcoll.create();
      // std::cout<<"vertex "<<vtx.index()<<"/"<<vtx.containerID()<<std::endl;
      vtx.mod().Position.X = vpos.x();
      vtx.mod().Position.Y = vpos.y();
      vtx.mod().Position.Z = vpos.z();
      vtx.mod().Ctau = vpos.t();
      vtx_map.emplace(*iv, vtx);
      // std::cout<<" in map "<<vtx_map[*iv].index()<<"/"<<vtx_map[*iv].containerID()<<std::endl;
    }
    //     std::cout<<"Nptc = "<<hepmcevt->particles_size()<<std::endl;
    for ( HepMC::GenEvent::particle_iterator ip = hepmcevt->particles_begin();
	  ip != hepmcevt->particles_end(); ++ip ) {
      HepMC::GenParticle* hepmcptc = *ip; 
      MCParticleHandle ptc = pcoll.create();
      BareParticle& core = ptc.mod().Core; 
      core.Type = hepmcptc->pdg_id();
      core.Status = hepmcptc->status();
      core.P4.Pt = hepmcptc->momentum().perp();
      core.P4.Eta = hepmcptc->momentum().eta();
      core.P4.Phi = hepmcptc->momentum().phi();
      core.P4.Mass = hepmcptc->momentum().m();
      // hepmcptc->print();

      typedef VertexMap::const_iterator IVM;
      IVM prodvtx = vtx_map.find(hepmcptc->production_vertex());
      if(prodvtx!=vtx_map.end()) {
	ptc.mod().StartVertex = prodvtx->second;
	// std::cout<<"prod vertex found "
		 // <<hepmcptc->production_vertex()<<" "
		 // <<prodvtx->first<<" "
		 // <<prodvtx->second.index()<<"/"
		 // <<prodvtx->second.containerID()<<std::endl;	
      }
      else{
	// std::cout<<"no prod vertex found"<<std::endl;
      }
      IVM endvtx = vtx_map.find(hepmcptc->end_vertex());
      if(endvtx!=vtx_map.end()) {
	ptc.mod().EndVertex = endvtx->second;
      }
      else{
	// std::cout<<"no end vertex found"<<std::endl;
	// hepmcptc->print();
      }
    }

    writer.writeEvent();
    store.next();
  }

  writer.finish();

  return 0;
}
