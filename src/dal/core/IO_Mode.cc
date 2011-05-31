/***************************************************************************
 *   Copyright (C) 2010                                                    *
 *   Lars B"ahren (bahren@astron.nl)                                       *
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

#include <core/IO_Mode.h>

namespace DAL { // Namespace DAL -- begin
  
  // ============================================================================
  //
  //  Construction
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                                      IO_Mode
  
  IO_Mode::IO_Mode ()
  {
    init();
  }

  //_____________________________________________________________________________
  //                                                                      IO_Mode
  
  IO_Mode::IO_Mode (IO_Mode::Flags const &flag)
  {
    setFlag (flag);
  }
  
  //_____________________________________________________________________________
  //                                                                      IO_Mode
  
  IO_Mode::IO_Mode (int const &flags)
  {
    setFlags (flags);
  }

  //_____________________________________________________________________________
  //                                                                      IO_Mode
  
  /*!
    \param other -- Another HDF5Property object from which to create this new
           one.
  */
  IO_Mode::IO_Mode (IO_Mode const &other)
  {
    copy (other);
  }
  
  // ============================================================================
  //
  //  Destruction
  //
  // ============================================================================
  
  IO_Mode::~IO_Mode ()
  {
    destroy();
  }
  
  void IO_Mode::destroy ()
  {;}
  
  // ============================================================================
  //
  //  Operators
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                                    operator=
  
  IO_Mode& IO_Mode::operator= (IO_Mode const &other)
  {
    if (this != &other) {
      destroy ();
      copy (other);
    }
    return *this;
  }
  
  //_____________________________________________________________________________
  //                                                                         copy
  
  void IO_Mode::copy (IO_Mode const &other)
  {
    itsFlags = other.itsFlags;
  }

  // ============================================================================
  //
  //  Parameters
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                                      summary
  
  /*!
    \param os -- Output stream to which the summary is written.
  */
  void IO_Mode::summary (std::ostream &os)
  {
    std::vector<std::string> names = flagDescriptions();
    unsigned int nofNames          = names.size();

    os << "[IO_Mode] Summary of internal parameters." << std::endl;
    os << "-- I/O mode value    = " << itsFlags << std::endl;

    os << "-- Flag descriptions = [";
    for (unsigned int n=0; n<nofNames; ++n) {
      os << " " << names[n];
    }
    os << " ]" << std::endl;
  }
  
  // ============================================================================
  //
  //  Public methods
  //
  // ============================================================================

  /*!
    \param flag      -- Flag to check for.
    \return haveFlag -- Returns \e true in case the \c flag is part of the I/O
            mode settings, \e false in case it is not.
  */
  bool IO_Mode::haveFlag (IO_Mode::Flags const &which)
  {
    IO_Mode::Flags in = which;
    int tmp           = itsFlags;

    if ( tmp & in ) {
      return true;
    } else {
      return false;
    } 
  }
  
  //_____________________________________________________________________________
  //                                                             flagDescriptions
  
  std::vector<std::string> IO_Mode::flagDescriptions ()
  {
    return flagDescriptions (itsFlags);
  }
  
  //_____________________________________________________________________________
  //                                                                      addFlag
  
  /*!
    \param flag    -- 
    \return status -- 
  */
  bool IO_Mode::addFlag (IO_Mode::Flags const &flag)
  {
    itsFlags = itsFlags | flag;
    return verifyFlags (itsFlags);
  }

  //_____________________________________________________________________________
  //                                                                   removeFlag
  
  bool IO_Mode::removeFlag (IO_Mode::Flags const &flag)
  {
    bool status = true;

    /* Remove flag and verify that the resulting settings are ok */
    itsFlags = itsFlags & (~flag);
    status   = verifyFlags (itsFlags);
    /* If settings are not ok, revert previous action */
    if (!status) {
      status = addFlag (flag);
    }

    return status;
  }

  //_____________________________________________________________________________
  //                                                                   resetFlags
  
  bool IO_Mode::resetFlags ()
  {
    init();
    return true;
  }

  // ============================================================================
  //
  //  Static methods
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                                     flagsMap

  /*!
    \return flags -- 
   */
  std::map<IO_Mode::Flags,std::string> IO_Mode::flagsMap ()
  {
    std::map<IO_Mode::Flags,std::string> flags;

    flags[IO_Mode::Create]       = "Create";
    flags[IO_Mode::CreateNew]    = "CreateNew";
    flags[IO_Mode::Open]         = "Open";
    flags[IO_Mode::OpenOrCreate] = "OpenOrCreate";
    flags[IO_Mode::Truncate]     = "Truncate";

    flags[IO_Mode::ReadOnly]     = "ReadOnly";
    flags[IO_Mode::WriteOnly]    = "WriteOnly";
    flags[IO_Mode::ReadWrite]    = "ReadWrite";

    return flags;
  }

  //_____________________________________________________________________________
  //                                                                    flagsType

  std::vector<IO_Mode::Flags> IO_Mode::flagsType ()
  {
    std::map<IO_Mode::Flags,std::string> flags = flagsMap();
    std::map<IO_Mode::Flags,std::string>::iterator it;
    std::vector<IO_Mode::Flags> types;

    for (it=flags.begin(); it!=flags.end(); ++it) {
      types.push_back(it->first);
    } 

    return types;
  }

  //_____________________________________________________________________________
  //                                                                    flagsName

  std::vector<std::string> IO_Mode::flagsName ()
  {
    std::map<IO_Mode::Flags,std::string> flags = flagsMap();
    std::map<IO_Mode::Flags,std::string>::iterator it;
    std::vector<std::string> names;

    for (it=flags.begin(); it!=flags.end(); ++it) {
      names.push_back(it->second);
    } 

    return names;
  }

  //_____________________________________________________________________________
  //                                                             flagDescriptions

  std::vector<std::string> IO_Mode::flagDescriptions (IO_Mode::Flags const &flag)
  {
    return flagDescriptions(flag);
  }

  //_____________________________________________________________________________
  //                                                             flagDescriptions

  /*!
    \param flags  -- 
    \return names -- 
  */
  std::vector<std::string> IO_Mode::flagDescriptions (int const &flags)
  {
    std::map<IO_Mode::Flags,std::string> m = flagsMap();
    std::map<IO_Mode::Flags,std::string>::iterator it;
    std::vector<std::string> names;

    for (it=m.begin(); it!=m.end(); ++it) {
      /* Check if the flags is set */
      if ( (flags & it->first) == it->first ) {
	names.push_back(it->second);
      }
    }

    return names;
  }
  
  //_____________________________________________________________________________
  //                                                                  verifyFlags

  /*!
    Attempt to verify a given combination of I/O mode flags. While obviously
    there is no 100% secure method to perform this in a completely generic
    fashion, we at least try to cover the most basic setting conflicts -- e.g.
    \c ReadOnly and \c WriteOnly are mutually exclusive.

    \param flags        -- I/O mode settings to be checked.
    \param correctFlags -- Attempt to correct the flags? If set to \c false, only
           reporting will be done, in case a potential conflict has been
	   disovered, but the provided \c flags will not be altered.
    \return status      -- Status of the operation; returns \c false in case an
            error was encountered, i.e. the provided combination of flags is 
	    considered to be incorrect.
  */
  bool IO_Mode::verifyFlags (int &flags,
			     bool const &correctFlags)
  {
    bool status = true;

    /*______________________________________________________
      Check settings containing 'ReadOnly'
    */
    
    if ( (flags & IO_Mode::ReadOnly) == IO_Mode::ReadOnly ) {
      
      if ( (flags & IO_Mode::WriteOnly) == IO_Mode::WriteOnly ) {
	std::cerr << "[IO_Mode::verifyFlags]"
		  << " Can't be ReadOnly and WriteOnly at the same time!"
		  << std::endl;
	status = false;
	if (correctFlags) {
	  flags = flags & (~IO_Mode::WriteOnly);
	}
      }
      
      if ( (flags & IO_Mode::ReadWrite) == IO_Mode::ReadWrite ) {
	std::cerr << "[IO_Mode::verifyFlags]"
		  << " Can't be ReadOnly and ReadWrite at the same time!"
		  << std::endl;
	status = false;
	if (correctFlags) {
	  flags = flags & (~IO_Mode::ReadWrite);
	}
      }
      
    }
    
    return status;
  }

  //_____________________________________________________________________________
  //                                                                flagH5Fcreate

#ifdef DAL_WITH_HDF5
  hid_t IO_Mode::flagH5Fcreate (int const &flags)
  {
    int tmp = flags;

    if ( (tmp & IO_Mode::CreateNew) == IO_Mode::CreateNew ) {
      return H5F_ACC_EXCL;
    } else if ( (tmp & IO_Mode::Create) == IO_Mode::Create ) {
      return H5F_ACC_TRUNC;
    } else if ( (tmp & IO_Mode::Truncate) == IO_Mode::Truncate ) {
      return H5F_ACC_TRUNC;
    } else {
      return H5F_ACC_EXCL;
    }
    
  }
#endif
  
} // Namespace DAL -- end