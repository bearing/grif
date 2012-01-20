#!/bin/bash
#
# Copyright (C) 2012 Gamma-ray Imaging Framework Team
# 
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 3.0 of the License, or (at your option) any later version.
# 
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
# 
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
# 
# The license can be found in the LICENSE.txt file.
# 
# Contact:
# Dr. Daniel Chivers
# dhchivers@lbl.gov

# Installation script for the Gamma Ray Imaging Framerwork

# Run this script in the directory in which you would like GRIF to be installed.
# This script should work on any POSIX compliant system.

# TODO (amidvidy): make this work for Windows (cygwin)
# Note: Cygwin binaries for ROOT are considered "very poor" by CERN,
# the final version of this script will probably ask Windows users
# to manually install ROOT.

## 
# Setting up Environment
##

OS=$(uname -s)
ARCH=$(uname -m)

##
# Utility Functions
##

# getroot: getroot <OS> <ARCH>
# Function to download the correct ROOT binaries to the current directory
# OS: can be either Darwin or Linux
# ARCH: can be i386 or X86_64
getroot () {
    case "$1" in
	"Darwin" ) 
	    if [ $2 = "X86_64" ]
	    then
		URL="ftp://root.cern.ch/root/root_v5.32.00.macosx106-x86_64-gcc-4.2.tar.gz"
	    else
		URL="ftp://root.cern.ch/root/root_v5.32.00.macosx106-i386-gcc-4.2.tar.gz"
	    fi;;
	"Linux" )
	    if [ $2 = "X86_64" ]
	    then
		URL="ftp://root.cern.ch/root/root_v5.32.00.Linux-slc5_amd64-gcc4.3.tar.gz"
	    else
		URL="ftp://root.cern.ch/root/root_v5.32.00.Linux-slc5-gcc4.3.tar.gz"
	    fi;;
    esac
    echo "Downloading ROOT binaries. This may take awhile..."
    curl -O -L $(URL)
    FILE=${URL##*root/}
    tar -xvzf $(FILE)
}
##
# Installation Logic
##
which git
if [ $? = 1 ]; 
then
    echo "Please install git before continuing."
    exit
fi

echo "Pulling GRIF source from the github repository..."
git clone git@github.com:bearing/grif.git

echo -n "Do you have ROOT installed? (y/n)"
read $ROOT
if [ $ROOT = "y" ];
then
    echo "What is the path to your root directory?"
    read $ROOTDIR
else
    mkdir root
    cd root
    getroot $(OS) $(ARCH)
    $ROOTDIR=`pwd`
    cd -
fi

# build GRIF using the right environment variables.
    