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
