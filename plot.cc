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
  auto readerMarlin = podio::makeReader("output_Marlin.root");
  auto readerGaudi = podio::makeReader("output_Gaudi.root");

  auto myhistofile = new TFile("myhistos.root","RECREATE");
  
  // Create histograms
  auto hSimEnergyMarlin = new TH1F("hSimEnergyMarlin",";SimCalorimeterHit.energy;N",200,0,0.02);
  auto hSimEnergyGaudi = new TH1F("hSimEnergyGaudi",";SimCalorimeterHit.energy;N",200,0,0.02);
  auto hEnergyMarlin = new TH1F("hEnergyMarlin",";CalorimeterHit.energy;N",250,0,2.5);
  auto hEnergyGaudi = new TH1F("hEnergyGaudi",";CalorimeterHit.energy;N",250,0,2.5);
  auto hTimeMarlin = new TH1F("hTimeMarlin",";CalorimeterHit.time;N",100,0,1000);
  auto hTimeGaudi = new TH1F("hTimeGaudi",";CalorimeterHit.time;N",100,0,1000);
  auto hEnergyRatioMarlin = new TH1F("hEnergyRatioMarlin",";CaloHit.energy/SimCaloHit.energy;N",250,0,150000);
  auto hEnergyRatioGaudi = new	TH1F("hEnergyRatioGaudi",";CaloHit.energy/SimCaloHit.energy;N",250,0,150000);
  
  // Loop over the events - MARLIN
  for (size_t i = 0; i < readerMarlin.getEvents(); ++i) {
    cout << "event " << i << endl;
    auto event = readerMarlin.readNextEvent();
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
  for (size_t i = 0; i < readerGaudi.getEvents(); ++i) {
    cout << "event " << i << endl;
    auto event = readerGaudi.readNextEvent();
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
  auto c1 = new TCanvas();
  gStyle->SetOptStat(0);
  c1->SetLogy();
  hSimEnergyMarlin->SetFillColorAlpha(kAzure+1,0.2);
  hSimEnergyMarlin->Draw();
  hSimEnergyGaudi->SetLineColor(kOrange+1);
  hSimEnergyGaudi->SetLineWidth(3);
  hSimEnergyGaudi->Draw("same");

  auto l1 = new TLegend(0.7,0.8,0.9,0.9);
  l1->AddEntry(hSimEnergyMarlin,"SimCaloHit.energy (Marlin)");
  l1->AddEntry(hSimEnergyGaudi,"SimCaloHit.energy (Gaudi)");
  l1->Draw();
  
  auto c2 = new TCanvas();
  gStyle->SetOptStat(0);
  c2->SetLogy();
  hEnergyMarlin->SetFillColorAlpha(kAzure+1,0.2);
  hEnergyMarlin->Draw();
  hEnergyGaudi->SetLineColor(kOrange+1);
  hEnergyGaudi->SetLineWidth(3);
  hEnergyGaudi->Draw("same");

  auto l2 = new TLegend(0.1,0.85,0.3,0.9);
  l2->AddEntry(hEnergyMarlin,"CaloHit.energy (Marlin)");
  l2->AddEntry(hEnergyGaudi,"CaloHit.energy (Gaudi)");
  l2->Draw();
  
  auto c3 = new TCanvas();
  gStyle->SetOptStat(0);
  c3->SetLogy();
  hTimeMarlin->SetFillColorAlpha(kAzure+1,0.2);
  hTimeMarlin->Draw();
  hTimeGaudi->SetLineColor(kOrange+1);
  hTimeGaudi->SetLineWidth(3);
  hTimeGaudi->Draw("same");

  auto l3 = new TLegend(0.7,0.85,0.9,0.9);
  l3->AddEntry(hTimeMarlin,"CaloHit.time (Marlin)");
  l3->AddEntry(hTimeGaudi,"CaloHit.time (Gaudi)");
  l3->Draw();
  
  auto c4 = new TCanvas();
  gStyle->SetOptStat(0);
  hEnergyRatioMarlin->SetFillColorAlpha(kAzure+1,0.2);
  hEnergyRatioMarlin->Draw();
  hEnergyRatioGaudi->SetLineColor(kOrange+1);
  hEnergyRatioGaudi->SetLineWidth(3);
  hEnergyRatioGaudi->Draw("same");

  auto l4 = new TLegend(0.6,0.85,0.9,0.9);
  l4->AddEntry(hEnergyRatioMarlin,"CaloHit.energy/SimCaloHit.energy (Marlin)");
  l4->AddEntry(hEnergyRatioGaudi,"CaloHit.energy/SimCaloHit.energy (Gaudi)");
  l4->Draw();

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
