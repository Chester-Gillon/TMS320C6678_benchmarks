TMS320C6678_benchmarks
======================

Benchmarks for testing a TMS320C6678 DSP.

Uses CCS v6 and TI-RTOS for C6000 v2.0.1.23

The target is a TMDSEVM6678L EVM revision 3.

The evm6678_1G_DDR3 contains a custom platform in which:
- All of the L2 has been set use by the cache
- The DDR3 size has been set to 1Gbyte as fitted to the EVM revision 3
