import ROOT
import os,sys,math
from ROOT import TGraph
#from ROOT import TGraph
from array import array
from ROOT import *
alphaEM = 0.00729682004
ratioR = 11./3.

massHyp=array('d')
excludeEps2Hyp=array('d')

file=ROOT.TFile.Open("trimscout_RunD_noidiso.root")

highMassOS=file.Get("highMassOS")
highMassLS=file.Get("highMassLS")
lowMassOS=file.Get("lowMassOS")
lowMassLS=file.Get("lowMassLS")
specMassOS=file.Get("specMassOS")
specMassLS=file.Get("specMassLS")
yMassOS=file.Get("yMassOS")
yMassLS=file.Get("yMassSS")
lowOrYOrHighMassOS=file.Get("lowOrYOrHighMassOS")
lowOrYOrHighMassLS=file.Get("lowOrYOrHighMassLS")
lowOrHighMassOS=file.Get("lowOrHighMassOS")
lowOrHighMassLS=file.Get("lowOrHighMassLS")
specOrYOrHighMassOS=file.Get("specOrYOrHighMassOS")
specOrYOrHighMassLS=file.Get("specOrYOrHighMassLS")
specOrHighMassOS=file.Get("specOrHighMassOS")
specOrHighMassLS=file.Get("specOrHighMassLS")

ROOT.gStyle.SetOptStat(000000000)

c2=ROOT.TCanvas("c2","c2",700,400)
c2.SetLogy()
c2.SetLogx()
highMassOS.SetStats(0)
highMassLS.SetStats(0)
highMassOS.Draw()
highMassLS.SetLineColor(kRed)
highMassLS.Draw("same")
highMassOS.GetYaxis().SetTitle("#Events/(0.025 GeV)")
highMassOS.GetXaxis().SetTitle("m(2Mu)[GeV]")
c2.Update()
c2.Print("comparisonHighOS.root")

c3=ROOT.TCanvas("c2","c2",700,400)
c3.SetLogy()
c3.SetLogx()
specMassOS.SetStats(0)
specMassLS.SetStats(0)
specMassOS.Draw()
specMassLS.SetLineColor(kRed)
specMassLS.Draw("same")
specMassOS.GetYaxis().SetTitle("#Events/(0.025 GeV)")
specMassOS.GetXaxis().SetTitle("m(2Mu)[GeV]")
c3.Update()
c3.Print("comparisonSpecOS.root")

c4=ROOT.TCanvas("c2","c2",700,400)
c4.SetLogy()
c4.SetLogx()
specMassOS.SetStats(0)
specMassLS.SetStats(0)
lowMassOS.SetStats(0)
specMassOS.Draw()
specMassLS.SetLineColor(kRed)
#specMassLS.Draw("same")
lowMassOS.SetLineColor(kGreen)
lowMassOS.Draw("same")
specMassOS.GetYaxis().SetTitle("#Events/(0.025 GeV)")
specMassOS.GetXaxis().SetTitle("m(2Mu)[GeV]")
c4.Update()
c4.Print("comparisonSpecLowOS.root")


c5=ROOT.TCanvas("c2","c2",700,400)
c5.SetLogy()
c5.SetLogx()
lowOrHighMassOS.SetStats(0)
lowOrYOrHighMassOS.SetStats(0)
allMassOS.SetStats(0)

lowOrHighMassOS.Draw()
lowOrYOrHighMassOS.SetLineColor(kRed)
lowOrYOrHighMassOS.Draw("same")
allMassOS.SetLineColor(kGreen)
allMassOS.Draw("same")
lowOrHighMassOS.GetYaxis().SetTitle("#Events/(0.025 GeV)")
lowOrHighMassOS.GetXaxis().SetTitle("m(2Mu)[GeV]")
c5.Update()
c5.Print("comparisonAllOS.root")



 
