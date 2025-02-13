/** \class MuGEML1FETableProducer MuGEML1FETableProducer.cc DPGAnalysis/MuonTools/src/MuGEML1FETableProducer.cc
 *  
 * Helper class : the Phase-1 local trigger FlatTableProducer for TwinMux in/out and BMTF in (the DataFormat is the same)
 *
 * \author C. Battilana (INFN BO)
 *
 *
 */

#include "FWCore/ParameterSet/interface/allowedValues.h"

#include <iostream>
#include <vector>

#include "DPGAnalysis/MuonTools/interface/MuBaseFlatTableProducer.h"

#include "FWCore/ParameterSet/interface/ConfigurationDescriptions.h"
#include "FWCore/ParameterSet/interface/ParameterSetDescription.h"

#include "DataFormats/TCDS/interface/TCDSRecord.h"

class MuGEML1FETableProducer : public MuBaseFlatTableProducer {
public:

  /// Constructor
  MuGEML1FETableProducer(const edm::ParameterSet&);

  /// Fill descriptors
  static void fillDescriptions(edm::ConfigurationDescriptions&);

protected:
  /// Fill tree branches for a given event
  void fillTable(edm::Event&) final;

  /// Get info from the ES by run
  void getFromES(const edm::Run&, const edm::EventSetup&) final;

private:
  nano_mu::EDTokenHandle<TCDSRecord> m_token;
};

MuGEML1FETableProducer::MuGEML1FETableProducer(const edm::ParameterSet& config)
    : MuBaseFlatTableProducer{config},
      m_token{config, consumesCollector(), "src"} {
  produces<nanoaod::FlatTable>();
}

void MuGEML1FETableProducer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  edm::ParameterSetDescription desc;

  desc.add<std::string>("name", "L1A");
  desc.add<edm::InputTag>("src", edm::InputTag{"tcdsDigis"});

  descriptions.addWithDefaultLabel(desc);
}

void MuGEML1FETableProducer::getFromES(const edm::Run& run, const edm::EventSetup& environment) {
}

void MuGEML1FETableProducer::fillTable(edm::Event& ev) {
	std::vector<int> l1aDiff;

	auto record = m_token.conditionalGet(ev);
  constexpr size_t max_trigger = 16;

  for (size_t i = 0; i < max_trigger; ++i) {
    int l1a_diff = 3564 * (record->getOrbitNr() - record->getL1aHistoryEntry(i).getOrbitNr())
        + record->getBXID() - record->getL1aHistoryEntry(i).getBXID();
		l1aDiff.push_back(l1a_diff);
  }

	auto table = std::make_unique<nanoaod::FlatTable>(max_trigger, m_name, false, false);
	addColumn(table, "L1Adiff", l1aDiff, "BX differences between event and L1As");

	ev.put(std::move(table));
  }

#include "FWCore/PluginManager/interface/ModuleDef.h"
#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(MuGEML1FETableProducer);
