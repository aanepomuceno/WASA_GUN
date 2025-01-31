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
/// \file WASAPhysicsList.cc
/// \brief Implementation of the WASAPhysicsList class

#include "WASAPhysicsList.hh"
#include "globals.hh"
#include "G4ParticleDefinition.hh"
#include "G4ProcessManager.hh"
#include "G4ProcessVector.hh"
#include "G4ParticleTypes.hh"
#include "G4ParticleTable.hh"
#include "G4Material.hh"
#include "G4MaterialTable.hh"
#include "G4ios.hh"
#include "G4SystemOfUnits.hh"
#include <iomanip>
#include "G4FastSimulationManagerProcess.hh"

#include "G4Decay.hh"
#include "G4LeptonConstructor.hh"
#include "G4MesonConstructor.hh"
#include "G4BaryonConstructor.hh"
#include "G4IonConstructor.hh"

#include "G4ComptonScattering.hh"
#include "G4GammaConversion.hh"
#include "G4PhotoElectricEffect.hh"

#include "G4eMultipleScattering.hh"
#include "G4MuMultipleScattering.hh"
#include "G4hMultipleScattering.hh"

#include "G4eIonisation.hh"
#include "G4eBremsstrahlung.hh"
#include "G4eplusAnnihilation.hh"

#include "G4MuIonisation.hh"
#include "G4MuBremsstrahlung.hh"
#include "G4MuPairProduction.hh"

#include "G4hIonisation.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

WASAPhysicsList::WASAPhysicsList() :  G4VUserPhysicsList() {
  SetVerboseLevel( 1 );
  defaultCutValue = 0.1*m;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

WASAPhysicsList::~WASAPhysicsList() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void WASAPhysicsList::ConstructParticle() {
  // In this method, static member functions should be called for all particles
  // which you want to use.
  // This ensures that objects of these particle types will be created in the program.
  ConstructBosons();
  ConstructLeptons();
  ConstructMesons();
  ConstructBaryons();
  ConstructIons();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void WASAPhysicsList::ConstructBosons() {
  G4Geantino::GeantinoDefinition();
  G4ChargedGeantino::ChargedGeantinoDefinition();
  G4Gamma::GammaDefinition();
  G4OpticalPhoton::OpticalPhotonDefinition();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void WASAPhysicsList::ConstructLeptons() {
  G4LeptonConstructor pConstructor;
  pConstructor.ConstructParticle();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void WASAPhysicsList::ConstructMesons() {
  G4MesonConstructor pConstructor;
  pConstructor.ConstructParticle();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void WASAPhysicsList::ConstructBaryons() {
  G4BaryonConstructor  pConstructor;
  pConstructor.ConstructParticle();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void WASAPhysicsList::ConstructIons() {
  G4IonConstructor pConstructor;
  pConstructor.ConstructParticle();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void WASAPhysicsList::ConstructProcess() {
  AddTransportation();
  AddParameterisation();
  ConstructGeneral();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void WASAPhysicsList::AddTransportation() {
  //UseCoupledTransportation();
  G4VUserPhysicsList::AddTransportation();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void WASAPhysicsList::ConstructGeneral() {
  G4Decay* theDecayProcess = new G4Decay();
  auto particleIterator=GetParticleIterator();
  particleIterator->reset();
  while ( (*particleIterator)() ) {
    G4ParticleDefinition* particle = particleIterator->value();
    G4ProcessManager* pmanager = particle->GetProcessManager();
    if ( theDecayProcess->IsApplicable( *particle ) ) {
      pmanager->AddProcess( theDecayProcess );
      // set ordering for PostStepDoIt and AtRestDoIt
      pmanager->SetProcessOrdering( theDecayProcess, idxPostStep );
      pmanager->SetProcessOrdering( theDecayProcess, idxAtRest );
    }
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void WASAPhysicsList::AddParameterisation() {
  G4FastSimulationManagerProcess* fastSimProcess = 
    new G4FastSimulationManagerProcess( "G4FSMP" );

  // Registers the fastSimProcess with all the particles as a discrete and
  // continuous process (this works in all cases; in the case that parallel
  // geometries are not used, as in this example, it would be enough to
  // add it as a discrete process).
  auto particleIterator=GetParticleIterator();
  particleIterator->reset();
  while ( (*particleIterator)() ) {
    G4ParticleDefinition* particle = particleIterator->value();
    G4ProcessManager* pmanager = particle->GetProcessManager();
    //pmanager->AddDiscreteProcess( fastSimProcess );    // No parallel geometry
    pmanager->AddProcess( fastSimProcess, -1, 0, 0 );  // General
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void WASAPhysicsList::SetCuts() {
  if ( verboseLevel > 1 ) {
    G4cout << "WASAPhysicsList::SetCuts:";
  }
  SetCutsWithDefault();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

