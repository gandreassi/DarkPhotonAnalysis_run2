import ROOT
import os,sys,math
from ROOT import TGraph
#from ROOT import TGraph
from array import array
from ROOT import *
import tdrstyle
ROOT.gROOT.ForceStyle()
ROOT.gStyle.SetOptStat(0)
#nominal  file=ROOT.TFile.Open("./allL12018ABCD.root")
file=ROOT.TFile.Open("./L118_xyznew.root")
#file=ROOT.TFile.Open("./alltheL1.root")

#names=['L1_DoubleMu_12_5','L1_DoubleMu_12_8','L1_DoubleMu_13_6','L1_DoubleMu_15_5','L1_DoubleMu_15_7','L1_TripleMu_4_4_4','L1_TripleMu_5_0_0','L1_TripleMu_5_3_3','L1_TripleMu_5_5_3','L1_QuadMu0','L1_DoubleMu0er1p5_SQ_OS_dR_Max1p4','L1_DoubleMu4p5er2p0_SQ_OS_Mass7to18','L1_DoubleMu4_SQ_OS_dR_Max1p2','L1_DoubleMu5_SQ_OS_Mass7to18','L1_DoubleMu_20_2_SQ_Mass_Max20','L1_DoubleMu0er1p4_SQ_OS_dR_Max1p4','L1_DoubleMu4p5_SQ_OS_dR_Max1p2','L1_DoubleMu6_SQ_OS','L1_DoubleMu0er1p5_SQ_dR_Max1p4','L1_DoubleMu0er2_SQ_dR_Max1p4','L1_DoubleMu0_SQ']

c2=ROOT.TCanvas("c2","c2",900,600)
c2.SetLogy()
c2.SetLogx()

names=['L1_DoubleMu4p5er2p0_SQ_OS_Mass_Min7', 'L1_DoubleMu_12_5','L1_DoubleMu_15_7','L1_TripleMu_5_3_3','L1_TripleMu_5_5_3','L1_QuadMu0','L1_DoubleMu0er1p5_SQ_OS_dR_Max1p4','L1_DoubleMu4p5er2p0_SQ_OS_Mass7to18','L1_DoubleMu4_SQ_OS_dR_Max1p2','L1_SingleMu22','L1_DoubleMu0er1p4_SQ_OS_dR_Max1p4','L1_DoubleMu4p5_SQ_OS_dR_Max1p2','L1_DoubleMu4p5_SQ_OS','L1_DoubleMu0er1p5_SQ_dR_Max1p4','L1_DoubleMu0er2p0_SQ_dR_Max1p4','L1_DoubleMu0_SQ']

#labels=['2#mu, p_{T} > 12/5 GeV','2#mu, p_{T} > 12/8 GeV','2#mu, p_{T} > 13/6 GeV','2#mu, p_{T} > 15/5 GeV','2#mu, p_{T} > 15/7 GeV','3#mu, p_{T} > 4 GeV', '3#mu, p_{T} > 5/0/0 GeV', '3#mu, p_{T} > 5/3/3 GeV','3#mu, p_{T} > 5/5/3 GeV','4#mu, p_{T} > 0 GeV', '2#mu, p_{T} > 0 GeV, |#eta|<1.5, OS, #DeltaR<1.4', '2#mu, p_{T} > 4.5 GeV, |#eta|<2, OS, 7<m(2#mu)<18 GeV', '2#mu, p_{T} > 4 GeV, OS, #DeltaR<1.2', '2#mu, p_{T} > 5 GeV, OS, 7<m(2#mu)<18 GeV','2#mu, p_{T} > 20/2 GeV, m(2#mu)<20 GeV','2#mu, p_{T} > 0 GeV, |#eta|<1.4, OS, #DeltaR<1.4','2#mu, p_{T} > 4.5 GeV, OS, #DeltaR<1.2','2#mu, p_{T} > 6 GeV, OS','2#mu, p_{T} > 0 GeV, |#eta|<1.5, #DeltaR<1.4','2#mu, p_{T} > 0 GeV, |#eta|<2, #DeltaR<1.4','2#mu, p_{T} > 0 GeV']
labels=['2#mu, p_{T} > 4.5 GeV, |#eta|<2, OS, m(2#mu)>7 GeV','XYZ','2#mu, p_{T} > 15/7 GeV', '3#mu, p_{T} > 5/3/3 GeV','3#mu, p_{T} > 5/5/3 GeV', 'XYZ', '2#mu, p_{T} > 0 GeV, |#eta|<1.5, OS, #DeltaR<1.4', '2#mu, p_{T} > 4.5 GeV, |#eta|<2, OS, 7<m(2#mu)<18 GeV', '2#mu, p_{T} > 4 GeV, OS, #DeltaR<1.2', '1#mu, p_{T} > 22 GeV','2#mu, p_{T} > 0 GeV, |#eta|<1.4, OS, #DeltaR<1.4', '2#mu, p_{T} > 4.5 GeV, OS, #DeltaR<1.2','2#mu, p_{T} > 4.5 GeV, OS','2#mu, p_{T} > 0 GeV, |#eta|<1.5, OS, #DeltaR<1.4','2#mu, p_{T} > 0 GeV, |#eta|<2, #DeltaR<1.4','2#mu, p_{T} > 0 GeV']

#'2#mu, p_{T} > 5 GeV, OS, 7<m(2#mu)<18 GeV','2#mu, p_{T} > 20/2 GeV, m(2#mu)<20 GeV','2#mu, p_{T} > 0 GeV, |#eta|<1.4, OS, #DeltaR<1.4','2#mu, p_{T} > 4.5 GeV, OS, #DeltaR<1.2','2#mu, p_{T} > 6 GeV, OS','2#mu, p_{T} > 0 GeV, |#eta|<1.5, #DeltaR<1.4','2#mu, p_{T} > 0 GeV, |#eta|<2, #DeltaR<1.4','2#mu, p_{T} > 0 GeV']

#bphys orange 
#highpt blue
#lowpt red

colors=[kBlue,kBlue-4,kBlue-7,kBlue-9,kBlue-10,kRed,kRed-4,kRed-7,kRed-9,kRed-10,kOrange,kOrange-4,kOrange-7,kOrange-9,kOrange-10,kGreen,kGreen-4,kGreen-7,kGreen-9,kGreen-10,kRed-1]


legend = ROOT.TLegend(0.21,0.18,0.74,0.38)
legend.SetNColumns(2)
#legend.SetLegendTextSize(0.5)
#legend.SetNDC()
for i in range(0,15):
	hist=file.Get("massOS_L1_"+str(i))
	for n in range(0,hist.GetNbinsX()):
		hist.SetBinContent(n,hist.GetBinContent(n)/hist.GetBinWidth(n))

	 
	#hist.SetLineWidth(3)
	ROOT.gStyle.SetOptStat(0)

#	if names[i] in ['L1_DoubleMu_20_2_SQ_Mass_Max20','L1_TripleMu_5_0_0','L1_QuadMu0','L1_DoubleMu6_SQ_OS','L1_DoubleMu0er1p5_SQ_dR_Max1p4','L1_DoubleMu0er2_SQ_dR_Max1p4']:
#		continue

	#hist.SetMarkerStyle(19)	
	#hist.SetMarkerSize(16)	
	hist.SetLineColor(colors[i])
	if i<6:      
		hist.SetLineColor(kBlue-i)
		hist.SetMarkerColor(kBlue-i)
	else:
		hist.SetLineColor(kRed-i+6+1)
		hist.SetMarkerColor(kRed-i+6+1)

	if names[i]=='L1_DoubleMu4p5er2p0_SQ_OS_Mass_Min7':
		hist.SetLineColor(kOrange+2)
		hist.SetMarkerColor(kOrange+2)
	if names[i]=='L1_DoubleMu4p5er2p0_SQ_OS_Mass7to18':
		hist.SetLineColor(kOrange+7)
		hist.SetMarkerColor(kOrange+7)

	if names[i]=='L1_DoubleMu0_SQ':
		hist.SetLineColor(kGreen-3)
		hist.SetMarkerColor(kGreen-3)
	if names[i]=='L1_DoubleMu4p5_SQ_OS':
		hist.SetLineColor(kGreen-1)
		hist.SetMarkerColor(kGreen-1)

	if names[i]=='L1_SingleMu22':
		hist.SetLineColor(kCyan-3)
		hist.SetMarkerColor(kCyan-3)
	

#'L1_DoubleMu4p5er2p0_SQ_OS_Mass_Min7'
	if hist.Integral() < 1.:
		print str(names[i])
		continue
		
	'''
	if i<5.:
		hist.SetLineColor(kPink+(i%5))
	if i>=5 and i<10.:
		hist.SetLineColor(kAzure+(i%5))
	if i>=10 and i<15.:
		hist.SetLineColor(kMagenta+(i%5))
	if i>=16:
		hist.SetLineColor(kCyan+(i%5))
	'''
	#if hist.Integral() > 0.:
	#	hist.Scale(1./hist.Integral())
	#if i==20:
	#	hist.Scale(200.)
	
	
	legend.AddEntry(hist  ,str(labels[i]),"l")
	hist.SetTitle("")
	hist.GetYaxis().SetRangeUser(10.,10**12)
	hist.GetYaxis().SetTitle("Events/GeV #times Prescale")
	#hist.GetYaxis().SetTitle("Events/GeV")
	hist.GetXaxis().SetTitle("#mu^{+} #mu^{-} invariant mass [GeV]")
	#hist.SetMarkerSize(2)
	#hist
	ROOT.gStyle.SetOptStat(0)
	hist.SetLineWidth(1)
	#hist.Draw("same E")
	hist.Draw("same")

#Plot(25000., 8., True)
ROOT.gStyle.SetOptStat(0)
#tdrstyle.cmsPrel(40000,13.,False,True,0,2.,0.,True)
ROOT.gStyle.SetOptStat(0)
legend.SetFillStyle(0)
legend.Draw()

cmsTag=ROOT.TLatex(0.2,0.88,"CMS")
cmsTag.SetNDC()
cmsTag.SetTextAlign(11)
#cmsTag.SetTextFont(61)
cmsTag.Draw()
cmsTag2=ROOT.TLatex(0.2,0.83,"#scale[0.75]{#bf{#it{Preliminary}}}")
cmsTag2.SetNDC()
cmsTag2.SetTextAlign(11)
#cmsTag.SetTextFont(61)
cmsTag2.Draw()

cmsTag3=ROOT.TLatex(0.94,0.960,"#scale[0.9]{#bf{34 fb^{-1} (13 TeV, 2018)}}")
cmsTag3.SetNDC()
cmsTag3.SetTextAlign(31)
#cmsTag.SetTextFont(61)
cmsTag3.Draw()



LTag3=ROOT.TLatex(0.47,0.4,"#scale[0.5]{L1-Trigger Selection Requirements}")
LTag3.SetNDC()
LTag3.SetTextAlign(31)
#cmsTag.SetTextFont(61)
LTag3.Draw()

cmsTag4=ROOT.TLatex(0.87,0.85,"#splitline{#scale[0.6]{Online Reconstructed Dimuon Events}}{#scale[0.6]{p_{T}(#mu) > 3 GeV, #eta(#mu) < 2.4, opposite sign}}")
cmsTag4.SetNDC()
cmsTag4.SetTextAlign(31)
#cmsTag.SetTextFont(61)
cmsTag4.Draw()

a=-0.00
eta=ROOT.TLatex(0.28,0.73+a,"#scale[0.75]{#eta}")
eta.SetNDC()
eta.SetTextAlign(31)
#cmsTag.SetTextFont(61)
eta.Draw()

rho=ROOT.TLatex(0.335,0.76+a,"#scale[0.75]{#rho,#omega}")
rho.SetNDC()
rho.SetTextAlign(31)
#cmsTag.SetTextFont(61)
rho.Draw()

phi=ROOT.TLatex(0.365,0.75+a,"#scale[0.75]{#phi}")
phi.SetNDC()
phi.SetTextAlign(31)
#cmsTag.SetTextFont(61)
phi.Draw()

jpsi=ROOT.TLatex(0.48,0.82+a,"#scale[0.75]{#bf{J}/#Psi}")
jpsi.SetNDC()
jpsi.SetTextAlign(31)
#cmsTag.SetTextFont(61)
jpsi.Draw()

psip=ROOT.TLatex(0.53,0.71+a,"#scale[0.75]{#Psi'}")
psip.SetNDC()
psip.SetTextAlign(31)
#cmsTag.SetTextFont(61)
psip.Draw()

yps=ROOT.TLatex(0.65,0.71+a,"#scale[0.75]{#bf{Y(nS)}}")
yps.SetNDC()
yps.SetTextAlign(31)
#cmsTag.SetTextFont(61)
yps.Draw()

z0=ROOT.TLatex(0.875,0.59+a,"#scale[0.75]{#bf{Z}}")
z0.SetNDC()
z0.SetTextAlign(31)
#cmsTag.SetTextFont(61)
z0.Draw()


c2.RedrawAxis()
c2.Update()
c2.SaveAs("alltheL1new18.root")
c2.SaveAs("alltheL1new18.pdf")



'''
from ROOT import gPad
    
test1 = Plot('test1')
# tdrstyle.cmsPrel(25000., 8., True)
test1.save()
    
test2 = Plot('test2')
tdrstyle.cmsPrel(25000., 13., False)
test2.save()
test3 = Plot('test3')
tdrstyle.cmsPrel(-1, 8., True)
test3.save()
    
test4 = Plot('test4', onLeft=True)
tdrstyle.cmsPrel(-1, 8., True, onLeft=False)
test4.save()
    
gPad.Update()
'''




	

