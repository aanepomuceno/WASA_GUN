#include <iostream>  
#include <fstream>
#include<string>  
#include "TVector3.h"

void signal_analysis(){

   const char *filename = "";
   TFile *f = NULL;
   const char *tName = "";
   TTree *t1 = NULL;
   TH1F *hnpions_plus = new TH1F("N pi+","N pi+",10,0,10);
   TH1F *hnpions_minus = new TH1F("N pi-","N pi-",10,0,10); 
   TH1F *hpion_plus_eng  = new TH1F("Pi+ Energy","Pi+ Energy",50,0,300);
   TH1F *hpion_minus_eng  = new TH1F("P- Energy","Pi- Energy",50,0,300);
   TH1F *hproton_eng  = new TH1F("Proton Energy","Proton Energy",50,0,300);
   TH1F *hmass  = new TH1F("Pi0 Mass","Pi0 Inv Mass",100,5,300);
    
   Int_t nevents = 50000;
   Int_t nfiles = 4;
   int npions_plus = 0;
   int npions_minus = 0;
   int nphotons = 0;
   double em = 0;
   double pdg = 0;
   double x=0; double y=0; double z = 0;
   double angle = 0;
   double mass = 0;
   TVector3 v1; 
   TVector3 v2;
   vector<double> v;
   vector<double> energy;

   for (int ifile =0; ifile < nfiles; ifile++) {
          std::string str1 = "WASAFastOutput_t"+to_string(ifile)+".root";
          filename = str1.c_str();
       f = new TFile(filename);
  
     for (int i=0;i<nevents;i++) {
     std::string str2 = "Event_"+to_string(i);
     tName = str2.c_str();
     t1 = (TTree*)f->Get(tName);
     if (t1 == NULL) continue;
     t1->SetBranchAddress("emcal_E",&em);
     t1->SetBranchAddress("emcal_X",&x);
     t1->SetBranchAddress("emcal_Y",&y);
     t1->SetBranchAddress("emcal_Z",&z);
     t1->SetBranchAddress("emcal_PDG",&pdg);
     
     npions_plus = 0;
     npions_minus = 0;
     v1.Clear();
     v2.Clear();
     v.clear();
     energy.clear();
     for (int j=0; j< t1->GetEntries(); j++) {
     t1->GetEntry(j);
     if (em < 1.0 ) continue;
     if (pdg == 211)  {
        hpion_plus_eng->Fill(em);
        npions_plus++;
      }
     if (pdg == -211) {
         hpion_minus_eng->Fill(em);
         npions_minus++;
      }
      
     if (pdg == 2212) hproton_eng->Fill(em);
     
     if (pdg == 22) {
        nphotons++;
        v.push_back(x);
        v.push_back(y);
        v.push_back(z);
        energy.push_back(em);
     }
     
   }
     //The code below reconstruct only one pi0. It is just a test
     v1.SetXYZ(v[0],v[1],v[2]);
     v2.SetXYZ(v[3],v[4],v[5]);
     angle = v1.Angle(v2);
     mass = sqrt( 2*energy[0]*energy[1]*(1.-cos(angle) ) );
     
     hmass->Fill(mass);
     hnpions_plus->Fill(npions_plus);
     hnpions_minus->Fill(npions_minus);
     npions_plus = 0;
     npions_minus = 0;
  
   }
    
     f->Close();
     delete f;
  }

   TCanvas * c1 = new TCanvas("c1", "c1", 100,200,600, 500);
   hnpions_plus->Draw();
   TCanvas * c2 = new TCanvas("c2", "c2", 400,200,600, 500);
   hnpions_minus->Draw();
   TCanvas * c3 = new TCanvas("c3", "c3", 600,400,600, 500);
   c3->Divide(2,2);
   c3->cd(1);
   hpion_plus_eng->Draw();
   c3->cd(2);  
   hpion_minus_eng->Draw();
   c3->cd(3);
   hproton_eng->Draw();
   c3->cd(4);
   hmass->Draw();
   
 }
