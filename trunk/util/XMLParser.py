#!/usr/bin/env python

import sys
import os
import xml
from xml.etree.ElementTree import ElementTree

'''
GRIF Code Generation
'''

'''
XMLParser is the base class for the GRIF XML parsing.
'''
class XMLParser:
  def __init__(self, prefix, suffix, dir, out):
    self.prefix = prefix
    self.suffix = suffix
    self.gen = ''
    self.out = out
    self.dir = dir
    self.files = []


  def ParseDirectory(self, path):
    # Check each file in the directory. If the file ends in .xml or .XML, add
    # it to the list of files
    dirents = os.listdir(self.dir)
    for dirent in dirents:
      if not os.path.isdir(dirent):
        if str(dirent).find('.xml') != -1 or str(dirent).find('.XML') != -1:
          self.files.append(self.dir + '/' + str(dirent))


  def ParseFile(self, path):
    pass


  def Parse(self):
    self.ParseDirectory(self.dir)
    for file in self.files:
      self.ParseFile(file)
    self.PrintToFile(self.out)


  def PrintToFile(self, path):
    f = open(self.out, 'w')
    f.write(self.prefix + self.gen + self.suffix)


'''
GRIUserProcessesParser is for generating the necessary #include statements for
GRIUserProcceses.h.
'''
class GRIUserProcessesParser(XMLParser):
  def ParseFile(self, path):
    tree = ElementTree()
    tree.parse(path)
    # get the header name
    header = tree.find('Header')
    name = header.attrib['hname']
    self.gen += '#include \"' + name + '\"\n'
