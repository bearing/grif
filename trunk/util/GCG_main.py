#!/usr/bin/env python

'''
usage: python GCG_main.py /path/to/xml/files /path/to/grif/skeletons

This is a basic XML-reading code generation tool for GRIF

This script looks at each .XML file in /path/to/xml/files and parses out
serveral pieces of information:

(1) Run-time parameters (including support for QVector and vector types)
(2) Run-time actions
(3) Initial values for run-time parameters
(4) Class name
(5) Header name

A sample XML file would like the following:

<Class>
   <Name cname = "SIMDAQThread" isdaq = "true" />
   <Auxiliary apath = "/Users/benson/Desktop/GCG_test/GCG_daq_aux.h" />
   <Header hname = "SIMDAQThread.h" />
   <Skeleton spath = "/Users/benson/Desktop/GCG_test" />
   <RunTimeParams>
       <Param pname = "e1" type = "double" default = "10.5" />
       <Param pname = "ind" type = "int" default = "2" />
       <Param pname = "e1_vec" type = "vector &lt; double &gt;" />
       <Param pname = "ind_vec" type = "vector &lt; int &gt;" />
       <Param pname = "e1_qvec" type = "QVector &lt; double &gt;" />
       <Param pname = "ind_qvec" type = "QVector &lt; int &gt;"  />
   </RunTimeParams>
   <RunTimeActions>
       <Action method = "DynamicAddPeak" />
       <Action method = "DynamicRemovePeak" />
       <Action method = "DynamicChangePeak" />
   </RunTimeActions>
</Class>

The &lt; and &gt; are the XML characters for '<' and '>'.  The type attribute is simply
stripped from the XML file, so the type can be any legally-define C/C++ structure.

Written by Austin Benson: arbenson @ berkeley
Berkeley Applied Research on the Imaging of Neutrons and Gamma-rays
Domestic Nuclear Threat Security Initiative
'''

import XMLParser
import sys

if len(sys.argv) < 3:
  print 'usage: python GCG_main.py /path/to/xml/files /path/to/grif/skeletons'
  exit()

parser = XMLParser.ClassParser(sys.argv[1], None)
parser.SetSkelDir(sys.argv[2])
parser.Parse()
