/**********************************************************************************
* 
* ResourceManager.cpp
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

#include "jam/ResourceManager.h"
#include "jam/core/filesystem.h"
#include "jam/Dir.h"
#include <fstream>
#include <stdexcept>


namespace jam
{

//*****************************************************************************
FileSystemResourceFile::FileSystemResourceFile(const String& resFileName)
{
//	String p = jam::getCurrentDirectory() ;
//	m_AssetsDir = jam::appendPath(p,"Assets") ;
	m_AssetsDir = resFileName ;
}

int FileSystemResourceFile::find( const String& name )
{
	String lowerCase = jam::makeLower(name) ;

	auto i = m_DirectoryContentsMap.find(lowerCase);
	if( i == m_DirectoryContentsMap.end() )
		return -1;

	return i->second;
}



bool FileSystemResourceFile::open()
{
	// open the asset directory and read in the non-hidden contents
	readAssetsDirectory();
	return true;
}


size_t FileSystemResourceFile::getRawResourceSize(const Resource& r)
{
	int size = 0;

	int num = find( r.getName() );
	if( num == -1 )
		return -1;

	return m_AssetFileInfo[num].m_fileSize ;
}

size_t FileSystemResourceFile::getRawResource(const Resource& r, char *buffer)
{
	int num = find( r.getName() );
	if (num == -1)
		return -1;

#if defined(_MSC_VER)
	WString fullFileSpec = s2ws( jam::appendPath( m_AssetsDir, r.getName() ) );
	FILE* f = _wfopen(fullFileSpec.c_str(), L"rb");
#else
	String fullFileSpec = jam::appendPath( m_AssetsDir, r.getName() );
	FILE* f = fopen(fullFileSpec.c_str(), "rb");
#endif
	size_t bytes = fread(buffer, 1, m_AssetFileInfo[num].m_fileSize, f);
	fclose(f);

	return bytes;
}

size_t FileSystemResourceFile::getNumResources() const 
{
	return m_AssetFileInfo.size();
}

std::string FileSystemResourceFile::getResourceName(size_t num) const 
{
	return m_AssetFileInfo[num].m_fileName;
}


void FileSystemResourceFile::readAssetsDirectory(const String& fileSpec /* = "" */)
{
	String pathSpec = appendPath( m_AssetsDir, fileSpec ) ;

	JAM_TRACE( "Try opening the folder '%s' for reading...\n", pathSpec.c_str() ) ;

	if( exists(pathSpec) && isDirectory(pathSpec) ) {
		DirEntry entry ;
		Dir dir( pathSpec, Dir::DO_NOT_GET_PARENT_AND_CURRENT ) ;
		while( dir.getEntry( entry ) ) {
			String fileName = entry.getFullName() ;
			String relativeEntryPath = fileName.substr( m_AssetsDir.size()+1 ) ;
			if( entry.isDirectory() ) {
				readAssetsDirectory(relativeEntryPath);
			}
			else if( entry.isRegularFile() ) {
				String lower = makeLower(relativeEntryPath);
				DirEntryData dirEntryData = { entry.getName(), entry.getStat().st_size } ;
				m_DirectoryContentsMap[lower] = m_AssetFileInfo.size();
				m_AssetFileInfo.emplace_back(dirEntryData) ;
			}
		}
	}
	else {
		JAM_ERROR( "Cannot open '%s' for reading", pathSpec.c_str() ) ;
	}
}


//*****************************************************************************
ResHandle::ResHandle( Resource& resource, char* buffer, size_t size, ResourceManager* pResManager ) :
	m_resource( resource )
{
	m_buffer = buffer ;
	m_size = size ;
	m_pExtra = nullptr ;
	m_pResManager = pResManager ;
}

ResHandle::~ResHandle()
{
	JAM_DELETE_ARRAY( m_buffer ) ;
	m_pResManager->memoryHasBeenFreed( m_size ) ;
}

//
// ResourceManager
//
ResourceManager::ResourceManager(const size_t sizeInMb,IResourceFile* resFile) :
	m_cacheSize( sizeInMb * 1024 * 1024 ),
	m_allocated( 0 ),
	m_file( resFile )
{
}

ResourceManager::~ResourceManager()
{
	while( !m_lru.empty() ) {
		freeOneResource() ;
	}
	// m_file is deleted here
}

bool ResourceManager::init()
{
	bool retValue = false ;
	if( m_file->open() ) {
		registerLoader( new (GC) DefaultResourceLoader() ) ;
		retValue = true ;
	}
	return retValue ;
}

void ResourceManager::registerLoader(IResourceLoader* loader)
{
	m_resourceLoaders.emplace_front(loader);
}

ResHandle* ResourceManager::getHandle(Resource* r)
{
	ResHandle* handle( find(r) );
	if( !handle ) {
		handle = load(r) ;
		JAM_ASSERT( handle != nullptr ) ;
	}
	else {
		update(handle) ;
	}

	return handle ;
}

int ResourceManager::preload(const String& pattern,SA::delegate<void(int,bool&)> progressDelegate)
{
	return 0;
}

//    Frees every handle in the cache - this would be good to call if you are loading a new
//    level, or if you wanted to force a refresh of all the data in the cache - which might be 
//    good in a development environment.
void ResourceManager::flush()
{
	while( !m_lru.empty() )	{
		ResHandle* handle = *(m_lru.begin());
		free(handle);
		m_lru.pop_front();
	}
}

ResHandle* ResourceManager::find(Resource* r)
{
	auto i = m_resources.find(r->getName());
	if( i == m_resources.end() ) {
		return nullptr;
	}

	return i->second;
}

void ResourceManager::update(ResHandle* handle)
{
	m_lru.remove(handle);
	m_lru.emplace_front(handle);
}

ResHandle* ResourceManager::load(Resource* r)
{
	// Create a new resource and add it to the lru list and map

	IResourceLoader*loader = nullptr ;
	ResHandle* handle = nullptr;

	// find the right loader
	for( const auto& testLoader : m_resourceLoaders )
	{
		for( auto pattern : testLoader->getPatterns() ) {
			if( wildcardMatch(pattern.c_str(), r->getName().c_str()) ) {
				loader = testLoader;
				break;
			}
		}

		if( loader ) break ;
	}

	if( !loader ) {
		JAM_ASSERT_MSG( loader!=nullptr, "Default resource loader not found!" );
		return handle;		// Resource not loaded!
	}


	size_t rawSize = m_file->getRawResourceSize(*r);

    int allocSize = rawSize + ((loader->addNullZero()) ? (1) : (0));
	char *rawBuffer = loader->useRawFile() ? allocate(allocSize) : new char[allocSize];
    if( rawBuffer ) memset(rawBuffer, 0, allocSize);

	if( rawBuffer==nullptr || m_file->getRawResource(*r, rawBuffer)==0 ) {
		// resource cache out of memory
		return nullptr;
	}
	
	char *buffer = nullptr ;
	unsigned int size = 0;

	if( loader->useRawFile() ) {
		buffer = rawBuffer;
		handle = new (GC) ResHandle(*r, buffer, rawSize, this);
	}
	else {
		size = loader->getLoadedResourceSize(rawBuffer, rawSize);
        buffer = allocate(size);
		if( rawBuffer == nullptr || buffer == nullptr ) {
			// resource cache out of memory
			return nullptr;
		}
		handle = new (GC) ResHandle(*r, buffer, size, this);
		bool success = loader->loadResource(rawBuffer, rawSize, *handle);
		
		// [mrmike] - This was added after the chapter went to copy edit. It is used for those
		//            resoruces that are converted to a useable format upon load, such as a compressed
		//            file. If the raw buffer from the resource file isn't needed, it shouldn't take up
		//            any additional memory, so we release it.
		//
		if( loader->discardRawBufferAfterLoad() ) {
			JAM_DELETE_ARRAY(rawBuffer);
		}

		if( !success ) {
			// resource cache out of memory
			return nullptr;
		}
	}

	if (handle)	{
		m_lru.push_front(handle);
		m_resources[r->getName()] = handle;
	}

	JAM_ASSERT_MSG( loader != nullptr , "Default resource loader not found!" );
	return handle;		// ResCache is out of memory!
}

void ResourceManager::free(ResHandle* gonner)
{
	m_lru.remove( gonner );
	m_resources.erase( gonner->m_resource.getName() );
	// Note - the resource might still be in use by something,
	// so the cache can't actually count the memory freed until the
	// ResHandle pointing to it is destroyed.

	//m_allocated -= gonner->m_resource.m_size;
	//delete gonner;
}

bool ResourceManager::makeRoom(size_t size)
{
	if( size > m_cacheSize )	{
		return false;
	}

	// return null if there's no possible way to allocate the memory
	while( size > (m_cacheSize - m_allocated) ) {
		// The cache is empty, and there's still not enough room.
		if( m_lru.empty() )
			return false;

		freeOneResource();
	}

	return true;
}

char* ResourceManager::allocate(size_t size)
{
	if( !makeRoom(size) )
		return nullptr;

	char *mem = new char[size];
	if( mem != nullptr ) {
		m_allocated += size;
	}

	return mem;
}

void ResourceManager::freeOneResource()
{
	// get the last resource handle from the list
	ResHandleList::iterator gonner = m_lru.end();
	gonner--;

	ResHandle* handle = *gonner;

	m_lru.pop_back();							
	m_resources.erase( handle->m_resource.getName() );
	// Note - you can't change the resource cache size yet - the resource bits could still actually be
	// used by some sybsystem holding onto the ResHandle. Only when it goes out of scope can the memory
	// be actually free again.
}

//     This is called whenever the memory associated with a resource is actually freed
void ResourceManager::memoryHasBeenFreed(size_t size)
{
	m_allocated -= size;
}


//*****************************************************************************
DefaultResourceLoader::DefaultResourceLoader()
{
	m_patterns.clear() ;
	m_patterns.emplace_back("*.*") ;
}

bool DefaultResourceLoader::useRawFile() const
{
	return true;
}

size_t DefaultResourceLoader::getLoadedResourceSize(char* rawBuffer,size_t rawSize)
{
	return rawSize;
}

bool DefaultResourceLoader::loadResource(char* rawBuffer,size_t rawSize,ResHandle& handle)
{
	return true;
}

bool DefaultResourceLoader::discardRawBufferAfterLoad() const
{
	return true ;
}

bool DefaultResourceLoader::addNullZero() const
{
	return false ;
}

}
