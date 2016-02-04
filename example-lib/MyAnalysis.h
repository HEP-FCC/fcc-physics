#ifndef __ANALYSISCPP_MYANALYSIS_H__
#define __ANALYSISCPP_MYANALYSIS_H__

#include "TObject.h"
#include "TH1F.h"

namespace podio {
  class EventStore;
  class ROOTReader;
}

class MyAnalysis {
 public:
  MyAnalysis();
  void loop(const char* filename);
  void processEvent(podio::EventStore& store, bool verbose,
		    podio::ROOTReader& reader);
  TH1F m_hjetenergy;
  TH1F m_hjetnparts;
};

#endif
