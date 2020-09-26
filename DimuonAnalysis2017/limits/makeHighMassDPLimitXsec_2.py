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
import glob

fff = open("eps2.txt", "w")

fileEff=ROOT.TFile.Open("/afs/cern.ch/user/j/jsalfeld/public/DarkPhotonInput/DPAcceptancesEfficiencies.root")
effGraph=fileEff.Get("eff2017")
acctot=fileEff.Get("totacc")
accoverlap=fileEff.Get("overlapacc")
accbarrel=fileEff.Get("barrelacc")

dpxsec=array('d',[484.3,292.8,193.6,98.95,57.97,43.98,37.16,31.69,25.39,18.26,13.66,10.59])
#dpxsec=array('d',[484.3,292.8,193.6,98.95,57.97,43.98,37.16,31.69,25.39,18.26,13.66,10.59])
dpm=array('d',[10,12.5,15,20,25,28,30,32,35,40,45,50])
gdpxsec=TGraph(len(dpxsec),dpm,dpxsec)

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

#xsec=1.#pb


#some counters
m=0.2
#t=0
#make the loop

#a=10#1./0.1143
a = (0.02**2)/5 #the factor 5 is for lumi (4->100)

for d in range(0,400):
	
	m=m+(m*0.01)
	#print m
	#print d

	file=glob.glob("higgsCombineasympMassIndex_"+str(d)+".AsymptoticLimits.mH*.root")
	if len(file)==0:
		continue

	#file90=glob.glob("higgsCombineIterV9_CL90_ForPress_2017_"+str(d)+".AsymptoticLimits.mH*.root")

	f=ROOT.TFile.Open(file[0])
	tree=f.Get("limit")
	tree.GetEntry(2)
	limit1.append(tree.limit*a)

	# f90=ROOT.TFile.Open(file90[0])
	# tree90=f90.Get("limit")
	# tree90.GetEntry(2)
	# limit190.append(tree90.limit*a)
	
	# dpxsecpred=gdpxsec.Eval(m,0,"")*acctot.Eval(m)
	# limiteps2.append((float(tree.limit)/float(dpxsecpred))*0.0004)
	# limiteps290.append((float(tree90.limit)/float(dpxsecpred))*0.0004)

	tree=f.Get("limit")
	tree.GetEntry(0)
	limit195up.append(abs(tree.limit*a-limit1[-1]))
	tree=f.Get("limit")
	tree.GetEntry(4)
	limit195down.append(abs(tree.limit*a-limit1[-1]))
	
	
	tree.GetEntry(1)
	limit168up.append(abs(tree.limit*a-limit1[-1]))
	tree=f.Get("limit")
	tree.GetEntry(3)
	limit168down.append(abs(tree.limit*a-limit1[-1]))
	
	#tree.GetEntry(5)
	#limitObserved.append(tree.limit*a)
		
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
mg.GetYaxis().SetRangeUser(1e-9,1e-1)
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
cmsTag3=ROOT.TLatex(0.90,0.917,"#scale[0.9]{#bf{4 fb^{-1} (13 TeV, 2017)}}")
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
c1.SaveAs("limit2018DarkPhoton2017Press.root")
c1.SaveAs("limit2018DarkPhoton2017Press.pdf")
c2=ROOT.TCanvas("c2","c2",700,500)
c2.SetLogy()
# graph_limiteps2=ROOT.TGraph(len(mass),mass,limiteps2)
# graph_limiteps2.SetTitle("")
# graph_limiteps2.Draw("ALP")
# graph_limiteps290=ROOT.TGraph(len(mass),mass,limiteps290)
# graph_limiteps290.SetTitle("")
# graph_limiteps290.Draw("ALP")

cmsTag.Draw()
cmsTag2.Draw()
cmsTag3.Draw()
# graph_limiteps2.SetMarkerSize(1)
# graph_limiteps2.SetMarkerStyle(20)
# graph_limiteps2.SetMarkerColor(kBlack)
# graph_limiteps2.SetLineWidth(2)
# graph_limiteps2.SetLineStyle(7)
# graph_limiteps2.SetLineColor(kBlue)
# graph_limiteps2.GetYaxis().SetRangeUser(0.0000001,0.0002)
# graph_limiteps2.GetXaxis().SetRangeUser(10,70)
# graph_limiteps2.GetYaxis().SetTitle("#epsilon^{2}")
# graph_limiteps2.GetYaxis().SetTitleSize(0.05)
# graph_limiteps2.GetXaxis().SetTitle("Dark Photon Mass [GeV]")
# graph_limiteps290.SetMarkerSize(1)
# graph_limiteps290.SetMarkerStyle(20)
# graph_limiteps290.SetMarkerColor(kGray)
# graph_limiteps290.SetLineWidth(2)
# graph_limiteps290.SetLineStyle(7)
# graph_limiteps290.SetLineColor(kRed)
# mgeps.Add(graph_limiteps2,"pl")
# mgeps.Add(graph_limiteps290,"pl")
mgeps.Draw("APC")
leg2=ROOT.TLegend(0.65, 0.65,0.87, 0.85)  
leg2.SetBorderSize( 0 )
leg2.SetFillStyle( 1001 )
leg2.SetFillColor(kWhite) 
# leg2.AddEntry( graph_limiteps2 , "Expected 95%",  "LP" )
#leg2.AddEntry( graph_limiteps290 , "Expected 90%",  "LP" )
leg2.Draw("same")
c2.SaveAs("thep.root")
gdpxsec.SaveAs("gdpxsec.root")
acctot.SaveAs("acctot.root")
