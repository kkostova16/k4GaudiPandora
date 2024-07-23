#include "podio/Reader.h"
#include "edm4hep/CalorimeterHitCollection.h"
#include <edm4hep/MCRecoCaloAssociationCollection.h>

void plot() {
  // Load the PODIO library
  gSystem->Load("libpodioIO");
  // Load EDM4hep
  gSystem->Load("libedm4hep");

  // Open the file and make a reader
  auto oldfile = podio::makeReader("old_output_digi.root");
  auto newfile = podio::makeReader("build/output_digi.root");

  auto myhistofile = new TFile("myhistos.root","RECREATE");
  
  // Create histograms
  auto hEnergyMarlin = new TH1F("hEnergyMarlin",";CalorimeterHit.energy;N",200,0,2000);
  auto hEnergyGaudi = new TH1F("hEnergyGaudi",";CalorimeterHit.energy;N",200,0,2000);
  auto hSimEnergyMarlin = new TH1F("hSimEnergyMarlin",";SimCalorimeterHit.energy;N",200,0,2000);
  auto hSimEnergyGaudi = new TH1F("hSimEnergyGaudi",";SimCalorimeterHit.energy;N",200,0,2000);
  auto hTimeMarlin = new TH1F("hTimeMarlin",";CalorimeterHit.time;N",200,0,2000);
  auto hTimeGaudi = new TH1F("hTimeGaudi",";CalorimeterHit.time;N",200,0,2000);
  auto hEnergyRatioMarlin = new TH1F("hEnergyRatioMarlin",";outputEnergy/inputEnergy;N",1000,0,200000);
  auto hEnergyRatioGaudi = new	TH1F("hEnergyRatioGaudi",";outputEnergy/inputEnergy;N",1000,0,200000);
  
  // Loop over the events
  for (size_t i = 0; i < oldfile.getEvents(); ++i) {
    auto event = oldfile.readNextEvent();
    auto& coll = event.get<edm4hep::CalorimeterHitCollection>("CalorimeterHit");
    cout << "event " << i << " CalorimeterHit.size " << coll.size() << endl;
    // Loop over the hits
    for (size_t n = 0; n < coll.size(); ++n) {
      //cout << coll[n].getEnergy() << endl;
      hEnergyMarlin->Fill(coll[n].getEnergy());
      hTimeMarlin->Fill(coll[n].getTime());
    }
    auto& rel = event.get<edm4hep::MCRecoCaloAssociationCollection>("RelationMuonHit");
    cout << "event " << i << " MCRecoCaloAssociation.size " << rel.size() << endl;
    for (size_t n = 0; n < rel.size(); ++n) {
      hSimEnergyMarlin->Fill(rel[n].getSim().getEnergy());
      auto ratio = (rel[n].getRec().getEnergy())/(rel[n].getSim().getEnergy());
      hEnergyRatioMarlin->Fill(ratio);
    }
  }
  
  // Loop over the events
  for (size_t i = 0; i < newfile.getEvents(); ++i) {
    auto event = newfile.readNextEvent();
    auto& coll = event.get<edm4hep::CalorimeterHitCollection>("CalorimeterHit");
    cout << "event " << i << " CalorimeterHit.size " << coll.size() << endl;
    // Loop over the hits
    for (size_t n = 0; n < coll.size(); ++n) {
      //cout << coll[n].getEnergy() << endl;
      hEnergyGaudi->Fill(coll[n].getEnergy());
      hTimeGaudi->Fill(coll[n].getTime());
    }
    auto& rel = event.get<edm4hep::MCRecoCaloAssociationCollection>("RelationMuonHit");
    cout << "event " << i << " MCRecoCaloAssociation.size " << rel.size() << endl;
    for (size_t n = 0; n < rel.size(); ++n) {
      hSimEnergyGaudi->Fill(rel[n].getSim().getEnergy());
      auto ratio = (rel[n].getRec().getEnergy())/(rel[n].getSim().getEnergy());
      hEnergyRatioGaudi->Fill(ratio);
    }
  }
  // Show the histograms
  auto c1 = new TCanvas();
  hEnergyMarlin->Draw();
  auto c2 = new TCanvas();
  hEnergyGaudi->Draw();
  auto c3 = new TCanvas();
  hTimeMarlin->Draw();
  auto c4 = new TCanvas();
  hTimeGaudi->Draw();
  auto c5 = new TCanvas();
  hEnergyRatioMarlin->Draw();
  auto c6 = new TCanvas();
  hEnergyRatioGaudi->Draw();

  // Write histograms to file
  hEnergyMarlin->Write();
  hEnergyGaudi->Write();
  hSimEnergyMarlin->Write();
  hSimEnergyGaudi->Write();
  hTimeMarlin->Write();
  hTimeGaudi->Write();
  hEnergyRatioMarlin->Write();
  hEnergyRatioGaudi->Write();
}
