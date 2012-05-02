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

#ifndef GRIF_FRAMEWORK_INCLUDE_CONFIG_GRIPARSER_H_
#define GRIF_FRAMEWORK_INCLUDE_CONFIG_GRIPARSER_H_

#include <QHash>
#include <QList>
#include <QString>

struct linkParsingDetails {
  QString writer;
  QString reader;
  QString dataBlock;
};

struct objectParsingDetails {
  QString objectName;
  QString className;
  QList<struct linkParsingDetails*> *links;
};

class GRIParser {
 public:
  /// Parse the XML file path provided by FilePath.  Sets up the
  /// objectsAndLinks data structure which contains the details
  /// of the parsing.  This data is used by GRILoader.  Use
  /// the getObjectsAndLinks() method to retrieve the data.
  ///
  /// @return true if the parsing was successful and false otherwise
  bool Parse(QString FilePath);

  /// returns the objects and links necessary for InitRegulatorDetails()
  QList<struct objectParsingDetails> get_objs_and_links() {
    return objs_and_links_;
  }

 private:
  QHash<QString, struct objectParsingDetails *> object_hash_;
  QList<struct objectParsingDetails> objs_and_links_;
};

#endif  // GRIF_FRAMEWORK_INCLUDE_CONFIG_GRIPARSER_H_
