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

#ifndef GRIF_FRAMEWORK_INCLUDE_CORE_GRIDEFINES_H_
#define GRIF_FRAMEWORK_INCLUDE_CORE_GRIDEFINES_H_

#include <QtGlobal>
#include <QThread>

// Constants and macros for GRI framework (wrap Qt constants/macros).

//  To determine operating system information:
//      OPERATING_SYSTEM will be == to LINUX, WINDOWS, or MAC
//      IS_64_BIT will be == to BIT_32, or BIT_64
//      SYSTEM_ENDIAN will be == to LITTLE_END or BIG_END
//
//  Types defined:
//      uint8_g for 8 bit unsigned integers
//      uint16_g for 16 bit unsigned integers
//      uint32_g for 32 bit unsigned integers
//      uint64_g for 64 bit unsigned integers
//      int8_g for 8 bit signed integers
//      int16_g for 16 bit signed integers
//      int32_g for 32 bit signed integers
//      int64_g for 64 bit signed integers
//
//  Macros defined:
//      CAST_LITERAL_TO_UINT64_G(c) for casting a literal constant (e.g. 36028797018963968)
//          to uint64_g.
//      CAST_LITERAL_TO_INT64_G(c) for casting a literal constant (e.g. 36028797018963968)
//          to int64_g.
//

#define LINUX 0
#define WINDOWS 1
#define MAC 2

#ifdef Q_OS_LINUX
#define OPERATING_SYSTEM LINUX
#endif

#ifdef Q_OS_WIN
#define OPERATING_SYSTEM WINDOWS
#endif

#ifdef Q_OS_DARWIN
#define OPERATING_SYSTEM MAC
#endif

#define BIT_32 0
#define BIT_64 1

#if QT_POINTER_SIZE == 4
#define IS_64_BIT BIT_32
#elif QT_POINTER_SIZE == 8
#define IS_64_BIT BIT_64
#endif

typedef qint8 int8_g;
typedef qint16 int16_g;
typedef qint32 int32_g;
typedef qint64 int64_g;

typedef quint8 uint8_g;
typedef quint16 uint16_g;
typedef quint32 uint32_g;
typedef quint64 uint64_g;

enum ENDIAN { LITTLE_END, BIG_END };

#if Q_BYTE_ORDER == Q_LITTLE_ENDIAN
#define SYSTEM_ENDIAN LITTLE_END
#elif Q_BYTE_ORDER == Q_BIG_ENDIAN
#define SYSTEM_ENDIAN BIG_END
#endif

#define CAST_LITERAL_TO_UINT64_G(c) Q_UINT64_C(c)
// Usage: uint64_g aNumber = CAST_LITERAL_TO_UINT64_G(36028797018963968);

#define CAST_LITERAL_TO_INT64_G(c) Q_INT64_C(c)
// Usage: int64_g aNumber = CAST_LITERAL_TO_INT64_G(36028797018963968);

//Alternate way of testing if system is 64 bit.
//const bool systemIs64Bit = (sizeof(void *) == 8 ? 1 : 0);
//const bool systemIs32Bit = (sizeof(void *) == 4 ? 1 : 0);

#define GRILOG_DEBUG 0
#define GRILOG_VERBOSE 1
#define GRILOG_MESSAGE 2
#define GRILOG_WARNING 3
#define GRILOG_ERROR 4

enum COMMAND_TYPE {
  SET = 0,
  GET,
  RUN_ACTION
};

enum DATA_TYPE {
  BOOL = 0,
  CHAR,
  INT,
  FLOAT,
  DOUBLE
};

struct ProcessCommand {
  COMMAND_TYPE command_type;
  DATA_TYPE data_type;
  union data {
    bool bool_val;
    char char_val;
    int int_val;
    float float_val;
    double double_val;
  } data;
  QString key;
};

class GRIProcessThread;
class GRIDataBlock;

/// GRISleep is used as a workaround to portable thread sleeping
/// To sleep, call GRISleep::msleep()
class GRISleep : public QThread {
 public:
  static void msleep(unsigned long msecs) { QThread::msleep(msecs); }
};

// For Qt's internally defined globals see http://doc.trolltech.com/4.5/qtglobal.html#details
// For definitions of macros see qglobal.h

#endif  // GRIF_FRAMEWORK_INCLUDE_CORE_GRIDEFINES_H_
