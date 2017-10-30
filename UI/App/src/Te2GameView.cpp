/* ====================================================================
 * File: Te2GameView.cpp
 * Created: 01/14/09
 * Modified: 04/11/09
 * Author: Sun Jinbo
 * Copyright (c): Tieto, All rights reserved
 * ==================================================================== */

// INCLUDE FILES
#include <eikbtgpc.h>
#include <aknviewappui.h>
#include <aknconsts.h>
#include <Tetris2.rsg>
#include <akndef.h>
#include <aknutils.h>
#include <akntitle.h> 
#include <barsread.h>
#include <bautils.h>
#include <GameContainer.h>
#include "Te2GameView.h"
#include "Te2AppUi.h"
#include "Tetris2.hrh"
#include "Tetris2ViewIds.h"

// CONSTANTS
_LIT( KResourceFileName, "\\resource\\apps\\Tetris2.rsc" );

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CTe2GameView::NewL
//
// ---------------------------------------------------------------------------
// 
CTe2GameView* CTe2GameView::NewL( CTe2AppUi* aAppUi )
    {
    CTe2GameView* self = new (ELeave) CTe2GameView( aAppUi );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// CTe2GameView::~CTe2GameView
//
// ---------------------------------------------------------------------------
// 
CTe2GameView::~CTe2GameView()
    { // No implementation required
    }

// ---------------------------------------------------------------------------
// CTe2GameView::Id
//
// ---------------------------------------------------------------------------
// 
TUid CTe2GameView::Id() const
    {
    return KTe2GameViewId;
    }

// ---------------------------------------------------------------------------
// CTe2GameView::HandleCommandL
//
// ---------------------------------------------------------------------------
// 
void CTe2GameView::HandleCommandL( TInt aCommand )
    {
    switch ( aCommand )
        {
        case ETetris2CmdStart:
            {
            DoStartL();
            }
            break;
        case ETetris2CmdRestart:
            {
            iContainer->Restart();
            }
            break;
        case ETetris2CmdStop:
            {
            iContainer->Stop();
            }
            break;
        case ETetris2CmdPause:
            {
            iContainer->Pause();
            }
            break;
        case ETetris2CmdResume:
            {
            iContainer->Resume();
            }
            break;
        default:
            break;
        }
    }

// ---------------------------------------------------------------------------
// CTe2GameView::DoActivateL
//
// ---------------------------------------------------------------------------
// 
void CTe2GameView::DoActivateL( const TVwsViewId& /*aPrevViewId*/,
    TUid /*aCustomMessageId*/, const TDesC8& /*aCustomMessage*/ )
    {
    iContainer = CGameContainer::NewL( iAppUi->ClientRect(), *iAppUi );
    InitContainerL();
    DoStartL();
    AppUi()->AddToViewStackL( *this, iContainer );
    }

// ---------------------------------------------------------------------------
// CTe2GameView::DoDeactivate
//
// ---------------------------------------------------------------------------
//
void CTe2GameView::DoDeactivate()
    {
    if ( iContainer )
        {
        AppUi()->RemoveFromStack( iContainer );
        delete iContainer;
        iContainer = NULL;
        }
    }

// ---------------------------------------------------------------------------
// CTe2GameView::HandleClientRectChange
//
// ---------------------------------------------------------------------------
//
void CTe2GameView::HandleClientRectChange()
    {
    if ( iContainer )
        {
        iContainer->SetRect( ClientRect() );
        }
    }

// ---------------------------------------------------------------------------
// CTe2GameView::ChangeMenuBarL
//
// ---------------------------------------------------------------------------
//
void CTe2GameView::ChangeMenuBarL( TInt aMenuTitleResourceId )
    {
    if ( iContainer )
        {
        RFs fs;
        User::LeaveIfError( fs.Connect() );
        CleanupClosePushL( fs );
        RResourceFile resourceFile;
        
        TFileName fileName( KResourceFileName );
        TInt err = CompleteWithAppPath( fileName );
        if ( err != KErrNotSupported )
            {
            User::LeaveIfError( err );
            }

        CCoeEnv* env = CCoeEnv::Static();
        BaflUtils::NearestLanguageFile( env->FsSession(), fileName );

        resourceFile.OpenL ( fs, fileName );
        CleanupClosePushL( resourceFile );
        resourceFile.ConfirmSignatureL ();

        HBufC8* resourceData( NULL );
        switch( aMenuTitleResourceId )
            {
            case R_TETRIS2_SOFTKEYS_PAUSESTOP:
                {
                resourceData = resourceFile.AllocReadLC ( R_TETRIS2_SOFTKEYS_PAUSESTOP );
                }
                break;
            case R_TETRIS2_SOFTKEYS_RESUMESTOP:
                {
                resourceData = resourceFile.AllocReadLC ( R_TETRIS2_SOFTKEYS_RESUMESTOP );
                }
                break;
            case R_TETRIS2_SOFTKEYS_RESTARTBACK:
                {
                resourceData = resourceFile.AllocReadLC ( R_TETRIS2_SOFTKEYS_RESTARTBACK );
                }
                break;
            default:
                break;
            }
        
        if ( resourceData )
            {
            TResourceReader reader;
            reader.SetBuffer( resourceData );
            TInt16 count = reader.ReadInt16 ();
            TInt32 command1 = reader.ReadInt32 ();
            TInt32 flag1 = reader.ReadInt32 ();
            HBufC* txt1 = reader.ReadHBufCL();
            iContainer->SetMenuBar1L( *txt1, command1 );
            delete txt1;
            txt1 = NULL;
            
            TInt32 command2 = reader.ReadInt32 ();
            TInt32 flag2 = reader.ReadInt32 ();
            HBufC* txt2 = reader.ReadHBufCL();
            iContainer->SetMenuBar2L( *txt2, command2 );
            delete txt2;
            txt2 = NULL;
            
            CleanupStack::PopAndDestroy(); // resourceData
            }

        CleanupStack::PopAndDestroy(2); // resourceFile, fs
        }
    else
        {
        iMenuTitleResourceId = aMenuTitleResourceId;
        }
    }

// ---------------------------------------------------------------------------
// CTe2GameView::SetMode
//
// ---------------------------------------------------------------------------
//
void CTe2GameView::SetMode( TTe2Mode aType )
    {
    iModeType = aType;
    }

// ---------------------------------------------------------------------------
// CTe2GameView::SetAudio
//
// ---------------------------------------------------------------------------
//
void CTe2GameView::SetAudio( TBool aAudio )
    {
    iAudio = aAudio;
    }

// ---------------------------------------------------------------------------
// CTe2GameView::StoreL
// ---------------------------------------------------------------------------
//
void CTe2GameView::StoreL()
    {
    if ( iContainer )
        {
        iContainer->StoreL();
        }
    }

// ---------------------------------------------------------------------------
// CTe2GameView::CTe2GameView
//
// ---------------------------------------------------------------------------
//
CTe2GameView::CTe2GameView( CTe2AppUi* aAppUi )
    : iAppUi( aAppUi )
    { // No implementation required
    }

// ---------------------------------------------------------------------------
// CTe2GameView::ConstructL
//
// ---------------------------------------------------------------------------
//
void CTe2GameView::ConstructL()
    {
    BaseConstructL( R_TETRIS2_GAME_VIEW );
    //CEikCba* eikCba = static_cast< CEikCba* >( Cba()->ButtonGroup() );
    //eikCba->MakeVisible( EFalse );
    }

// ---------------------------------------------------------------------------
// CTe2GameView::InitContainerL
//
// ---------------------------------------------------------------------------
//
void CTe2GameView::InitContainerL()
    {
    if ( iMenuTitleResourceId )
        {
        ChangeMenuBarL( iMenuTitleResourceId );
        iMenuTitleResourceId = 0;
        }
    iContainer->SetAudio( iAudio );
    }

// ---------------------------------------------------------------------------
// CTe2GameView::DoStart
//
// ---------------------------------------------------------------------------
//
void CTe2GameView::DoStartL()
    {
    if ( iContainer )
        {
        switch( iModeType )
            {
            case ETe2ModeNormal:
                iContainer->StartL( EModeCommon );
                break;
            case ETe2ModeAdvanced:
                iContainer->StartL( EModeExtra );
                break;
            default:
                break;
            }
        }
    }

// End of File
