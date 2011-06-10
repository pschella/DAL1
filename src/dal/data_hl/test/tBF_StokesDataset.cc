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

#include <data_hl/BF_StokesDataset.h>

// Namespace usage
using std::cerr;
using std::cout;
using std::endl;
using DAL::BF_StokesDataset;

/*!
  \file tBF_StokesDataset.cc

  \ingroup DAL
  \ingroup data_hl

  \brief A collection of test routines for the DAL::BF_StokesDataset class
 
  \author Lars B&auml;hren
 
  \date 2010/12/05

  The generated HDF5 file will have the following structure:
  \verbatim
  tBF_StokesDataset.h5
  \endverbatim
*/

//_______________________________________________________________________________
//                                                              test_constructors

/*!
  \brief Test constructors for a new BF_StokesDataset object

  \param fileID -- Object identifier for the HDF5 file to work with

  \return nofFailedTests -- The number of failed tests encountered within this
          function.
*/
int test_constructors (hid_t const &fileID)
{
  cout << "\n[tBF_StokesDataset::test_constructors]" << endl;

  int nofFailedTests       = 0;
  unsigned int rank        = 2;     /* Rank of the Stokes dataset           */
  unsigned int index       = 0;     /* Index number of the Stokes dataset   */
  unsigned int nofSamples  = 1000;  /* nof. samples along the time axis     */
  unsigned int nofSubbands = 36;    /* nof. frequency sub-bands             */
  unsigned int nofChannels = 128;   /* nof. frequency channels per sub-band */
  std::vector<hsize_t> shape (rank);
  std::string nameDataset;

  shape[0] = nofSamples;
  shape[1] = nofSubbands*nofChannels;
  
  /*_______________________________________________________________________
    Test 1: Default constructor (no dataset created)
  */
  
  cout << "\n[1] Testing BF_StokesDataset() ..." << endl;
  try {
    BF_StokesDataset stokes;
    //
    stokes.summary(); 
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }
  
  /*_______________________________________________________________________
    Test 2: Argumented constructor to open existing dataset.
  */
  
  cout << "\n[2] Testing BF_StokesDataset(hid_t, string) ..." << endl;
  try {
    index       = 0;
    nameDataset = BF_StokesDataset::getName(index);
    BF_StokesDataset stokes (fileID, nameDataset);
    stokes.summary(); 
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }
  
  /*_______________________________________________________________________
    Test 3: Argumented constructor to open existing dataset.
  */
  
  cout << "\n[3] Testing BF_StokesDataset(hid_t, uint) ..." << endl;
  try {
    index       = 0;
    BF_StokesDataset stokes (fileID, index);
    stokes.summary(); 
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }
  
  /*_______________________________________________________________________
    Test 4: Argumented constructor, containing minimal required parameters
            for creation of a new dataset.
  */

  cout << "\n[4] Testing BF_StokesDataset(hid_t, string, vector<hsize_t>) ..."
	    << endl;
  try {
    index = 4;
    BF_StokesDataset stokes (fileID, index, shape);
    //
    stokes.summary(); 
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }

  /*_______________________________________________________________________
    Test 5: Argumented constructor.
  */

  cout << "\n[5] Testing BF_StokesDataset(hid_t, string, vector<hsize_t>, Stokes::Component) ..."
	    << endl;
  try {
    index = 5;
    BF_StokesDataset stokes (fileID,
			     index,
			     shape,
			     DAL::Stokes::Q);
    //
    stokes.summary(); 
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }
  
  /*_______________________________________________________________________
    Test 6: Argumented constructor providing individual values for the
            number of bins along the time axis, the number of frequency
	    bands and the number of frequency channels per frequency band.
  */

  cout << "\n[6] Testing BF_StokesDataset(hid_t,string,uint,uint,uint,Stokes::Component) ..." << endl;
  try {
    index = 6;
    BF_StokesDataset stokes (fileID,
			     index,
			     nofSamples,
			     nofSubbands,
			     nofChannels,
			     DAL::Stokes::Q);
    //
    stokes.summary(); 
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }

  /*_______________________________________________________________________
    Test 7: Argumented constructor providing individual values for the
            number of bins along the time axis, the number of frequency
	    bands and the number of frequency channels per frequency band.
  */

  cout << "\n[7] Testing BF_StokesDataset(hid_t,string,uint,vector<uint>,Stokes::Component) ..." << endl;
  try {
    std::vector<unsigned int> channels (nofSubbands);
    index       = 7;
    nameDataset = BF_StokesDataset::getName(index);

    /* Assign number of channels per sub-band */
    for (unsigned int n=0; n<nofSubbands; ++n) {
      channels[n] = 10+n;
    }

    /* Create object */
    BF_StokesDataset stokes (fileID,
			     index,
			     nofSamples,
			     channels,
			     DAL::Stokes::Q);
    //
    stokes.summary(); 
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }

  /*_______________________________________________________________________
    Test 8: Copy constructor
  */

  cout << "[8] Testing BF_StokesDataset(BF_StokesDataset) ..." << endl;
  try {
    index = 3;
    // Create the first of the two objects ...
    BF_StokesDataset stokes (fileID, index);
    // ... and provide a summary
    stokes.summary(); 
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }
  
  return nofFailedTests;
}

//_______________________________________________________________________________
//                                                                test_attributes

/*!
  \brief Test access to the attributes

  \param fileID -- Object identifier for the HDF5 file to work with

  \return nofFailedTests -- The number of failed tests encountered within this
          function.
*/
int test_attributes (hid_t const &fileID)
{
  cout << "\n[tBF_StokesDataset::test_attributes]\n" << endl;

  int nofFailedTests      = 0;
  std::string nameDataset = "Stokes005";

  /* Open dataset to work with */
  BF_StokesDataset stokes (fileID, nameDataset);
  stokes.summary();

  /* Variable for reading in attributes */
  std::vector<int> nofSubbands;
  std::vector<int> nofChannels;
  std::string groupType;
  std::string dataType;
  std::string stokesComponent;

  /*__________________________________________________________________
    Test 1: Read in the attribute values
  */
  
  cout << "[1] Testing getAttribute(string, T) ..." << endl;
  try {
    // stokes.getAttribute ("GROUPTYPE",        groupType);
    // stokes.getAttribute ("DATATYPE",         dataType);
    // stokes.getAttribute ("NOF_CHANNELS",     nofChannels);
    // stokes.getAttribute ("NOF_SUBBANDS",     nofSubbands);
    // stokes.getAttribute ("STOKES_COMPONENT", stokesComponent);
    
    cout << "-- GROUPTYPE        = " << groupType       << endl;
    cout << "-- DATATYPE         = " << dataType        << endl;
    cout << "-- NOF_CHANNELS     = " << nofChannels     << endl;
    cout << "-- NOF_SUBBANDS     = " << nofSubbands     << endl;
    cout << "-- STOKES_COMPONENT = " << stokesComponent << endl;
  } catch (std::string message) {
    std::cerr << message << endl;
    ++nofFailedTests;
  }

  return nofFailedTests;
}

//_______________________________________________________________________________
//                                                                      test_data

/*!
  \brief Test read/write access to the data

  \param fileID -- Object identifier for the HDF5 file to work with

  \return nofFailedTests -- The number of failed tests encountered within this
          function.
*/
int test_data (hid_t const &fileID)
{
  cout << "\n[tBF_StokesDataset::test_data]\n" << endl;

  int nofFailedTests (0);

  //________________________________________________________
  // Create new dataset to work with

  cout << "--> Create new dataset to work with ..." << endl;

  unsigned int index = 0;
  std::vector<hsize_t> shape (2);
  std::vector<int> start (2,0);
  std::vector<int> stride;
  std::vector<int> count;
  std::vector<int> block (2,0);
  int nofSteps;
  unsigned int nofDatapoints;

  shape[0] = 100;
  shape[1] = 2048;

  //________________________________________________________
  // Test 1

  cout << "[1] Test writing single rows to dataset ..." << endl;
  try {
    index         = 101;
    nofSteps      = shape[0];
    block[0]      = shape[0]/nofSteps;
    block[1]      = shape[1];
    nofDatapoints = DAL::HDF5Hyperslab::nofDatapoints (count,block);
    float *data   = new float [nofDatapoints];

    cout << "-- Shape        = " << shape    << endl;
    cout << "-- nof. steps   = " << nofSteps << endl;
    cout << "-- block        = " << block    << endl;

    BF_StokesDataset stokes (fileID,
			     index,
			     shape,
			     DAL::Stokes::U);
    
    for (int step(0); step<nofSteps; ++step) {
      // set position marker
      start[0] = step*block[0];
      // update data array values
      for (unsigned int n(0); n<nofDatapoints; ++n) {
	data[n] = step;
      }
      // write data to dataset
      stokes.writeData (data, start, block);
    }
    
    delete [] data;
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }
  
  //________________________________________________________
  // Test 2

  cout << "[2] Test writing multiple rows to dataset ..." << endl;
  try {
    index         = 102;
    nofSteps      = 20;
    start[1]      = 0;
    block[0]      = shape[0]/nofSteps;
    block[1]      = shape[1];
    nofDatapoints = DAL::HDF5Hyperslab::nofDatapoints (count,block);
    float *data   = new float [nofDatapoints];

    cout << "-- Shape        = " << shape    << endl;
    cout << "-- nof. steps   = " << nofSteps << endl;
    cout << "-- block        = " << block    << endl;

    BF_StokesDataset stokes (fileID,
			     index,
			     shape,
			     DAL::Stokes::U);
    
    for (int step(0); step<nofSteps; ++step) {
      // set position marker
      start[0] = step*block[0];
      // update data array values
      for (unsigned int n(0); n<nofDatapoints; ++n) {
	data[n] = step;
      }
      // feedback
      cout << "-> writing datablock " << step << "/" << nofSteps
	   << " starting from " << start
	   << " ..." << endl;
      // write data to dataset
      stokes.writeData (data, start, block);
    }

    delete [] data;
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }

  //________________________________________________________
  // Test 3

  cout << "[3] Test writing single columns to dataset ..." << endl;
  try {
    index         = 103;
    nofSteps      = shape[1];
    start[0]      = 0;
    block[0]      = shape[0];
    block[1]      = shape[1]/nofSteps;
    nofDatapoints = DAL::HDF5Hyperslab::nofDatapoints (count,block);
    float *data   = new float [nofDatapoints];

    cout << "-- Shape        = " << shape    << endl;
    cout << "-- nof. steps   = " << nofSteps << endl;
    cout << "-- block        = " << block    << endl;

    BF_StokesDataset stokes (fileID,
			     index,
			     shape,
			     DAL::Stokes::U);
    
    for (int step(0); step<nofSteps; ++step) {
      // set position marker
      start[1] = step*block[1];
      // update data array values
      for (unsigned int n(0); n<nofDatapoints; ++n) {
	data[n] = step;
      }
      // write data to dataset
      stokes.writeData (data, start, block);
    }
    
    delete [] data;
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }

  //________________________________________________________
  // Test 4

  cout << "[4] Test writing multiple columns to dataset ..." << endl;
  try {
    index         = 104;
    nofSteps      = 16;
    start[0]      = 0;
    block[0]      = shape[0];
    block[1]      = shape[1]/nofSteps;
    nofDatapoints = DAL::HDF5Hyperslab::nofDatapoints (count,block);
    float *data   = new float [nofDatapoints];

    cout << "-- Shape        = " << shape    << endl;
    cout << "-- nof. steps   = " << nofSteps << endl;
    cout << "-- block        = " << block    << endl;

    BF_StokesDataset stokes (fileID,
			     index,
			     shape,
			     DAL::Stokes::U);
    
    for (int step(0); step<nofSteps; ++step) {
      // set position marker
      start[1] = step*block[1];
      // update data array values
      for (unsigned int n(0); n<nofDatapoints; ++n) {
	data[n] = step;
      }
      // feedback
      cout << "-> writing datablock " << step << "/" << nofSteps
	   << " starting from " << start
	   << " ..." << endl;
      // write data to dataset
      stokes.writeData (data, start, block);
    }
    
    delete [] data;
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }

  //________________________________________________________
  // Test 5

  cout << "[5] Test extending the number of rows in the dataset ..." << endl;
  try {
    index         = 105;
    nofSteps      = 16;
    start[0]      = 0;
    block[0]      = shape[0];
    block[1]      = shape[1]/nofSteps;
    nofDatapoints = DAL::HDF5Hyperslab::nofDatapoints (count,block);
    float *data   = new float [nofDatapoints];
    nofSteps     *= 2;

    cout << "-- Shape        = " << shape    << endl;
    cout << "-- nof. steps   = " << nofSteps << endl;
    cout << "-- block        = " << block    << endl;

    BF_StokesDataset stokes (fileID,
			     index,
			     shape,
			     DAL::Stokes::U);
    
    for (int step(0); step<nofSteps; ++step) {
      // set position marker
      start[1] = step*block[1];
      // update data array values
      for (unsigned int n(0); n<nofDatapoints; ++n) {
	data[n] = step;
      }
      // feedback
      cout << "-> writing datablock " << step << "/" << nofSteps
	   << " starting from " << start
	   << " ..." << endl;
      // write data to dataset
      stokes.writeData (data, start, block);
    }
    
    delete [] data;
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }

  return nofFailedTests;
}

//_______________________________________________________________________________
//                                                                           main

/*!
  \brief Main routine of the test program

  \return nofFailedTests -- The number of failed tests encountered within and
          identified by this test program.
*/
int main ()
{
  int nofFailedTests   = 0;
  std::string filename = "tBF_StokesDataset.h5";

  //________________________________________________________
  // Create HDF5 file to work with
  
  hid_t fileID = H5Fcreate (filename.c_str(),
			    H5F_ACC_TRUNC,
			    H5P_DEFAULT,
			    H5P_DEFAULT);
  
  /* If file creation was successful, run the tests. */
  if (H5Iis_valid(fileID)) {

    // Test for the constructor(s)
    nofFailedTests += test_constructors (fileID);
    // // Test access to the attributes
    nofFailedTests += test_attributes (fileID);
    // Test read/write access to the data
    // nofFailedTests += test_data (fileID);

  } else {
    cerr << "-- ERROR: Failed to open file " << filename << endl;
    return -1;
  }
  
  //________________________________________________________
  // close HDF5 file
  
  H5Fclose(fileID);

  return nofFailedTests;
}
