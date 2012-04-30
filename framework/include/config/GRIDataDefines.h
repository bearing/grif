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

#ifndef GRIF_FRAMEWORK_INCLUDE_CONFIG_GRIDATADEFINES_H_
#define GRIF_FRAMEWORK_INCLUDE_CONFIG_GRIDATADEFINES_H_

/// This file is for defining the data structures that you need for an application
/// Please define all data structures that are posted to memory in this file.
/// This includes any data from the data acquisition and data analysis chains that
/// are posted to the framework's memory.  If it is a data structure that is
/// just used for mainpulation within the acquisition/analysis, but is not posted
/// to the memory, then it is not necessary to define them here.  However, you can
/// store them here for flexibility, in case you want to post it later.
/// An example of this is the following (for a DAQ Simulator Thread):
///
///   struct peak{
///     double value;
///     double sigma;
///     double rate;
///     int chan;
///   };
///
///   struct background{
///     double rate;
///     double min;
///     double max;
///     int chan;
///   };
///
///   struct dataOutput{
///     int chan;
///     double ADC;
///     double time;
///   };

#ifdef GRIF_CODE_GENERATION
#include <GCG/GRIDataDefines_aux.h>
#endif  // GRIF_CODE_GENERATION

#endif  // GRIF_FRAMEWORK_INCLUDE_CONFIG_GRIDATADEFINES_H_
