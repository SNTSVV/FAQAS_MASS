
## Preparation
- Download ASN folder as zip from [https://dropit.uni.lu/invitations/?share=b5654d5f4e23cc9020f2#](https://dropit.uni.lu/invitations/?share=b5654d5f4e23cc9020f2#)
- Extract it here into folder a folder named `WORKSPACE/DOWNLOADED`

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
