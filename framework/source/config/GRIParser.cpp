// Copyright (C) 2012 Gamma-ray Imaging Framework Team
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 3.0 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
//
// The license can be found in the LICENSE.txt file.
//
// Contact:
// Dr. Daniel Chivers
// dhchivers@lbl.gov

#include "iostream"
#include "qdom.h"
#include "QFile"
#include "GRIParser.h"

bool GRIParser::Parse(QString FilePath) {
  QFile f(FilePath);
  if (!f.exists()) {
    std::cout << "ERROR: Could not find file: " << FilePath.toStdString()
	      << std::endl;
    return false;
  }
  QDomDocument doc;
  if (!doc.setContent(&f)) {
    std::cout << "ERROR: Could not interpret file " << FilePath.toStdString()
	      << " as an xml file" << std::endl;
    return false;
  }

  std::cout << "Interpretation of file " << FilePath.toStdString() << ":"
            << std::endl;
  std::cout << doc.toString().toStdString() << std::endl;

  QDomElement root = doc.documentElement();

  QDomElement elem = root.firstChildElement("Objects");
  if(elem.isNull()) {
    std::cout << "ERROR: Could not find the Objects tag. "
              << "Please check your XML formatting."
	      << std::endl;
    return false;
  }
  int num = elem.elementsByTagName("object").count();
  if (num == 0) {
    std::cout << "WARNING: file " << FilePath.toStdString()
	      << " has no objects declared." << std::endl;
  }
  elem = elem.firstChildElement("object");

  // read the objects, constructing object_hash_ as we go along
  for (int i = 0; i < num; ++i) {
    QDomAttr name = elem.attributeNode("name");
    if (name.value().isNull()) {
      std::cout << "No object name found for an object.  Skipping that object..."
                << std::endl;
      continue;
    }
    QDomAttr c = elem.attributeNode("class");
    if (c.value().isNull()) {
      std::cout << "No class name found for an object.  Skipping that object..."
                << std::endl;
      continue;
    }
    struct objectParsingDetails *opd = new struct objectParsingDetails;
    opd->links = new QList<struct linkParsingDetails*>;

    opd->objectName = name.value();
    opd->className = c.value();
    object_hash_[opd->objectName] = opd;
    objs_and_links_.push_back(*opd);

    elem = elem.nextSiblingElement("object");
  }

  // read the links
  QDomElement e = root.firstChildElement("Links");
  if (e.isNull()) {
    std::cout << "WARNING: Could not find the Links tag.  Skipping links."
              << std::endl;
  } else {
    num = e.elementsByTagName("link").count();
    if (num == 0) {
      std::cout << "WARNING: file " << FilePath.toStdString()
                << " has no links declared." << std::endl;
    }
    e = e.firstChildElement("link");

    for (int i = 0; i < num; ++i) {
      QDomAttr w = e.attributeNode("writer");
      if (w.value().isNull()) {
	std::cout << "No writer found for a link.  Skipping that link..."
                  << std::endl;
	continue;
      }
      QDomAttr r = e.attributeNode("reader");
      if (r.value().isNull()) {
	std::cout << "No reader found for a link.  Skipping that link..."
                  << std::endl;
	continue;
      }
      QDomAttr data = e.attributeNode("data");
      if (data.value().isNull()) {
	std::cout << "No data found for a link.  Skipping that link..."
                  << std::endl;
	continue;
      }

      struct linkParsingDetails *lpd = new struct linkParsingDetails;
      lpd->writer = w.value();
      lpd->reader = r.value();
      lpd->dataBlock = data.value();

      struct objectParsingDetails *opd2 = object_hash_[lpd->writer];
      if (opd2 != 0) {
	opd2->links->push_back(lpd);
      }

      e = e.nextSiblingElement("link");
    }
  }

  return true;
}
