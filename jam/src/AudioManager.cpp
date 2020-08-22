/**********************************************************************************
* 
* AudioManager.cpp
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

#include "StdAfx.h"

#include "jam/AudioManager.h"
#include "jam/core/filesystem.h"

#include <exception>
// #include <s3eSound.h>
// #include <s3eFile.h>
// #include <IwRuntime.h>
// #include <IwPath.h>

#include <al.h>
#include <alc.h>
#include <vorbisfile.h>

using namespace std ;

#define GEOM_ONE_FP8			0x100		// volume is in .8 fixed point format
#define GEOM_ONE_FP16		0x10000		// pitch is in .16 fixed point format
#define JAM_AUDIOMGR_LOAD_BUFFER_SIZE		512*1024			// in bytes, now is 512KB

namespace jam
{
	const float ISound::DEFAULT_SOUND_VOLUME = 0.5f;
	const float ISound::DEFAULT_SOUND_PITCH = 1.0f ;
	const int32_t ISound::DEFAULT_SOUND_RATE = 11000 ;

	ISound::ISound() :
		m_volume(DEFAULT_SOUND_VOLUME),
		m_pitch(DEFAULT_SOUND_PITCH),
		m_rate(DEFAULT_SOUND_RATE),
		m_fadeTimer(0.0f),
		m_fadeTime(1.0f),
		m_fadeInitVol(0),
		m_fadeEndVol(1),
		m_fade(false),
		m_fadeEndAction(ISound::FC_NONE)
	{
	}

	void ISound::startFade(bool dir, float fadeTime, ISound::FadeControl actionOnCompletion/*=ISound::FC_NONE*/)
	{
		m_fade = true;
		m_fadeInitVol = dir ? 0.0f : getVolume();
		m_fadeEndVol  = dir ? 1.0f : 0.0f;
		m_fadeTimer = 0.0f;
		m_fadeEndAction = actionOnCompletion;
		m_fadeTime = fadeTime;
		// Automatically start if not currently playing
		if ( m_fadeEndVol == 1.0f )
			if ( !isPlaying() )
				this->play();
	}

	void ISound::update(float fTime)
	{
		_updateFade(fTime);
	}

	void ISound::_updateFade(float fTime/*=0.f*/)
	{
		if (m_fade) {
			if ( (m_fadeTimer+=fTime) > m_fadeTime ) {
				setVolume(m_fadeEndVol);
				m_fade = false;
				// Perform requested action on completion
				// NOTE:- Must go through SoundManager when using threads to avoid any corruption/mutex issues.
				switch ( m_fadeEndAction )
				{
				case FC_PAUSE:
					{
						pause();
					}
					break;
				case FC_STOP:
					{
						stop();
					}
					break;
				}
			}
			else {
				float vol = (m_fadeEndVol-m_fadeInitVol)*(m_fadeTimer/m_fadeTime);
				setVolume(m_fadeInitVol + vol);
			}
		}
	}

	void ISound::playOnce()
	{
		if( !isPlaying() ) {
			play();
		}
	}

	void ISound::playForce()
	{
		if( isPlaying() ) {
			stop() ;
		}
		play() ;
	}

	ALenum ISound::getALFormat(int numOfChannels,int bitsPerSample)
	{
		ALenum format = 0 ;
		if( numOfChannels == 1 && bitsPerSample == 8 ) {
			format = AL_FORMAT_MONO8 ;
		}
		else if( numOfChannels == 1 && bitsPerSample == 16 ) {
			format = AL_FORMAT_MONO16 ;
		}
		else if( numOfChannels == 2 && bitsPerSample == 8 ) {
			format = AL_FORMAT_STEREO8 ;
		}
		else if(numOfChannels == 2 && bitsPerSample == 16 ) {
			format = AL_FORMAT_STEREO16 ;
		}

		return format ;
	}

	/**
		Sound
	*/
	Sound::Sound( const String& filename, bool aLoop/*=false*/ ) :
		ISound(),
		m_pSoundData(0),
		m_numOfSamples(0),
		m_bitsPerSample(0),
		m_frequency(0),
		m_numOfChannels(false),
		m_channelId(-1),
		m_aLoop(aLoop),
		m_buffer(0),
		m_source(0)
	{
		ALenum alErr ;

		if( filename.empty() ) {
			JAM_ERROR( ("Empty audio file name") ) ;
		}

		strcpy(m_filename,filename.c_str()) ;

		// get extension
		char ext[32] = {0};
		jam::getFileNameExtension(m_filename,ext) ;

		int errCode = 0 ;
		if( !strcmp(ext,"ogg") ) {
			errCode  = loadOggVorbisFile() ;
		}
		else if( !strcmp(ext,"wav") ) {
			errCode = loadWaveFile() ;
		}
		else if( !strcmp(ext,"raw") ) {
			errCode  = loadRawFile() ;
		}
		else {
			JAM_ERROR( ("Unsupported type of audio file: %s", filename.c_str()) ) ;
		}

		// check for error
		if( errCode ) {
			JAM_ERROR( ("Error loading file: %s", filename.c_str()) ) ;
		}

		// source position 
		m_sourcePos[0] = 0.0f ;
		m_sourcePos[1] = 0.0f ;
		m_sourcePos[2] = 0.0f ;
		
		// source velocity
		m_sourceVel[0] = 0.0f ;
		m_sourceVel[1] = 0.0f ;
		m_sourceVel[2] = 0.0f ;
		
		// create AL buffer
		alGenBuffers(1,&m_buffer) ;
		if( (alErr = alGetError()) != AL_NO_ERROR ) {
			JAM_ERROR( "Error generating AL buffer: %d", alErr ) ;
		}

		m_format = getALFormat(m_numOfChannels,m_bitsPerSample) ;
		if( !m_format ) {
			JAM_ERROR("Unsupported AL format") ;
		}
		alBufferData( m_buffer, m_format, m_pSoundData, m_numOfSamples*(m_bitsPerSample / 8), m_frequency ) ;
		if((alErr = alGetError()) != AL_NO_ERROR) {
			JAM_ERROR("Error buffering AL data: %d",alErr) ;
		}

		JAM_DELETE(m_pSoundData) ;

		alGenSources(1,&m_source) ;
		if((alErr = alGetError()) != AL_NO_ERROR) {
			JAM_ERROR("Error generating AL source: %d",alErr) ;
		}

		alSourcei ( m_source,AL_BUFFER, m_buffer );
		alSourcef ( m_source,AL_PITCH, 1.0f );
		alSourcef ( m_source,AL_GAIN, 1.0f );
		alSourcefv( m_source,AL_POSITION, m_sourcePos );
		alSourcefv( m_source,AL_VELOCITY, m_sourceVel );
		alSourcei ( m_source,AL_LOOPING, (m_aLoop ? AL_TRUE : AL_FALSE) );
	}

	Sound::~Sound()
	{
		if( m_buffer ) alDeleteBuffers(1, &m_buffer) ;
		if( m_source ) alDeleteSources(1, &m_source ) ;
	}

	int Sound::loadOggVorbisFile()
	{
		OggVorbis_File	vf ;

		// TODO handle file not found
		FILE* f = fopen(m_filename,"rb");
		if( !f ) {
			return -1 ;
		}

		int errorCode = ov_open(f,&vf,0,0) ;
		if( errorCode ) {
			return -1 ;
		}

		vorbis_info* vi = ov_info( &vf, -1 ) ;

		m_numOfChannels = vi->channels ;	// 2 for stereo
		m_frequency = vi->rate ;			// in Hz, e.g. 44100
		m_bitsPerSample = 16 ;

		// retrieves the total pcm samples for the entire physical bitstream
		m_numOfSamples = ov_pcm_total(&vf,-1) ;

		ogg_int64_t size = m_numOfSamples * (m_bitsPerSample/8) * m_numOfChannels ;
		m_pSoundData = new char[size] ;

		uptr<char> buf( new char[JAM_AUDIOMGR_LOAD_BUFFER_SIZE] ) ;

		int current_section ;
		long i = 0 ;
		bool eof = false ;

		while( !eof ) {
			long bytesRead = ov_read(
				&vf,				// A pointer to the OggVorbis_File structure
				buf.get(),				// A pointer to an output buffer
				JAM_AUDIOMGR_LOAD_BUFFER_SIZE,				// Number of bytes to be read into the buffer. Should be the same size as the buffer
				0,					// Specifies big or little endian byte packing. 0 for little endian, 1 for big endian.
				2,					// Specifies word size. Possible arguments are 1 for 8-bit samples, or 2 or 16-bit samples
				1,					// Signed or unsigned data. 0 for unsigned, 1 for signed. Typically 1.
				&current_section	// [out] A pointer to the number of the current logical bitstream.
				) ;

			if( bytesRead > 0 ) {
				memcpy( &m_pSoundData[i], buf.get(), bytesRead ) ;
				i += bytesRead ;
			}
			else {
				eof = true ;
			}
		}

		ov_clear(&vf) ;

		return 0 ;
	}

	int Sound::loadRawFile()
	{
		int retCode = 0 ;

		m_numOfChannels = 1 ;
		m_frequency = 44100 ;
		m_bitsPerSample = 16 ;
		int64_t size = jam::getFileSize(m_filename) ;
		if( size == -1 ) {
			return -1 ;
		}
		m_numOfSamples = size/2 ;
		m_pSoundData = new char[size] ;
		FILE* f = fopen(m_filename,"rb") ;
		if( !f ) {
			return -1 ;
		}
		int32_t bytesRead = fread(m_pSoundData,1,(int32_t)size,f) ;
		if( bytesRead != size ) {
			retCode = -1 ;
		}
		fclose(f) ;

		return retCode ;
	}

	// Note: supports only PCM waves, other formats (e.g. compressed ADPCM) are not supported
	int Sound::loadWaveFile()
	{
		// https://ccrma.stanford.edu/courses/422/projects/WaveFormat/

		FILE* f = fopen(m_filename,"rb");
		if (!f) {
			return -1 ;
		}

		uint8_t id[4]; //four bytes to hold 'RIFF'
		int32_t size; //32 bit value to hold file size
		uint16_t format_tag, channels, block_align, bits_per_sample;
		int32_t format_length, sample_rate, avg_bytes_sec, data_size;

		//read in first four bytes
		fread(id, 1, 4, f);
		if (memcmp(id, "RIFF",4)) {
			return -1 ;
		}

		fread(&size, 4, 1, f); //read in 32bit size value
		fread(id, 1, 4, f); //read in 4 byte String now
		if (memcmp(id,"WAVE",4)) {
			return -1 ;
		}

		fread(id, 1, 4, f); //read in 4 bytes 'fmt '
		fread(&format_length, 4, 1,f);
		fread(&format_tag, 2, 1, f); // audio format, we handle only PCM
		if( format_tag != 1 ) {
			return -2 ;
		}
		fread(&channels, 2, 1,f); //1 mono, 2 stereo
		m_numOfChannels = channels ;

		fread(&sample_rate, 4, 1, f); //like 44100, 22050, etc...
		m_frequency = sample_rate ;

		fread(&avg_bytes_sec, 4, 1, f);
		fread(&block_align, 2, 1, f);
		fread(&bits_per_sample, 2, 1, f); //8 bit or 16 bit file
		m_bitsPerSample = bits_per_sample ;

		fread(id, 1, 4, f); //read in 'data'
		fread(&data_size, 4, 1, f); //how many bytes of sound data we have
		m_pSoundData = new char[data_size] ;
		fread(m_pSoundData, 1, data_size, f); //read in our whole sound data chunk

		m_numOfSamples = data_size / 2 ;

		fclose(f) ;
		return 0 ;
	}

	void Sound::play()
	{
		m_channelId = GetAudioMgr().getFreeChannel() ;
		if( m_channelId == -1 ) {
			return ;
		}
		GetAudioMgr().setChannel(m_channelId,this) ;
		alSourcePlay(m_source);
		setVolume( ISound::getVolume() ) ;
		setPitch( ISound::getPitch() ) ;
		setRate( ISound::getRate() ) ;
	}

	void Sound::stop()
	{
		if( isPlaying() ) {
			alSourceStop(m_source) ;
			GetAudioMgr().setChannel(m_channelId,0) ;
			m_channelId = -1 ;
		}
	}

	void Sound::pause()
	{
		if( isPlaying() ) {
			alSourcePause(m_source) ;
		}
	}

	void Sound::resume()
	{
		if( isPaused() ) {
			alSourcePlay(m_source);
		}
	}

	ALint Sound::getSourceState() const
	{
		ALint sourceState = 0 ;

		int chId = getChannelId() ;
		if(chId != -1) {
			if(GetAudioMgr().getChannel(chId) == this) {
				alGetSourcei(m_source,AL_SOURCE_STATE,&sourceState) ;
			}
		}

		return sourceState ;
	}

	bool Sound::isPlaying() const
	{
		return getSourceState() == AL_PLAYING ;
	}

	bool Sound::isPaused() const
	{
		return getSourceState() == AL_PAUSED ;
	}

	bool Sound::isStopped() const
	{
		return getSourceState() == AL_STOPPED ;
	}

	float Sound::getVolume() const
	{
		float volume = 0 ;

		if( isPlaying() || isPaused() ) {
			alGetSourcef(m_source,AL_GAIN,&volume) ;
		}
		else {
			volume = ISound::getVolume() ;
		}

		return volume ;
	}

	float Sound::getPitch() const
	{
		float pitch = 0 ;

		if( isPlaying() || isPaused() ) {
			alGetSourcef(m_source,AL_GAIN,&pitch) ;
		}
		else {
			pitch = ISound::getPitch() ;
		}

		return pitch ;
	}

	int32_t Sound::getRate() const
	{
		return ISound::getRate() ;
	}

	// 0 <= val <= 1.0f
	void Sound::setVolume(float val)
	{
		ISound::setVolume(val);
		if( isPlaying() || isPaused() ) {
			alSourcef(m_source,AL_GAIN,val) ;
		}
	}

	void Sound::setPitch(float val)
	{
		ISound::setPitch(val);
		if( isPlaying() || isPaused() ) {
			alSourcef(m_source,AL_PITCH,val) ;
		}
	}

	void Sound::setRate(int32_t val)
	{
		ISound::setRate(val) ;
	}

	/**
		Audio
	*/
	StreamingSound::StreamingSound( const String& filename, bool aLoop/*=false*/ ) :
		ISound(),
		m_aLoop(aLoop)
	{
		strcpy(m_filename,filename.c_str()) ;
	}

	StreamingSound::~StreamingSound()
	{
		stop() ;
	}

	void StreamingSound::play()
	{
	}

	void StreamingSound::stop()
	{
	}

	void StreamingSound::pause()
	{
	}

	void StreamingSound::resume()
	{
	}

	bool StreamingSound::isPlaying() const
	{
		return false ;
	}

	bool StreamingSound::isPaused() const
	{
		 return false ;
	}

	bool StreamingSound::isStopped() const
	{
		return true ;
	}

	float StreamingSound::getVolume() const
	{
		float volume = 0 ;
		return volume ;
	}

	void StreamingSound::setVolume( float val )
	{
	}

	/**
		AudioManager
	*/
	AudioManager::AudioManager()
		: Bank<ISound>(), m_musicVolume(0.5f), m_pMusicSound(0), m_pUpdateTimer(0),
		m_outputRate(0), m_usedChannels(0)
	{
		initOpenAL();

		m_channels = new ISound*[getMaxNumOfChannels()] ;
		for(int32_t i=0; i<getMaxNumOfChannels(); i++) {
			m_channels[i] = 0 ;
		}

		m_pUpdateTimer = Timer::create() ;
		m_pUpdateTimer->setReserved() ;
	}

	AudioManager::~AudioManager()
	{
// 		stopAllSounds() ;
// 		musicOff() ;
		cleanupOpenAL();

		JAM_RELEASE_NULL(m_pMusicSound);
		JAM_RELEASE_NULL(m_pUpdateTimer) ;
	}	

	void AudioManager::initOpenAL()
	{
		ALCdevice* pDevice = alcOpenDevice(NULL) ;
		ALCcontext* pContext = alcCreateContext(pDevice,NULL) ;
		alcMakeContextCurrent(pContext) ;

		// setup listener

		// Position of the listener.
		ALfloat listenerPos[] = { 0.0f, 0.0f, 0.0f } ;
		// Velocity of the listener.
		ALfloat listenerVel[] = { 0.0f, 0.0f, 0.0f } ;
		// Orientation of the listener. (first 3 elements are "at", second 3 are "up")
		ALfloat listenerOri[] = { 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f} ;
		alListenerfv(AL_POSITION,listenerPos);
		alListenerfv(AL_VELOCITY,listenerVel);
		alListenerfv(AL_ORIENTATION,listenerOri);
	}

	void AudioManager::cleanupOpenAL()
	{
		ALCcontext* pContext = alcGetCurrentContext() ;
		ALCdevice* pDevice = alcGetContextsDevice(pContext) ;
		alcMakeContextCurrent(NULL) ;
		alcDestroyContext(pContext) ;
		alcCloseDevice(pDevice) ;
	}

	ISound* AudioManager::loadSound(const String& afilename,const String& name,bool aloopFlag/*=false*/,float avolume/*=1.0f*/,float apitch/*=0*/)
	{
		ISound* iSound = loadSound_private(afilename,aloopFlag,avolume,apitch) ;
		addByName(iSound,name);
		return iSound ;
	}

	ISound* AudioManager::loadSound( const String& afilename, int id, bool aloopFlag/*=false*/, float avolume/*=1.0f*/, float apitch/*=0*/)
	{
		ISound* iSound = loadSound_private(afilename,aloopFlag,avolume,apitch) ;
		addById(iSound,id);
		return iSound ;
	}

	void AudioManager::play( const String& itemName )
	{
		ISound& iSound = getByName(itemName) ;
		iSound.play() ;
	}

	void AudioManager::play( int id )
	{
		ISound& iSound = getById(id) ;
		iSound.play() ;
	}

	void AudioManager::playOnce( const String& itemName )
	{
		ISound& iSound = getByName(itemName) ;
		iSound.playOnce() ;
	}

	void AudioManager::playOnce( int id )
	{
		ISound& iSound = getById(id) ;
		iSound.playOnce() ;
	}

	void AudioManager::playForce( const String& itemName )
	{
		ISound& iSound = getByName(itemName) ;
		iSound.playForce() ;
	}

	void AudioManager::playForce( int id )
	{
		ISound& iSound = getById(id) ;
		iSound.playForce() ;
	}

	void AudioManager::setSoundsVolume(float aVolume)
	{
		BankItemMap::iterator it ;
		for( it = m_objectsMap.begin(); it != m_objectsMap.end(); it++ ) {
			ISound* sound = (ISound*)it->second ;
			if( sound != m_pMusicSound ) {
				sound->setVolume(aVolume) ;
			}
		}
	}

	ISound* AudioManager::loadMusic( const String& afilename, const String& name, bool aloopFlag/*=false*/, bool start/*=false*/ )
	{
		ISound* iSound = loadMusic_private(afilename,aloopFlag,start) ;
		addByName(iSound,name); ;
		return iSound ;
	}

	ISound* AudioManager::loadMusic( const String& afilename, int id, bool aloopFlag/*=false*/, bool start/*=false*/ )
	{
		ISound* iSound = loadMusic_private(afilename,aloopFlag,start) ;
		addById(iSound,id); ;
		return iSound ;
	}

	void AudioManager::musicOff( float fadeTime/*=0.0f*/ )
	{
		if (m_pMusicSound && !m_pMusicSound->isStopped()) {
			if (fadeTime) m_pMusicSound->startFade(false, fadeTime);
			m_pMusicSound->stop();
		}
	}

	void AudioManager::musicOn( float fadeTime /*= 0.0f*/ )
	{
		if (m_pMusicSound && !m_pMusicSound->isPlaying()) {
			if (fadeTime) m_pMusicSound->startFade(true, fadeTime);
			m_pMusicSound->play();
		}
	}

	void AudioManager::musicPause()
	{
		if (m_pMusicSound && !m_pMusicSound->isPaused()) {
			m_pMusicSound->pause();
		}
	}

	void AudioManager::setMusicVolume(float volume)
	{
		if (m_pMusicSound) {
			m_pMusicSound->setVolume(volume);
		}
	}

	bool AudioManager::isMusicPlaying()
	{
		if (m_pMusicSound) {
			return m_pMusicSound->isPlaying();
		}
		else {
			return false;
		}
	}

	int32_t AudioManager::getMaxNumOfMusicChannels() const
	{
		return 1 ;
	}

	void AudioManager::pauseAllSounds()
	{
		for(int32_t i=0; i<getMaxNumOfChannels(); i++) {
			if( m_channels[i] ) m_channels[i]->pause() ;
		}
	}

	void AudioManager::resumeAllSounds()
	{
		for(int32_t i=0; i<getMaxNumOfChannels(); i++) {
			if( m_channels[i] ) m_channels[i]->resume() ;
		}
	}

	void AudioManager::stopAllSounds()
	{
		for(int32_t i=0; i<getMaxNumOfChannels(); i++ ) {
			if( m_channels[i] ) m_channels[i]->stop() ;
			m_channels[i] = 0 ;
		}
		m_usedChannels = 0 ;
	}

	bool AudioManager::isSoundAvailable() const
	{
		// fixit!
		return true ;
	}

	bool AudioManager::isStereoSoundEnabled() const
	{
		// fixit!
		return true ;
	}

	float AudioManager::getDefaultMasterVolume() const
	{
		// fixit!
		return 0 ;
	}

	float AudioManager::getMasterVolume() const
	{
		// fixit!
		return 0.5f ;
	}

	int32_t AudioManager::getDefaultFrequency() const
	{
		// fixit!
		return 22050 ;
	}

	int32_t AudioManager::getOutputFrequency() const
	{
		// fixit!
		return 22050 ;
	}

	int32_t AudioManager::getMaxNumOfChannels() const
	{
		// fixit!
		return 32 ;
	}

	int32_t AudioManager::getUsedChannels() const
	{
		return m_usedChannels ;
	}

	int32_t AudioManager::getFreeChannel()
	{
		int32_t newChannel = -1 ;
		for(int32_t i=0; i<getMaxNumOfChannels(); i++) {
			if( (m_channels[i] == 0) || (m_channels[i] && m_channels[i]->isStopped()) ) { 
				newChannel = i ;
				m_usedChannels |= 1 << i ;
				break ;
			}
		}

		JAM_ASSERT_MSG( newChannel>=0, "No audio channel available" ) ;
		return newChannel ;
	}

	void AudioManager::setChannel(int channelIdx,ISound* s)
	{
		m_channels[channelIdx] = s;
		m_usedChannels |= 1 << channelIdx ;
	}

	void AudioManager::setMasterVolume(float val)
	{
		// do nothing
	}

	void AudioManager::setDefaultFrequency(int32_t val)
	{
		// do nothing
	}

	void AudioManager::readOutputFrequency()
	{
		// do nothing
	}

	void AudioManager::update(float fTime/*=0.f*/)
	{
		JAM_BANK_ITERATION_START ;

		BankItemMap::iterator it ;
		for( it = m_objectsMap.begin(); it != m_objectsMap.end(); it++ ) {
			ISound* s = (ISound*)it->second ;
			s->update(fTime) ;
		}

		JAM_BANK_ITERATION_END ;
	}


	ISound* AudioManager::loadSound_private( const String& afilename, bool aloopFlag/*=false*/, float avolume/*=1.0f*/, float apitch/*=0.0f */ )
	{
		ISound* iSound = JAM_ALLOC_2(Sound, afilename, aloopFlag) ;
		iSound->autorelease();
		iSound->setVolume(avolume) ;
		iSound->setPitch(apitch) ;
		return iSound ;
	}

	ISound* AudioManager::loadMusic_private( const String& afilename,bool aloopFlag/*=false*/, bool start/*=false*/ )
	{
		if( m_pMusicSound ) {
			musicOff() ;
			m_pMusicSound->release() ;
			m_pMusicSound->destroy() ;
		}

		ISound* iSound = JAM_ALLOC_2(StreamingSound,afilename, aloopFlag) ;
		iSound->autorelease();
		iSound->setVolume(m_musicVolume) ;
		if( start ) {
			iSound->play();
		}

		m_pMusicSound = (StreamingSound*)iSound;
		m_pMusicSound->addRef() ;

		return iSound ;
	}

}