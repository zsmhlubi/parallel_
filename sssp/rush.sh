echo ---------sssp_omp is started----------
./sssp_omp |& tee -a terminal_output.out
echo ---------sssp_omp is done-------------
echo
echo ---------sssp is started-----
./sssp |& tee -a terminal_output.out
echo ---------sssp is done--------
echo
echo -----------sss_mpi is started----------------
./sss_mpi 3 |& tee -a terminal_output.out
echo -----------sss_mpi is done--------------------