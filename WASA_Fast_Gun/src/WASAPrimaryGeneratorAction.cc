//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
/// \file WASAPrimaryGeneratorAction.cc
/// \brief Implementation of the WASAPrimaryGeneratorAction class

#include "WASAPrimaryGeneratorAction.hh"
#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4IonTable.hh"
#include "G4ParticleDefinition.hh"
#include "WASAPrimaryParticleInformation.hh"
#include "globals.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"
#include <sstream>
#include <iostream>


#define pi 3.14159265

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

WASAPrimaryGeneratorAction::WASAPrimaryGeneratorAction() : G4VUserPrimaryGeneratorAction(), fParticleGun(nullptr) {
   
   fParticleGun = new G4ParticleGun(1);
   // Open the source particle input file
    fInputFile.open("hibeam_wasa_mc_50k_mod.dat");
    if (!fInputFile) {
        G4cerr << "Error: Cannot open source particle input file" << G4endl;
    }

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

WASAPrimaryGeneratorAction::~WASAPrimaryGeneratorAction() {
  delete fParticleGun;
   if (fInputFile.is_open()) {
        fInputFile.close();
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void WASAPrimaryGeneratorAction::GeneratePrimaries( G4Event* anEvent ) {


     if (!fInputFile.is_open()) {
        G4cerr << "Error: Input file is not open!" << G4endl;
        return;
    }
    
    std::string line;
    std::streampos oldpos;
    G4int index, pdg, evt1, evt2;
    G4double kineticEnergy, x, y, z, ux, uy, uz, time, weight, pol_x, pol_y, pol_z;

    std::getline(fInputFile, line);
    std::istringstream iss(line);
    iss >> index >> pdg >> kineticEnergy  >> x >> y >> z >> ux >> uy >> uz >> time >>  weight >> pol_x >> pol_y >> pol_z >> std::hex >> evt1 ;
    evt2 = evt1;
   
    while (evt2 == evt1) {

    G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
    G4ParticleDefinition* particle = particleTable->FindParticle(pdg);
    //if particle is not elementary, get Ion
    if ( !particle && (pdg/100000000 == 10) ) {
       particle = G4IonTable::GetIonTable()->GetIon(pdg);
     }
    
    fParticleGun->SetParticleDefinition(particle);
    fParticleGun->SetParticlePosition( G4ThreeVector(x * cm, y * cm, z * cm) );
    fParticleGun->SetParticleMomentumDirection( G4ThreeVector(ux, uy, uz) );
    fParticleGun->SetParticleEnergy(kineticEnergy * MeV);
    fParticleGun->GeneratePrimaryVertex(anEvent);
    
    //read next particle in the event (and keep record of the current line position in the file, for next event)
     oldpos = fInputFile.tellg();
     std::getline(fInputFile, line);
     std::istringstream iss(line);
     iss >> index >> pdg >> kineticEnergy  >> x >> y >> z >> ux >> uy >> uz >> time >>  weight >> pol_x >> pol_y >> pol_z >> std::hex >> evt2 ;
     
    }
    fInputFile.seekg(oldpos); //back to the previous line file


  // Loop over the vertices, and then over primary particles,
  // and for each primary particle create an info object, in
  // which to store "Monte Carlo true" information.
  // This approach could appear unnecessarily heavy in the present case
  // of a trivial particle gun generator, but it is useful in the more
  // realistic case of a Monte Carlo event generator like Pythia8.
  G4int count_particles = 0;
  for ( G4int ivtx = 0; ivtx < anEvent->GetNumberOfPrimaryVertex(); ivtx++ ) {
    for ( G4int ipp = 0; ipp < anEvent->GetPrimaryVertex( ivtx )->GetNumberOfParticle();
          ipp++ ) {
      G4PrimaryParticle* primary_particle = 
        anEvent->GetPrimaryVertex( ivtx )->GetPrimary( ipp );
      if ( primary_particle ) {
        primary_particle->SetUserInformation( new WASAPrimaryParticleInformation( 
          count_particles, primary_particle->GetPDGcode(), 
          primary_particle->GetMomentum() ) );
        count_particles++;              
      }
    } 
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4ParticleGun* WASAPrimaryGeneratorAction::GetParticleGun() {
  return fParticleGun;
} 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

