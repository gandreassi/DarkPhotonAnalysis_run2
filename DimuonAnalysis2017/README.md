How to produce limit plots (assuming the user already ran the mass calibration part):

1) compile:
<pre><code>
cmsrel CMSSW_10_3_2
cd CMSSW_10_3_2/src/
git clone https://github.com/cms-analysis/HiggsAnalysis-CombinedLimit.git HiggsAnalysis/CombinedLimit
cmsenv
scram b
git clone git@github.com:gandreassi/DarkPhotonAnalysis_run2.git
cd DarkPhotonAnalysis_run2/DimuonAnalysis2017/
make
</code></pre>

2) perform and store the fit to data
<pre><code>
cd limits
./makeCardsAndWS.exe 
</code></pre>

3) run the combine tool, computing the mass-dependent limit
<pre><code>
python pyDPLimitsProcessing.py <year>
</code></pre>
