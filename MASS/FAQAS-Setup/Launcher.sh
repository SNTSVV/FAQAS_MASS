#!/bin/bash

#
# Copyright (c) University of Luxembourg 2021.
# Created by Oscar Eduardo CORNEJO OLIVARES, oscar.cornejo@uni.lu, SnT, 2021.
#

# 1. Prepare SUT

source MASS_STEPS_LAUNCHERS/PrepareSUT.sh

# 2. Generate mutants

source MASS_STEPS_LAUNCHERS/GenerateMutants.sh

# 3. Compile optimized mutants

source MASS_STEPS_LAUNCHERS/CompileOptimizedMutants.sh

# 4. Process optimized mutants

source MASS_STEPS_LAUNCHERS/OptimizedPostProcessing.sh

# 5. Generate prioritized and reduced test suite

source MASS_STEPS_LAUNCHERS/GeneratePTS.sh

# 6. Execute mutants

source MASS_STEPS_LAUNCHERS/ExecuteMutants.sh

# 7. Identifiy equivalents based on coverage

source MASS_STEPS_LAUNCHERS/IdentifyEquivalents.sh

# 8. Report final information

source MASS_STEPS_LAUNCHERS/MutationScore.sh
