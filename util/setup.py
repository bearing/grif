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

from __future__ import print_function
import sys
import os

def format(path):
    return path.replace('\\','/')

grif_root = ".."
grif_root = os.path.abspath(grif_root)

try:
  app_dir = sys.argv[1]
except:
  print("usage: python setup.py /path/to/application/file")
  sys.exit(-1)

app_dir = os.path.abspath(app_dir)

data_xml = format(os.path.join(app_dir, "XML/data/data.XML"))
classes_xml = format(os.path.join(app_dir, "XML/classes"))
loader_xml = format(os.path.join("XML/app/APP.XML"))

output_directory = format(os.path.join(grif_root, "framework/include/GCG"))
if not os.path.exists(output_directory):
    os.mkdir(output_directory)

data_aux = format(os.path.join(grif_root,
                        "framework/include/GCG/GRIDataDefines_aux.h"))
loader_aux = format(os.path.join(grif_root,
                          "framework/include/GCG/GRILoader_aux.h"))
user_loader_aux = format(os.path.join(grif_root,
                               "framework/include/GCG/GRIUserLoader_aux.h"))
procs_aux = format(os.path.join(grif_root,
                         "framework/include/GCG/GRIUserProcesses_aux.h"))

print("-"*20)
print("Data XML file directory: {0}".format(data_xml))
print("Classes XML file directory: {0}".format(classes_xml))
print("data auxiliary file: {0}".format(data_aux))
print("loader auxiliary file: {0}".format(loader_aux))
print("user loader auxiliary file: {0}".format(user_loader_aux))
print("user processes auxiliary file: {0}".format(procs_aux))
print("-"*20)

print("Running code generation for data...")
os.system("python {0} {1} {2}".format("GCG_data.py", data_xml, data_aux))
print("Running code generation for loader...")
os.system("python {0} {1} {2}".format("GCG_GRILoader.py", loader_xml, loader_aux))
print("Running code generation for user loader...")
os.system("python {0} {1} {2}".format("GCG_GRIUserLoader.py", classes_xml, user_loader_aux))
print("Running code generation for user processes...")
os.system("python {0} {1} {2}".format("GCG_GRIUserProcesses.py", classes_xml, procs_aux))
