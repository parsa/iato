# IATO, The IAOO Toolkit
Project contacts: [Amaury Darsch](http://www.irisa.fr/caps/people/darsch/index_en.htm), [André Seznec](http://www.irisa.fr/caps/people/seznec/index_en.htm)

## Introduction
**IATO**, the **IAOO Toolkit** is a flexible environment that permits to analyze, emulate or simulate the IA64 Instruction Set Architecture (ISA) binary executables. **IATO** is a flexible and portable framework that is built around a set of C++ libraries and clients (i.e application programs). The fundamental clients are the IA64 emulator and simulator. Other clients provides supports for program analysis and statistical computation.
### Latest news
This is revision 1.0 of the **IAOO Toolkit**. This release incorporates severalfixes within the in-order simulator. This version is backward compatible with the release 1.5.
### Distribution
**IATO** is supported on various UNIX platforms, including the Linux IA32, IA64 and Solaris. **IATO** is distributed as a compressed tar file. **IATO** has been successfully tested with the GCC 3 compiler. **IATO** is completely written in C++ and uses the STL. The following table provides the pointer to the distribution and the associated documentation.

File | Description | Last update
--- | --- | ---
[iato-src-1.0.tar.gz](http://www.irisa.fr/caps/projects/ArchiCompil/iato/ftp/iato-src-1.0.tar.gz) | IATO destribution revision 1.0 | 09/15/2004
[iato-ref-1.0.pdf](http://www.irisa.fr/caps/projects/ArchiCompil/iato/ftp/iato-ref-1.0.pdf) | IATO reference manual revision 1.0 | 09/15/2004
[iato-api-1.0.pdf](http://www.irisa.fr/caps/projects/ArchiCompil/iato/ftp/iato-api-1.0.pdf) | IATO application programming manual revision 1.0 | 09/15/2004

Pre-compiled IA64 binaries are also available for your convenience. With them, it it possible to stimulate directly the IAOO toolkit. The binary files include the distribution test cases an some SPEC2000 benchmarks.

File | Description
--- | ---
[iato-tst-1.0.tar.gz](http://www.irisa.fr/caps/projects/ArchiCompil/iato/ftp/iato-tst-1.0.tar.gz) | IATO test programs revision 1.0
[iato-spec-2000.tar.gz](http://www.irisa.fr/caps/projects/ArchiCompil/iato/ftp/iato-spec-2000.tar.gz) | SPEC 2000 benchmarks compiled statically for the IA64 (~ 24Mb)

## IATO clients
The IAOO Toolkit is distributed with 4 clients as shown in the table below.

Client | Description
--- | ---
IAOS | A client program that analyzes IA64 binary executables.
IAKA | A client program that emulates and traces IA64 binary executables.
IAIO | A client program that simulates and traces IA64 binary executables in an in-order mode.
IAOO | A client program that simulates and traces IA64 binary executables in an out-of-order mode.
IATA | A client program that computes statistics from trace produced by other clients.

### IAOS client
**IAOS** is a simple client program that produces static information from a binary executable. The resulting information comprises, among other things, disassembled object code and various statistics. The primary reason to use the **IAOS** client is to collect information about a binary executable.

### IAKA client
**IAKA** is a client program that emulates and traces IA64 binary executables. The emulator executes the common loop: fetch, decode and execute. A more accurate description of the emulator loop is given in documentation. The resulting trace is binary encoded (for performance and space reasons). The trace can be decoded with the **IATA** client program.

### IAIO client
**IAIO** is a client program that simulates and traces binary IA64 executables in an in-order mode. The IAIO simulator implements an 8 stages in-order core engine, with true predicated instruction execution based on the Itanium 2 microarchitecture. The simulator is cycle accurate and provides numerous architectural options that permits to perform a resource fine tuning. During the course of the simulation, a stage by stage trace can be generated as well as a summarized statistic. The simulator is primarily used to computed the program IPC.

### IAOO client
**IAOO** is a client program that simulates and traces binary IA64 executables. The IAOO simulator implements a 10 stages out-of-order core engine, with true predicated instruction execution based on translation register buffer (TRB). The simulator is cycle accurate and provides numerous architectural options that permits to perform a resource fine tuning. During the course of the simulation, a stage by stage trace can be generated as well as a summarized statistic. The simulator is primarily used to computed the program IPC.

### IATA client
**IATA** is a client program that analyzes and prints binary traces produced by the other clients. It can also generate one trace from another. Trace generation can be seen as a post-processing operation. Another useful feature is the statistic computation. Given a trace file, the **IATA** client can extract, for example, the instruction distribution or other runtime information.

