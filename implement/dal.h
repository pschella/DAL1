/*-------------------------------------------------------------------------*
 | $Id::                                                                 $ |
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

/*!
  \file dal.h

  \brief Common header file for the Data Access Library (DAL)

  \defgroup DAL Data Access Library (DAL)

  \author Joseph Masters, Lars B&auml;hren

  <h3>Synopsis</h3>

  The Data Access Library (DAL) is a library that abstracts the underlying
  file format from the user when working with radio astronomy data.  Underlying
  formats may include HDF5, AIPS++/CASA tables, FITS or specific raw formats.
  This library is available to both the C/C++ developer and the Python user via
  the pydal module.

  <h3>Related pages</h3>

  - \ref dal_coding

*/

#ifndef DAL_H
#define DAL_H

#ifndef BEAMFORMED_H
#include "BeamFormed.h"
#endif

#ifndef DALDATASET_H
#include "dalDataset.h"
#endif

#ifdef WITH_MYSQL
#ifndef DATABASE_H
#include "Database.h"
#endif
#endif

#endif // DAL_H
