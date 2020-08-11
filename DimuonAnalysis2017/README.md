How to produce limit plots (assuming the user already ran the mass calibration part):

<pre><code>
cmsrel CMSSW_10_3_2
cd CMSSW_10_3_2/src/
cmsenv
git clone https://github.com/cms-analysis/HiggsAnalysis-CombinedLimit.git HiggsAnalysis/CombinedLimit
cd HiggsAnalysis/CombinedLimit
source env_standalone.sh 
make -j 8
cd ../../
git clone git@github.com:gandreassi/DarkPhotonAnalysis_run2.git
cd DarkPhotonAnalysis_run2/DimuonAnalysis2017/
make
cd limit
./makeCardsAndWS.exe 
</code></pre>
this code will perform and store the fit to data

<pre><code>
python pyDPLimitsProcessing.py <year>
</code></pre>

this will run the combine tool, computing the mass-dependent limit
