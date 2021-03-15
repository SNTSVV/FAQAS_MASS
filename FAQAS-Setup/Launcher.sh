#!/bin/bash

# 1. Prepare SUT

source MASS_STEPS_LAUNCHERS/PrepareSUT.sh

# 2. Compile optimized mutants

source MASS_STEPS_LAUNCHERS/CompileOptimizedMutants.sh

# 3. Process optimized mutants

source MASS_STEPS_LAUNCHERS/OptimizedPostProcessing.sh

# 4. Generate prioritized and reduced test suite

source MASS_STEPS_LAUNCHERS/GeneratePTS.sh

# 5. Execute mutants

source MASS_STEPS_LAUNCHERS/ExecuteMutants.sh

# 6. Identifiy equivalents based on coverage

source MASS_STEPS_LAUNCHERS/IdentifyEquivalents.sh

# 7. Report final information

source MASS_STEPS_LAUNCHERS/MutationScore.sh
