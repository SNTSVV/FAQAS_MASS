#! /bin/bash

set -u
set -e

error_exit()
{
    echo "ERROR: $1"
    exit 1
}

# XXX: We assume, for now, that MASS is already installed

TOPDIR=$(dirname $(readlink -f $0))

# Install python requirement
## envure python version
python -c "import platform; exit(int(not (platform.python_version_tuple() > ('3','4'))))" || error_exit "Python version too low"

# Install system packages (case study dependencies)
if [ $(id -u) -eq 0 ]; then
    # already sudo
    apt-get -y install jq
    apt-get -y install libjpeg-dev zlib1g-dev
else
    sudo apt-get -y install jq
    sudo apt-get -y install libjpeg-dev zlib1g-dev
fi

## install requirements
pip install -U -r $TOPDIR/requirements.txt