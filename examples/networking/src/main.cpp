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

#include <QtGui/QApplication>
#include <QtCore/QCoreApplication>
#include <QtGui/QtGui>
#include <QtCore>
#include <Qt>
#include <config/GRILoader.h>
#include <config/GRIUserLoader.h>
#include <core/GRIDefines.h>
#include <core/GRIMemoryManager.h>
#include <core/GRIRegulator.h>
#include <networking/GRIServer.h>
#include <tools/GRICLI.h>

int main(int argc, char* argv[]) {
  // the following will be included for all programs
  GRIServer *server = new GRIServer();
  server->start();
  QCoreApplication app(argc, argv);
  GRIMemoryManager *mm = new GRIMemoryManager();

  GRIRegulator *reg = new GRIRegulator(mm);

  GRILoader *loader = new GRIUserLoader(reg);
  loader->InitRegulatorDetails();
  reg->Start();
  sleep(100);
  reg->Stop();
  sleep(10);
  std::cout << "done..." << std::endl;
  return app.exec();
}
