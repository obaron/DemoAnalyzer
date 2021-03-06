import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")

# initialize MessageLogger and output report
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.threshold = 'INFO'
process.MessageLogger.categories.append('Demo')
process.MessageLogger.cerr.INFO = cms.untracked.PSet(
    limit = cms.untracked.int32(-1)
)
process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
                                # replace 'myfile.root' with the source file you want to use
                                fileNames = cms.untracked.vstring(
            #'file:/afs/cern.ch/cms/Tutorials/TWIKI_DATA/TTJets_8TeV_53X.root'
            '/store/data/Run2015D/JetHT/AOD/PromptReco-v3/000/256/630/00000/A20F1D45-3C5F-E511-8351-02163E0146AE.root'
                )
                            )

process.demo = cms.EDAnalyzer('DemoAnalyzer',
           minTracks = cms.untracked.uint32(1000)
         )
process.TFileService = cms.Service("TFileService",
                                       fileName = cms.string('histodemo.root')
                                   )


process.p = cms.Path(process.demo)
