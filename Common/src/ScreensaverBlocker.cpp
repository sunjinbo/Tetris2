/* ====================================================================
 * File: ScreensaverBlocker.cpp
 * Created: 07/06/10
 * Modified: 07/06/10
 * Author: Sun Jinbo
 * Copyright (c): Tieto, All rights reserved
 * ==================================================================== */

// INCLUDE FILES
#include <centralrepository.h>
#include "ScreensaverBlocker.h"

// CONSTANTS
const TUid KCRUidLightSettings = {0x10200C8C};
const TUint32 KDisplayLightsTimeout = 0x00000006;

// ---------------------------------------------------------------------------
// CScreenSaverBlocker::NewL()
// ---------------------------------------------------------------------------
//
EXPORT_C CScreenSaverBlocker* CScreenSaverBlocker::NewL()
	{
	CScreenSaverBlocker* self = new (ELeave) CScreenSaverBlocker;
	CleanupStack::PushL( self );
	self->ConstructL();
	CleanupStack::Pop( self );
	return self;
	}

// ---------------------------------------------------------------------------
// CScreenSaverBlocker::~CScreenSaverBlocker()
// ---------------------------------------------------------------------------
//
EXPORT_C CScreenSaverBlocker::~CScreenSaverBlocker()
	{
	delete iScreenSaverTimer;
	delete iRepository;
	}

// ---------------------------------------------------------------------------
// CScreenSaverBlocker::DeactivateLightTimeoutAndScreenSaverL()
// ---------------------------------------------------------------------------
//
EXPORT_C void CScreenSaverBlocker::DeactivateLightTimeoutAndScreenSaver()
	{
	if( !iScreenSaverTimer->IsActive() )
		{
		TCallBack callback( CScreenSaverBlocker::PeriodicCallBack, NULL);
		iScreenSaverTimer->Start( LightsTimeout(), LightsTimeout(), callback );
		}
	}

// ---------------------------------------------------------------------------
// CScreenSaverBlocker::ActivateLightTimeoutAndScreenSaver()
// ---------------------------------------------------------------------------
//
EXPORT_C void CScreenSaverBlocker::ActivateLightTimeoutAndScreenSaver()
    {
    iScreenSaverTimer->Cancel();
    }

// ---------------------------------------------------------------------------
// CScreenSaverBlocker::ConstructL()
// ---------------------------------------------------------------------------
//
void CScreenSaverBlocker::ConstructL()
	{
	iRepository = CRepository::NewL(KCRUidLightSettings);
	iScreenSaverTimer = CPeriodic::New( EPriorityNormal );
	DeactivateLightTimeoutAndScreenSaver();
	}

// ---------------------------------------------------------------------------
// CScreenSaverBlocker::GetLightsTimeoutValueL()
// ---------------------------------------------------------------------------
//
TTimeIntervalMicroSeconds32 CScreenSaverBlocker::LightsTimeout()
    {
    const TInt KMultiplyer = 1000000;
    TInt lightTimeoutValue(0);
    iRepository->Get(KDisplayLightsTimeout, lightTimeoutValue); // this is in seconds
    --lightTimeoutValue;
    TTimeIntervalMicroSeconds32 timeout = lightTimeoutValue * KMultiplyer; // convert to microseconds
    return timeout;
    }

// ---------------------------------------------------------------------------
// CScreenSaverBlocker::PeriodicCallBack()
// ---------------------------------------------------------------------------
//
TInt CScreenSaverBlocker::PeriodicCallBack(TAny*)
    {
    User::ResetInactivityTime();
    return ETrue;
    }

// End of file
