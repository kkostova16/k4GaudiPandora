#include "podio/Reader.h"
#include "edm4hep/SimCalorimeterHitCollection.h"
#include "edm4hep/CalorimeterHitCollection.h"
#include "edm4hep/MCRecoCaloAssociationCollection.h"

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
  auto hSimEnergyMarlin = new TH1F("hSimEnergyMarlin",";SimCalorimeterHit.energy;N",250,0,0.025);
  auto hSimEnergyGaudi = new TH1F("hSimEnergyGaudi",";SimCalorimeterHit.energy;N",250,0,0.025);
  auto hEnergyMarlin = new TH1F("hEnergyMarlin",";CalorimeterHit.energy;N",100,0,2.5);
  auto hEnergyGaudi = new TH1F("hEnergyGaudi",";CalorimeterHit.energy;N",100,0,2.5);
  auto hTimeMarlin = new TH1F("hTimeMarlin",";CalorimeterHit.time;N",200,0,2000);
  auto hTimeGaudi = new TH1F("hTimeGaudi",";CalorimeterHit.time;N",200,0,2000);
  auto hEnergyRatioMarlin = new TH1F("hEnergyRatioMarlin",";outputEnergy/inputEnergy;N",500,0,150000);
  auto hEnergyRatioGaudi = new	TH1F("hEnergyRatioGaudi",";outputEnergy/inputEnergy;N",500,0,150000);
  
  // Loop over the events - MARLIN
  for (size_t i = 0; i < oldfile.getEvents(); ++i) {
    cout << "event " << i << endl;
    auto event = oldfile.readNextEvent();
    auto& sim = event.get<edm4hep::SimCalorimeterHitCollection>("ECalBarrelCollection");
    cout << "SimCalorimeterHit.size " << sim.size() << endl;
    // Loop over the hits
    for (size_t n = 0; n < sim.size(); ++n) {
      hSimEnergyMarlin->Fill(sim[n].getEnergy());
    }
    
    auto& coll = event.get<edm4hep::CalorimeterHitCollection>("CalorimeterHit");
    cout <<  "CalorimeterHit.size " << coll.size() << endl;
    // Loop over the hits
    for (size_t n = 0; n < coll.size(); ++n) {
      //cout << coll[n].getEnergy() << endl;
      hEnergyMarlin->Fill(coll[n].getEnergy());
      hTimeMarlin->Fill(coll[n].getTime());
    }
       
    auto& rel = event.get<edm4hep::MCRecoCaloAssociationCollection>("RelationMuonHit");
    cout << "MCRecoCaloAssociation.size " << rel.size() << endl;
    for (size_t n = 0; n < rel.size(); ++n) {
      auto ratio = (rel[n].getRec().getEnergy())/(rel[n].getSim().getEnergy());
      hEnergyRatioMarlin->Fill(ratio);
    }
  }

  
  // Loop over the events - GAUDI
  for (size_t i = 0; i < newfile.getEvents(); ++i) {
    cout << "event " << i << endl;
    auto event = newfile.readNextEvent();
    auto& sim = event.get<edm4hep::SimCalorimeterHitCollection>("ECalBarrelCollection");
    cout << "SimCalorimeterHit.size " << sim.size() << endl;
    // Loop over the hits
    for (size_t n = 0; n < sim.size(); ++n) {
      hSimEnergyGaudi->Fill(sim[n].getEnergy());
    }
    
    auto& coll = event.get<edm4hep::CalorimeterHitCollection>("CalorimeterHit");
    cout <<  "CalorimeterHit.size " << coll.size() << endl;
    // Loop over the hits
    for (size_t n = 0; n < coll.size(); ++n) {
      //cout << coll[n].getEnergy() << endl;
      hEnergyGaudi->Fill(coll[n].getEnergy());
      hTimeGaudi->Fill(coll[n].getTime());
    }
       
    auto& rel = event.get<edm4hep::MCRecoCaloAssociationCollection>("RelationMuonHit");
    cout << "MCRecoCaloAssociation.size " << rel.size() << endl;
    for (size_t n = 0; n < rel.size(); ++n) {
      auto ratio = (rel[n].getRec().getEnergy())/(rel[n].getSim().getEnergy());
      hEnergyRatioGaudi->Fill(ratio);
    }
  }
  // Show the histograms
  //auto c1 = new TCanvas();
  //hSimEnergyMarlin->Draw();
  hSimEnergyGaudi->SetLineColor(kRed);
  //hSimEnergyGaudi->Draw("same");
  //auto c2 = new TCanvas();
  //hEnergyMarlin->Draw();
  hEnergyGaudi->SetLineColor(kRed);
  //hEnergyGaudi->Draw("same");
  //auto c3 = new TCanvas();
  //hTimeMarlin->Draw();
  hTimeGaudi->SetLineColor(kRed);
  //hTimeGaudi->Draw("same");
  //auto c4 = new TCanvas();
  //hEnergyRatioMarlin->Draw();
  hEnergyRatioGaudi->SetLineColor(kRed);
  //hEnergyRatioGaudi->Draw("same");

  // Write histograms to file
  hSimEnergyMarlin->Write();
  hSimEnergyGaudi->Write();
  hEnergyMarlin->Write();
  hEnergyGaudi->Write();
  hTimeMarlin->Write();
  hTimeGaudi->Write();
  hEnergyRatioMarlin->Write();
  hEnergyRatioGaudi->Write();
}
