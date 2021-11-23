# MASS

We present MASS, a mutation analysis tool for embedded software executed on cyber-physical systems (CPS). We target space CPS (e.g., satellites) and CPS with similar characteristics (e.g., UAVs). 

Mutation analysis measures the quality of test suites in terms of the percentage of artificial faults detected. There are many mutation analysis tools available but they are inapplicable to CPS because they are affected by scalability and accuracy problems.

To overcome such limitations, MASS implements a set of optimization techniques that enable the applicability of mutation analysis and address scalability and accuracy in the CPS context.

MASS has been successfully evaluated on a large study involving embedded software systems provided by industry partners; the study includes an on-board software system managing a microsatellite currently on-orbit, a set of libraries used in deployed cubesats, and a mathematical library provided by the European Space Agency.

## Dependencies

* Linux packages: clang 3.8, r-base, jq, Python 3.7 or higher 
* R packages: binom
* Python packages: numpy, scipy

## Installation

MASS depends on LLVM for source code mutation. For this reason, a full LLVM-3.8.1 installation is necessary preceding the installation of the SRCMutation component. For this procedure, a Bash script is provided.

The following shell command installs the corresponding dependencies and the SRCMutation component.

```
./llvm-build.sh
```

## Getting started

### Initialization of the MASS workspace

MASS creates a workspace folder where all the steps from the methodology are stored.

An installation Bash script is provided for the creation of this workspace, the script can be found on `MASS/FAQAS-Setup/install.sh`

To use the installation script the shell variable `INSTALL_DIR` has to be set:

```
export INSTALL_DIR=/opt/DIRECTORY
```

If the `INSTALL_DIR` directory must be binded inside a container. In addition, also the shell variable `EXECUTION_DIR` has to be set. This step is optional.

For instance, MASS has been installed on `/opt/MASS_WORKSPACE` (i.e., the `INSTALL_DIR`), and MASS will be executed inside a container, but on a different directory such as `/home/user/MASS_WORKSPACE` (i.e., the `EXECUTION_DIR`). The use of both environment variables enable this differentiation.


After setting the corresponding environment variables, the following commands are necessary to create the \MASS workspace folder:

```
cd MASS/FAQAS-Setup
./install.sh
```

For more details about MASS usage, please refer to the SUM document provided in the `documentation` folder.
