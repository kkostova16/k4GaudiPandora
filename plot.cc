#include "podio/Reader.h"

void plot() {
  // Load the PODIO library
  gSystem->Load("libpodioIO");
  // Load EDM4hep
  gSystem->Load("libedm4hep");

  // Open the file and make a reader
  auto oldfile = podio::makeReader("old_output_digi.root");
  auto newfile = podio::makeReader("build/output_digi.root");

  auto hEnergyMarlin = new TH1F("hEnergyMarlin",";CalorimeterHit.energy;N",200,0.,2000.);
  auto hEnergyGaudi = new TH1F("hEnergyGaudi",";CalorimeterHit.energy;N",200,0.,2000.);
  
  // Loop over the events
  for (size_t i = 0; i < oldfile.getEvents(); ++i) {
    auto event = oldfile.readNextEvent();
    auto& coll = event.get<edm4hep::CalorimeterHitCollection>("CalorimeterHit");
    cout << coll.size() << endl;
    // Loop over the hits
    for (size_t n = 0; n < coll.size(); ++n) {
      //cout << coll[n].getEnergy() << endl;
      hEnergyMarlin->Fill(coll[n].getEnergy());
    }
  }
  
  // Loop over the events
  for (size_t i = 0; i < newfile.getEvents(); ++i) {
    auto event = newfile.readNextEvent();
    auto& coll = event.get<edm4hep::CalorimeterHitCollection>("CalorimeterHit");
    cout << coll.size() << endl;
    // Loop over the hits
    for (size_t n = 0; n < coll.size(); ++n) {
      //cout << coll[n].getEnergy() << endl;
      hEnergyGaudi->Fill(coll[n].getEnergy());
    }
  }
  // Show the histograms
  auto c1 = new TCanvas();
  hEnergyMarlin->Draw();
  auto c2 = new TCanvas();
  hEnergyGaudi->Draw();
}
