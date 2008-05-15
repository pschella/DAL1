/*-------------------------------------------------------------------------*
 | $Id:: dal.h 1126 2007-12-10 17:14:20Z masters                         $ |
 *-------------------------------------------------------------------------*
 ***************************************************************************
 *   Copyright (C) 2007 by Joseph Masters                                  *
 *   jmasters@science.uva.nl                                               *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef TBB_H
#define TBB_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#ifndef DAL_H
#include <dal.h>
#endif

#ifndef DALDATASET_H
#include <dalDataset.h>
#endif

// socket headers
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

namespace DAL {

/*!
   \class TBB

   \ingroup DAL

   \brief High-level interface between TBB data and the DAL

   \author Joseph Masters
 */

  class TBB {

  string name;
  dalDataset * dataset;
  std::vector<dalGroup> station;

  public:

  TBB( string const& name );  // constructor

  int connectsocket( char* ipaddress, char* portnumber );

  }; // class TBB


  ///////////////////////////////////////////////////////////////
  //
  // constant values and structures
  //
  ///////////////////////////////////////////////////////////////

  const Int32 ETHEREAL_HEADER_LENGTH = 46;
  const Int32 FIRST_EXTRA_HDR_LENGTH = 40;
  const Int32 EXTRA_HDR_LENGTH = 16;

  typedef struct AntennaStruct {
    unsigned int frameno;
    unsigned int rsp_id;
    unsigned int rcu_id;
    unsigned int time;
    unsigned int sample_nr;
    unsigned int samples_per_frame;
    char feed[16];
    double ant_position[ 3 ];
    double ant_orientation[ 3 ];
    hvl_t data[1];
  };

  typedef struct writebuffer {
    AntennaStruct antenna;
  };

  typedef struct TBB_Header {
    unsigned char stationid;
    unsigned char rspid;
    unsigned char rcuid;
    unsigned char sample_freq;
    UInt32 seqnr;
    Int32 time;
    UInt32 sample_nr;
    UInt16 n_samples_per_frame;
    UInt16 n_freq_bands;
    char bandsel[64];
    Int16 spare;
    UInt16 crc;
  };

  typedef struct TransientSample {
    Int16 value;
  };

  typedef struct SpectralSample {
    complex<Int16> value;
  };

  typedef struct CosmicRayStruct {
    //int nofDatapoints;
    Int16 data;
  };

} // DAL namespace


#endif // TBB_H