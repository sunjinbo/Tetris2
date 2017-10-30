/* ====================================================================
 * File: AudioUtils.cpp
 * Created: 05/22/09
 * Modified: 05/22/09
 * Author: Sun Jinbo
 * Copyright (c): Tieto, All rights reserved
 * ==================================================================== */

// INCLUDE FILES
#include <MdaAudioTonePlayer.h>
#include <eikmenup.h>

#include "AudioUtils.h"
#include "AudioUtilsObserver.h"

// ======== MEMBER FUNCTIONS ========

// ----------------------------------------------------------------------------
// CAudioUtils::NewL
// ----------------------------------------------------------------------------
//
EXPORT_C CAudioUtils* CAudioUtils::NewL()
    {
    CAudioUtils* self = new (ELeave) CAudioUtils;
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );  
    return self;
    }

// ----------------------------------------------------------------------------
// CAudioUtils::~CAudioUtils
// ----------------------------------------------------------------------------
//
EXPORT_C CAudioUtils::~CAudioUtils()
    {
    Stop(); // stops sound first
    delete iPlayer;
    }

// ----------------------------------------------------------------------------
// CAudioUtils::Play
// ----------------------------------------------------------------------------
//
EXPORT_C void CAudioUtils::Play()
    {
    if ( iReadyToPlay )
        {
        Stop();
        iPlayer->Play();
        }
    }

// ----------------------------------------------------------------------------
// CAudioUtils::Stop
// ----------------------------------------------------------------------------
//
EXPORT_C void CAudioUtils::Stop()
    {
    iPlayer->Stop();
    }

// ----------------------------------------------------------------------------
// CAudioUtils::OpenFileL
// ----------------------------------------------------------------------------
//
EXPORT_C void CAudioUtils::OpenFileL( const TDesC& aFileName, TInt aUniqueId )
    {
    iReadyToPlay = EFalse;
    iUniqueId = aUniqueId;
    iPlayer->OpenFileL( aFileName );
    }

// ----------------------------------------------------------------------------
// CAudioUtils::UniqueId
// ----------------------------------------------------------------------------
//
EXPORT_C TInt CAudioUtils::UniqueId() const
    {
    return iUniqueId;
    }

// ----------------------------------------------------------------------------
// CAudioUtils::SetAudioInitObserver
// ----------------------------------------------------------------------------
//
EXPORT_C void CAudioUtils::SetAudioInitObserver( MAudioInitObserver* aObserver )
    {
    iInitObserver = aObserver;
    }

// ----------------------------------------------------------------------------
// CAudioUtils::SetAudioPlayObserver
// ----------------------------------------------------------------------------
//
EXPORT_C void CAudioUtils::SetAudioPlayObserver( MAudioPlayObserver* aObserver )
    {
    iPlayObserver = aObserver;
    }

// ----------------------------------------------------------------------------
// CAudioUtils::MapcInitComplete
// ----------------------------------------------------------------------------
//
void CAudioUtils::MapcInitComplete( TInt aError, const TTimeIntervalMicroSeconds& /*aDuration*/ )	
    {
    if ( aError == KErrNone )
        {
        TInt vol = iPlayer->MaxVolume() / 2;
        iPlayer->SetVolume( vol );
        
        iReadyToPlay = ETrue;
        }

    iInitObserver->AudioInitComplete( this, aError );
    }

// ----------------------------------------------------------------------------
// CAudioUtils::MapcPlayComplete
// ----------------------------------------------------------------------------
//
void CAudioUtils::MapcPlayComplete( TInt aError )
    {
    iPlayObserver->AudioPlayComplete( aError );
    }

// ----------------------------------------------------------------------------
// CAudioUtils::CAudioUtils
// ----------------------------------------------------------------------------
//
CAudioUtils::CAudioUtils()
    {
    // no implementation required
    }

// ----------------------------------------------------------------------------
// CAudioUtils::ConstructL
// ----------------------------------------------------------------------------
//
void CAudioUtils::ConstructL()
    {
    iPlayer = CMdaAudioPlayerUtility::NewL( *this, EMdaPriorityMax );
    }

// End of File
