# WASA_GUN
WASA Fast Simulation with MCPL Input File
Andre Nepomuceno - january 2025

Observations

    1. You need Geant4 version 4.11 and ROOT 6.x to run this application
    2. At the moment, only the electromagnetic calorimeter response is implemented (for electrons, photons, pions and protons)
    3. The source files are located in WASA_FAST/src directory
    4. The head files are located in WASA_FAST/include directory
    5. You will need to convert your input MCPL file to text file
    
 A. Prepare input file
    1. First, you need to convet your MCPL file to a ASCII using MCPL tools, available on https://mctools.github.io/mcpl/. For installation, it is recommended to use conda:
       $ conda install -c conda-forge mcpl
    
    2. Once installed, you can dump the content of the MCPL file using pymcpltool. Example:
       $ pymcpltool -l500000 HIBEAM_WASA_rwag_signal_GBL_jbar_100k_9012.mcpl > hibeam_wasa_mc.dat
    
    3. The last column of the hibeam_wasa_mc.dat file is the event number, in hexadecimal format. 
    
    5. Edit your *.dat file to remove the head (all lines with file and columns information). The file must have only numbers. 
    
    5. Put your text file name in the code WASA_FAST/src/WASAPrimaryGeneratorAction.cc (around line 52).
    
B. HOW TO COMPILE

    1. Copy the entire directory WASA_Fast_Gun to some location. For example, let us say that this location is /home/you/.
    In the /home/you/ directory, alongside WASA_Fast_Gun folder, create the folder WASA-build:

    $ mkdir WASA-build $ ls WASA-build WASA_FAST

    2. Inside the folder WASA-build, run CMake:

    $cmake -DCMAKE_PREFIX_PATH=<path_to_geant4-v11-install> /home/you/WASA_Fast_Gun

    where <path_to_geant4-v11-install> is the path to where Geant4.11 is installed.

    3. Compile:

    $ make

    If all goes well, the executable file wasa_main will be created in your WASA-build directory.

 C. HOW TO RUN
   
    1. Put your input particle source file in the WASA-build folder.
    2. Edit the wasa_simulation.in file to set the number of events to be simulated
    3. Run a simulation:
      $./wasa_main wasa_simulation.in
   
    The output of the simulation (energy recorded in the EM calorimeter and hit position) will be store in four ROOT file WASAFastOutput_t*.root, in the variable emcal_E. 
    It possible to check truth information in emcal_PDG and emcal_ETruth.
    
    
