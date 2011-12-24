#!/usr/bin/env python

'''
usage: python GCG_GRILoader.py /abs/path/to/app/xml/file /path/to/auxiliary/file

This is for generating code for the framework file GRILoader.cpp.
The generated code goes into the file located at /path/to/auxiliary/file

/path/to/xml/file should be the application XML file for the project

Written by Austin Benson: arbenson @ berkeley
Berkeley Applied Research on the Imaging of Neutrons and Gamma-rays
Domestic Nuclear Threat Security Initiative
'''

import sys

if len(sys.argv) < 3:
  print 'usage: python GCG_GRILoader.py /abs/path/to/app/xml/file /path/to/auxiliary/file'
  exit()
  
f = open(sys.argv[2], 'w')
out = "app_file = \"{0}\";".format(sys.argv[1])
f.write(out)
