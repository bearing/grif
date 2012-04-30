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

#include <logging/GRILogMessage.h>
#include <QDateTime>
#include "stdlib.h"
#include "stdio.h"

GRILogMessage::GRILogMessage() {
  level = 0;
  SetTimeString();
}

GRILogMessage::~GRILogMessage() {}

bool GRILogMessage::SetMessageTime(QString s, int l) {
  SetTimeString();
  if (l == 0) {
    MsgStr = DateTime + " (" + object_name_ + ") " + ": DEBUG - " + s;
  } else if (l == 1) {  // VERBOSE
      MsgStr = DateTime + " (" + object_name_ + ") " + ": " + s;
  } else if (l == 2) { // MSG
      MsgStr = DateTime + " (" + object_name_ + ") " + ": " + s;
  } else if (l == 3) {
    MsgStr = DateTime + " (" + object_name_ + ") " + ": WARNING - " + s;
  } else if (l == 4) {
    MsgStr = DateTime + " (" + object_name_ + ") " + ": ERROR - " + s;
  } else {
    return 0;
  }
  level = l;
  return 1;
}


void GRILogMessage::SetTimeString() {
  DateTime = QDateTime(QDate::currentDate(),
		       QTime::currentTime()).toString("dd.MM.yyyy hh:mm:ss.zzz");
}

bool GRILogMessage::IsLevelEnabled() {
  QString GlobalLevelEnv = getenv("GRIFLOGLEVEL");
  int GL;

  if (GlobalLevelEnv == "DEBUG") {
    GL = 0;
  } else if (GlobalLevelEnv == "VERBOSE") {
    GL = 1;
  } else if (GlobalLevelEnv == "MESSAGE") {
    GL = 2;
  } else if(GlobalLevelEnv == "WARNING") {
    GL = 3;
  } else if(GlobalLevelEnv == "ERROR") {
    GL = 4;
  } else {
    GL = 5;
  }
  return(level >= GL);
}

bool GRILogMessage::IsLevelEnabled(int l) {
  QString GlobalLevelEnv = getenv("GRIFLOGLEVEL");
  int GL;

  if (GlobalLevelEnv == "DEBUG") {
    GL = 0;
  } else if (GlobalLevelEnv == "VERBOSE") {
    GL = 1;
  } else if (GlobalLevelEnv == "MESSAGE") {
    GL = 2;
  } else if (GlobalLevelEnv == "WARNING") {
    GL = 3;
  } else if (GlobalLevelEnv == "ERROR") {
    GL = 4;
  } else {
    GL = 5;
  }
  return(l >= GL);
}
