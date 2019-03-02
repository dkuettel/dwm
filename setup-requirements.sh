#!/bin/zsh
set -eux -o pipefail

sudo apt-get -yqq install libx11-dev make gcc libxft-dev libxinerama-dev
# maybe xorg to install x11 completely (server side as well)
