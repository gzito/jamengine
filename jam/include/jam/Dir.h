/**********************************************************************************
* 
* Dir.h
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

#ifndef __JAM_DIR_H__
#define __JAM_DIR_H__

#include <jam/jam.h>
#include <jam/thirdparty/dirent.hpp>
#include <jam/String.h>

namespace jam {

/*!
	* \class DirEntry
	* \brief Represent a directory entry
	*
	* A directory entry is a file or a subdirectory
	*/
class JAM_API DirEntry {

	friend class Dir;

public:
							DirEntry		();
							~DirEntry		();
							DirEntry		( const DirEntry& );
	DirEntry&				operator=		( const DirEntry& );

	String					getName			() const;
	String					getFullName		() const;
	bool					isDirectory		() const;
	bool					isReadable		() const;
	bool					isRegularFile	() const;

	const struct stat&		getStat			() const;

private:
	void					copy			( const DirEntry& );

private:
	String					name_;
	String					fullName_;

	struct stat				st_;
	bool					statSuccess_;
};

/*!
	* \class Dir
	* \brief Allows to scan a directory for files or subfolders.
	*/
class JAM_API Dir {

public:
	/*!
	* GET_ALL, GET_FILES_ONLY and GET_FOLDERS_ONLY are mutually exclusives
	*/

	static const unsigned int	GET_ALL;
	static const unsigned int	GET_FILES_ONLY;
	static const unsigned int	GET_FOLDERS_ONLY;
	static const unsigned int	DO_NOT_GET_PARENT_AND_CURRENT;

							Dir			( const String& path, unsigned int scanMode = GET_ALL );
							~Dir		();
	bool					getEntry	( DirEntry& );

private:
	bool					first		();
	bool					next		();

private:
	DirEntry				entry_;

	String		 			path_;

	DIR*					dirp_;
	dirent*					rResult_;

	union {
							dirent	d;
							char	b[offsetof (dirent, d_name) + 640 + 1];
	} rEntry_;

	unsigned int			mode_;
};

}

#endif
