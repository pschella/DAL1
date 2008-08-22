/*-------------------------------------------------------------------------*
 | $Id::                                                                 $ |
 *-------------------------------------------------------------------------*
 ***************************************************************************
 *   Copyright (C) 2006 by Joseph Masters                                  *
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

#ifndef DALGROUP_H
#include "dalGroup.h"
#endif

namespace DAL
  {

// ------------------------------------------------------------ dalGroup

  /*!
    \brief Default constructor.

    Default constructor.
   */
  dalGroup::dalGroup()
  {
    file = NULL;
    groupname = "UNKNOWN";
    groupname_full = "UNKNOWN";
    group = NULL;
    filter = NULL;
    file_id = 0;
    group_id = 0;
    status = 0;
  }


// ------------------------------------------------------------ dalGroup

  /*!
    \brief Create a group in a certain file.

    Create a group in a certain file.

    \param groupname The name of the group to create.
    \param file A pointer to the file where you want to create the group.
   */
  dalGroup::dalGroup( const char * gname, void * voidfile )
  {

    dalGroup();

    hid_t * lclfile = (hid_t*)voidfile; // H5File object
    file = lclfile;
    file_id = *lclfile;  // get the file handle

    groupname = gname;
    groupname_full = "/" + stringify(gname);
    if ( ( group_id = H5Gcreate( file_id, groupname_full.c_str(),  H5P_DEFAULT,
                                 H5P_DEFAULT, H5P_DEFAULT ) ) < 0 )
      {
        std::cerr << "ERROR: Could not create group'" << groupname_full
                  << "'.\n";
      }

  }


// ------------------------------------------------------------ dalGroup

  /*!
    \brief Create a subgroup.

    Create a subgroup in an existing group.

    \param group_id The parent group identifier.
    \param gname The name of the subgroup.
   */
  dalGroup::dalGroup( hid_t obj_id, const char * gname )
  {
    dalGroup();

    if ( ( group_id = H5Gcreate( obj_id, gname, H5P_DEFAULT, H5P_DEFAULT,
                                 H5P_DEFAULT ) ) < 0 )
      {
        std::cerr << "ERROR: Could not create group'" << string( gname )
                  << "'.\n";
      }
  }


// ------------------------------------------------------------ open

  /*!
    \brief Open an existing group.

    Open an existing group.

    \param file A pointer to the file.
    \param groupname The name of the group you want to open.
    \return An identifier for the new group.
   */
  int dalGroup::open( void * voidfile, std::string gname )
  {

    groupname = gname;

    hid_t * lclfile = (hid_t*)voidfile; // H5File object
    file = lclfile;
    file_id = *lclfile;  // get the file handle

    groupname_full = "/" + groupname;
    if ( ( group_id = H5Gopen1( file_id, groupname_full.c_str() ) ) < 0 )
      {
        std::cerr << "ERROR: Could not create group'" << groupname_full
                  << "'.\n";
        return 0;
      }

    return( group_id );
  }

// ------------------------------------------------------------ close

  /*!
    \brief Close a group.

    Cloase a group.

   */
  bool dalGroup::close()
  {
    if ( 0 != group_id )
      {
        if ( H5Gclose(group_id) < 0 )
          {
            std::cerr << "ERROR: dalGroup::close() failed.\n";
            return DAL::FAIL;
          }
        group_id = 0;
      }
    return DAL::SUCCESS;
  }


// ------------------------------------------------------------ ~dalGroup

  /*!
    \brief Default destructor.

    Default destructor.
   */
  dalGroup::~dalGroup()
  {
    if ( 0 != group_id )
    {
      if ( H5Gclose(group_id) < 0 )
        {
          std::cerr << "ERROR: dalGroup::close() failed.\n";
        }
      group_id = 0;
    }
  }


// ------------------------------------------------------------ getId

  /*!
    \brief Get the group ID.

    Retrieve the identifier for the group.

    \return The group identifier as an integer.
  */
  hid_t dalGroup::getId()
  {
    return group_id;
  }

// -------------------------------------------------- dalGroup_file_info

  /*
   * Operator function.
   */
  herr_t dalGroup_file_info(hid_t loc_id, const char *name, void *opdata)
  {
    H5G_stat_t statbuf;

    /*
     * Get type of the object and display its name and type.
     * The name of the object is passed to this function by
     * the Library. Some magic :-)
     */
    if ( H5Gget_objinfo(loc_id, name, false, &statbuf) > 0 )
      {
        std::cerr << "ERROR: Could not get group object info.\n";
        return 1;
      }

    switch (statbuf.type)
      {
      case H5G_DATASET:
        (*(vector<string>*)opdata).push_back( std::string(name) );
        break;
      case H5G_GROUP:
      case H5G_TYPE:
      default:
        break;
      }

    return 0;
  }


// ------------------------------------------------------------ getMemberNames

  /*!
    \brief Retrieve the array or table member names.

    Retrief the array or table member names from the group.

    \return A vector of strings representing the member names.
   */
  vector<string> dalGroup::getMemberNames()
  {
    vector<string> member_names;
    H5Giterate( file_id, groupname.c_str(), NULL, dalGroup_file_info, &member_names );
    return member_names;
  }

// ------------------------------------------------------------ setName

  /*!
    \brief Set group name.

    Set the name of the group.

    \param gname The name of the group.
    \return bool -- DAL::FAIL or DAL::SUCCESS
   */
  bool dalGroup::setName ( std::string gname )
  {
    if ( gname.length() > 0 )
      {
        groupname = gname;
        return SUCCESS;
      }
    else
      {
        std::cerr << "Error:  Group name must not be empty.\n";
        return FAIL;
      }
  }


// ------------------------------------------------------- createShortArray

  /*!
    \brief Create an array of shorts within the group.

    Create an array of shorts with any dimensions.  This is usually
    called from the dataset object and not from the developer.

    \param arrayname A string containing he name of the array.
    \param dims A vector specifying the array dimensions.
    \param data A structure containing the data to be written.  The size
                of the data must match the provided dimensions.
    \param cdims The chunk dimensions for an extendible array.

    \return dalArray * A pointer to an array object.
  */
  dalArray *
  dalGroup::createShortArray( string arrayname,
                              vector<int> dims,
                              short data[],
                              vector<int> cdims )
  {
    dalShortArray * la;
    la = new dalShortArray( group_id, arrayname, dims, data, cdims );
    return la;
  }



// ------------------------------------------------------------ createIntArray

  /*!
    \brief Create an array of integers within the group.

    Create an array of integers with any dimensions.  This is usually
    called from the dataset object and not from the developer.

    \param arrayname A string containing he name of the array.
    \param dims A vector specifying the array dimensions.
    \param data A structure containing the data to be written.  The size
                of the data must match the provided dimensions.
    \param cdims The chunk dimensions for an extendible array.

    \return dalArray * A pointer to an array object.
  */
  dalArray *
  dalGroup::createIntArray( string arrayname,
                            vector<int> dims,
                            int data[],
                            vector<int> cdims )
  {
    dalIntArray * la;
    la = new dalIntArray( group_id, arrayname, dims, data, cdims );
    return la;
  }


// ------------------------------------------------------- createFloatArray

  /*!
    \brief Create an array of floating point values within the group.

    Create an array of floating point values with any dimensions.  This
    is usually called from the dataset object and not from the developer.

    \param arrayname A string containing he name of the array.
    \param dims A vector specifying the array dimensions.
    \param data A structure containing the data to be written.  The size
                of the data must match the provided dimensions.
    \param cdims The chunk dimensions for an extendible array.

    \return dalArray * A pointer to an array object.
  */
  dalArray *
  dalGroup::createFloatArray( string arrayname,
                              vector<int> dims,
                              float data[],
                              vector<int> cdims )
  {
    dalFloatArray * la;
    la = new dalFloatArray( group_id, arrayname, dims, data, cdims );
    return la;
  }


// ------------------------------------------------ createComplexFloatArray

  /*!
    \brief Create an array of complex floating point values within the group.

    Create an array of complex floating point values with any dimensions.
    This is usually called from the dataset object and not from the
    developer.

    \param arrayname A string containing he name of the array.
    \param dims A vector specifying the array dimensions.
    \param data A structure containing the data to be written.  The size
                of the data must match the provided dimensions.
    \param cdims The chunk dimensions for an extendible array.

    \return dalArray * A pointer to an array object.
  */
  dalArray *
  dalGroup::createComplexFloatArray( string arrayname,
                                     vector<int> dims,
                                     complex<float> data[],
                                     vector<int> cdims )
  {
    dalComplexArray_float32 * la;
    la = new dalComplexArray_float32( group_id, arrayname, dims, data, cdims );
    return la;
  }


// ------------------------------------------------- createComplexShortArray

  /*!
    \brief Create an array of complex int16 values within the group.

    Create an array of complex int16 values with any dimensions.
    This is usually called from the dataset object and not from the
    developer.

    \param arrayname A string containing he name of the array.
    \param dims A vector specifying the array dimensions.
    \param data A structure containing the data to be written.  The size
                of the data must match the provided dimensions.
    \param cdims The chunk dimensions for an extendible array.

    \return dalArray * A pointer to an array object.
  */
  dalArray *
  dalGroup::createComplexShortArray( string arrayname,
                                     vector<int> dims,
                                     complex<Int16> data[],
                                     vector<int> cdims )
  {
    dalComplexArray_int16 * la;
    la = new dalComplexArray_int16( group_id, arrayname, dims, data, cdims );
    return la;
  }

// ------------------------------------------------------------ getName

  /*!
     \brief Get group name.

    Retrieve the name of the group object.

    \return The name of the group.
   */
  string dalGroup::getName ()
  {
    return groupname;
  }


  // ---------------------------------------------- setAttribute

  /*!
    \brief Define a char attribute.

    Define a char attribute.

    \param attrname The name of the attribute you want to create.
    \param data The value of the attribute you want to create.
    \param size Optional parameter specifying the array size of the
                attribute.  Default is scalar.
    \return bool -- DAL::FAIL or DAL::SUCCESS
  */
  bool dalGroup::setAttribute( std::string attrname, char * data, int size )
  {
    return h5setAttribute( H5T_NATIVE_CHAR, group_id, attrname, data, size );
  }

  // ---------------------------------------------- setAttribute

  /*!
    \brief Define a short attribute.

    Define a short attribute.

    \param attrname The name of the attribute you want to create.
    \param data The value of the attribute you want to create.
    \param size Optional parameter specifying the array size of the
                attribute.  Default is scalar.
    \return bool -- DAL::FAIL or DAL::SUCCESS
  */
  bool dalGroup::setAttribute( std::string attrname, short * data, int size )
  {
    return h5setAttribute( H5T_NATIVE_SHORT, group_id, attrname, data, size );
  }

  // ---------------------------------------------- setAttribute

  /*!
    \brief Define a integer attribute.

    Define a integer attribute.

    \param attrname The name of the attribute you want to create.
    \param data The value of the attribute you want to create.
    \param size Optional parameter specifying the array size of the
                attribute.  Default is scalar.
    \return bool -- DAL::FAIL or DAL::SUCCESS
  */
  bool dalGroup::setAttribute( std::string attrname, int * data, int size )
  {
    return h5setAttribute( H5T_NATIVE_INT, group_id, attrname, data, size );
  }

  // ---------------------------------------------- setAttribute

  /*!
    \brief Define a unsigned integer attribute.

    Define a unsigned integer attribute.

    \param attrname The name of the attribute you want to create.
    \param data The value of the attribute you want to create.
    \param size Optional parameter specifying the array size of the
                attribute.  Default is scalar.
    \return bool -- DAL::FAIL or DAL::SUCCESS
  */
  bool dalGroup::setAttribute( std::string attrname, uint * data, int size )
  {
    return h5setAttribute( H5T_NATIVE_UINT, group_id, attrname, data, size );
  }

  // ---------------------------------------------- setAttribute

  /*!
    \brief Define a long integer attribute.

    Define a long integer attribute.

    \param attrname The name of the attribute you want to create.
    \param data The value of the attribute you want to create.
    \param size Optional parameter specifying the array size of the
                attribute.  Default is scalar.
    \return bool -- DAL::FAIL or DAL::SUCCESS
  */
  bool dalGroup::setAttribute( std::string attrname, long * data, int size )
  {
    return h5setAttribute( H5T_NATIVE_LONG, group_id, attrname, data, size );
  }

  // ---------------------------------------------- setAttribute

  /*!
    \brief Define a floating point attribute.

    Define a floating point attribute.

    \param attrname The name of the attribute you want to create.
    \param data The value of the attribute you want to create.
    \param size Optional parameter specifying the array size of the
                attribute.  Default is scalar.
    \return bool -- DAL::FAIL or DAL::SUCCESS
  */
  bool dalGroup::setAttribute( std::string attrname, float * data, int size )
  {
    return h5setAttribute( H5T_NATIVE_FLOAT, group_id, attrname, data, size );
  }

  // ---------------------------------------------- setAttribute

  /*!
    \brief Define a double precision floating point attribute.

    Define a double precision floating point attribute.

    \param attrname The name of the attribute you want to create.
    \param data The value of the attribute you want to create.
    \param size Optional parameter specifying the array size of the
                attribute.  Default is scalar.
    \return bool -- DAL::FAIL or DAL::SUCCESS
  */
  bool dalGroup::setAttribute( std::string attrname, double * data, int size )
  {
    return h5setAttribute( H5T_NATIVE_DOUBLE, group_id, attrname, data, size );
  }

  // ---------------------------------------------- setAttribute_string

  /*!
    \brief Define a string attribute.

    Define a string attribute.

    \param attrname The name of the attribute you want to create.
    \param data The value of the attribute you want to create.
    \return bool -- DAL::FAIL or DAL::SUCCESS
  */
  bool dalGroup::setAttribute( std::string attrname, std::string data )
  {
    return h5setAttribute_string( group_id, attrname, &data, 1 );
  }

  // ---------------------------------------------- setAttribute_string

  /*!
    \brief Define a string attribute.

    Define a string attribute.

    \param attrname The name of the attribute you want to create.
    \param data The value of the attribute you want to create.
    \param size Optional parameter specifying the array size of the
                attribute.  Default is scalar.
    \return bool -- DAL::FAIL or DAL::SUCCESS
  */
  bool dalGroup::setAttribute( std::string attrname, std::string * data,
                               int size )
  {
    return h5setAttribute_string( group_id, attrname, data, size );
  }

  // ---------------------------------------------------------- createGroup

  /*!
    \brief Create a new group.

    Create a new group.

    \param groupname
    \return dalGroup
  */
  dalGroup * dalGroup::createGroup( const char * gname )
  {
        dalGroup * lg = NULL;
        lg = new dalGroup( group_id, gname );
        return lg;
  }

#ifdef PYTHON
  /************************************************************************
   *
   * The following functions are boost wrappers to allow some previously
   *   defined functions to be easily called from a python prompt.
   *
   ************************************************************************/

  /******************************************************
   * wrappers for createIntArray
   ******************************************************/

// ------------------------------------------------------------ cia_boost1

  dalArray * dalGroup::cia_boost1( string arrayname, bpl::list pydims,
                                   bpl::list pydata )
  {
    bpl::list cdims;

    for (int ii=0; ii<bpl::len(pydims); ii++)
      cdims.append(10);

    dalArray * array;
    array = cia_boost2( arrayname, pydims, pydata, cdims );

    return array;
  }

// ------------------------------------------------------------ cia_boost2

  dalArray * dalGroup::cia_boost2(
    string arrayname,
    bpl::list pydims,
    bpl::list pydata,
    bpl::list cdims )
  {

    vector<int> dims;
    vector<int> chnkdims;

    for (int ii=0; ii<bpl::len(pydims); ii++)
      dims.push_back(bpl::extract<int>(pydims[ii]));

    for (int ii=0; ii<bpl::len(cdims); ii++)
      chnkdims.push_back(bpl::extract<int>(cdims[ii]));

    long size = bpl::len(pydata);
    int * data = NULL;
    data = new int[size];

    for (int ii=0; ii<size; ii++)
      data[ii] = bpl::extract<int>(pydata[ii]);

    dalArray * array = createIntArray(arrayname, dims, data, chnkdims);

    delete [] data;
    data = NULL;

    return array;
  }

// --------------------------------------------- cia_boost_numarray1

  dalArray * dalGroup::cia_boost_numarray1(
    string arrayname,
    bpl::list pydims,
    bpl::numeric::array pydata )
  {

    bpl::list cdims;

    for (int ii=0; ii<bpl::len(pydims); ii++)
      cdims.append(10);

    dalArray * array;
    array = cia_boost_numarray2(arrayname, pydims, pydata, cdims);

    return array;
  }

// ------------------------------------------- cia_boost_numarray2

  dalArray * dalGroup::cia_boost_numarray2(
    string arrayname,
    bpl::list pydims,
    bpl::numeric::array pydata,
    bpl::list cdims )
  {

    vector<int> dims;

    for (int ii=0; ii<bpl::len(pydims); ii++)
      dims.push_back(bpl::extract<int>(pydims[ii]));

    pydata.setshape( pydata.nelements() );
    bpl::object flat_data = pydata.getflat();
    bpl::list list_data( flat_data );

    dalArray * array = cia_boost2(arrayname, pydims, list_data, cdims);

    return array;
  }

// ------------------------------------------------------------ ria_boost

  bpl::numeric::array dalGroup::ria_boost( string arrayname )
  {
    hid_t lclfile;
    hid_t  status;

    // get the dataspace
    lclfile = H5Dopen1( group_id, arrayname.c_str() );
    hid_t filespace = H5Dget_space(lclfile);

    // what is the rank of the array?
    hid_t data_rank = H5Sget_simple_extent_ndims(filespace);
    hsize_t dims[ data_rank ];
#ifdef DEBUGGING_MESSAGES
    std::cerr << "data rank: " << data_rank << endl;
#endif
    status = H5Sget_simple_extent_dims(filespace, dims, NULL);

    int size = 1;
    bpl::list dims_list;
    for (int ii=0; ii<data_rank; ii++)
      {
#ifdef DEBUGGING_MESSAGES
        std::cerr << "dims["  << ii << "]: " << dims[ii] << endl;
#endif
        size *= dims[ii];
        dims_list.append(dims[ii]);
      }

#ifdef DEBUGGING_MESSAGES
    std::cerr << "size: " << size << endl;
#endif

    int * data = NULL;
    data = new int[size];

    status = H5LTread_dataset_int( group_id, arrayname.c_str(), data );

#ifdef DEBUGGING_MESSAGES
    for (int ii=0; ii<size; ii++)
      {
        std::cerr << data[ii] << endl;
      }
#endif

    bpl::list data_list;
    // for each dimension
    for (int ii=0; ii<size; ii++)
      {
        data_list.append(data[ii]);
      }

    delete [] data;
    data = NULL;

    bpl::numeric::array nadata(
      bpl::make_tuple(
        bpl::make_tuple(data_list)
      )
    );

    nadata.setshape(dims_list);
    return nadata;
  }

  /******************************************************
   * wrappers for createFloatArray
   ******************************************************/

// ------------------------------------------------------------ cfa_boost

  dalArray * dalGroup::cfa_boost( string arrayname, bpl::list pydims,
                                  bpl::list pydata, bpl::list cdims )
  {

    vector<int> dims;
    vector<int> chnkdims;

    for (int ii=0; ii<bpl::len(pydims); ii++)
      dims.push_back(bpl::extract<int>(pydims[ii]));

    for (int ii=0; ii<bpl::len(cdims); ii++)
      chnkdims.push_back(bpl::extract<int>(cdims[ii]));

    long size = bpl::len(pydata);
    float * data = NULL;
    data = new float[size];

    for (int ii=0; ii<size; ii++)
      data[ii] = bpl::extract<float>(pydata[ii]);

    dalArray * array = createFloatArray(arrayname, dims, data, chnkdims);

    delete [] data;
    data = NULL;

    return array;
  }

// ---------------------------------------------------- cfa_boost_numarray

  dalArray * dalGroup::cfa_boost_numarray( string arrayname,
      bpl::list pydims,
      bpl::numeric::array pydata, bpl::list cdims )
  {

    vector<int> dims;

    for (int ii=0; ii<bpl::len(pydims); ii++)
      dims.push_back(bpl::extract<int>(pydims[ii]));

    pydata.setshape( pydata.nelements() );
    bpl::object flat_data = pydata.getflat();
    bpl::list list_data( flat_data );

    dalArray * array = cfa_boost(arrayname, pydims, list_data, cdims);

    return array;
  }

  bool dalGroup::setAttribute_char( std::string attrname, char data )
  {
     return setAttribute( attrname, &data );
  }
  bool dalGroup::setAttribute_short( std::string attrname, short data )
  {
     return setAttribute( attrname, &data );
  }
  bool dalGroup::setAttribute_int( std::string attrname, int data )
  {
     return setAttribute( attrname, &data );
  }
  bool dalGroup::setAttribute_uint( std::string attrname, uint data )
  {
     return setAttribute( attrname, &data );
  }
  bool dalGroup::setAttribute_long( std::string attrname, long data )
  {
     return setAttribute( attrname, &data );
  }
  bool dalGroup::setAttribute_float( std::string attrname, float data )
  {
     return setAttribute( attrname, &data );
  }
  bool dalGroup::setAttribute_double( std::string attrname, double data )
  {
     return setAttribute( attrname, &data );
  }
  bool dalGroup::setAttribute_string( std::string attrname, std::string data )
  {
     return setAttribute( attrname, &data );
  }

#endif // end #ifdef PYTHON

} // end namespace DAL
