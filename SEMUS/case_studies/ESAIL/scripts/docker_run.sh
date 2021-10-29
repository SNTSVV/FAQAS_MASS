#! /bin/bash

# This script uses the version of the repository code in the docker image (it calls the run.sh script of the image)

set -u

topdir=$(dirname $(readlink -f $0))

docker_image="faqas_semu"

in_docker_case_dir="/home/FAQAS/faqas_semu/case_studies/LIBUTIL"

in_docker_run_script="$in_docker_case_dir/scripts/run.sh"

in_docker_workspace_dir="$in_docker_case_dir/WORKSPACE"
local_workspace_dir="$(readlink -f $topdir/../WORKSPACE)"

args="${@:1}"

more_args=""
[ "${ENV_FAQAS_SEMU_SRC_FILE:-}" != "" ] && more_args="-e ENV_FAQAS_SEMU_SRC_FILE=$ENV_FAQAS_SEMU_SRC_FILE"

sudo docker run -it --rm --cap-add=SYS_PTRACE --security-opt seccomp=unconfined \
    --mount type=bind,src=$local_workspace_dir,dst=$in_docker_workspace_dir $more_args \
    $docker_image bash -c "$in_docker_run_script $args"
