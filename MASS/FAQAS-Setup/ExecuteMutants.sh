#!/bin/bash

# 1. FAQAS-GeneratePrioritizedTestSuite

cd INSTALL_DIR_REPL
. ./mass_conf.sh

source $MASS/FAQAS-CompileAndExecuteMutants/FAQAS-GeneratePrioritizedTestSuite/FAQAS-GeneratePrioritizedTestSuite.sh

# 2. FAQAS-CompileAndExecute

source $MASS/FAQAS-CompileAndExecuteMutants/FAQAS-CompileAndExecute/FAQAS-CompileAndExecute.sh $SAMPLING $RATE $PRIORITIZED

# 3. FAQAS-IdentifyEquivalentAndRedundantMutants

source $MASS/FAQAS-CompileAndExecuteMutants/FAQAS-IdentifyEquivalentAndRedundantMutants/FAQAS-IdentifyEquivalentAndRedundantMutants.sh
