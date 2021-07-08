#! /bin/bash

set -u

topdir=$(dirname $(readlink -f $0))

docker_image="faqas_semu"

in_docker_case_dir="/home/FAQAS/faqas_semu/case_studies/ASN"

in_docker_run_script="$in_docker_case_dir/scripts/run.sh"

in_docker_workspace_dir="$in_docker_case_dir/WORKSPACE"
local_workspace_dir="$(readlink -f $topdir/../WORKSPACE)"

sudo docker run -it --rm --cap-add=SYS_PTRACE --security-opt seccomp=unconfined \
    --mount type=bind,src=$local_workspace_dir,dst=$in_docker_workspace_dir \
    $docker_image bash #-c "$in_docker_run_script ${@:1}"
