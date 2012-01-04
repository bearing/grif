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

import sys
import os

grif_root = "../../"
grif_root = os.path.abspath(grif_root)

data = os.path.join(grif_root, "util/GCG_data.py")
user_loader = os.path.join(grif_root,
                           "util/GCG_GRIUserLoader.py")
user_procs = os.path.join(grif_root,
                          "util/GCG_GRIUserProcesses.py")

classes_xml = os.path.abspath("./XML/classes")

loader_aux = os.path.join(grif_root,
                          "framework/include/GCG/GRIUserLoader_aux.h")
procs_aux = os.path.join(grif_root,
                         "framework/include/GCG/GRIUserProcesses_aux.h")

print "-"*20
print "Classes XML file directory: {0}".format(classes_xml)
print "user loader code gen script: {0}".format(user_loader)
print "user processes code gen script: {0}".format(user_procs)
print "user loader auxiliary file: {0}".format(loader_aux)
print "user processes auxiliary file: {0}".format(procs_aux)
print "-"*20

print "Running code generation for user loader..."
os.system("python {0} {1} {2}".format(user_loader, classes_xml, loader_aux))
print "Running code generation for user processes..."
os.system("python {0} {1} {2}".format(user_procs, classes_xml, procs_aux))
