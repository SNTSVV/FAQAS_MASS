
## Preparation
- Download MLFS folder as zip from [https://dropit.uni.lu/invitations/?share=46d83f64940adf814043#/MLFS-TestGeneration/MLFS-QDP_I1_R1.zip](https://dropit.uni.lu/invitations/?share=46d83f64940adf814043#/MLFS-TestGeneration/MLFS-QDP_I1_R1.zip)
- Extract it here into folder a folder named `WORKSPACE/DOWNLOADED` as `WORSPACE/DOWNLOADED/MLFS-QDP_I1_R1`

## Optional Configurations
In order to change the values of configuration such as the test generation timeout or memory limit (see the file [scripts/faqas_semu_config.sh](scripts/faqas_semu_config.sh)):

1. Make a copy of the file in the `WORSPACE` folder
```
cp scripts/faqas_semu_config.sh WORKSPACE/faqas_semu_config.sh
```

2. Change the configuration values in the resulting file `WORKSPACE/faqas_semu_config.sh`

The scripts will use the configuration file `WORKSPACE/faqas_semu_config.sh` if present. if not, it will use `scripts/faqas_semu_config.sh`

## Running Using FAQAS-SEMu Docker image
#### Requirement
The `faqas_semu` Docker image must be build as specified in the header of the [Dockerfile](../../Dockerfile)

#### Running
use the script [scripts/docker_run.sh](scripts/docker_run.sh) to run. The help is available by running:
```
scripts/docker_run.sh --help
```
Note that the [scripts/docker_run.sh](scripts/docker_run.sh) calls the script [scripts/run.sh](scripts/run.sh) within a `faqas_semu` docker container.

## Running Without Docker image
#### Requirement
- Install MASS and setup its environment variables
- Install `faqas_semu` dependencies (which include `jq` needed by this case study) by running the topdir [install_requirements.sh](../../install_requirements.sh) script.

#### Running
use the script [scripts/run.sh](scripts/run.sh) to run. The help is available by running:
```
scripts/run.sh --help
```
