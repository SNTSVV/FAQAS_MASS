#! /bin/bash

set -u

topdir=$(dirname $(readlink -f $0))

presemu_exec=$(readlink -f $topdir/../../faqas_semu/pre_semu/main.py)

$presemu_exec $topdir/x.MetaMu.c $topdir/x.c $topdir/mutants $topdir/full_sdl_mutants

exit $?
