import FWCore.ParameterSet.Config as cms

from DPGAnalysis.MuonTools.muGEML1FETableProducer_cfi import muGEML1FETableProducer

muGEML1FETable = muGEML1FETableProducer.clone(
    name = cms.string("L1A"),
    src = cms.InputTag("tcdsDigis", "tcdsRecord")
)

muFlowerEventTable = cms.Sequence(
    muGEML1FETable
)

