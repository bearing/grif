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

#ifndef GRIF_FRAMEWORK_INCLUDE_GRILOADER_H_
#define GRIF_FRAMEWORK_INCLUDE_GRILOADER_H_

#include <QList>
#include <QFile>
#include <QHash>
#include <QString>
#include "GRIDefines.h"
#include "GRIParser.h"
#include "GRIProcessThread.h"
#include "GRIThread.h"
#include "GRIRegulator.h"

class GRIRegulator;
class GRIProcessThread;

typedef std::pair<int, int> Edge;

class GRILoader {
 public:
  explicit GRILoader(GRIRegulator *regulator);

  virtual ~GRILoader() {}

  // Initialization of process threads based on the list fileNames
  // returns pointer to list of GRIProcessThread pointers for the framework
  void initRegulatorDetails();

 protected:
  virtual GRIProcessThread* load(QString process_name, QString object_name) = 0;

 private:
  void DetectCycles();
  void UpdateGraph(const QString& reader, const QString& writer);

  QString local_grif_path_;
  GRIRegulator* regulator_;
  QList<Edge> edges_;
  QHash<QString, int> node_index_;
};

#endif  // GRIF_FRAMEWORK_INCLUDE_GRILOADER_H_
