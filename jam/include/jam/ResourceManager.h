/**********************************************************************************
* 
* ResourceManager.h
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

#ifndef __JAM_RESOURCEMANAGER_H__
#define __JAM_RESOURCEMANAGER_H__


#include <jam/jam.h>
#include <jam/Resource.h>
#include <jam/core/interfaces.hpp>
#include <jam/thirdparty/Delegate.h>

#include <map>
#include <list>
#include <vector>

namespace jam
{

//*****************************************************************************
class JAM_API FileSystemResourceFile : public IResourceFile
{
public:
							FileSystemResourceFile(const String& resFileName);

	bool					open() override ;
	size_t					getRawResourceSize(const Resource& r) override;
	size_t					getRawResource(const Resource& r, char *buffer) override;
	size_t					getNumResources() const override;
	String					getResourceName(size_t num) const override;

	int						find(const String& path);

protected:
	void					readAssetsDirectory(const String& fileSpec = "" );

private:
	using ZipContentsMap = std::map<String, int> ;

	struct DirEntryData
	{
		String				m_fileName ;
		_off_t				m_fileSize ;
	};

	String					m_AssetsDir;
	std::vector<DirEntryData>	m_AssetFileInfo;
	ZipContentsMap			m_DirectoryContentsMap;
};

class ResourceManager ;

class JAM_API ResHandle
{
	friend class			ResourceManager ;

public:
							ResHandle( Resource& resource, char* buffer, size_t size, ResourceManager* pResManager ) ;
	virtual					~ResHandle() ;

	size_t					getSize() const ;

	char*					getBuffer() const ;

	char*					getWritableBuffer() ;

	sptr<IResourceExtraData>	getExtra() ;
	void					setExtra( sptr<IResourceExtraData> extra ) ;

	const Resource&			getResource() const ;

protected:
	Resource				m_resource ;

	// This can be raw buffer (in the case the resource doesn't need extra procession)
	// or the actual buffer. Memory is allocated and released by ResourceManager
	char*					m_buffer ;

	// The size of buffer
	size_t					m_size ;

	sptr<IResourceExtraData>	m_extra ;

	ResourceManager*		m_pResManager ;
};

JAM_INLINE size_t			ResHandle::getSize() const { return m_size; }
JAM_INLINE char*			ResHandle::getBuffer() const { return m_buffer; }
JAM_INLINE char*			ResHandle::getWritableBuffer() { return m_buffer; }
JAM_INLINE sptr<IResourceExtraData>	ResHandle::getExtra() { return m_extra; }
JAM_INLINE void				ResHandle::setExtra( sptr<IResourceExtraData> extra ) { m_extra = extra; }
JAM_INLINE const Resource&	ResHandle::getResource() const { return m_resource; }


//*****************************************************************************
class JAM_API IResourceLoader
{
public:
	// returns wildcard patterns used to distinguish which loaders are used with which files
	virtual const std::vector<String>&	getPatterns() const ;

	// specifies that the loader may directly use the bits stored in the raw file, i.e. no extra processing is needed
	// the memory for this buffer is managed by ResCache
	virtual bool			useRawFile() const = 0 ;

	// returns size of the loaded resources if it is different from the size stored in the file
	virtual size_t			getLoadedResourceSize( char* rawBuffer, size_t rawSize ) = 0 ;

	// specifies how actually the resource is loaded from file
	// rawBuffer and rawSize are initialized by the IResourceFile with the content and size of resource
	// handle is initialized by ResourceManager and reference the resource
	// returns true on successfull load, otherwise return false
	virtual bool			loadResource( char* rawBuffer, size_t rawSize, sptr<ResHandle> handle ) = 0 ;

	// specifies if raw buffer is useless after load, thus it can be discarded
	virtual bool			discardRawBufferAfterLoad() const = 0 ;

	// specifies if room for a null-terminated zero should be counted when allocating raw buffer
	virtual bool			addNullZero() const = 0 ;

	virtual					~IResourceLoader() = default ;

protected:
	std::vector<String>		m_patterns ;
};
JAM_INLINE const std::vector<String>&	IResourceLoader::getPatterns() const { return m_patterns; }


//*****************************************************************************
/**
    Manager of all game resources
*/
class JAM_API ResourceManager
{
	friend class			ResHandle;

public:
							ResourceManager( const size_t sizeInMb, IResourceFile* resFile ) ;
							~ResourceManager() ;
	bool					init() ;
	void					registerLoader( sptr<IResourceLoader> loader ) ;

	sptr<ResHandle>			getHandle( Resource* r ) ;
	int						preload( const String& pattern, SA::delegate<void(int,bool&)> progressDelegate ) ;
	void					flush() ;

protected:
	using					ResHandleList = std::list<sptr<ResHandle>> ;
	using					ResHandleMap = std::map<String, sptr<ResHandle>> ;
	using					ResourceLoaders = std::list<sptr<IResourceLoader>> ;

	ResHandleList			m_lru ;
	ResHandleMap			m_resources ;
	ResourceLoaders			m_resourceLoaders ;

	uptr<IResourceFile>		m_file ;

	size_t					m_cacheSize ;
	size_t					m_allocated ;

	sptr<ResHandle>			find( Resource* r ) ;
	void					update( sptr<ResHandle> handle ) ;
	sptr<ResHandle>			load( Resource* r ) ;
	void					free( sptr<ResHandle> gonner ) ;

	bool					makeRoom( size_t size ) ;
	char*					allocate( size_t size ) ;
	void					freeOneResource() ;
	void					memoryHasBeenFreed( size_t size ) ;
};


//*****************************************************************************
class JAM_API DefaultResourceLoader : public IResourceLoader
{
public:
							DefaultResourceLoader() ;

	bool					useRawFile() const override ;
	size_t					getLoadedResourceSize( char* rawBuffer, size_t rawSize ) override ;
	bool					loadResource( char* rawBuffer, size_t rawSize, sptr<ResHandle> handle ) override ;
	bool					discardRawBufferAfterLoad() const override ;
	bool					addNullZero() const override ;
};

}


#endif // #ifndef __JAM_RESOURCEMANAGER_H__
