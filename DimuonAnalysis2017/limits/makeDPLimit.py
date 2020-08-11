import ROOT
import os,sys
from ROOT import TGraphAsymmErrors
from ROOT import TGraphErrors
from ROOT import TColor
#from ROOT import TGraph
from array import array
from ROOT import *
from operator import truediv
import random
import math
from glob import glob
import re 
import sys
from math import sqrt

year = sys.argv[1]

fff = open("eps2.txt", "w")



limit1=array('d')
limiteps2=array('d')
limit190=array('d')
limiteps290=array('d')
limit195up=array('d')
limit195down=array('d')
limit168up=array('d')
limit168down=array('d')
limitObserved=array('d')
mass=array('d')
masserr=array('d')

lumi = 6.6 if year == "2018" else 4.
lumi_project = 100

#ACCEPTANCE
acc_file = TFile.Open("acceptances.root")
acc_teff = acc_file.Get("fit_func")
#nbins_acc = acc_teff.GetPassedHistogram().GetNbinsX()
#acceptances = array('d')
#m_acceptances = array('d')
#for j in range(nbins_acc):
#	acceptances.append(acc_teff.GetEfficiency(j+1))
#	m_acceptances.append(acc_teff.GetPassedHistogram().GetBinCenter(j+1))
#accgraph = TGraph(nbins_acc,m_acceptances,acceptances);

#THEO CROSS SECTION FOR EPS=0.02
m= array('d',[2.0,  3.0,  4.0,  5.0,  6.0,  7.0,  8.0,   9.0,   10.0,  12.5,  20.0])
xSec= array('d',[8541, 7514, 3323, 2055, 1422, 1043, 793.6, 621.1, 484.3, 292.8, 98.95])#[pb], model-dependent
xsecgraph = TGraph(len(m),m,xSec);
eps2scale = 1.
base_eps = 0.02 #epsilon for which the cross sections are computed

a = (base_eps**2)*eps2scale/sqrt(lumi_project/lumi) # for lumi projection (6.6->100)

files = glob("combine_output/"+year+"/higgsCombineasympMassIndex_*.AsymptoticLimits.mH*.root")

d_m = {}
for fname in files:
        m = float(re.search(r"mH(\d+\.?\d+).root", fname).group(1))
        d = int(re.search(r"Index_(\d+).Asymptotic", fname).group(1))
        d_m[d] = [m, fname]


d_m = sorted(d_m.items())
print d_m

for d,m_fname in d_m:
    m, fname = m_fname
    #file90=glob.glob("higgsCombineIterV9_CL90_ForPress_2018_"+str(d)+".AsymptoticLimits.mH*.root")

    acc = acc_teff.Eval(m)
    xsec = xsecgraph.Eval(m,0,"S")

    f=ROOT.TFile.Open(fname)
    tree=f.Get("limit")
    tree.GetEntry(2)
    limit1.append(tree.limit*a/(acc*xsec))

    tree.GetEntry(0)
    limit195up.append(abs(tree.limit*a/(acc*xsec)-limit1[-1]))
    tree=f.Get("limit")
    tree.GetEntry(4)
    limit195down.append(abs(tree.limit*a/(acc*xsec)-limit1[-1]))
    
    
    tree.GetEntry(1)
    limit168up.append(abs(tree.limit*a/(acc*xsec)-limit1[-1]))
    tree=f.Get("limit")
    tree.GetEntry(3)
    limit168down.append(abs(tree.limit*a/(acc*xsec)-limit1[-1]))

    mass.append(m)
    masserr.append(0.)
    fff.write("{0} {1}\n".format(m, math.sqrt(tree.limit*a)))
print limit1

c1=ROOT.TCanvas("c1","c1",700,500)
#c1.SetGrid()
c1.SetLogy()
#c1.SetLogx()

mg=ROOT.TMultiGraph()
mgeps=ROOT.TMultiGraph()
graph_limit1=ROOT.TGraph(len(mass),mass,limit1)
graph_limit1.SetTitle("graph_limit1")
graph_limit1.SetMarkerSize(1)
graph_limit1.SetMarkerStyle(20)
graph_limit1.SetMarkerColor(kBlack)
graph_limit1.SetLineWidth(2)
graph_limit1.SetLineStyle(7)
graph_limit1.GetYaxis().SetRangeUser(0,100)
graph_limit1.GetXaxis().SetRangeUser(10,70)
graph_limit1.GetYaxis().SetTitle("#sigma(pp#rightarrow A)#times BR(A#rightarrow #mu#mu)[pb]")
graph_limit1.GetYaxis().SetTitleSize(2)
graph_limit1.GetXaxis().SetTitle("Dark Photon Mass [GeV]")

#graph_limit=ROOT.TGraph(len(mass),mass,limitObserved)
#graph_limit.Draw("same")
graph_limit95up=ROOT.TGraphAsymmErrors(len(mass),mass,limit1,masserr,masserr,limit195up,limit195down)
graph_limit95up.SetTitle("graph_limit95up")
#graph_limit95up.SetMarkerSize(1)
#graph_limit95up.SetMarkerStyle(23)
graph_limit95up.SetFillColor(ROOT.TColor.GetColor(252,241,15))
#graph_limit95up.Draw("same")
graph_limit95down=ROOT.TGraph(len(mass),mass,limit195down)
graph_limit95down.SetTitle("graph_limit95down")
graph_limit95down.SetMarkerSize(1)
graph_limit95down.SetMarkerStyle(23)
graph_limit95down.SetMarkerColor(kYellow)
#graph_limit95down.Draw("same")

graph_limit68up=ROOT.TGraphAsymmErrors(len(mass),mass,limit1,masserr,masserr,limit168up,limit168down)
graph_limit68up.SetTitle("graph_limit68up")
#graph_limit68up.SetMarkerSize(1)
graph_limit68up.SetFillColor(kGreen);
#graph_limit68up.SetMarkerStyle(22)
graph_limit68up.SetMarkerColor(kGreen)
##graph_limit68up.Draw("same")
graph_limit68down=ROOT.TGraph(len(mass),mass,limit168down)
graph_limit68down.SetTitle("graph_limit68down")
graph_limit68down.SetMarkerSize(1)
graph_limit68down.SetMarkerStyle(22)
graph_limit68down.SetMarkerColor(kGreen)

mg.Add(graph_limit95up,"3")
mg.Add(graph_limit68up,"3")
mg.Add(graph_limit1,"pl")
#mg.Add(graph_limit,"pl")

mg.Draw("APC")
mg.GetXaxis().SetRangeUser(1.2,9.)
mg.GetYaxis().SetRangeUser(5e-7,1e-2)
#mg.GetYaxis().SetTitle("xSec*BR [pb]")
#mg.GetXaxis().SetTitle("Dark Photon Mass [GeV]")
mg.GetYaxis().SetTitle("#epsilon^{2}")
mg.GetYaxis().SetTitleOffset(0.9)
mg.GetYaxis().SetTitleSize(0.05)
mg.GetXaxis().SetTitle("Dark Photon Mass [GeV]")
mg.GetXaxis().SetTitleSize(0.05)
c1.Update()
legend=ROOT.TLegend(0.5,0.6,0.8,0.9)
cmsTag=ROOT.TLatex(0.13,0.917,"#scale[1.1]{CMS}")
cmsTag.SetNDC()
cmsTag.SetTextAlign(11)
cmsTag.Draw()
cmsTag2=ROOT.TLatex(0.215,0.917,"#scale[0.825]{#bf{#it{Preliminary}}}")
cmsTag2.SetNDC()
cmsTag2.SetTextAlign(11)
#cmsTag.SetTextFont(61)
cmsTag2.Draw()
cmsTag3=ROOT.TLatex(0.90,0.917,"#scale[0.9]{#bf{"+str(lumi)+" fb^{-1} (13 TeV, "+year+")}}")
cmsTag3.SetNDC()
cmsTag3.SetTextAlign(31)
#cmsTag.SetTextFont(61)
cmsTag3.Draw()
leg=ROOT.TLegend(0.65, 0.65,0.90, 0.85)  
leg.SetBorderSize( 0 )
leg.SetFillStyle( 1001 )
leg.SetFillColor(kWhite) 
#leg.AddEntry( obse , "Observed",  "LP" )
leg.AddEntry( graph_limit1 , "Expected",  "LP" )
leg.AddEntry( graph_limit68up, "#pm 1#sigma",  "F" ) 
leg.AddEntry( graph_limit95up, "#pm 2#sigma",  "F" ) 
leg.Draw("same")
c1.SaveAs("limit"+year+"DarkPhoton_eps2.root")
c1.SaveAs("limit"+year+"DarkPhoton_eps2.pdf")
c2=ROOT.TCanvas("c2","c2",700,500)
c2.SetLogy()
cmsTag.Draw()
cmsTag2.Draw()
cmsTag3.Draw()
mgeps.Draw("APC")
leg2=ROOT.TLegend(0.65, 0.65,0.87, 0.85)  
leg2.SetBorderSize( 0 )
leg2.SetFillStyle( 1001 )
leg2.SetFillColor(kWhite) 

leg2.Draw("same")
c2.SaveAs("thep.root")
