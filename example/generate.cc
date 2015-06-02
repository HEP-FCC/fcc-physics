// Data model
#include "datamodel/EventInfo.h"
#include "datamodel/EventInfoCollection.h"
#include "datamodel/MCParticle.h"
#include "datamodel/MCParticleCollection.h"
#include "datamodel/GenJet.h"
#include "datamodel/GenJetCollection.h"
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
#include "Pythia8/ParticleData.h"

#include "fastjet/PseudoJet.hh"
#include "fastjet/ClusterSequence.hh"

int main(){
  std::cout<<"start processing"<<std::endl;

  albers::Registry   registry;
  albers::EventStore store(&registry);
  albers::Writer     writer("example.root", &registry);


  EventInfoCollection& evinfocoll = store.create<EventInfoCollection>("EventInfo");
  MCParticleCollection& pcoll = store.create<MCParticleCollection>("GenParticle");
  GenVertexCollection& vcoll = store.create<GenVertexCollection>("GenVertex");
  GenJetCollection& genjetcoll = store.create<GenJetCollection>("GenJet");

  writer.registerForWrite<EventInfoCollection>("EventInfo");
  writer.registerForWrite<MCParticleCollection>("GenParticle");
  writer.registerForWrite<GenVertexCollection>("GenVertex");
  writer.registerForWrite<GenJetCollection>("GenJet");

  unsigned nevents=5000;

  // Generator. Process selection. LHC initialization. Histogram.
  Pythia8::Pythia pythia;
  pythia.readString("Beams:idA = 11");
  pythia.readString("Beams:idB = -11");

  pythia.readString("Beams:eCM = 91.");
  pythia.readString("WeakSingleBoson:all = on");
  pythia.readString("WeakZ0:gmZmode = 0");
  pythia.readString("PhaseSpace:pTHatMin = 20.");
  // pythia.readString("23:onMode = off");
  // pythia.readString("23:onIfMatch = 1 1");
  pythia.init();

  // pythia.readString("Beams:eCM = 180.");
  // pythia.readString("WeakDoubleBoson:ffbar2WW = on");

  // pythia.readString("Beams:eCM = 240.");
  // pythia.readString("HiggsSM:ffbar2HZ = on");
  // pythia.readString("23:onMode = off");
  // pythia.readString("23:onIfMatch = 13 13");
  // pythia.readString("25:onMode = off");
  // pythia.readString("25:onIfMatch = 5 5");

  // pythia.readString("Beams:eCM = 350.");
  // pythia.readString("Top:ffbar2ttbar(s:gmZ) = on");

  // pythia.readString("PhaseSpace:pTHatMin = 20.");
  // pythia.init();

  
  // Interface for conversion from Pythia8::Event to HepMC event.
  HepMC::Pythia8ToHepMC ToHepMC;
  Pythia8::ParticleData PDG; 

  // Fastjet

  fastjet::Strategy strategy = fastjet::Best;
  fastjet::RecombinationScheme recomb_scheme = fastjet::E_scheme;
  double rparam = 0.5;
  fastjet::JetDefinition jet_def(fastjet::antikt_algorithm, rparam,
                                 recomb_scheme, strategy);
  
  vector<fastjet::PseudoJet> input_particles;
  // input_particles.push_back( fastjet::PseudoJet(10,0,0,10) );
  // input_particles.push_back( fastjet::PseudoJet(-10,0,0,10) );
  // fastjet::ClusterSequence clust_seq(input_particles, jet_def);
  // double ptmin = 0.5;
  // vector<fastjet::PseudoJet> jets = clust_seq.inclusive_jets(ptmin);
  // cout<<"jets:"<<endl;
  // for(unsigned i=0; i<jets.size(); ++i) {
  //   cout<<"\t"<<jets[i].pt()<<" "<<jets[i].eta()<<" "<<jets[i].phi()<<endl;
  // }
  
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
    input_particles.clear();
    for ( HepMC::GenEvent::particle_iterator ip = hepmcevt->particles_begin();
	  ip != hepmcevt->particles_end(); ++ip ) {
      HepMC::GenParticle* hepmcptc = *ip; 
      MCParticleHandle ptc = pcoll.create();
      BareParticle& core = ptc.mod().Core; 
      core.Type = hepmcptc->pdg_id();
      core.Charge = pythia.particleData.charge(core.Type);
      core.Status = hepmcptc->status();
      core.P4.Px = hepmcptc->momentum().px();
      core.P4.Py = hepmcptc->momentum().py();
      core.P4.Pz = hepmcptc->momentum().pz();
      core.P4.Mass = hepmcptc->momentum().m();
      // hepmcptc->print();

      if(core.Status==1) {
	input_particles.push_back( fastjet::PseudoJet(hepmcptc->momentum().px(),
						      hepmcptc->momentum().py(),
						      hepmcptc->momentum().pz(),
						      hepmcptc->momentum().e() ));
      }
      vector<fastjet::PseudoJet> input_particles;
      
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
    fastjet::ClusterSequence clust_seq(input_particles, jet_def);
    double ptmin = 10;
    vector<fastjet::PseudoJet> jets = clust_seq.inclusive_jets(ptmin);
    // cout<<"jets:"<<endl;
    for(unsigned i=0; i<jets.size(); ++i) {
      // cout<<"\t"<<jets[i].pt()<<" "<<jets[i].eta()<<" "<<jets[i].phi()<<endl;
      GenJetHandle genjet = genjetcoll.create();
      BareJet& core = genjet.mod().Core;
      core.P4.Px = jets[i].px();
      core.P4.Py = jets[i].py();
      core.P4.Pz = jets[i].pz();
      core.P4.Mass = jets[i].m();    
    }

    writer.writeEvent();
    store.next();
  }

  writer.finish();

  return 0;
}
