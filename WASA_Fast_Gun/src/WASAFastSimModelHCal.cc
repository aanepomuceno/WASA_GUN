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
/// \file WASAFastSimModelHCal.cc
// Based on Geant4 WASA parametrization example
// Andre Nepomuceno - Winter 2023


#include "WASAFastSimModelHCal.hh"
#include "WASAEventInformation.hh"
#include "WASAPrimaryParticleInformation.hh"
#include "WASASmearer.hh"
#include "WASAOutput.hh"

#include "G4Track.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4AnalysisManager.hh"

#include "Randomize.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

WASAFastSimModelHCal::WASAFastSimModelHCal( G4String aModelName, 
  G4Region* aEnvelope, WASADetectorParametrisation::Parametrisation aType ) :
  G4VFastSimulationModel( aModelName, aEnvelope ), fCalculateParametrisation(),
  fParametrisation( aType ) {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

WASAFastSimModelHCal::WASAFastSimModelHCal( G4String aModelName, 
                                              G4Region* aEnvelope ) : 
  G4VFastSimulationModel( aModelName, aEnvelope ), fCalculateParametrisation(),
  fParametrisation( WASADetectorParametrisation::eWASA ) {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

WASAFastSimModelHCal::WASAFastSimModelHCal( G4String aModelName ) :
  G4VFastSimulationModel( aModelName ), fCalculateParametrisation(), 
  fParametrisation( WASADetectorParametrisation::eWASA ) {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

WASAFastSimModelHCal::~WASAFastSimModelHCal() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool WASAFastSimModelHCal::IsApplicable( const G4ParticleDefinition& aParticleType ) {
  G4bool isOk = false;
  // Applicable to all hadrons, i.e. any particle made of quarks
  if ( aParticleType.GetQuarkContent(1) +
       aParticleType.GetQuarkContent(2) +
       aParticleType.GetQuarkContent(3) +
       aParticleType.GetQuarkContent(4) +
       aParticleType.GetQuarkContent(5) +
       aParticleType.GetQuarkContent(6) != 0 ) {
    isOk = true;
  }
   return isOk;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool WASAFastSimModelHCal::ModelTrigger( const G4FastTrack& /*aFastTrack*/ ) {
  return true;  // No kinematical restrictions to apply the parametrisation
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void WASAFastSimModelHCal::DoIt( const G4FastTrack& aFastTrack, 
                                  G4FastStep& aFastStep ) {
  //G4cout << " ________HCal model triggered _________" << G4endl;

  G4int pdgID = 0;
  G4double p = 1.0;
  // Kill the parameterised particle at the entrance of the hadronic calorimeter
  aFastStep.KillPrimaryTrack();
  aFastStep.ProposePrimaryTrackPathLength( 0.0 );
  G4double Edep = aFastTrack.GetPrimaryTrack()->GetKineticEnergy();
  G4ThreeVector Pos = aFastTrack.GetPrimaryTrack()->GetPosition();
  G4double time = aFastTrack.GetPrimaryTrack()->GetGlobalTime();

   WASAEventInformation* info = (WASAEventInformation*) G4EventManager::GetEventManager()->GetUserInformation();

    if ( info->GetDoSmearing() ) {
      // Smearing according to the hadronic calorimeter resolution
      G4ThreeVector Porg = aFastTrack.GetPrimaryTrack()->GetMomentum();
      G4double res = fCalculateParametrisation->GetResolution( WASADetectorParametrisation::eHCAL, 
                     fParametrisation, Porg.mag() ,p );
//                       fParametrisation, Porg.mag() ,p );


      G4double med = fCalculateParametrisation->GetMedian( 
                     WASADetectorParametrisation::eHCAL, fParametrisation, Edep, p );
      
      G4double eff = fCalculateParametrisation->GetEfficiency( WASADetectorParametrisation::eHCAL, 
                     fParametrisation, Edep );
                     
      G4double Esm;
      Esm = std::abs( WASASmearer::Instance()->
                      SmearEnergy( aFastTrack.GetPrimaryTrack(), res , med) );

//Save histogram and trees
      WASAOutput::Instance()->FillHistogram( 2, (Esm/MeV) / (Edep/MeV) );
      
      pdgID = aFastTrack.GetPrimaryTrack()-> GetDefinition()->GetPDGEncoding();
      WASAOutput::Instance()->SaveTrack( WASAOutput::eSaveHCal,
                                        0,
                                        pdgID,
                                        Pos/mm,
                                        res,
                                        eff,
                                        Esm/MeV,
                                        Edep/MeV,
                                        time/ns);
      
      // The (smeared) energy of the particle is deposited in the step
      // (which corresponds to the entrance of the hadronic calorimeter)
      aFastStep.ProposeTotalEnergyDeposited( Esm );
    } else {
      // No smearing: simply setting the value of Edep
      // The (initial) energy of the particle is deposited in the step
      // (which corresponds to the entrance of the hadronic calorimeter)
      aFastStep.ProposeTotalEnergyDeposited( Edep );
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

