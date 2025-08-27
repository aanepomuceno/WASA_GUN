#include "WASAPrimaryGeneratorMessenger.hh"
#include "WASAPrimaryGeneratorAction.hh"
#include "G4UIdirectory.hh"
#include "G4SystemOfUnits.hh"

WASAPrimaryGeneratorMessenger::WASAPrimaryGeneratorMessenger(WASAPrimaryGeneratorAction* generator)
    : fGenerator(generator) {
    
    fSetFileCmd = new G4UIcmdWithAString("/gun/setInputFile", this);
    fSetFileCmd->SetGuidance("Set input file for primary particles.");
    fSetFileCmd->SetParameterName("filename", false);
    fSetFileCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
    
    fSetFileTypeCmd = new G4UIcmdWithAnInteger("/gun/setFileType", this);
    fSetFileTypeCmd->SetGuidance("Set input file type (0 = bkg, 1 = signal)");
    fSetFileTypeCmd->SetParameterName("filetype", false);
    fSetFileTypeCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

}

WASAPrimaryGeneratorMessenger::~WASAPrimaryGeneratorMessenger() {
    delete fSetFileCmd;
    delete fSetFileTypeCmd;
}

void WASAPrimaryGeneratorMessenger::SetNewValue(G4UIcommand* command, G4String newValue) {
    if (command == fSetFileCmd) {
        fGenerator->SetInputFileName(newValue);
    }
    else if (command == fSetFileTypeCmd) {
        fGenerator->SetFileType(fSetFileTypeCmd->GetNewIntValue(newValue));
    }
}




