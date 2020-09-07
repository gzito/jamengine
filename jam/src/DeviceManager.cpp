/**********************************************************************************
* 
* DeviceManager.cpp
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

#include "jam/DeviceManager.h"
#include "jam/Application.h"

namespace jam
{

int32_t DeviceManager::getNativeDisplayWidth() const
{
	int width, height;
	SDL_GL_GetDrawableSize(GetAppMgr().getWindowPtr(), &width, &height);
	return width ;
}

int32_t DeviceManager::getNativeDisplayHeight() const
{
	int width, height;
	SDL_GL_GetDrawableSize(GetAppMgr().getWindowPtr(), &width, &height);
	return height ;
}


/*
	You can retrieve a device's ID with these calls:
	s3eDeviceGetString(S3E_DEVICE_ID) & s3eDeviceGetInt(S3E_DEVICE_ID)

	Current iOS Device List:

	iPhone3G				iOS		"iPhone1,2"		DE4D7697
	iPhone3GS			iOS		"iPhone2,1"		DE4D7AD7	(320x480)
	iPhone4				iOS		"iPhone3,1"		DE4D7F18	(640x960)
	iPhone5				iOS		""					(640x1136)
	iTouch 1st gen		iOS		"iPod1,1"		590F86FF
	iTouch 2nd gen		iOS		"iPod2,1"		590F8B40
	iPad wifi			iOS		"iPad1,1"		58122FF1	(768x1024)
	Samsung Galaxy S	Android	"GT-I9000"		DC17BD5F	(480x800)
	LG Optimus			Android	"LG-P500"		7101388A	(320x480)		
*/


bool DeviceManager::isIOS()
{
	return false ;
}

bool DeviceManager::isAndroid()
{
	return false ;
}

bool DeviceManager::isSimulator()
{
	return false ;
}
bool DeviceManager::isWindows()
{
	return true ;
}

bool DeviceManager::isiPhone()
{
	return false ;
}

bool DeviceManager::isiPhone3g()
{
	return false ;
}
// bool DeviceManager::isiPhone5()
// {
// 	return s3eDeviceGetInt(S3E_DEVICE_ID) == (int32_t)0xDE4D7697 ;
// }
bool DeviceManager::isiPhone3gs()
{
	return false ;
}

bool DeviceManager::isiPhone4()
{
	return false ;
}

bool DeviceManager::isiPad()
{
	return false ;
}

const char* DeviceManager::getDeviceIdName() const
{
	return "DeviceIdName" ;
}

const char* DeviceManager::getDeviceOsName() const
{
	return "Windows" ;
}

int32_t DeviceManager::getDeviceId() const
{
	return 0 ;
}

int DeviceManager::getDeviceOsId() const
{
	return 0 ;
}

void DeviceManager::calculateLetterBox( int gameAreaW, int gameAreaH, float& scale, float& hPos, float& vPos )
{
	int nativeWidth = getNativeDisplayWidth() ;
	int nativeHeight = getNativeDisplayHeight() ;

	float gameIdealScreenRatio = gameAreaW / (float)gameAreaH ;
	float nativeScreenRatio = nativeWidth / (float)nativeHeight ;

	if( nativeScreenRatio > gameIdealScreenRatio ) {
		// the game will have space to the left and right side of the game area
		scale = nativeHeight / (float)gameAreaH ;
		hPos = (nativeWidth/2.0f)-(gameAreaW/2.0f) ;
		vPos = 0 ;
	}
	else if (nativeScreenRatio < gameIdealScreenRatio ) {
		// the game will have space at the top and bottom of the game area
		scale = nativeWidth / (float)gameAreaW ;
		hPos = 0 ;
		vPos = (nativeHeight/2.0f)-(gameAreaH/2.0f) ;
	}
	else {
		// same ratio
		scale = nativeWidth / (float)gameAreaW ;
		hPos = 0 ;
		vPos = 0 ;
	}
}

void DeviceManager::calculateZoomToBiggerSide( int gameAreaW, int gameAreaH, float& scale, float& hPos, float& vPos )
{
/*
	int nativeWidth = getNativeDisplayWidth() ;
	int nativeHeight = getNativeDisplayHeight() ;

	float gameIdealScreenRatio = gameAreaW / (float)gameAreaH ;
	float nativeScreenRatio = nativeWidth / (float)nativeHeight ;

	if( nativeScreenRatio > gameIdealScreenRatio ) {
		// 
		scale = nativeHeight / (float)gameAreaH ;
		hPos = 0 ;
		vPos = 0 ;
	}
	else if (nativeScreenRatio < gameIdealScreenRatio ) {
		// 
		scale = nativeWidth / (float)gameAreaW ;
		hPos = 0 ;
		vPos = 0 ;
	}
	else {
		// same ratio
		scale = nativeWidth / (float)gameAreaW ;
		hPos = 0 ;
		vPos = 0 ;
	}
*/
}


//	320dp: a typical phone screen (240x320 ldpi, 320x480 mdpi, 480x800 hdpi, etc).
//	480dp: a tweener tablet like the Streak (480x800 mdpi).
//	600dp: a 7” tablet (600x1024 mdpi).
//	720dp: a 10” tablet (720x1280 mdpi, 800x1280 mdpi, etc).
/*
	To create alternative bitmap drawables for different densities, 
	you should follow the 3:4:6:8 scaling ratio between the four generalized densities. 
	For example, if you have a bitmap drawable that's 48x48 pixels for medium-density screen (the size for a launcher icon), 
	all the different sizes should be:

	36x36 for low-density
	48x48 for medium-density
	72x72 for high-density
	96x96 for extra high-density

	s3eSurfaceGetInt(S3E_SURFACE_DEVICE_WIDTH_QUANTISED)=320
	xxxhdpi	(640 dpi).
	xxhdpi	(480 dpi)
	xhdpi		(320 dpi)
	hdpi		(240 dpi)
	mdpi		(160 dpi)
	ldpi		(120 dpi)


	xlarge screens are at least 960dp x 720dp 
	1024x600	WXGA 
	(1280x800)
	1024x768
	1280x768	
	1536x1152
	1920x1152 
	1920x1200	
	2048x1536
	2560x1536 
	2560x1600

	large screens are at least 640dp x 480dp
			WVGA800 (480x800) 
			WVGA854 (480x854)	
			WVGA800 (480x800) 
			WVGA854 (480x854) 
			600x1024		
	normal screens are at least 470dp x 320dp 
		WQVGA400 (240x400)
		WQVGA432 (240x432)	
		HVGA (320x480)	
		WVGA800 (480x800) 
		WVGA854 (480x854) 
		600x1024	640x960
	small screens are at least 426dp x 320dp:
		QVGA (240x320)		
		480x640	

	// The gesture threshold expressed in dp
	private static final float GESTURE_THRESHOLD_DP = 16.0f;

	// Get the screen's density scale
	final float scale = getResources().getDisplayMetrics().density;
	// Convert the dps to pixels, based on density scale
	mGestureThreshold = (int) (GESTURE_THRESHOLD_DP * scale + 0.5f);

	// Use mGestureThreshold as a distance in pixels...

*/

}
