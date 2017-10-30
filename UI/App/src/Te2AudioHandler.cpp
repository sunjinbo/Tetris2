/* ====================================================================
 * File: Te2AudioHandler.cpp
 * Created: 06/17/10
 * Author: Sun Jinbo
 * Copyright (c): Tieto, All rights reserved
 * ==================================================================== */

// INCLUDE FILES
#include <e32math.h>
#include <eikapp.h>
#include <coemain.h>
#include <PropertyListener.h>
#include <AudioUtils.h>
#include <AknUtils.h>
#include <f32file.h>
#include <Logger.h>

#include "Te2AudioHandler.h"
#include "Te2AudioRegister.h"
#include "Te2AudioScheduler.h"
#include "Te2AudioTask.h"
#include "Te2PSKeys.h"
#include "Tetris2.hrh"

// CONSTANTS
const TInt KBasePlayTimeout = 8;
const TInt KExtraPlayTimeout = 4;
const TInt KMicSecondPerSecond = 1000000; // 1 sec
const TInt KExpiriedInterval = 15000000; // 15 sec
const TInt KFastExpiriedInterval = 1000000; // 1 sec

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CTe2Document::NewL
// ---------------------------------------------------------------------------
// 
CTe2AudioHandler* CTe2AudioHandler::NewL( CEikonEnv& aCoeEnv )
    {
    CTe2AudioHandler* self = NewLC( aCoeEnv );
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// CTe2AudioHandler::NewLC
// ---------------------------------------------------------------------------
//
CTe2AudioHandler* CTe2AudioHandler::NewLC( CEikonEnv& aCoeEnv )
    {
    CTe2AudioHandler* self = new (ELeave) CTe2AudioHandler( aCoeEnv );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// ---------------------------------------------------------------------------
// CTe2AudioHandler::~CTe2AudioHandler
// ---------------------------------------------------------------------------
//
CTe2AudioHandler::~CTe2AudioHandler()
    {
    delete iListener;
    delete iTimeout;
    delete iRegister;
    delete iScheduler;
    }

// ---------------------------------------------------------------------------
// CTe2AudioHandler::StartL
// ---------------------------------------------------------------------------
//
void CTe2AudioHandler::StartL()
    {
    iListener->Listen();
    }

// ---------------------------------------------------------------------------
// CTe2AudioHandler::HandlePropertyChangedL
// ---------------------------------------------------------------------------
//
void CTe2AudioHandler::HandlePropertyChangedL()
    {
    TInt value(0);
    TInt err = iListener->GetValue( value );
    User::LeaveIfError( err );

    switch ( value )
        {
        case EAudioNotSet:
            break;
        case EAudioOn:
            {
            TAudioRequest request;
            request.iSourceId = KOpeningSourceId;
            request.iPriority = TAudioPriorityNormal;
            request.iTime.HomeTime();
            request.iExpiriedInterval = KExpiriedInterval;
            request.iAudioUtilsPointer 
                = iRegister->AudioUtils( KOpeningSourceId );

            CTe2AudioTask* task = CTe2AudioTask::NewL( *iScheduler );
            task->SetRequest( request );
            iScheduler->AddTask( task );
            
            StartTimeout();
            }
            break;

        case EAudioOff:
            {
            iRegister->Stop();
            iTimeout->Stop();
            iScheduler->Clear();
            }
            break;

        case EAudioIncVolume:
        case EAudioDecVolume:
            break;
            
        case EAudioRewardSound:
            {
            TAudioRequest request;
            request.iSourceId = EAudioReward;
            request.iPriority = TAudioPriorityHigh;
            request.iTime.HomeTime();
            request.iExpiriedInterval = KFastExpiriedInterval;
            request.iAudioUtilsPointer 
                = iRegister->AudioUtils( EAudioReward );

            CTe2AudioTask* task = CTe2AudioTask::NewL( *iScheduler );
            task->SetRequest( request );
            iScheduler->AddTask( task );
            }
            break;

        case EAudioGameOverSound:
            {
            TAudioRequest request;
            request.iSourceId = EAudioGameOver;
            request.iPriority = TAudioPriorityHigh;
            request.iTime.HomeTime();
            request.iExpiriedInterval = KFastExpiriedInterval;
            request.iAudioUtilsPointer 
                = iRegister->AudioUtils( EAudioGameOver );

            CTe2AudioTask* task = CTe2AudioTask::NewL( *iScheduler );
            task->SetRequest( request );
            iScheduler->AddTask( task );
            }
            break;

        case EAudioExit:
            {
            err = RProperty::Set( 
                    KPSUidTetris2, 
                    KTetris2AudioFeedback, 
                    EExitCompleted 
                    );
            User::LeaveIfError( err );
            CActiveScheduler::Stop();
            }
            break;
        default:
            break;
        }
    }

// ---------------------------------------------------------------------------
// CTe2AudioHandler::HandleTimeoutL
// ---------------------------------------------------------------------------
//
void CTe2AudioHandler::HandleTimeoutL()
    {
    TAudioSourceId sourceId
        = (TAudioSourceId)( Math::Random() % EAudioCaw );
    TAudioRequest request;
    request.iSourceId = sourceId;
    request.iPriority = TAudioPriorityLow;
    request.iTime.HomeTime();
    request.iExpiriedInterval = KExpiriedInterval;
    request.iAudioUtilsPointer = iRegister->AudioUtils( sourceId );

    CTe2AudioTask* task = CTe2AudioTask::NewL( *iScheduler );
    task->SetRequest( request );
    iScheduler->AddTask( task );

    StartTimeout();
    }

// ---------------------------------------------------------------------------
// CTe2AudioHandler::StateChangedL
// ---------------------------------------------------------------------------
//
void CTe2AudioHandler::StateChangedL( TInt aNewState )
    {
    switch ( aNewState )
        {
        case ERegisterOpeningSuccess:
            RProperty::Set( 
                    KPSUidTetris2, 
                    KTetris2AudioFeedback, 
                    EPreparedCompleted 
                    );
            break;
        case ERegisterOpeningFailed:
            RProperty::Set( 
                    KPSUidTetris2, 
                    KTetris2AudioFeedback, 
                    EPreparedFailed 
                    );
            break;
        case ERegisterAllClipSuccess:
            StartTimeout();
            break;
        case ERegisterAllClipFailed:
            RProperty::Set( 
                    KPSUidTetris2, 
                    KTetris2AudioFeedback, 
                    EPreparedFailed 
                    );
            break;
        default:
            break;
        }
    }

//-------------------------------------------------------------------------
// CTe2AudioHandler::ConstructL
// ---------------------------------------------------------------------------
//
void CTe2AudioHandler::ConstructL()
    {
    iListener = CPropertyListener::NewL( 
            KPSUidTetris2, 
            KTetris2AudioCommand,
            *this );

    iTimeout = CRegistrationTimeout::NewL( *this );
    
    iScheduler = CTe2AudioScheduler::NewL();

    iRegister = CTe2AudioRegister::NewL( iCoeEnv, *this );
    }

// ---------------------------------------------------------------------------
// CTe2AudioHandler::CTe2AudioHandler
// ---------------------------------------------------------------------------
//
CTe2AudioHandler::CTe2AudioHandler( CEikonEnv& aCoeEnv )
: iCoeEnv( aCoeEnv )
    {
    // no implementation required
    }

// ----------------------------------------------------------------------------
// CTe2AudioHandler::StartTimeout
// ----------------------------------------------------------------------------
//
void CTe2AudioHandler::StartTimeout()
    {
    TInt extraTimeout = Math::Random() % KExtraPlayTimeout;
    TInt timeout = KBasePlayTimeout + extraTimeout;
    iTimeout->Stop();
    iTimeout->Start( timeout * KMicSecondPerSecond );
    }

// End of File
