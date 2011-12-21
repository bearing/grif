#!/usr/bin/env python

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
                          "include/GCG/GRIUserLoader_aux.h")
procs_aux = os.path.join(grif_root,
                         "include/GCG/GRIUserProcesses_aux.h")

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
