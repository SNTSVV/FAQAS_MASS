#!/bin/bash

cd INSTALL_DIR_REPL
. ./mass_conf.sh

# 1. Execute FAQAS-GenerateCodeCoverageMatrixes

# Generate global code coverage matrix
echo "generating global code coverage matrix"
source $MASS/FAQAS-GenerateCodeCoverageMatrixes/FAQAS-GenerateCodeCoverageMatrixes.sh 1

# Generate individual code coverage matrices
echo "generating individual code coverage matrices"
source $MASS/FAQAS-GenerateCodeCoverageMatrixes/FAQAS-GenerateCodeCoverageMatrixes.sh 0

# Generate SRCIRor configuration file
echo "generating SRCIRor coverage file"
source $MASS/FAQAS-GenerateCodeCoverageMatrixes/FAQAS-GenerateSRCIRorConf.sh

# 2. Generate mutants

# commands_json must contain full paths
source $MASS/FAQAS-GenerateMutants/FAQAS-GenerateMutants.sh
