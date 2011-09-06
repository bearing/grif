#!/usr/bin/env python

'''
usage: python GCG_GRIUserLoader.pl /path/to/xml/files /path/to/auxiliary/file

This is for generating code for the framework file GRIUserLoader.cpp.
The generated code goes into the file located at /path/to/auxiliary/file

Written by Austin Benson: arbenson @ berkeley
Berkeley Applied Research on the Imaging of Neutrons and Gamma-rays
Domestic Nuclear Threat Security Initiative
'''

import XMLParser
import os

if len(sys.argv) < 3:
  print 'usage: python GCG_GRIUserLoader.py /path/to/xml/files /path/to/auxiliary/file'
  exit()
  
parser = GRIUserLoaderParser(sys.argv[1], sys.argv[2])
parser.Parse()
