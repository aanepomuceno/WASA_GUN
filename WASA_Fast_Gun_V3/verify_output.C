
{

   int nevt = 3;
   std::vector<int>    *pdg;
   std::vector<double> *mc_ke;
   
   std::string filename = "WASAFastOutput_t0.root";
   std::unique_ptr<TFile> f(TFile::Open(filename.c_str(), "READ"));  //auto-close file
   
   TTree *t1 = (TTree*)f->Get("MC");
   t1->SetBranchAddress("PID",&pdg);
   t1->SetBranchAddress("MC_KE",&mc_ke);
   
   cout <<"Printing truth information of the first " <<  nevt << " Events" << endl;
   for (int i =0; i < nevt; i++) {
      t1->GetEntry(i);
      cout <<"-------------------------------------------------" << endl;
      cout <<"Event " << i << endl;
      cout << left << setw(12) << "PDG" << "KE[MeV]" << endl;
      for (int i=0; i<  pdg->size(); i++) {
      cout << left << setw(12) << (*pdg)[i] <<  (*mc_ke)[i] << endl;
      }
      //cout <<"-----------------------" << endl;
   }
 }
