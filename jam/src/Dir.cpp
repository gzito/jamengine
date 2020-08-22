/**********************************************************************************
* 
* Dir.cpp
* 
* This file is part of Jam
* 
* Copyright (c) 2014-2019 Giovanni Zito.
* Copyright (c) 2014-2019 Jam contributors (cf. AUTHORS.md)
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
* 
**********************************************************************************/

#include "stdafx.h"

#include <jam/Dir.h>
#include <jam/core/filesystem.h>

namespace jam {

	/*!
	 * \brief Default constructor
	 * Create and initialize a directory entry
	 */
	DirEntry::DirEntry () :
		statSuccess_ ( false ) {

		memset ( &st_, 0, sizeof ( st_ ) );
	}

	/*!
	 * \brief Copy constructor
	 */
	DirEntry::DirEntry ( const DirEntry& other ) {
		copy ( other );
	}

	/*!
	 * \brief Destructor
	 */
	DirEntry::~DirEntry () {
	}

	/*!
	 * \brief Assignment operator
	 */
	DirEntry& DirEntry::operator= ( const DirEntry& other ) {

		if ( this != &other ) {
			copy ( other );
		}

		return *this;
	}

	/*!
	 * \brief Returns the name of this directory entry
	 * \note The path of this entry is not included in the returned name
	 */
	String DirEntry::getName () const {

		return name_;
	}

	/*!
	 * \brief Returns the full name of this directory entry
	 * \sa getName()
	 *
	 * \note The absolute path of this entry is included in the returned name
	 */
	String DirEntry::getFullName () const {

		return fullName_;
	}

	/*!
	 * \return true if this entry is a directory
	 */
	bool DirEntry::isDirectory () const {

		bool	isDir = statSuccess_ && S_ISDIR( st_.st_mode );
		return isDir;
	}

	/*!
	 * \return true if this entry has readable attribute set
	 */
	bool DirEntry::isReadable () const {

		bool isDir;

		uint16_t	readable = S_IRUSR | S_IRGRP | S_IROTH; 
		isDir = statSuccess_ && ( st_.st_mode & readable );

		return isDir;
	}

	/*!
	 * \return true if this entry is a file
	 * \note Directories, device files, pipes are not considered regular files.
	 */
	bool DirEntry::isRegularFile () const {

		bool isRegularFile = statSuccess_ && S_ISREG( st_.st_mode );
		return isRegularFile;
	}

	/*!
	 * \return the stat info for this entry
	 */
	const struct stat& DirEntry::getStat () const {

		return st_;
	}

	/**************************
	* private methods
	**************************/
	void DirEntry::copy ( const DirEntry& other ) {

		name_ = other.name_;
		fullName_ = other.fullName_;
		st_ = other.st_;
		statSuccess_ = other.statSuccess_;
	}

	const unsigned int Dir::GET_ALL = 0;
	const unsigned int Dir::GET_FILES_ONLY = 1;
	const unsigned int Dir::GET_FOLDERS_ONLY = 2;
	const unsigned int Dir::DO_NOT_GET_PARENT_AND_CURRENT = 4;

	/*!
	 * \brief Constructor
	 * \param path - The path that identifies a directory
	 *
	 * Creates an instance of Dir object.
	 */
	Dir::Dir ( const String& path, unsigned int scanMode /* = GET_ALL */ ) :
		entry_(),
		path_ ( path ),
		dirp_ ( 0 ),
		rResult_ ( 0 ),
		mode_ ( scanMode ),
		rEntry_() 
	{
	}

	/*!
	 * \brief Destructor
	 */
	Dir::~Dir () {

		if ( dirp_ ) {
			closedir ( dirp_ );
			dirp_ = 0;
		}
	}

	/*!
	 * \brief Get the next directoty entry
	 * \return true if there is a valid next entry, false otherwise
	 */
	bool Dir::getEntry ( DirEntry& entry ) {

		bool retval = true;

		while ( 1 ) {
			retval = ( dirp_ ) ? next () : first ();

			if ( retval ) {
				if ( ( mode_ & DO_NOT_GET_PARENT_AND_CURRENT ) && ( entry_.getName () == ".." || entry_.getName () == "." ) ) {
					continue;
				}
				else if ( ( mode_ & GET_FILES_ONLY ) && ( entry_.isDirectory () ) ) {
					continue;
				}
				else if ( ( mode_ & GET_FOLDERS_ONLY ) && ( ! entry_.isDirectory () ) ) {
					continue;
				}
				else {
					break;
				}
			}
			else {
				break;
			}
		}

		entry = entry_;

		return retval;
	}

	//******************
	//*
	//* private methods
	//*
	//******************

	bool Dir::first () {

		dirp_ = opendir ( path_.c_str () );
		if ( ! dirp_ ) {
			JAM_ERROR( "Cannot open directory \"%s\"", path_.c_str() );
		}
		return next ();
	}

	bool Dir::next () {

		int	return_code = readdir_r ( dirp_, &rEntry_.d, &rResult_ );

		bool	retval = ( return_code == 0 ) && ( rResult_ != 0 );

		if ( retval ) {
			entry_.name_ = rResult_->d_name;
			entry_.fullName_ = jam::appendPath( path_, rResult_->d_name );
			try {
				stat( entry_.fullName_.c_str(), &entry_.st_ );
				entry_.statSuccess_ = true;
			}
			catch ( ... ) {
				entry_.statSuccess_ = false;
			}
		}
		else {
			if ( dirp_ ) {
				closedir ( dirp_ );
				dirp_ = 0;
			}
		}

		return retval;
	}

}
