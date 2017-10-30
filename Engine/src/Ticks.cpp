/* ====================================================================
 * File: Ticks.cpp
 * Created: 09/10/10
 * Author: Sun Jinbo
 * Copyright (c): Tieto, All rights reserved
 * ==================================================================== */

// INCLUDE FILES
#include "Ticks.h"

// CONSTANTS
const TInt KTicksPerSecond = 64;

// ============================ MEMBER FUNCTIONS ===============================

// ----------------------------------------------------------------------------
// CTicks::NewL
// ----------------------------------------------------------------------------
//
CTicks* CTicks::NewL( MTicksCallback& aCallback )
    {
    CTicks* self = new( ELeave ) CTicks( aCallback );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// ----------------------------------------------------------------------------
// CTicks::~CTicks
// ----------------------------------------------------------------------------
//
CTicks::~CTicks()
    {
    iTimer.Cancel();
    iTimer.Close();
    }

// ----------------------------------------------------------------------------
// CTicks::StartL
// ----------------------------------------------------------------------------
//
void CTicks::Start( TInt aSeconds )
    {
    if ( !IsActive() )
        {
        iInitTickCount = User::TickCount();
        iTickCount = aSeconds * KTicksPerSecond;
        iTimer.AfterTicks( iStatus, iTickCount );
        SetActive();
        }
    }

// ----------------------------------------------------------------------------
// CTicks::Pause
// ----------------------------------------------------------------------------
//
void CTicks::Pause()
    {
    iTickCount -= ( User::TickCount() - iInitTickCount );
    Cancel();
    }

// ----------------------------------------------------------------------------
// CTicks::Resume
// ----------------------------------------------------------------------------
//
void CTicks::Resume()
    {
    if ( !IsActive() )
        {
        iInitTickCount = User::TickCount(); 
        iTimer.AfterTicks( iStatus, iTickCount );
        SetActive();
        }
    }

// ----------------------------------------------------------------------------
// CTicks::Stop
// ----------------------------------------------------------------------------
//
void CTicks::Stop()
    {
    Cancel();
    iTickCount = 0;
    }

// ----------------------------------------------------------------------------
// CTicks::RunL
// ----------------------------------------------------------------------------
//
void CTicks::RunL()
    {
    if ( iStatus.Int() == KErrNone )
        {
        iTickCount = 0;
        iCallback.TicksCompletedL();
        }
    }

// ----------------------------------------------------------------------------
// CTicks::DoCancel
// ----------------------------------------------------------------------------
//
void CTicks::DoCancel()
    {
    iTimer.Cancel();
    }

// ----------------------------------------------------------------------------
// CTicks::CTicks
// ----------------------------------------------------------------------------
//
CTicks::CTicks( MTicksCallback& aCallback ) 
    : CActive( EPriorityStandard ), iCallback( aCallback )
    {
    CActiveScheduler::Add( this );
    }

// ----------------------------------------------------------------------------
// CTicks::ConstructL
// ----------------------------------------------------------------------------
//
void CTicks::ConstructL()
    {
    iTimer.CreateLocal();
    }

// End of file
