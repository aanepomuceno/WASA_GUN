
#Convert MCPL to dat file and write a macro for Geant4
filename_in="HIBEAM_WASA_rwag_signal_GBL_jbar_100k_9012.mcpl"
filename_out="hibeam_wasa_mc.dat"
pymcpltool --stats $filename_in > out
nparticles=$(grep "nparticles" out | awk '{print $3}')
pymcpltool -l${nparticles} $filename_in > $filename_out
sed -i 1,24d $filename_out  #remove head lines
rm -f out
#Now write a macro for FastSim
nevt=$(tail -1 $filename_out | awk '{print $NF}' | python3 -c "print(int(input(), 0))")
echo "Number of events: $nevt"
python create_macro.py $filename_out $nevt
cp wasa_simulation_1.in ../

