/* ====================================================================
 * File: ScreensaverBlocker.h
 * Created: 07/06/10
 * Modified: 07/06/10
 * Author: Sun Jinbo
 * Copyright (c): Tieto, All rights reserved
 * ==================================================================== */

#ifndef C_SCREENSAVERBLOCKER_H
#define C_SCREENSAVERBLOCKER_H

// INCLUDE FILE
#include <e32base.h>

// FORWARD DECLARATIONS
class CRepository;

// CLASS DECLARATION

/**
 *  CScreenSaverBlocker
 */
class CScreenSaverBlocker : public CBase
	{
public:
	IMPORT_C static CScreenSaverBlocker* NewL();
	IMPORT_C virtual ~CScreenSaverBlocker();

public:
	IMPORT_C void DeactivateLightTimeoutAndScreenSaver();

	IMPORT_C void ActivateLightTimeoutAndScreenSaver();

private:
	void ConstructL();

private:
	TTimeIntervalMicroSeconds32 LightsTimeout();

	static TInt PeriodicCallBack(TAny*);

private:
	CPeriodic* iScreenSaverTimer;
	CRepository* iRepository;
	};

#endif // C_SCREENSAVERBLOCKER_H
