#ifndef TRANSLATEHEPMC_H
#define TRANSLATEHEPMC_H

#include "datamodel/MCParticleCollection.h"
#include "datamodel/GenVertexCollection.h"
#include "HepMC/GenEvent.h"

#include <unordered_map>

void translateEvent(HepMC::GenEvent* event,
                    fcc::MCParticleCollection& particles,
                    fcc::GenVertexCollection& vertices) {
  HepMC::FourVector tmp;
  std::unordered_map<HepMC::GenVertex*, unsigned> vertexMap;
  for(auto vertex_i = event->vertices_begin();
      vertex_i != event->vertices_end(); ++vertex_i ) {
    tmp = (*vertex_i)->position();
    auto vertex = vertices.create();
    auto& position = vertex.Position();
    position.X = tmp.x();
    position.Y = tmp.y();
    position.Z = tmp.z();
    vertex.Ctau(tmp.t()); // is ctau like this?
    vertexMap[*vertex_i] = vertex.getObjectID().index;
  }
  for (auto particle_i = event->particles_begin();
       particle_i != event->particles_end(); ++particle_i) {
    tmp = (*particle_i)->momentum();
    fcc::MCParticle particle = particles.create();
    fcc::BareParticle& core = particle.Core();
    core.Type = (*particle_i)->pdg_id();
    core.Status = (*particle_i)->status();
    core.P4.Px = tmp.px();
    core.P4.Py = tmp.py();
    core.P4.Pz = tmp.pz();
    core.P4.Mass = (*particle_i)->generated_mass();

    auto prodVtxIdx = vertexMap.find((*particle_i)->production_vertex());
    if (prodVtxIdx != end(vertexMap)) {
      particle.StartVertex(vertices.at(prodVtxIdx->second));
    }
    auto endVtxIdx = vertexMap.find((*particle_i)->end_vertex());
    if (endVtxIdx != end(vertexMap)) {
      particle.EndVertex(vertices.at(endVtxIdx->second));
    }
  }
  return;
}
#endif
