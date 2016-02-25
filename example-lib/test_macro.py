from ROOT import gSystem
gSystem.Load("libanalysiscpp-myanalysis")
from ROOT import MyAnalysis, TCanvas, gPad

ma = MyAnalysis()
ma.loop("example.root")
ma.m_h_ptc_pdgid.Draw()
gPad.Update()
