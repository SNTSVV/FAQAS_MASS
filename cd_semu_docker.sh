#! /bin/bash

# ../faqas_semu/underlying_test_generation/main.py x.MetaMu.bc --output_top_directory xxxx --clear_existing --generation_timeout 10
#

set -u

error_exit ()
{
    echo "Error: $1"
    exit 1
}

docker_image="thierrytct/klee-semu:llvm-3.4.2"

topdir=$(dirname $(readlink -f $0))

in_docker_cmd=""
if [ $# -eq 2 ]; then
    in_docker_cmd=$2
else
    [ $# -eq 1 ] || error_exit "expected one or two argument(s): <workspace directory> [command to execute in docker]"
fi
workspace_dir=$1
test -d $workspace_dir || error_exit "Specified workspace dir not existant"
workspace_dir=$(readlink -f $workspace_dir)

indocker_topdir=/home/FAQAS/faqas_semu
indocker_workspace_dir=/home/FAQAS/workspace

prepare_cmd="pip install -U -r $indocker_topdir/requirements.txt && mkdir -p /home/FAQAS && cd /home/FAQAS"

sudo docker run -it --rm --cap-add=SYS_PTRACE --security-opt seccomp=unconfined \
    --mount type=bind,src=$topdir,dst=$indocker_topdir \
    --mount type=bind,src=$workspace_dir,dst=$indocker_workspace_dir \
    $docker_image bash -c "export  C_INCLUDE_PATH=\$C_INCLUDE_PATH:/home/klee-semu/klee_src/include/ && $prepare_cmd && bash $in_docker_cmd"

