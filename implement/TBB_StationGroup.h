/*-------------------------------------------------------------------------*
 | $Id::                                                                 $ |
 *-------------------------------------------------------------------------*
 ***************************************************************************
 *   Copyright (C) 2007                                                    *
 *   Lars B"ahren (lbaehren@gmail.com)                                     *
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

#ifndef TBB_STATIONGROUP_H
#define TBB_STATIONGROUP_H

// Standard library header files
#include <iostream>
#include <string>

#include <casa/aips.h>
#include <casa/Arrays/Matrix.h>
#include <casa/Arrays/Vector.h>
#include <casa/Containers/Record.h>
#include <measures/Measures/MDirection.h>
#include <measures/Measures/MPosition.h>

#ifndef TBB_DIPOLEDATASET_H
#include <TBB_DipoleDataset.h>
#endif

namespace DAL { // Namespace DAL -- begin
  
  /*!
    \class TBB_StationGroup
    
    \ingroup DAL
    
    \brief Container for the data in the StationGroup of LOFAR times-series data
    
    \author Lars B&auml;hren
    
    \date 2007/12/10
    
    \test tTBB_StationGroup.cpp
    
    <h3>Prerequisite</h3>
    
    <ul type="square">
      <li>Definition of the LOFAR time-series data format
      <li>DAL::TBB_Timeseries
      <li>DAL::TBB_DipoleDataset
      <li>CR::LOFAR_TBB -- Interface between Data Access Library (DAL) and
      CR::DataReader framework
    </ul>
    
    <h3>Synopsis</h3>
    
    <ol>
      <li>Structure of the HDF5 group inside the time-series data format:
      \verbatim
      /
      |-- Station001                    ... Group
      |   |-- TRIGGER_TYPE              ... Attribute       ... string
      |   |-- TRIGGER_OFFSET            ... Attribute       ... double
      |   |-- TRIGGERED_ANTENNAS        ... Attribute       ... array<int,1>
      |   |-- BEAM_DIRECTION_VALUE      ... Attribute       ... array<double,1>
      |   |-- BEAM_DIRECTION_UNIT       ... Attribute       ... string
      |   |-- BEAM_DIRECTION_FRAME      ... Attribute       ... string
      |   |-- STATION_POSITION_VALUE    ... Attribute       ... array<double,1>
      |   |-- STATION_POSITION_UNIT     ... Attribute       ... string
      |   |-- STATION_POSITION_FRAME    ... Attribute       ... string
      |   |-- 001000000                 ... Dataset         ... array<uint,1>
      |   |   `-- 
      |   |-- 001000001                 ... Dataset         ... array<uint,1>
      |   |   `-- 
      \endverbatim
    </ol>

    <h3>Example(s)</h3>

    <ol>
      <li>Object construction using filename and full name/path of the group
      within the file:
      \code
      std::string filename = "data.h5";
      std::string group    = "Station001";

      TBB_StationGroup group (filename,
                                group);
      \endcode
      <li>Retrieve the list of channels IDs contained within this group:
      \code
      std::string <std::vector> channelID = group.channelID();
      \endcode
      which will return something along the lines of
      \verbatim
      [ 001000000 001000001 .. 001002003 ]
      \endverbatim
      <li>Retrieve a block of samples from the data channels (i.e. dipoles) 
      belonging to this station:
      \code
      // TBB_StationGroup constructed somewhere above...
      
      // set sample at which to start reading
      int start      = 0;
      // set the number of samples to read per data channel (dipole)
      int nofSamples = 1024;

      casa::Matrix<short> data = group.fx (start,
                                           nofSamples);
      \endcode
    </ol>
    
  */
  class TBB_StationGroup {
    
    //! Identifier for this group within the HDF5 file
    hid_t groupID_p;
    //! Datasets contained within this group
    std::vector<TBB_DipoleDataset> datasets_p;
    
  public:
    
    // ==========================================================================
    //
    //  Construction / Destruction
    //
    // ==========================================================================
    
    /*!
      \brief Default constructor
      
      Default constructor does not connect to a file, but simply sets up internal
      parameters.
    */
    TBB_StationGroup ();
    
    //! Argumented constructor
    TBB_StationGroup (std::string const &filename,
		      std::string const &group);
    
    //! Argumented constructor
    TBB_StationGroup (hid_t const &location,
		      std::string const &group);
    
    //! Argumented constructor
    TBB_StationGroup (hid_t const &group_id);
    
    //! Copy constructor
    TBB_StationGroup (TBB_StationGroup const &other);
    
    //! Destructor
    ~TBB_StationGroup ();
    
    // ==========================================================================
    //
    //  Operators
    //
    // ==========================================================================
    
    /*!
      \brief Overloading of the copy operator
      
      \param other -- Another TBB_StationGroup object from which to make a
             copy.
    */
    TBB_StationGroup& operator= (TBB_StationGroup const &other); 
    
    // ==========================================================================
    //
    //  Parameter access - station group
    //
    // ==========================================================================
    
    /*!
      \brief Get the identifier for this group within the HDF5 file
      
      \return groupID -- The identifier for this group within the HDF5 file
    */
    inline hid_t group_id () const { return groupID_p; }

    /*!
      \brief Get the name for this group within the HDF5 file

      \param stripPath -- Strip away the path and just return the name of the 
             group? By default the full path is returned, so make sure to set
	     <tt>stripPath=true</tt> if you want the name only.
      
      \return name -- The name for this group within the HDF5 file
    */
    std::string group_name (bool const &stripPath=false);
    
    /*!
      \brief Get the trigger type which cause recording this data

      \return trigger_type -- The trigger type which cause recording this data
    */
    std::string trigger_type ();

    /*!
      \brief Get the trigger offset

      \return trigger_offset -- The trigger offset.
    */
    double trigger_offset ();
    
    /*!
      \brief Get the list of triggered antennas
      
      \return triggered_antennas -- List of antennas, for which the trigger
              condition was fullfilled.
    */
#ifdef HAVE_CASA
    casa::Vector<uint> triggered_antennas ();
#else 
    std::vector<uint> triggered_antennas ();
#endif

    /*!
      \brief Get the numerical values of the beam direction

      \return value -- The numerical value for the direction of the station
              beam.
    */
#ifdef HAVE_CASA
    casa::Vector<double> beam_direction_value ();
#else 
    std::vector<double> beam_direction_value ();
#endif
    
    /*!
      \brief Get the physical units for the beam direction
      
      \return unit -- The physical units within which the beam direction
              is given; this might be <i>radian</i> or <i>degree</i>
    */
    std::string beam_direction_unit ();

    /*!
      \brief Get the coordinate frame identifier for the beam direction
      
      \return frame -- 
    */
    std::string beam_direction_frame ();

    /*!
      \brief Get the direction of the station beam as CASA Measure

      \return direction -- The direction of the station beam.
    */
    casa::MDirection beam_direction ();

    /*!
      \brief Get the numerical values of the station position
    */
    casa::Vector<double> station_position_value ();

    /*!
      \brief Get the physical units for the station position
    */
    std::string station_position_unit ();
    std::string station_position_frame ();
    casa::MPosition station_position ();

    /*!
      \brief Get the name of the class
      
      \return className -- The name of the class, TBB_StationGroup.
    */
    std::string className () const { return "TBB_StationGroup"; }

    /*!
      \brief Provide a summary of the internal status
    */
    inline void summary () { summary (std::cout); }

    /*!
      \brief Provide a summary of the internal status
    */
    void summary (std::ostream &os);    

    // ------------------------------------------------------------------ Methods

    /*!
      \brief Get the number of dipole datasets within this station group
      
      \return nofDipoleDatasets -- The number of dipole datasets contained with
              this station group.
    */
    inline uint nofDipoleDatasets () { return datasets_p.size(); }

    // ==========================================================================
    //
    //  Parameter access - dipole dataset
    //
    // ==========================================================================
    
    /*!
      \brief Retrieve the station IDs from the antenna datasets within this group
      
      \return stationIDs -- The station IDs from the antenna datasets within this
              group
    */
#ifdef HAVE_CASA
    casa::Vector<uint> station_id ();
#else
    std::vector<uint> station_id ();
#endif
    
    /*!
      \brief Retrieve the RSP IDs from the antenna datasets within this group

      \return rspIDs -- The RSP IDs from the antenna datasets within this
              group
    */
#ifdef HAVE_CASA
    casa::Vector<uint> rsp_id ();
#else
    std::vector<uint> rsp_id ();
#endif
    
    /*!
      \brief Retrieve the RCU IDs from the antenna datasets within this group

      \return rcuIDs -- The RCU IDs from the antenna datasets within this
              group
    */
#ifdef HAVE_CASA
    casa::Vector<uint> rcu_id ();
#else
    std::vector<uint> rcu_id ();
#endif
    
#ifdef HAVE_CASA
    casa::Vector<double> sample_frequency_value ();
#else
    std::vector<double> sample_frequency_value ();
#endif

#ifdef HAVE_CASA
    casa::Vector<std::string> sample_frequency_unit ();
#else
    std::vector<std::string> sample_frequency_unit ();
#endif

#ifdef HAVE_CASA
    casa::Vector<casa::MFrequency> sample_frequency ();
#endif
    
    /*!
      \brief Get the Nyquist zone in which the ADC is performed
      
      \return zone -- The Nyquist zone in which the analog-to-digital conversion
              (ADC) is performed
    */
#ifdef HAVE_CASA
    casa::Vector<uint> nyquist_zone ();
#else
    std::vector<uint> nyquist_zone ();
#endif
    
    /*!
      \brief Get the values of TIME for all present datasets
      
      \return times -- Values of the TIME attribute for all datasets present in
              this station group
    */
#ifdef HAVE_CASA
    casa::Vector<uint> time ();
#else
    std::vector<uint> time ();
#endif

#ifdef HAVE_CASA
    casa::Vector<uint> sample_number ();
#else
    std::vector<uint> sample_number ();
#endif

#ifdef HAVE_CASA
    casa::Vector<uint> samples_per_frame ();
#else
    std::vector<uint> samples_per_frame ();
#endif

    /*!
      \brief Get the values of DATA_LENGTH for all present datasets
      
      \return data_lengths -- Values of the DATA_LENGTH attribute for all
              datasets present in this station group
    */
#ifdef HAVE_CASA
    casa::Vector<uint> data_length ();
#else
    std::vector<uint> data_length ();
#endif

    //! Get the type of the feeds.
#ifdef HAVE_CASA
    casa::Vector<std::string> feed ();
#else
    std::vector<std::string> feed ();
#endif

    //! Get the numerical values of the antenna positions within this station.    
#ifdef HAVE_CASA
    casa::Matrix<double> antenna_position_value ();
#endif

    //! Get the physical units for the antenna positions within this station.
#ifdef HAVE_CASA
    casa::Matrix<std::string> antenna_position_unit ();
#endif

    //! Get the positions of the antennas within this station as Measure.
#ifdef HAVE_CASA
    casa::Vector<casa::MPosition> antenna_position ();
#endif
    
    /*!
      \brief Retrieve the list of channels IDs contained within this group

      \return channelID -- A list of the channel IDs for all the dipoles within
              this LOFAR station.
    */
#ifdef HAVE_CASA
    casa::Vector<int> channelID ();
#else
    std::vector<int> channelID ();
#endif
    
    /*!
      \brief Retrieve the list of channels names contained within this group

      \return channelNames -- A list of the channel names for all the dipoles
              within this LOFAR station.
    */
#ifdef HAVE_CASA
    casa::Vector<casa::String> channelNames ();
#else
    std::vector<std::string> channelNames ();
#endif

    /*!
      \brief Convert individual ID number to joint unique ID

      \param station_id -- Identification number of the LOFAR station within the
             complete array.
      \param rsp_id -- Identification number of a Remote Station Processing
             (RSP) board within a given LOFAR station.
      \param rcu_id -- Identification number of a Receiver Unit (RCU) attached to
             a given RSP board.

      \return channel_id -- Unique identifier string for an individual dipole 
              within the full LOFAR array.
    */
    inline std::string channelID (unsigned int const &station_id,
				  unsigned int const &rsp_id,
				  unsigned int const &rcu_id) {
      char uid[10];
      sprintf(uid, "%03d%03d%03d",
	      station_id,rsp_id,rcu_id);
      std::string channel_id (uid);
      return channel_id;
    }

    /*!
      \brief Get identifiers to the datasets within the station group

      \return dataset_ids -- Vector with a list of the identifiers to the
              HDF5 dataset objects within this station group.
    */
#ifdef HAVE_CASA
    casa::Vector<hid_t> datasetIDs ();
#else
    std::vector<hid_t> datasetIDs ();
#endif

    /*!
      \brief Get the positions of all antennas in the station

      \return positions --
    */
    casa::Vector<casa::MPosition> antennaPositions ();
    
    /*!
      \brief Retrieve a block of ADC values for the dipoles in this station

      \param start      -- Number of the sample at which to start reading
      \param nofSamples -- Number of samples to read, starting from the position
             given by <tt>start</tt>.

      \return fx -- [nofSamples,dipole] Array of raw ADC samples representing
              the electric field strength as function of time.
     */
    casa::Matrix<double> fx (int const &start=0,
			     int const &nofSamples=1);
    
    /*!
      \brief Retrieve a block of ADC values for the dipoles in this station

      \param start      -- Number of the sample at which to start reading
      \param nofSamples -- Number of samples to read, starting from the position
             given by <tt>start</tt>.
      \param dipoleSelection -- Selection of dipoles, for which to retrieve the
             data.

      \return fx -- [nofSamples,dipole] Vector of raw ADC samples representing
              the electric field strength as function of time.
    */
    casa::Matrix<double> fx (int const &start,
			     int const &nofSamples,
			     std::vector<uint> const &dipoleSelection);
    
    //! Get a casa::Record containing the values of the attributes
    casa::Record attributes2record (bool const &addRecursive=false);
    
  private:
    
    //! Initialize the internal dataspace
    void init ();
    
    /*!
      \brief Initialize the internal dataspace

      \param dataset_id -- Identifier for the group within the HDF5 file
     */
    void init (hid_t const &group_id);
    
    /*!
      \brief Initialize the internal dataspace

      \param filename -- HDF5 file within which the dataset in question is
             contained
      \param group    -- Name of the group which this object is to encapsulate.
     */
    void init (std::string const &filename,
	       std::string const &group);
    
    /*!
      \brief Initialize the internal dataspace

      \param location -- Location below which the group is found within the
             file.
      \param group  -- Name of the group which this object is to encapsulate.
     */
    void init (hid_t const &location,
	       std::string const &group);
    
    /*!
      \brief Unconditional copying

      \param other -- Another TBB_StationGroup object from which to create
             this new one.
    */
    void copy (TBB_StationGroup const &other);
    
    /*!
      \brief Unconditional deletion 
    */
    void destroy(void);
    
    /*!
      \brief Set up the list of dipole datasets contained within this group

      Essentially there are two ways by which to construct the list of dipole
      datasets contained within this station group:
      <ol>
        <li>Test if the possible names for the datasets belong to a valid 
	object.
      </ol>

      \return status -- Status of the operation; returns <tt>false</tt> in case
              an error was encountered.
    */
    bool setDipoleDatasets ();

  };
  
} // Namespace DAL -- end

#endif /* TBB_STATIONGROUP_H */
  
