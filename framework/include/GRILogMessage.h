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

#ifndef GRIF_FRAMEWORK_INCLUDE_GRILOGMESSAGE_H_
#define GRIF_FRAMEWORK_INCLUDE_GRILOGMESSAGE_H_

#include <iostream>
#include <QString>

class GRILogMessage {
 public:
  GRILogMessage();
  ~GRILogMessage();

  bool SetMessageTime(QString s, int level = 0);
  void SetObjectName(QString s) {
    object_name_ = s;
  }
  bool IsLevelEnabled(int level);
  bool IsLevelEnabled();
  QString GetObjectName() {
    return object_name_;
  }
  int level;
  QString DateTime;
  QString MsgStr;

 private:
  QString object_name_;
  void SetTimeString();
};

#endif  // GRIF_FRAMEWORK_INCLUDE_GRILOGMESSAGE_H_
