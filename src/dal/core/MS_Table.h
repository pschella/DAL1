/***************************************************************************
 *   Copyright (C) 2011                                                    *
 *   Lars Baehren (lbaehren@gmail.com)                                     *
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

#ifndef MS_TABLE_H
#define MS_TABLE_H

// Standard library header files
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>

// DAL header files
#include <core/dalObjectBase.h>
#include <core/MS_TableColumn.h>

#ifdef DAL_WITH_CASA
#include <casa/Arrays/Slicer.h>
#include <ms/MeasurementSets.h>
#include <tables/Tables/Table.h>
#include <tables/Tables/TableDesc.h>
#include <tables/Tables/TableRecord.h>
#endif

namespace DAL { // Namespace DAL -- begin
  
  /*!
    \class MS_Table
    
    \ingroup DAL
    \ingroup core
    
    \brief Interface to MeasurementSet table
    
    \author Lars B&auml;hren

    \date 2011-07-18

    \test tMS_Table.cc
    
    <h3>Prerequisite</h3>
    
    <ul type="square">
      <li>\c casa::ROArrayColumn<T> - Readonly access to an array table column
      with arbitrary data type.
      \code
      // Get the data from a particular cell in the table column
      casa::ROArrayColumn<T>::get (uInt rownr, Array<T> &array, Bool resize=False);
      // Get the data from a set of cells in the table column
      void casa::ROArrayColumn< T >::getColumnRange (const Slicer &rowRange,
                                                     Array<T> &arr,
                                                     Bool resize = False)
      \endcode
      <li>\c casa::ROScalarColumn<T> - Readonly access to a scalar table column
      with arbitrary data type.
      \code
      // Get the data from a particular cell in the table column
      casa::ROScalarColumn<T>::get (uInt rownr, T &value);
      // Get the data from a set of cells in the table column
      void casa::ROScalarColumn<T>::getColumnRange (const Slicer &rowRange,
                                                    Vector<T> &vec,
                                                    Bool resize=False)
      \endcode
      <li>\c casa::Slicer - Specify which elements to extract from an n-dimensional
      array.
      <li><a href="http://www.astron.nl/aips++/docs/notes/199">AIPS++ Note 
      199</a>: Table Query Language
    </ul>
    
    <h3>Synopsis</h3>

    The MAIN table of the MS contains the bulk of all data. For each interferometer
    and for each sample time. The ordering is usually time-baseline, i.e. the MAIN
    table is divided in subsequent time-blocks and within each time block there is
    an ordering based on the interferometer antenna pair. Each interferometer pair
    will appear only once per sample time. Auto- and crosscorrelations are usually
    mixed. The MAIN table links directly to many other tables through index numbers
    in several of its columns.

    \verbatim
    /  MAIN                        Table     Data of all samples for individual interferometers
    |-- UVW                        Column    
    |-- FLAG_CATEGORY              Column    
    |-- WEIGHT                     Column    
    |-- SIGMA                      Column    
    |-- ANTENNA1                   Column    
    |-- ANTENNA2                   Column    
    |-- ARRAY_ID                   Column    
    |-- DATA_DESC_ID               Column    
    |-- EXPOSURE                   Column    
    |
    |-- ANTENNA                    Table     Antenna information
    |   |-- ANTENNA_ID             Column
    |   |-- FEED_ID                Column
    |   |-- SPECTRAL_WINDOW_ID     Column
    |   |-- TIME                   Column
    |   |-- NUM_RECEPTORS          Column
    |   |-- BEAM_ID                Column
    |   |-- BEAM_OFFSET            Column
    |   |-- POLARIZATION_TYPE      Column
    |   |-- POL_RESPONSE           Column
    |   |-- POSITION               Column
    |   `-- RECEPTOR_ANGLE         Column
    |
    |-- DATA_DESCRIPTION           Table
    |  |-- SPECTRAL_WINDOW_ID
    |  |-- POLARIZATION_ID
    |  `-- FLAG_ROW
    |-- FEED                       Table     Feed (Frontend) related information
    |-- FLAG_CMD                   Table     Flag information
    |-- FIELD                      Table     Information on observed positions
    |-- HISTORY                    Table     History log of MS
    |-- OBSERVATION                Table     General observation information
    |-- POINTING                   Table     Antenna pointing information
    |-- POLARIZATION               Table     Polarization information description
    |   |-- NUM_CORR
    |   |-- CORR_TYPE
    |   |-- CORR_PRODUCT
    |   `-- FLAG_ROW
    |-- PROCESSOR                  Table     Correlator information
    |-- SPECTRAL_WINDOW            Table     Frequency/IF information
    |   |-- NUM_CHAN
    |   |-- NAME
    |   |-- REF_FREQUENCY
    |   |-- CHAN_FREQ
    |   |-- CHAN_WIDTH
    |   |-- MEAS_FREQ_REF
    |   |-- EFFECTIVE_BW
    |   |-- RESOLUTION
    |   |-- TOTAL_BANDWIDTH
    |   |-- NET_SIDEBAND
    |   |-- IF_CONV_CHAIN
    |   |-- FREQ_GROUP
    |   |-- FREQ_GROUP_NAME
    |   `-- FLAG_ROW
    `-- STATE
    \endverbatim
    
    <h3>Example(s)</h3>
    
  */  
  class MS_Table : public dalObjectBase {

#ifdef DAL_WITH_CASA

    //! Table object
    casa::Table itsTable;

  public:
    
    // === Construction =========================================================

    //! Default constructor
    MS_Table ();
    
    //! Argumented constructor
    MS_Table (std::string const &name,
	      IO_Mode const &flags=IO_Mode());
    
    //! Copy constructor
    MS_Table (MS_Table const &other);
    
    // === Destruction ==========================================================
    
    //! Destructor
    ~MS_Table ();
    
    // === Operators ============================================================
    
    //! Overloading of the copy operator
    MS_Table& operator= (MS_Table const &other); 
    
    // === Parameter access =====================================================
    
    /*!
      \brief Get the name of the class
      \return className -- The name of the class, MS_Table.
    */
    inline std::string className () const {
      return "MS_Table";
    }

    /*!
      \brief Provide a summary of the object's internal parameters and status
      \param showColumns -- Also show summary of the table columns?
    */
    inline void summary (bool const &showColumns=true) {
      summary (std::cout,showColumns);
    }

    //! Provide a summary of the object's internal parameters and status
    void summary (std::ostream &os,
		  bool const &showColumns=true);    

    // === Public methods =======================================================
    
    //! Open table by \e name
    bool open (std::string const &name,
	       IO_Mode const &flags=IO_Mode());
    
    //! Open sub-table by \e name
    bool open (MS_Table &table,
	       std::string const &name,
	       IO_Mode const &flags=IO_Mode());
    
    //! Test if the table is the root table
    bool isRootTable () {
      return itsTable.isRootTable();
    }

    //! Get a table description object for the table
    inline casa::TableDesc tableDescription () {
      return itsTable.tableDesc ();
    }

    //! Get the names of the table columns
    std::vector<std::string> columnNames ();

    //! Get the names of the sub-tables
    std::vector<std::string> tableNames ();

    //! Get column data-types
    std::map<casa::String,casa::DataType> columnDataTypes ();

    /*!
      \brief Read data from a table column.
      \retval data   -- Array returning the data stored inside the designated
              table \e column.
      \param column  -- Name of the \e column from which to read the \e data.
      \return status -- Status of the operation; returns \e false in case an
              error was encountered.
    */
    template <class T>
      bool readData (casa::Array<T> &data,
		     std::string const &column)
      {
	// Check if table is ok
	if (itsTable.isNull()) {
	  return false;
	}

	bool status                 = true;
	casa::TableDesc tableDesc   = itsTable.tableDesc();
	casa::ColumnDesc columnDesc = tableDesc.columnDesc(column);
	casa::IPosition cellShape   = columnDesc.shape();

	{
	  unsigned int rank = cellShape.nelements()+1;
	  casa::IPosition dataShape (rank,0);

	  for (unsigned int n=0; n<(rank-1); ++n) {
	    dataShape(n) = cellShape(n);
	  }
	  dataShape(rank-1) = itsTable.nrow();

	  data.resize(dataShape);
	}
	
	if (columnDesc.isScalar()) {
	  // Set up reader object for the column ...
	  casa::ROScalarColumn<T> columReader (itsTable, column);
	  // .... and retrieve the data
	  data = columReader.getColumn();
	} else if (columnDesc.isArray()) {
	  // Set up reader object for the column ...
	  casa::ROArrayColumn<T> columReader (itsTable, column);
	  // .... and retrieve the data
	  data = columReader.getColumn();
	} else {
	  std::cerr << "[MS_Table::readData] Unsupported type of column data!"
		    << std::endl;
	  status = false;
	}

	return status;
      }

    /*!
      \brief Read data from a selected number of rows in a table column.
      \retval data   -- Array returning the data stored inside the designated
              table \e column.
      \param column    -- Name of the \e column from which to read the \e data.
      \param selection -- Specification of elements to selected from the table
             column.
      \return status   -- Status of the operation; returns \e false in case an
              error was encountered.
    */
    template <class T>
      bool readData (casa::Array<T> &data,
		     std::string const &column,
		     casa::Slicer const &selection)
      {
	// Check if table is ok
	if (itsTable.isNull()) {
	  return false;
	}

	bool status                 = true;
	casa::TableDesc tableDesc   = itsTable.tableDesc();
	casa::ColumnDesc columnDesc = tableDesc.columnDesc(column);
	casa::IPosition cellShape   = columnDesc.shape();

	{
	  unsigned int rank = cellShape.nelements()+1;
	  casa::IPosition dataShape (rank,0);

	  for (unsigned int n=0; n<(rank-1); ++n) {
	    dataShape(n) = cellShape(n);
	  }
	  dataShape(rank-1) = selection.length()(0);

	  data.resize(dataShape);
	}
	
	if (columnDesc.isScalar()) {
	  // Set up reader object for the column ...
	  casa::ROScalarColumn<T> columReader (itsTable, column);
	  // .... and retrieve the data
	  data = columReader.getColumnRange (selection);
	} else if (columnDesc.isArray()) {
	  // Set up reader object for the column ...
	  casa::ROArrayColumn<T> columReader (itsTable, column);
	  // .... and retrieve the data
	  columReader.getColumnRange (selection, data, true);
	} else {
	  std::cerr << "[MS_Table::readData] Unsupported type of column data!"
		    << std::endl;
	  status = false;
	}

	return status;
      }

    /*!
      \brief Read data from a selected number of rows in a table column.
      \retval data   -- Array returning the data stored inside the designated
              table \e column.
      \param column  -- Name of the \e column from which to read the \e data.
      \param start   -- Index of the row from which to start reading.
      \param nofRows -- Number of rows from which to read data.
      \return status -- Status of the operation; returns \e false in case an
              error was encountered.
    */
    template <class T>
      bool readData (casa::Array<T> &data,
		     std::string const &column,
		     unsigned int const &start,
		     unsigned int const &nofRows=1)
      {
	casa::Slicer selection (casa::IPosition(1,start),
				casa::IPosition(1,nofRows),
				casa::IPosition(1,1));
	return readData (data, column, selection);
      }

    /*!
      \brief Read data from a table column.
      \retval data   -- Array returning the data stored inside the designated
              table \e column.
      \param column  -- Name of the \e column from which to read the \e data.
      \return status -- Status of the operation; returns \e false in case an
              error was encountered.
    */
    template <class T>
      bool readData (std::vector<T> &data,
		     std::string const &column)
      {
	bool status = true;
	
	// Check if table is ok
	if (itsTable.isNull()) {
	  status = false;
	} else {
	  // casa::Array into which the column data are read initially
	  casa::Array<T> buffer;
	  // read column data
	  if (readData (buffer,column)) {
	    unsigned int nelem = buffer.nelements();
	    // resize the array returning the data
	    data.resize(nelem);
	    // get the data from the buffer array
	    data.assign(buffer.data(),buffer.data()+nelem);
	  } else {
	    data.clear();
	    status = false;
	  }
	}
	
	return status;
      }
    
    /*!
      \brief Read data from a selected number of rows in a table column.
      \retval data   -- Array returning the data stored inside the designated
              table \e column.
      \param column  -- Name of the \e column from which to read the \e data.
      \param start   -- Index of the row from which to start reading.
      \param nofRows -- Number of rows from which to read data.
      \return status -- Status of the operation; returns \e false in case an
              error was encountered.
    */
    template <class T>
      bool readData (std::vector<T> &data,
		     std::string const &column,
		     unsigned int const &start,
		     unsigned int const &nofRows=1)
      {
	bool status = true;
	
	// Check if table is ok
	if (itsTable.isNull()) {
	  status = false;
	} else {
	  // casa::Array into which the column data are read initially
	  casa::Array<T> buffer;
	  // read column data
	  if (readData (buffer,column,start,nofRows)) {
	    unsigned int nelem = buffer.nelements();
	    // resize the array returning the data
	    data.resize(nelem);
	    // get the data from the buffer array
	    data.assign(buffer.data(),buffer.data()+nelem);
	  } else {
	    data.clear();
	    status = false;
	  }
	}
	
	return status;
      }
    
    // === Static methods =======================================================
    
    //! Resolve the \e name for the MS table
    static bool resolveName (std::string &absoluteName,
			     std::string const &name);
    
  private:
    
    //! Unconditional copying
    void copy (MS_Table const &other);
    
    //! Unconditional deletion 
    void destroy(void);
    
#else 

  public:
    MS_Table ();
    
#endif
    
  }; // Class MS_Table -- end
  
} // Namespace DAL -- end

#endif /* MS_TABLE_H */
  
