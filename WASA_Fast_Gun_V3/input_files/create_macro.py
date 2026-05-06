
#This script takes a file name and number of event as arguments 
import sys
def main():
    # Check if correct number of arguments provided
    if len(sys.argv) != 3:
        print("Usage: python create_macro.py <string> <decimal_value>")
        sys.exit(1)
    
    # Get arguments
    input_dat_file = sys.argv[1]
    s1 = str(sys.argv[2])
    
    filename_macro = "wasa_simulation_1.in"
    f_out = open(filename_macro, "w")
    f_out.write('#flag setFileType: set 1 for signal file, and 0 for bkg file\n')
    f_out.write('/globalField/setValue 0 0 0 tesla\n')
    f_out.write('/globalField/verbose 1\n')
    f_out.write('/gun/setInputFile input_files/'+input_dat_file+'\n')
    f_out.write('/gun/setFileType 1\n')
    f_out.write('/run/beamOn '+s1+'\n')
    f_out.write('exit')
    f_out.close()
    print('File '+filename_macro+' was written in the folder ../')
 
if __name__ == "__main__":
    main()
