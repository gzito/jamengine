/**********************************************************************************
* 
* AudioManager.h
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

#ifndef __JAM_AUDIOMANAGER_H__
#define __JAM_AUDIOMANAGER_H__


#include <jam/jam.h>
#include <jam/Bank.h>
#include <jam/Singleton.h>
#include <jam/Timer.h>
#include <jam/String.h>

#include <map>
#include <al.h>

namespace jam
{

class JAM_API ISound : public BankItem
{
public:
	enum FadeControl
	{
		FC_NONE		= 0x00,
		FC_PAUSE	= 0x01,
		FC_STOP		= 0x02
	};

	static const float DEFAULT_SOUND_VOLUME ;
	static const float DEFAULT_SOUND_PITCH ;
	static const int32_t DEFAULT_SOUND_RATE ;

							ISound() ;
	virtual					~ISound() {} ;

	virtual void			play() = 0 ;
	virtual void			stop() = 0 ;
	virtual void			pause() = 0 ;
	virtual void			resume() = 0 ;

	virtual bool			isPlaying() const = 0 ;
	virtual bool			isPaused() const = 0 ;
	virtual bool			isStopped() const = 0 ;

	virtual float			getVolume() const { return m_volume; }
	virtual float			getPitch() const { return m_pitch; }
	virtual int32_t			getRate() const { return m_rate; }
	
	virtual void			setVolume(float val) { m_volume = val; }
	virtual void			setPitch(float val) { m_pitch = val ; }
	virtual void			setRate(int32_t val) { m_rate = val ; }

	void					playOnce() ;
	void					playForce() ;

	void					startFade(bool dir, float fadeTime, ISound::FadeControl actionOnCompletion=ISound::FC_NONE);
	bool					isFading() const { return m_fade; }
	virtual void			update(float fTime);

protected:
	void					_updateFade(float fTime=0.f);
	ALenum					getALFormat(int numOfChannels,int bitsPerSample);

private:
	float					m_fadeTimer;
	float					m_fadeTime;
	float					m_fadeInitVol;
	float					m_fadeEndVol;
	bool					m_fade;
	FadeControl				m_fadeEndAction;

	float					m_volume ;
	float					m_pitch ;
	int32_t					m_rate ;
};


class JAM_API Sound : public ISound
{
public:
							Sound( const String& filename, bool aLoop=false );
	virtual					~Sound();

	const char*				getFilename() const { return m_filename; };
	bool					isMono() const { return m_numOfChannels == 1; } ;
	bool					isStereo() const { return m_numOfChannels == 2; } ;
	int						getFrequency() const { return m_frequency; } ;		// in Hz, i.e. 44100
	int64_t					getNumOfSamples() const { return m_numOfSamples; } ;
	int						getBitsPerSample() const { return m_bitsPerSample; } ;
	int16_t*				getSoundData() const { return (int16_t*)m_pSoundData; } ;

	virtual void			play();
	virtual void			stop();
	virtual void			pause();
	virtual void			resume();

	int						getChannelId() const { return m_channelId; }

	virtual bool			isPlaying() const ;
	virtual bool			isPaused() const ;
	virtual bool			isStopped() const ;

	virtual float			getVolume() const ;
	virtual float			getPitch() const ;
	virtual int32_t			getRate() const ;

	virtual void			setVolume(float val) ;
	virtual void			setPitch(float val) ;
	virtual void			setRate(int32_t val) ;

protected:
	ALint					getSourceState() const ;

private:
	int						loadOggVorbisFile() ;
	int						loadRawFile() ;
	int						loadWaveFile() ;

	char					m_filename[256] ;
	char*					m_pSoundData ;
	int64_t					m_numOfSamples ;
	int						m_bitsPerSample ;
	int						m_frequency ;
	int						m_numOfChannels ;
	int						m_channelId ;
	bool					m_aLoop ;

	ALenum					m_format ;		// this represent both numOfChannels and bitsPerSample
	ALsizei					m_size ;		// numOfsamples
	ALsizei					m_freq ;		// frequency
	ALboolean				m_loop;

	ALuint					m_buffer ;
	ALuint					m_source ;
	ALfloat					m_sourcePos[3] ;
	ALfloat					m_sourceVel[3] ;
};


/**
* Audio
*
* \remark pitch and rate are not supported. Only one channel is currently supported
*/
class JAM_API StreamingSound : public ISound
{
public:
							StreamingSound( const String& filename, bool aLoop=false );
	virtual					~StreamingSound();

	const char*				getFilename() const { return m_filename; };

	virtual void			play();
	virtual void			stop();
	virtual void			pause();
	virtual void			resume();
	
	virtual bool			isPlaying() const ;
	virtual bool			isPaused() const ;
	virtual bool			isStopped() const ;

	virtual float			getVolume() const ;
	virtual void			setVolume(float val) ;

private:
	char					m_filename[256] ;
	bool					m_aLoop ;
};

class JAM_API AudioManager : public Singleton<AudioManager>, public Bank<ISound>
{
	friend class Singleton<AudioManager> ;

public:
	// sound handling 
	ISound*					loadSound( const String& afilename, const String& name, bool aloopFlag=false, float avolume=1.0f, float apitch=0.0f ) ;
	ISound*					loadSound( const String& afilename, int id, bool aloopFlag=false, float avolume=1.0f, float apitch=0.0f ) ;

	/// play the sound on a new channel unconditionally
	void					play( const String& itemName );
	void					play( int id );

	/// play the sound only if it isn't already playing
	void					playOnce( const String& itemName );
	void					playOnce( int id );

	/// play the sound again, stopping the current if it's already playing
	void					playForce( const String& itemName );
	void					playForce( int id );

	/// Sets the volume for all loaded sounds
	void					setSoundsVolume(float aVolume);

	void					pauseAllSounds();
	void					resumeAllSounds();
	void					stopAllSounds();

	/// Returns true is sound is supported in the device
	bool					isSoundAvailable() const ;

	/**
	* Return true if the sound device is in stereo mode.
	* This is enabled by setting the [S3E] StereoSound option in your ICF.
	* If the hardware does not support stereo this will always return false.
	*/
	bool					isStereoSoundEnabled() const ;

	// Returns the default sound volume
	float					getDefaultMasterVolume() const ;

	// Returns the master sound volume
	float					getMasterVolume() const ;

	/// Default frequency at which all channels will play at (in Hz) 
	int32_t					getDefaultFrequency() const ;

	/// Device output frequency
	int32_t					getOutputFrequency() const ;

	/// The number of (software) available audio channels
	int32_t					getMaxNumOfChannels() const ;

	/// A 32-bit mask where 0 in bit n indicates channel n is free. Any invalid channel will be marked with a '1'.
	int32_t					getUsedChannels() const ;

	int32_t					getFreeChannel() ;

	ISound*					getChannel(int channelIdx) { return m_channels[channelIdx]; }
	void					setChannel( int channelIdx, ISound* s );

	/// Sets master sound volume
	void					setMasterVolume(float val) ;

	/** 
	* Default frequency at which all channels will play at, in Hz. This defaults to 11000.
	* This has no effect on channels that play sound using the audio generation callbacks.
	* This value can be set to match the frequency of input audio samples.
	*/
	void					setDefaultFrequency(int32_t val) ;
	
	void					update(float fTime=0.f);	// time since last frame (in seconds)

	// music handling
	ISound*					loadMusic(const String& afilename, const String& name, bool aloopFlag=false, bool start=false);
	ISound*					loadMusic(const String& afilename, int id, bool aloopFlag=false, bool start=false);
	void					setMusicVolume(float volume);
	float					getMusicVolume() { return m_musicVolume; } ;
//	void					getMusicName(String val) { m_musicName = val; }
	void					musicOff(float fadeTime = 0.0f);
	void					musicOn(float fadeTime = 0.0f);
	void					musicPause();
	bool					isMusicPlaying();
	int32_t					getMaxNumOfMusicChannels() const ;

	Timer&					getUpdateTimer() { return *m_pUpdateTimer; }
	const Timer&			getUpdateTimer() const { return *m_pUpdateTimer; } 

private:
							AudioManager();
	virtual					~AudioManager();

	void					initOpenAL() ;
	void					cleanupOpenAL() ;

	ISound*					loadSound_private( const String& afilename, bool aloopFlag=false, float avolume=1.0f, float apitch=0.0f ) ;
	ISound*					loadMusic_private(const String& afilename,bool aloopFlag=false, bool start=false);
	/** 
	* Read the output frequency at which all channels will play at, in Hz. 
	* This has only effect on channels that play sound using the audio generation callbacks,
	* since it can be used to perform custom re-sampling from input to output frequency in the
	* callback. 
	* When not using a generate audio callback, s3eSound will
	* re-sample automatically based on the value of setDefaultFrequency()
	* It effectively set an internal property used by callbacks.
	*
	* Note that on supported devices, the output frequency can be set at
	* deploy time by using the [s3e] SoundOutputFreq ICF setting.	*/
	void					readOutputFrequency();

	int						m_outputRate;		     // Hardware output rate

	float					m_musicVolume;
	StreamingSound*			m_pMusicSound;

	ISound**				m_channels ;
	uint32_t				m_usedChannels ;

	Timer*					m_pUpdateTimer ;
};

JAM_INLINE AudioManager& GetAudioMgr() { return AudioManager::getSingleton(); }

}
#endif // __JAM_AUDIOMANAGER_H__
