import FWCore.ParameterSet.Config as cms

# Define the process
process = cms.Process("SKIM")

# How many events to process
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

# Input EDM files
process.source = cms.Source("PoolSource",
	fileNames = cms.untracked.vstring(
        '/store/data/Run2016B/ScoutingPFHT/RAW/v1/000/272/680/00000/CE8246B6-3913-E611-8815-02163E0128EB.root',
        '/store/data/Run2016B/ScoutingPFHT/RAW/v1/000/272/680/00000/D8376DB6-5813-E611-A451-02163E011BFF.root',
        '/store/data/Run2016B/ScoutingPFHT/RAW/v1/000/272/681/00000/B6837F6F-4013-E611-A63F-02163E0133D1.root',
        '/store/data/Run2016B/ScoutingPFHT/RAW/v1/000/272/728/00000/6AD7A7C3-AC13-E611-AD76-02163E011CA6.root',
        '/store/data/Run2016B/ScoutingPFHT/RAW/v1/000/272/729/00000/9C9CB45D-AF13-E611-B108-02163E011ADA.root',
        '/store/data/Run2016B/ScoutingPFHT/RAW/v1/000/272/748/00000/4E4F02EF-C013-E611-92E8-02163E0129B3.root',
        '/store/data/Run2016B/ScoutingPFHT/RAW/v1/000/272/750/00000/54274C30-C113-E611-8A5F-02163E0126FE.root',
        '/store/data/Run2016B/ScoutingPFHT/RAW/v1/000/272/760/00000/C8F06C80-D613-E611-8CE6-02163E011E5F.root',
        '/store/data/Run2016B/ScoutingPFHT/RAW/v1/000/272/761/00000/1AD1CCA3-DA13-E611-A2C0-02163E013872.root',
        '/store/data/Run2016B/ScoutingPFHT/RAW/v1/000/272/761/00000/40D7CEA7-DA13-E611-A760-02163E0145AC.root',
        '/store/data/Run2016B/ScoutingPFHT/RAW/v1/000/272/761/00000/965A297E-DA13-E611-AF4F-02163E01373A.root',
        '/store/data/Run2016B/ScoutingPFHT/RAW/v1/000/272/761/00000/D86F9FE2-DA13-E611-8EC0-02163E01185F.root',
	)
)

# Output EDM file
process.out = cms.OutputModule("PoolOutputModule",
    fileName = cms.untracked.string("copy.root")
)

process.p = cms.EndPath(process.out)

