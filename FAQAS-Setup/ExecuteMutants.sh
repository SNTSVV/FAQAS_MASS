#!/bin/bash

# 1. FAQAS-GeneratePrioritizedTestSuite

. ./mass_conf.sh

PRIORITIZED="true"

source $MASS/FAQAS-CompileAndExecuteMutants/FAQAS-GeneratePrioritizedTestSuite/FAQAS-GeneratePrioritizedTestSuite.sh

# 2. FAQAS-CompileAndExecute

SAMPLING="uniform"
#SAMPLING="stratified"
#SAMPLING="fsci"

RATE="0.05"

source $MASS/FAQAS-CompileAndExecuteMutants/FAQAS-CompileAndExecute/FAQAS-CompileAndExecute.sh $SAMPLING $RATE $PRIORITIZED

# 3. FAQAS-IdentifyEquivalentAndRedundantMutants

source $MASS/FAQAS-CompileAndExecuteMutants/FAQAS-IdentifyEquivalentAndRedundantMutants/FAQAS-IdentifyEquivalentAndRedundantMutants.sh
