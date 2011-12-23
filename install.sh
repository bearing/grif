#!/bin/bash
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
    