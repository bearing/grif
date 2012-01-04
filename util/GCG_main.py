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

'''
usage: python GCG_main.py /path/to/class/xml/files /path/to/grif/skeletons [aux_only]

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
  print 'usage: python GCG_main.py /path/to/class/xml/files /path/to/grif/skeletons [aux_only]'
  exit()

aux_only = False
if len(sys.argv) > 3:
  aux_only = True
parser = XMLParser.ClassParser(sys.argv[1], None)
parser.SetSkelDir(sys.argv[2])
parser.SetAuxOnly(aux_only)
parser.Parse()
