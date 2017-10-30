/* ====================================================================
 * File: Te2MenuView.cpp
 * Created: 01/14/09
 * Modified: 03/27/09
 * Author: Sun Jinbo
 * Copyright (c): Tieto, All rights reserved
 * ==================================================================== */

// INCLUDE FILES
#include <eikbtgpc.h>
#include <aknviewappui.h>
#include <aknconsts.h>
#include <akndef.h>
#include <aknutils.h>
#include <akntitle.h>
#include <barsread.h>
#include <bautils.h>
#include <MainMenuContainer.h>
#include <Tetris2.rsg>
#include <Logger.h>
#include "Te2MenuView.h"
#include "Te2AppUi.h"
#include "Tetris2.hrh"
#include "Tetris2ViewIds.h"

// CONSTANTS
const TInt32 KItemFlagLowBitMask = 0x0000FFFF; // HEX
const TInt32 KItemFlagHeighBitMask = 0xFFFF0000; // HEX
_LIT( KResourceFileName, "\\resource\\apps\\Tetris2.rsc" );

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CTe2MenuView::NewL
//
// ---------------------------------------------------------------------------
// 
CTe2MenuView* CTe2MenuView::NewL( CTe2AppUi* aAppUi )
    {
    CTe2MenuView* self = new (ELeave) CTe2MenuView( aAppUi );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// CTe2MenuView::~CTe2MenuView
//
// ---------------------------------------------------------------------------
// 
CTe2MenuView::~CTe2MenuView()
    { // No implementation required
    }

// ---------------------------------------------------------------------------
// CTe2MenuView::Id
//
// ---------------------------------------------------------------------------
// 
TUid CTe2MenuView::Id() const
    {
    return KTe2MainViewId;
    }

// ---------------------------------------------------------------------------
// CTe2MenuView::DoActivateL
//
// ---------------------------------------------------------------------------
// 
void CTe2MenuView::DoActivateL( const TVwsViewId& , TUid , const TDesC8& )
    {
    if ( !iContainer )
        {
        iContainer = CMainMenuContainer::NewL( iAppUi->ClientRect(), *iAppUi );
        InitContainerL();
        AppUi()->AddToViewStackL( *this, iContainer );
        ChangeMenuBarL( R_TETRIS2_SOFTKEYS_SELECTQUIT );
        }
    }

// ---------------------------------------------------------------------------
// CTe2MenuView::DoDeactivate
//
// ---------------------------------------------------------------------------
//
void CTe2MenuView::DoDeactivate()
    {
    if ( iContainer )
        {
        AppUi()->RemoveFromStack( iContainer );
        delete iContainer;
        iContainer = NULL;
        }
    }

// ---------------------------------------------------------------------------
// CTe2MenuView::HandleClientRectChange
//
// ---------------------------------------------------------------------------
//
void CTe2MenuView::HandleClientRectChange()
    {
    if ( iContainer )
        {
        iContainer->SetRect( ClientRect() );
        }
    }

// ---------------------------------------------------------------------------
// CTe2MenuView::ChangeMenuBarL
//
// ---------------------------------------------------------------------------
//
void CTe2MenuView::ChangeMenuBarL( TInt aMenuTitleResourceId )
    {
    if ( iContainer )
        {
        RFs fs;
        User::LeaveIfError( fs.Connect() );
        CleanupClosePushL( fs );
        
        TFileName fileName( KResourceFileName );
        TInt err = CompleteWithAppPath( fileName );
        if ( err != KErrNotSupported )
            {
            User::LeaveIfError( err );
            }

        CCoeEnv* env = CCoeEnv::Static();
        BaflUtils::NearestLanguageFile( env->FsSession(), fileName );

        RResourceFile resourceFile;
        resourceFile.OpenL ( fs, fileName );
        CleanupClosePushL( resourceFile );
        resourceFile.ConfirmSignatureL ();
        
        
        HBufC8* resourceData( NULL );
        switch( aMenuTitleResourceId )
            {
            case R_TETRIS2_SOFTKEYS_SELECTQUIT:
                {
                resourceData = resourceFile.AllocReadLC ( R_TETRIS2_SOFTKEYS_SELECTQUIT );
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
        
        CleanupStack::PopAndDestroy( 2 ); // resourceFile, fs
        }
    }

// ---------------------------------------------------------------------------
// CTe2MenuView::SetMode
//
// ---------------------------------------------------------------------------
//
void CTe2MenuView::SetMode( TTe2Mode aType )
    {
    switch ( aType )
        {
        case ETe2ModeNormal:
            {
            iModeType = ETe2ModeNormal;
            if ( iContainer )
                {
                iContainer->DisableMenuItem( ETetris2CmdNormal );
                iContainer->EnableMenuItem( ETetris2CmdAdvanced );
                iContainer->SetSelectedItem( ETetris2CmdAdvanced );
                }
            }
            break;
        case ETe2ModeAdvanced:
            {
            iModeType = ETe2ModeAdvanced;
            if ( iContainer )
                {
                iContainer->DisableMenuItem( ETetris2CmdAdvanced );
                iContainer->EnableMenuItem( ETetris2CmdNormal );
                iContainer->SetSelectedItem( ETetris2CmdNormal );
                }
            }
            break;
        default:
            break;
        }
    }

// ---------------------------------------------------------------------------
// CTe2MenuView::SetAudio
//
// ---------------------------------------------------------------------------
//
void CTe2MenuView::SetAudio( TTe2Audio aType )
    {
    switch ( aType )
        {
        case ETe2AudioOn:
            {
            iAudioType = ETe2AudioOn;
            if ( iContainer )
                {
                iContainer->DisableMenuItem( ETetris2CmdAudioOn );
                iContainer->EnableMenuItem( ETetris2CmdAudioOff );
                iContainer->SetSelectedItem( ETetris2CmdAudioOff );
                }
            }
            break;
        case ETe2AudioOff:
            {
            iAudioType = ETe2AudioOff;
            if ( iContainer )
                {
                iContainer->DisableMenuItem( ETetris2CmdAudioOff );
                iContainer->EnableMenuItem( ETetris2CmdAudioOn );
                iContainer->SetSelectedItem( ETetris2CmdAudioOn );
                }
            }
            break;
        default:
            break;
        }
    }

// ---------------------------------------------------------------------------
// CTe2MenuView::StoreL
// ---------------------------------------------------------------------------
//
void CTe2MenuView::StoreL()
    {
    if ( iContainer )
        {
        iContainer->StoreL();
        }
    }

// ---------------------------------------------------------------------------
// CTe2MenuView::CTe2MenuView
//
// ---------------------------------------------------------------------------
//
CTe2MenuView::CTe2MenuView( CTe2AppUi* aAppUi )
    : iAppUi( aAppUi )
    { // No implementation required
    }

// ---------------------------------------------------------------------------
// CTe2MenuView::ConstructL
//
// ---------------------------------------------------------------------------
//
void CTe2MenuView::ConstructL()
    {
    BaseConstructL( R_TETRIS2_MENU_VIEW );
    //CEikCba* eikCba = static_cast< CEikCba* >( Cba()->ButtonGroup() );
    //eikCba->MakeVisible( EFalse );
    }

// ---------------------------------------------------------------------------
// CTe2MenuView::InitContainerL
//
// ---------------------------------------------------------------------------
//
void CTe2MenuView::InitContainerL()
    {
    if ( iContainer )
        {
        RFs fs;
        User::LeaveIfError( fs.Connect() );
        CleanupClosePushL( fs );
        
        TFileName fileName( KResourceFileName );
        TInt err = CompleteWithAppPath( fileName );
        if ( err != KErrNotSupported )
            {
            User::LeaveIfError( err );
            }

        CCoeEnv* env = CCoeEnv::Static();
        BaflUtils::NearestLanguageFile( env->FsSession(), fileName );

        RResourceFile resourceFile;
        resourceFile.OpenL ( fs, fileName );
        CleanupClosePushL( resourceFile );
        resourceFile.ConfirmSignatureL ();

        HBufC8* resourceData = resourceFile.AllocReadLC ( R_TETRIS2_MAINMENU );
        TResourceReader reader;
        reader.SetBuffer( resourceData );

        TInt16 count = reader.ReadInt16 ();
        for ( TInt i = 0; i < count; ++i )
            {
            TInt32 command = reader.ReadInt32 ();
            TInt32 flag = reader.ReadInt32 ();
            HBufC* txt = reader.ReadHBufCL();

            if (  IsEnableItem( flag ) )
                {
                iContainer->AddMenuItemL( *txt, command, ETrue );    
                }
            else
                {
                iContainer->AddMenuItemL( *txt, command, EFalse );
                }
            
            delete txt;
            txt = NULL;
            
            if ( IsSelectedItem( flag ) )
                {
                iContainer->SetSelectedItem( command );
                }
            }

        CleanupStack::PopAndDestroy(3); // resourceData, resourceFile, fs
        
        SetAudio( iAudioType );
        SetMode( iModeType );

        iContainer->SetSelectedItem( ETetris2CmdStart );
        }
    }

// ---------------------------------------------------------------------------
// CTe2MenuView::IsSelectedItem
// ---------------------------------------------------------------------------
//
TBool CTe2MenuView::IsSelectedItem( TInt32 aFlag ) const
    {
    TBool retVal( EFalse );
    if ( ( aFlag & KItemFlagHeighBitMask )== ETetris2CmdSelected )
        {
        retVal = ETrue;
        }
    return retVal;
    }

// ---------------------------------------------------------------------------
// CTe2MenuView::IsEnableItem
// ---------------------------------------------------------------------------
//
TBool CTe2MenuView::IsEnableItem( TInt32 aFlag ) const
    {
    TBool retVal( EFalse );
    if ( ( aFlag & KItemFlagLowBitMask ) == ETetris2CmdEnabled )
        {
        retVal = ETrue;
        }
    return retVal;
    }

// End of File
