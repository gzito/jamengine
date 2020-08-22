/**********************************************************************************
* 
* DeviceManager.h
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

#ifndef __JAM_DEVICEMANAGER_H__
#define __JAM_DEVICEMANAGER_H__

#include <jam/jam.h>

#include <jam/Singleton.h>


namespace jam
{
	
class JAM_API DeviceManager : public Singleton<DeviceManager>
{
	friend class Singleton<DeviceManager> ;
	
public:
	
	/** Returns the native device's display width, in pixels */
	int32_t					getNativeDisplayWidth() const ;

	/** Returns the native device's display height, in pixels */
	int32_t					getNativeDisplayHeight() const ;

	/** Returns device name */
	const char*				getDeviceIdName() const ;

	/** Returns device OS name */
	const char*				getDeviceOsName() const ;

	/** Returns device identifier */
	int32_t					getDeviceId() const ;
	
	/** Returns device OS identifier */
	int32_t					getDeviceOsId() const ;

	/** Returns true if device OS is IOS */
	bool					isIOS();

	/** Returns true if device OS is Android */
	bool					isAndroid();

	/** Returns true if device OS is Simulator */
	bool					isSimulator();

	/** Returns true if device is iPhone */
	bool					isiPhone();

	/** Returns true if device is iPhone 3g */
	bool					isiPhone3g();

	/** Returns true if device is iPhone 3gs */
	bool					isiPhone3gs();

	/** Returns true if device OS is iPhone 4 */
	bool					isiPhone4();

	/** Returns true if device OS is iPad */
	bool					isiPad();

	/** Returns true if device OS is iWindows */
	bool					isWindows();

	void					calculateLetterBox( int gameAreaW, int gameAreaH, float& scale, float& hPos, float& vPos ) ;
	void					calculateZoomToBiggerSide( int gameAreaW, int gameAreaH, float& scale, float& hPos, float& vPos );

private:
	// to prevent the use
							DeviceManager() = default ;
							~DeviceManager() = default ;
	DeviceManager&			operator=(const DeviceManager&) = delete ;
};

JAM_INLINE DeviceManager& GetDeviceMgr() { return DeviceManager::getSingleton(); }

}

#endif // __JAM_DEVICEMANAGER_H__
