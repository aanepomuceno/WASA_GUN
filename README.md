WASA Fast Simulation with MCPL Input File \
Andre Nepomuceno - August 2025

Observations

    1. You need Geant4 version 4.11 and ROOT 6.x to run this application
    2. At the moment, only the electromagnetic calorimeter response is implemented (for electrons, photons, pions and protons)
    3. The source files are located in WASA_Fast_Gun_V2/src directory
    4. The head files are located in WASA_Fast_Gun_V2/include directory
    5. You will need to convert your input MCPL file to a dat file (more below)
    6. You will also need MCPL tools, available at https://mctools.github.io/mcpl/
    
A. Compilation

    1. Copy the entire directory WASA_Fast_Gun_V2 to some location. For example, let us say that this location is /home/you/.
    In the /home/you/ directory, alongside WASA_Fast_Gun_V2 folder, create the folder WASA-build:

    $ mkdir WASA-build 
    $ ls 
      WASA-build WASA_Fast_Gun_V2

    2. Inside the folder WASA-build, run CMake:

    $cmake -DCMAKE_PREFIX_PATH=<path_to_geant4-v11-install> /home/you/WASA_Fast_Gun_V2

    where <path_to_geant4-v11-install> is the path to where Geant4.11 is installed.

    3. Compile:

    $ make
  
    If successful, the executable wasa_main will be created in your WASA-build directory.
  
 B. Preparing the input file
 
    1. Install MCPL tools (recommended via conda):
    
        $ conda install -c conda-forge mcpl
        
    2. Copy the folder WASA_Fast_Gun_V2/input_files to WASA-build:
    
       $ cp -r /home/you/WASA_Fast_Gun_V2/input_files/ /home/you/WASA-build
       $ cd input_files/
       
       Inside this folder, you will find the script mcpl_to_dat_1F.sh, to convert the a MCPL file to a ASCII file.
       Edit the first two lines of this script with the names of your input MCPL file and the desired .dat file name. 
   
    3. Copy your MCPL file (or files) to WASA_Fast_Gun_V2/input_files/, or put the full MCPL file path in the script  
 
    4. Run the script mcpl_to_dat_1F.sh. Besides converting the MCPL file to a .dat file, it will create the macro wasa_simulation_1.in in the WASA-build folder
    
       $ source mcpl_to_dat_1F.sh
                
    5. Go back to the WASA-build folder. Edit the wasa_simulation_1.in macro to set the value of the flag setFileType (1 for signal files, or 0 for background).
    
   C. Running the Simulation
   
    1. Run a simulation:
    
      $ ./wasa_main wasa_simulation_1.in
      
     The output of the simulation (energy recorded in the EM calorimeter and hit position) will be store in the 
     ROOT file WASAFastOutput_t0.root. The file has the two folders: MC, where the truth information is stored, and EMCAL, with the calorimeter responses. 
     Reconstructed energy is stotred in the variable emcal_E. It is possible to check truth information in emcal_PDG and emcal_ETruth.
    
    2. You can use the macro verify_output.C to check the integrity of the ROOT output file. It will print informations on the first three events.
    
        $ root -l
        root [0] .x verify_output.C 
    
    3. Use the macro signal_analysis_v2.C to plot the energy of charged pions and protons, in case of a signal simulation
    
       $ root -l
       root [0] .x signal_analysis_v2.C
    
    
    
