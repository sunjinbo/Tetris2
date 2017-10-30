/* ====================================================================
 * File: MatrixTask.cpp
 * Created: 04/19/09
 * Modified: 04/19/09
 * Author: Sun Jinbo
 * Copyright (c): Tieto, All rights reserved
 * ==================================================================== */

// INCLUDE FILES
#include <e32math.h>
#include <AudioUtils.h>
#include "Te2AudioTask.h"

// CONSTANTS

// ======== MEMBER FUNCTIONS ========

// ----------------------------------------------------------------------------
// CTe2AudioTask::NewL
// ----------------------------------------------------------------------------
//
CTe2AudioTask* CTe2AudioTask::NewL( MAudioTaskObserver& aObserver )
    {
    CTe2AudioTask* self = new (ELeave) CTe2AudioTask( aObserver );
    CleanupStack::PushL (self );
    self->ConstructL ();
    CleanupStack::Pop();
    return self;
    }

// ----------------------------------------------------------------------------
// CTe2AudioTask::~CTe2AudioTask
// ----------------------------------------------------------------------------
//
CTe2AudioTask::~CTe2AudioTask()
    {
    }

// ----------------------------------------------------------------------------
// CTe2AudioTask::SetRequest
// ----------------------------------------------------------------------------
//
void CTe2AudioTask::SetRequest( const TAudioRequest& aRequest )
    {
    iRequest = aRequest;
    }

// ----------------------------------------------------------------------------
// CTe2AudioTask::AudioRequest
// ----------------------------------------------------------------------------
//
TAudioRequest CTe2AudioTask::AudioRequest() const
    {
    return iRequest;
    }

// ----------------------------------------------------------------------------
// CTe2AudioTask::Start
// ----------------------------------------------------------------------------
//
void CTe2AudioTask::Start()
    {
    if ( iRequest.iAudioUtilsPointer )
        {
        TTime time;
        time.HomeTime();
        TTimeIntervalMicroSeconds interval 
            = time.MicroSecondsFrom( iRequest.iTime );
        
        if ( interval < iRequest.iExpiriedInterval )
            {
            iRequest.iAudioUtilsPointer->SetAudioPlayObserver( this );
            iRequest.iAudioUtilsPointer->Play();
            }
        else
            {
            iObserver.TaskCompleted();
            }
        }
    }

// ----------------------------------------------------------------------------
// CTe2AudioTask::Compare
// ----------------------------------------------------------------------------
//
TInt CTe2AudioTask::Compare( const CTe2AudioTask& aFirst, const CTe2AudioTask& aSecond )
    {
    if ( aFirst.iRequest.iPriority < aSecond.iRequest.iPriority )
     return 1; 
    if ( aFirst.iRequest.iPriority > aSecond.iRequest.iPriority )
     return -1;
    return 0;
    }

// ----------------------------------------------------------------------------
// CTe2AudioTask::AudioPlayComplete
// ----------------------------------------------------------------------------
//
void CTe2AudioTask::AudioPlayComplete( TInt /*aError*/ )
    {
    iObserver.TaskCompleted();    
    }

// ----------------------------------------------------------------------------
// CTe2AudioTask::CTe2AudioTask
// ----------------------------------------------------------------------------
//
CTe2AudioTask::CTe2AudioTask( MAudioTaskObserver& aObserver )
: iObserver( aObserver )
    {
    // no implementation required
    }

// ----------------------------------------------------------------------------
// CTe2AudioTask::ConstructL
// ----------------------------------------------------------------------------
//
void CTe2AudioTask::ConstructL()
    {
    // no implementation required
    }

// End of File
