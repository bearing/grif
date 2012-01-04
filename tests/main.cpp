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

#include <QCoreApplication>
#include "QtTestUtil.h"
#include "TestRegistry.cpp"


/**
  * This is the runner for all the test cases. In order to add a test, include the test file
  * instantiate an instance of your test, and then add QTest::qExec(&[testobj], argc, arv)
  * Separate tests for each class in a separate .cpp, naming convention is [ClassName]_Test.cpp
  * The structure of each individual test class should be similar to GRIProcList_Test.cpp
  * Be sure to register your test at the end and include the *.moc file.
  */
int main(int argc, char *argv[])
{
    QCoreApplication application(argc, argv);
    return QtTestUtil::TestRegistry::getInstance()->runTests(argc, argv);
}
