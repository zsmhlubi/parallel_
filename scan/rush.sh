echo ---------scan_omp is started----------
./scan_omp |& tee -a terminal_output.out
echo ---------scan_omp is done-------------
echo
echo ---------scan is started-----
./scan |& tee -a terminal_output.out
echo ---------scan is done--------
# echo
# echo -----------pi is started----------------
# ./pi 3 |& tee -a terminal_output.out
# echo -----------pi is done--------------------