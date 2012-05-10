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

#ifndef GRIF_FRAMEWORK_INCLUDE_CORE_ACCUM_GRIDAQACCUMULATOR_H_
#define GRIF_FRAMEWORK_INCLUDE_CORE_ACCUM_GRIDAQACCUMULATOR_H_

#include <core/accum/GRIAccumBuff.h>
#include <core/accum/GRIDAQBaseAccumNode.h>
#include <logging/GRILogger.h>
#include <QString>

template <class T>
class GRIDAQAccumulator : public GRIDAQBaseAccumNode {
 public:
  GRIDAQAccumulator(QString  bname, qint64 ticks, int NBuff, int AccumMsec)
    : logger_("accumulator_log.txt") {
    set_buffer_name(bname);
    set_ticks_per_sec(ticks);
    set_num_accum_buff(NBuff);
    // Set accumulation time in ticks
    set_accum_time(AccumMsec*1E-3*ticks);

    for (int i = 0; i < NBuff; ++i) {
      GRIAccumBuff<T>* b = new GRIAccumBuff<T>();
      buff_.push_back(b);
    }
  }

  ~GRIDAQAccumulator() {}

  void ResetAccumBuffs() {
    // NOTE: need typename when defining a template iterator...
    typename QList<GRIAccumBuff<T>*>::iterator  buff_it;

    // reset all times in the order of the buffers
    int buffcnt = 0;
    qint64 t1_,t2_;

    for (buff_it = buff_.begin(); buff_it != buff_.end(); buff_it++) {
      t1_ = get_ts0() + buffcnt*get_accum_time();
      t2_ = t1_ + get_accum_time();
      GRIAccumBuff<T>* b = *buff_it;
      b->ResetBuffer(t1_,t2_);

      // First buffer in list is on the bubble
      if(buffcnt == 0) {
        b->set_bubble(true);
      } else {
        b->set_bubble(false);
      }

      // Last buffer in list is leading Edge
      if(buffcnt == get_num_accum_buff()-1) {
        b->set_leading_edge(true);
      } else {
        b->set_leading_edge(false);
      }

      buffcnt+=1;
    }
  }

  void Accumulate(int numel, T data[], qint64 timestamps[],bool runflag) {
    // Trigger write when timestamps are greater than any window
    // Use bubble member to determine who is next...

    // NOTE: need typename when defining a template iterator...
    typename QList<GRIAccumBuff<T>*>::iterator  buff_it;

    bool FlushFlag = !runflag;

    bool BubbleTrigger = false;
    int BubbleWriteNum = 0;
    qint64 BubbleDT = 0;

    if(!FlushFlag) {
      for (buff_it = buff_.begin(); buff_it != buff_.end(); ++buff_it) {
        GRIAccumBuff<T>* b = *buff_it;
        // Buffer Data and sense the bubble
        BubbleDT = (b->BufferData(numel, timestamps,data));
        if (BubbleDT) {
          BubbleTrigger = true;
          // TODO(arbenson): BubbleWriteNum is used when data is missing for accumulation times
          BubbleWriteNum = (int)((BubbleDT)/get_accum_time() + 1);
        }
      }
    }

    if (BubbleTrigger) {
      for (int i = 0; i < BubbleWriteNum; ++i) {
        // We want to go through the buffers again and
        // 1) Write the bubble buffer to the mm
        // 2) Reset the bubble buffer to the leading edge
        // 3) Re-buffer the data for the new buffer
        // 4) Set the new bubble

        int nbuffcnt = 0;
        bool NewBubble = false;
        bool NewBubbleSet = false;

        for(buff_it = buff_.begin(); buff_it != buff_.end(); ++buff_it) {
          nbuffcnt = nbuffcnt + 1;
          GRIAccumBuff<T>* b = *buff_it;
          if (NewBubble) {
            b->set_bubble(true);
            NewBubbleSet = true;
            NewBubble = false;
          }

          if (b->get_leading_edge()) {
            b->set_leading_edge(false);
          }

          // Check for bubble
          if (b->get_bubble() && !NewBubbleSet) {
            // Write Bubble Accumulator to mm
            QString str;
            QTextStream qts(&str);
            qts << QTime::currentTime().toString("hh:mm:ss.zzz").toStdString().c_str()
                << ": " << get_buffer_name().toStdString().c_str()
                << "(" << nbuffcnt << ") accumulator writing "
                << b->GetDataSize() << " events. Timestamp = "
                << (double)timestamps[0]/(double)get_ticks_per_sec()
                << "\n";
            logger_.WriteLogFile(str);

            T* da = b->DataArray();

            get_p_DAQ()->WriteMemory(get_p_DAQ()->get_name(), get_buffer_name(),
                                     b->GetDataSize(), da);
            delete [] da;

            NewBubble = true;  // Sets up next buffer to be the bubble
            b->set_bubble(false);
            b->ResetBuffer(b->get_t1() + get_accum_time()*get_num_accum_buff(),
                           b->get_t2() + get_accum_time()*get_num_accum_buff());
            b->set_leading_edge(true);
            b->BufferData(numel, timestamps,data);
          }
        }

        // If we are at the end of the buffer list and need a new bubble
        if (!NewBubbleSet) {
          GRIAccumBuff<T>* b2 = buff_.front();
          b2->set_bubble(true);   
        }
      }
    }
  }

  void FlushBuffers() {
    typename QList<GRIAccumBuff<T>*>::iterator  buff_it;

    // Loop over all buffers and write the bubbles out...
    for (int i = 0; i < get_num_accum_buff(); ++i) {
      bool NewBubble = false;
      bool NewBubbleSet = false;
      std::cout << "Flushing AccumBuff Iteration " << i << std::endl;

      int buffctr = 0;
      for (buff_it = buff_.begin(); buff_it != buff_.end(); ++buff_it) {
        ++buffctr;
        GRIAccumBuff<T>* b = *buff_it;

        if (NewBubble) {
          b->set_bubble(true);
          NewBubbleSet = true;
          NewBubble = false;
        }

        if(b->get_leading_edge()) {
          b->set_leading_edge(false);
        }

        // Check for bubble
        if (b->get_bubble() && !NewBubbleSet) {
          // Write Bubble Accumulator to mm
          QString str;
          QTextStream qts(&str);
          qts << "FLUSH (" << buffctr << "): "
              << QTime::currentTime().toString("hh:mm:ss.zzz").toStdString().c_str() << ": "
              << get_buffer_name().toStdString().c_str() << " accumulator writing "
              << b->GetDataSize() << " events\n.";
          logger_.WriteLogFile(str);

          T* da = b->DataArray();

          get_p_DAQ()->WriteMemory(get_p_DAQ()->get_name(),get_buffer_name(),b->GetDataSize(),da);
          delete [] da;

          NewBubble = true;  // Sets up next buffer to be the bubble
          b->set_bubble(false);
          b->ResetBuffer(b->get_t1() + get_accum_time()*get_num_accum_buff(),
                         b->get_t2() + get_accum_time()*get_num_accum_buff());
          b->set_leading_edge(true);
        }
      }

      // If we are at the end of the buffer list and need a new bubble
      if (!NewBubbleSet) {
        GRIAccumBuff<T>* b2 = buff_.front();
        b2->set_bubble(true);
      }
    }
  }

  bool UpdateLogDir(QString logDir) { return logger_.UpdateLogDir(logDir); }
  void UpdateLogFile(QString fileName) { logger_.set_file_name(fileName); }

 private:
  QList<GRIAccumBuff<T>*> buff_;
  GRILogger logger_;
};

#endif  // GRIF_FRAMEWORK_INCLUDE_CORE_ACCUM_GRIDAQACCUMULATOR_H_
