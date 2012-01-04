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
# TODO (amidvidy): make this work for Linux and Windows (cygwin)

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
    # download and unpack ROOT binaries
    $ROOTDIR=`pwd`
fi

# build GIT using the right environment variables.
    