// -*- C++ -*-
//
// Package:    Demo/DemoAnalyzer
// Class:      DemoAnalyzer
// 
/**\class DemoAnalyzer DemoAnalyzer.cc Demo/DemoAnalyzer/plugins/DemoAnalyzer.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Owen Baron
//         Created:  Thu, 25 Feb 2016 21:51:25 GMT
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
//Histogram Headers
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "TH1.h"
#include "TH2.h"

//Jet Headers 
#include "DataFormats/JetReco/interface/PFJet.h"
#include "DataFormats/JetReco/interface/PFJetCollection.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidateFwd.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"

//
// class declaration
//

class DemoAnalyzer : public edm::EDAnalyzer {
   public:
      explicit DemoAnalyzer(const edm::ParameterSet&);
      ~DemoAnalyzer();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


   private:
      virtual void beginJob() override;
      virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
      virtual void endJob() override;

      //virtual void beginRun(edm::Run const&, edm::EventSetup const&) override;
      //virtual void endRun(edm::Run const&, edm::EventSetup const&) override;
      //virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;
      //virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;

      // ----------member data ---------------------------
	  unsigned int minTracks_;
	TH1D *demohisto; //declare histogram
	TH1D *akPF4plot;
	TH1D *jetmult;
	TH2F *ptmult;
};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
DemoAnalyzer::DemoAnalyzer(const edm::ParameterSet& iConfig) :
minTracks_(iConfig.getUntrackedParameter<unsigned int>("minTracks",0))
{
	
   //now do what ever initialization is needed

   edm::Service<TFileService> fs;
   demohisto = fs->make<TH1D>("tracks" , "Tracks" , 100 , 0 , 5000 );
   akPF4plot = fs->make<TH1D>("akPF4plot" , "PF4jets" , 100 , 0 , 500 );
   jetmult = fs->make<TH1D>("jetmult", "multiplicity",100,0,100);
   ptmult = fs->make<TH2F>("ptmult","Multiplicity vs pT",50,0,50,50,0,50);
//^Initialize. First invoking the TFileService, and then defining a TH1D.
}


DemoAnalyzer::~DemoAnalyzer()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called for each event  ------------
void
DemoAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;
Handle<reco::TrackCollection> tracks;
iEvent.getByLabel("generalTracks", tracks); 
//LogInfo("Demo") << "number of tracks "<<tracks->size(); //replaced by if statement lines below

Handle<reco::PFJetCollection> chhad;
iEvent.getByLabel("chargedHadronMultiplicity", chhad);
 	
edm::Handle<reco::PFJetCollection> pfjetH;
iEvent.getByLabel("ak4PFJets", pfjetH);
for ( reco::PFJetCollection::const_iterator jet = pfjetH->begin(); jet != pfjetH->end(); ++jet ) {
    double pt = jet->pt();
	akPF4plot->Fill(pt);
	double mult = jet->chargedMultiplicity();
	jetmult->Fill(mult);
	ptmult->Fill(mult,pt);
}
	
	
demohisto->Fill(tracks->size()); //Fill Histogram

if( minTracks_ <= tracks->size() ) {
   LogInfo("Demo") << "number of tracks "<<tracks->size();
}


#ifdef THIS_IS_AN_EVENT_EXAMPLE
   Handle<ExampleData> pIn;
   iEvent.getByLabel("example",pIn);
#endif
   
#ifdef THIS_IS_AN_EVENTSETUP_EXAMPLE
   ESHandle<SetupData> pSetup;
   iSetup.get<SetupRecord>().get(pSetup);
#endif
}


// ------------ method called once each job just before starting event loop  ------------
void 
DemoAnalyzer::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
DemoAnalyzer::endJob() 
{
}

// ------------ method called when starting to processes a run  ------------
/*
void 
DemoAnalyzer::beginRun(edm::Run const&, edm::EventSetup const&)
{
}
*/

// ------------ method called when ending the processing of a run  ------------
/*
void 
DemoAnalyzer::endRun(edm::Run const&, edm::EventSetup const&)
{
}
*/

// ------------ method called when starting to processes a luminosity block  ------------
/*
void 
DemoAnalyzer::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/

// ------------ method called when ending the processing of a luminosity block  ------------
/*
void 
DemoAnalyzer::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
DemoAnalyzer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(DemoAnalyzer);
