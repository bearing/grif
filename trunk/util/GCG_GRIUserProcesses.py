#!/usr/bin/env python

'''
usage: python GCG_GRIUserProcesses.py /path/to/xml/files /path/to/auxiliary/file
This code is for generating the necessary #include statements for 
GRIUserProcceses.h.
The generated code goes into the file located at /path/to/auxiliary/file,
which should be GRIUserProcceses.h.

Written by Austin Benson: arbenson @ berkeley
Berkeley Applied Research on the Imaging of Neutrons and Gamma-rays
Domestic Nuclear Threat Security Initiative
'''

import XMLParser


if len(sys.argv) < 3:
  print 'usage: python GCG_GRIUserProcesses.py /path/to/xml/files /path/to/auxiliary/file'
  exit()

parser = GRIUserProcessesParser(sys.argv[1], sys.argv[2])
parser.Parse()
