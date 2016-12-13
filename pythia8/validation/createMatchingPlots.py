from ROOT import TFile, gROOT, TH1D, kRed, TLegend
import os, ROOT, sys

# define print canvas function
def printCanvas(canvas, name, format, directory):
    if format != "":
        if not os.path.exists(directory) :
                os.system("mkdir "+directory)
        outFile = os.path.join(directory, name) + "." + format
        canvas.Print(outFile)

# define function for drawing multiple histograms in same canvas
def drawMultiHisto(name, ylabel, legend, leftText, rightText, format, directory, logY, histos):
    if logY : 
        name = name+"_logY"
    canvas = ROOT.TCanvas(name, name, 700, 600) 
    if logY : 
        canvas.SetLogy(1)

    # define font (TNR)
    font = 132

    # style for this canvas
    ROOT.gPad.SetLeftMargin(0.20)
    ROOT.gPad.SetRightMargin(0.10)
    ROOT.gPad.SetBottomMargin(0.20)
    ROOT.gStyle.SetOptStat(0000000)
    ROOT.gStyle.SetTextFont(font)    
    Tleft = ROOT.TLatex(0.20, 0.92, leftText)
    Tleft.SetNDC(ROOT.kTRUE)
    Tleft.SetTextSize(0.044)
    Tleft.SetTextFont(font)   
    Tright = ROOT.TText(0.90, 0.92, rightText)
    Tright.SetTextAlign(31);
    Tright.SetNDC(ROOT.kTRUE)
    Tright.SetTextSize(0.044)
    Tright.SetTextFont(font)

    # define colors
    colors = []
    colors.append(ROOT.kCyan-6);
    colors.append(ROOT.kBlue-6);
    colors.append(ROOT.kGreen-6);
    colors.append(ROOT.kMagenta-6);
    colors.append(ROOT.kOrange-6);

    # first retrieve maximum
    maxes = []
    for h in histos:
       maxes.append(h.GetMaximum())
    maxh = max(maxes)

    # setup and draw first histogram 
    histos[0].SetLineWidth(3)
    histos[0].SetLineColor(ROOT.kBlack)
    histos[0].GetXaxis().SetTitleFont(font)
    histos[0].GetXaxis().SetLabelFont(font)
    histos[0].GetYaxis().SetTitle(ylabel)
    histos[0].GetYaxis().SetTitleFont(font)
    histos[0].GetYaxis().SetLabelFont(font)
    histos[0].GetXaxis().SetTitleOffset(1.5)
    histos[0].GetYaxis().SetTitleOffset(1.6)
    histos[0].GetXaxis().SetLabelOffset(0.02)
    histos[0].GetYaxis().SetLabelOffset(0.02)
    histos[0].GetXaxis().SetTitleSize(0.06)
    histos[0].GetYaxis().SetTitleSize(0.06)
    histos[0].GetXaxis().SetLabelSize(0.06)
    histos[0].GetYaxis().SetLabelSize(0.06)
    histos[0].SetNdivisions(505);
    histos[0].GetYaxis().SetNdivisions(505);
    histos[0].SetMaximum(10*maxh) 
    histos[0].SetTitle("") 
    histos[0].Draw("")
    
    # loop over remaining histograms
    iterhist = iter(histos)
    next(iterhist)
    j = 0
    for h in iterhist:
       h.SetLineWidth(3)
       h.SetLineColor(colors[j])
       h.Draw("same")
       j += 1

    # re-draw first histo 
    histos[0].Draw("same")

    # draw legend and text
    legend.SetTextFont(font) 
    legend.Draw() 
    Tleft.Draw() 
    Tright.Draw() 

    # print histogram to file
    printCanvas(canvas, name, format, directory) 


##########################
# Produce matching plots 
##########################

gROOT.SetBatch()

if len(sys.argv) < 2:
  print " Usage: createMatchingPlots.py fccOutput.root"
  sys.exit(1)

treeName = "events"
File = sys.argv[1]

# define root file and tree
rootFile = TFile(File,"read")
tree = rootFile.Get(treeName)

# loop over tree to find out max number of generated partons
nJetsMax = 0
for event in tree :
    njets = int(event.mePsMatchingVars[0].value)  
    if njets > nJetsMax : 
        nJetsMax = njets

# exit code if sample was generated without extra jets
if nJetsMax == 0 : 
    print 'This sample was generated without matching/merging ...'
    print 'No need to produce validation plots. '
    sys.exit()


# define variable dict, needed for histograms
Vars = {   
    "d01":{"name":"log_{10} DRJ_{01}","title":"log(DJR)","bin":100,"xmin":0,"xmax":3},
    "d12":{"name":"log_{10} DRJ_{12}","title":"log(DJR)","bin":100,"xmin":0,"xmax":3},
    "d23":{"name":"log_{10} DRJ_{23}","title":"log(DJR)","bin":100,"xmin":0,"xmax":3},
    "d34":{"name":"log_{10} DRJ_{34}","title":"log(DJR)","bin":100,"xmin":0,"xmax":3},
    "pt1":{"name":"p_{T}^{1}","title":"p_{T} [GeV]","bin":100,"xmin":0,"xmax":200},
    "pt2":{"name":"p_{T}^{2}","title":"p_{T} [GeV]","bin":100,"xmin":0,"xmax":200},
    "pt3":{"name":"p_{T}^{3}","title":"p_{T} [GeV]","bin":100,"xmin":0,"xmax":100},
    "pt4":{"name":"p_{T}^{4}","title":"p_{T} [GeV]","bin":100,"xmin":0,"xmax":50}
}

# define index corresponding to each variable in the input tree
index = {
   "d01": 1, 
   "d12": 3, 
   "d23": 5, 
   "d34": 7, 
   "pt1": 2, 
   "pt2": 4, 
   "pt3": 6, 
   "pt4": 8 
}

# define histogram double dictionnary
dict_hist = {var: { npart: TH1D(var+"_"+str(npart),Vars[var]["name"]+";"+Vars[var]["title"]+";",Vars[var]["bin"],Vars[var]["xmin"],Vars[var]["xmax"]) for npart in xrange(nJetsMax+1) } for var in Vars }


#loop over tree 
i = 0
for event in tree :
     i += 1
     if (i%100 == 0) :
         print 'event number '+str(i)
     for var in sorted(Vars) :   
         for npart in xrange(nJetsMax+1):    
             if npart == event.mePsMatchingVars[0].value :
                dict_hist[var][npart].Fill(event.mePsMatchingVars[index[var]].value)

# draw histograms        
for var in sorted(Vars) :

    yAxisLabel = "# events"
    rightText = "Jet matching/merging validation"
    leftText = Vars[var]["name"]
    format = "png"
    
    if File.endswith('.root'):
        File = File[:-5]
    outputDirectory = "matching_plots_"+File

    leg = TLegend(0.65,0.70,0.88,0.88)
    leg.SetFillColor(0)
    leg.SetFillStyle(0)
    leg.SetLineColor(0)
    
    histos = []
    sumhisto = TH1D(var,var+";"+Vars[var]["title"]+";",Vars[var]["bin"],Vars[var]["xmin"],Vars[var]["xmax"])
    for npart in xrange(nJetsMax+1):    
        sumhisto.Add(dict_hist[var][npart])
        histos.append(dict_hist[var][npart])
        leg.AddEntry(dict_hist[var][npart],str(npart)+ " jet sample","l")

    leg.AddEntry(sumhisto,"all jet samples","l")
    drawMultiHisto(var,yAxisLabel,leg,leftText,rightText,format,outputDirectory,1,[sumhisto]+histos)


