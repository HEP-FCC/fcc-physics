from ROOT import gSystem
gSystem.Load("libanalysiscpp-myanalysis")
from ROOT import MyAnalysis, TCanvas

ma = MyAnalysis()
ma.loop("example.root")
c1 = TCanvas()
c1.Divide(2,1)
c1.cd(1)
ma.m_hjetenergy.Draw()
c1.cd(2)
ma.m_hjetnparts.Draw()

