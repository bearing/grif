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

#ifndef GRIF_FRAMEWORK_INCLUDE_GRIDATABLOCK_H_
#define GRIF_FRAMEWORK_INCLUDE_GRIDATABLOCK_H_

#define DATA_BLOCK_DEBUG

#include <cstdio>
#include <cstdlib>
#include <QList>
#include <QString>
#include "GRIProcessThread.h"
#include "GRIRegulator.h"

// Load balancing constant
#define MAX_THRESHOLD 2 // When the difference in packets with writers from readers is this big
// load balancing will be performed
#define LOAD_BALANCING_FACTOR 0.75 // When to ask thread to adjust their priority

class GRIProcessThread;
class GRIMemoryManager;

/// GRIBufferObject class is a buffer descriptor (eg: energy buffer). The reason
/// this class is created is because it could monitor the flow of the packets
/// easily since each object of this class will have pointers to the thread
/// writing to this buffer and the thread reading from it.

struct ReaderDataObject {
  QString name;
  QString buffer;
};

struct AnalysisStructureObject {
  QString data;
  QString From;
  QList<ReaderDataObject> To;
};

struct reader_t {
  long read_counter;
  GRIProcessThread* reader;
  QString reader_name;
  QString reader_data;
};

class GRIDataBlock: public GRIObject {
  friend class GRICommandAndControl;
  friend class GRIMemoryManager;

 public:
  GRIDataBlock(GRIRegulator *reg, GRIMemoryManager *mm, QString readerName,
	       QString readerBuffer, QString objectDataName, 
	       QString objectFromName);

  ~GRIDataBlock();

  /// GetReaderList() returns the threads that are reading to this data block
  QList<reader_t*>* GetReaderList() { return &readers_; }

  /// SetLink() sets up the pointers to the processes objects that are directly
  /// involved with this buffer (ie: those who will be writing to or reading from
  /// this buffer)
  void SetLink(QLinkedList<GRIProcessThread*>* processes);

  /// load_balancing() monitors the flow of the packets. If an inbalance is
  /// detected (which is based on the value of threshold), the buffer will call
  /// for an adjustment of priority of the processes that are writing/reading
  /// from it
  void LoadBalancing();

  /// update_reader() updates the state of the reader (ie: the # of times it's reading
  /// from this buffer) specified in the parameter
  bool UpdateReader();

  /// UpdateWriter() updates the state of the writer (ie: the # of times it's
  /// writing from this buffer) specified in the parameter
  bool UpdateWriter();

  /// DeletePacket() decides whether some spaces could be freed by deleting
  /// packets
  void DeletePacket();

  /// If the data block is active, then data will pass through this block in the
  /// system.  If the data block is not active, then data will not pass through
  /// this block, and a warning message will be posted.  The block's active
  /// status can be changed as often as the user wants during run time via
  /// GRIProcessThread::EnableDataBlock() and 
  /// GRIProcessThread::DisableDataBlock().  By default, the data block is
  /// enabled for use.
  void set_is_enabled(bool is_enabled) { is_enabled_ = is_enabled; }

  /// get_is_enabled() returns an indicator of whether or not the data block is
  /// currently in use.  This allows the user to dynamically disable and
  /// re-enable links at run time.
  bool get_is_enabled() { return is_enabled_; }

  /// get_name() returns the name of this data block
  QString get_name() { return name_; }

  /// get_writer_name() returns the thread's name that's writing to this block
  QString get_writer_name() { return writer_name_; }

  /// get_writer() returns the thread that's writing to this data block
  GRIProcessThread* get_writer() { return writer_; }

 private:
  struct AnalysisStructureObject *obj_;
  GRIRegulator* reg_;
  GRIMemoryManager* mm_;
  QString name_;  ///< name of this data block
  GRIProcessThread* writer_;  ///< the thread that's writing to this object
  QString writer_name_;  ///< name of writer writing to this object
  QList<reader_t*> readers_;  ///< list of threads reading from this object
  long write_counter_;  ///< # of times this buffer is written
  long first_packet_;  ///< last packet # deleted + 1
  bool is_enabled_;
};

#endif  // GRIF_FRAMEWORK_INCLUDE_GRIDATABLOCK_H_
