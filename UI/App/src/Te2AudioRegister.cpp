/* ====================================================================
 * File: Te2AudioRegister.cpp
 * Created: 06/17/10
  * Modified: 12/11/09
 * Author: Sun Jinbo
 * Copyright (c): Tieto, All rights reserved
 * ==================================================================== */
 
// INCLUDE FILES
#include <eikenv.h>
#include <eikapp.h>
#include <eikappui.h>
#include <coemain.h>
#include <AudioUtils.h>
#include "Te2AudioRegister.h"

// Constants
_LIT( KAudioSuffix, ".wav" );

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CTe2AudioRegister::NewL
// ---------------------------------------------------------------------------
// 
CTe2AudioRegister* CTe2AudioRegister::NewL( 
        CEikonEnv& aCoeEnv, MAudioRegisterObserver& aObserver )
    {
    CTe2AudioRegister* self 
        = new (ELeave) CTe2AudioRegister( aCoeEnv, aObserver );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// CTe2AudioRegister::NewL
// ---------------------------------------------------------------------------
//
CTe2AudioRegister::~CTe2AudioRegister()
    {
    iAudioUtilsList.ResetAndDestroy();
    iAudioUtilsList.Close();
    }

// ---------------------------------------------------------------------------
// CTe2AudioRegister::AudioUtils
// ---------------------------------------------------------------------------
//
CAudioUtils* CTe2AudioRegister::AudioUtils( TAudioSourceId aSourceId  )
    {
    CAudioUtils* utils = NULL;
    
    for ( TInt i = 0; i < iAudioUtilsList.Count(); i++ )
        {
        if ( iAudioUtilsList[i]->UniqueId() == aSourceId )
            {
            utils = iAudioUtilsList[i];
            break;
            }
        }
    
    return utils;
    }

// ---------------------------------------------------------------------------
// CTe2AudioRegister::Stop
// ---------------------------------------------------------------------------
//
void CTe2AudioRegister::Stop()
    {
    for ( TInt i = 0; i < iAudioUtilsList.Count(); i++ )
        {
        iAudioUtilsList[i]->Stop();
        }
    }

// ---------------------------------------------------------------------------
// CTe2AudioRegister::AudioUtils
// ---------------------------------------------------------------------------
//
void CTe2AudioRegister::AudioInitComplete( CAudioUtils* aUtils, TInt aError )
    {
    TInt id = aUtils->UniqueId();

    switch ( id )
        {
        case EAudioWater:
            {
            if ( aError == KErrNone )
                {
                ++iRegistionCount;
                iObserver.StateChangedL( ERegisterOpeningSuccess );
                }
            else
                {
                iObserver.StateChangedL( ERegisterOpeningFailed );
                }
            }
            break;

        case EAudioTwitter:
        case EAudioWind:
        case EAudioChirp:
        case EAudioTweedle:
        case EAudioBreeze:
        case EAudioBillow:
        case EAudioCaw:
        case EAudioGameOver:
        case EAudioReward:
            {
            if ( aError == KErrNone )
                {
                ++iRegistionCount;
                }
            else
                {
                iObserver.StateChangedL( ERegisterAllClipFailed );
                }
            }
            break;
        default:
            break;
        }

    if ( iRegistionCount == EAudioLastElement )
        {
        iObserver.StateChangedL( ERegisterAllClipSuccess );
        }
    }

// ---------------------------------------------------------------------------
// CTe2AudioRegister::CTe2AudioRegister
// ---------------------------------------------------------------------------
//
CTe2AudioRegister::CTe2AudioRegister( 
        CEikonEnv& aCoeEnv, MAudioRegisterObserver& aObserver )
    : iCoeEnv( aCoeEnv ), iObserver( aObserver )
    {
    // no implementation required
    }

// ---------------------------------------------------------------------------
// CTe2AudioRegister::ConstructL
// ---------------------------------------------------------------------------
//
void CTe2AudioRegister::ConstructL()
    {
    for ( TInt i = EAudioWater; i < EAudioLastElement; i++ )
        {
        TFileName fileName;
        TInt err = GetFileName( fileName, (TAudioSourceId)i );
        User::LeaveIfError( err );
        CAudioUtils* utils = CAudioUtils::NewL();
        CleanupStack::PushL( utils );
        utils->SetAudioInitObserver( this );
        utils->OpenFileL( fileName, i );
        CleanupStack::Pop( utils );

        iAudioUtilsList.Append( utils );
        }
    }

// ----------------------------------------------------------------------------
// CTe2AudioRegister::GetPrivatePath
// ----------------------------------------------------------------------------
//
TInt CTe2AudioRegister::GetPrivatePath( TFileName& aPrivatePath )
	{
	const TInt KPathNameLength = 100;

	TBuf< KPathNameLength > pathWithoutDrive;
	TBuf< KPathNameLength > driveAndPath;
	TBuf< 2 > appDrive;

	// Get application's private path
	//   This is e.g. '\Private\20001978\'
	//   Does not contain drive.
	iCoeEnv.FsSession().PrivatePath( pathWithoutDrive );

	// Extract drive letter into appDrive:
	appDrive.Copy( iCoeEnv.EikAppUi()->Application()->AppFullName().Left(2));

    // Combine drive letter and private path
#if defined( __WINS__ )
    aPrivatePath.Copy( _L( "c:" ) );
#else
    aPrivatePath.Copy( appDrive );
#endif // __WINS__

	aPrivatePath.Append(pathWithoutDrive);

	return KErrNone;
	}

// ---------------------------------------------------------------------------
// CTe2AudioRegister::GetFileName
// ---------------------------------------------------------------------------
//
TInt CTe2AudioRegister::GetFileName( TFileName& aFileName, TAudioSourceId aId )
    {
    TAudioSource confs[] = 
        {
        /* Water */
            {
            EAudioWater,
            _L("Water")
            },
        /* Twitter */
            {
            EAudioTwitter,
            _L("Twitter")
            },
        /* Wind */
            {
            EAudioWind,
            _L("Wind")
            },
        /* Chirp */
            {
            EAudioChirp,
            _L("Chirp")
            },
        /* Tweedle */
            {
            EAudioTweedle,
            _L("Tweedle")
            },
        /* Breeze */
            {
            EAudioBreeze,
            _L("Breeze")
            },
        /* Billow */
            {
            EAudioBillow,
            _L("Billow")
            },
        /* Caw */
            {
            EAudioCaw,
            _L("Caw")
            },
        /* GameOver */
            {
            EAudioGameOver,
            _L("GameOver")
            },
        /* Reward */
            {
            EAudioReward,
            _L("Reward")
            }
        };

	TInt err = GetPrivatePath( aFileName );
    if ( err == KErrNone )
    	{
        TInt totalConfs 
            = sizeof(confs) / sizeof(TAudioSource);
        for ( TInt i = 0; i < totalConfs; i++ )
            {
            if ( confs[i].iSourceId == aId )
                {
                aFileName.Append( confs[i].iAudioFileName );
                aFileName.Append( KAudioSuffix );
                }
            }
    	}

    return err;
    }

// End of File
