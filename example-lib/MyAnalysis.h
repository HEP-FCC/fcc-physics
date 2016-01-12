#ifndef __ANALYSISCPP_MYANALYSIS_H__
#define __ANALYSISCPP_MYANALYSIS_H__

#include "TObject.h"
#include "TH1F.h"

namespace albers {
  class EventStore;
  class Reader;
}

class MyAnalysis {
 public:
  MyAnalysis();
  void loop(const char* filename);
  void processEvent(albers::EventStore& store, bool verbose,
		    albers::Reader& reader);
  TH1F m_hjetenergy;
  TH1F m_hjetnparts;
};

#endif
