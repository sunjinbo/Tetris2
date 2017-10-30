/* ====================================================================
 * File: Te2HelpView.cpp
 * Created: 01/14/09
 * Modified: 03/15/09
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
#include <StringLoader.h>
#include <Tetris2.rsg>
#include <HelpContainer.h>
#include <LayoutManager.h>
#include "Te2HelpView.h"
#include "Te2AppUi.h"
#include "Tetris2.hrh"
#include "Tetris2ViewIds.h"

// CONSTANTS
_LIT( KResourceFileName, "\\resource\\apps\\Tetris2.rsc" );

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CTe2HelpView::NewL
// ---------------------------------------------------------------------------
// 
CTe2HelpView* CTe2HelpView::NewL( CTe2AppUi* aAppUi )
    {
    CTe2HelpView* self = new (ELeave) CTe2HelpView( aAppUi );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// CTe2HelpView::~CTe2HelpView
// ---------------------------------------------------------------------------
// 
CTe2HelpView::~CTe2HelpView()
    { // No implementation required
    }

// ---------------------------------------------------------------------------
// CTe2HelpView::Id
// ---------------------------------------------------------------------------
// 
TUid CTe2HelpView::Id() const
    {
    return KTe2HelpViewId;
    }

// ---------------------------------------------------------------------------
// CTe2HelpView::HandleCommandL
// ---------------------------------------------------------------------------
// 
void CTe2HelpView::HandleCommandL( TInt /*aCommand*/ )
    { // No implementation required
    }

// ---------------------------------------------------------------------------
// CTe2HelpView::DoActivateL
// ---------------------------------------------------------------------------
// 
void CTe2HelpView::DoActivateL( const TVwsViewId& /*aPrevViewId*/,
    TUid /*aCustomMessageId*/, const TDesC8& /*aCustomMessage*/ )
    {
    iContainer = CHelpContainer::NewL( iAppUi->ClientRect(), *iAppUi );
    InitContainerL();
    AppUi()->AddToViewStackL( *this, iContainer );
    iContainer->Start();
    }

// ---------------------------------------------------------------------------
// CTe2HelpView::DoDeactivate
// ---------------------------------------------------------------------------
//
void CTe2HelpView::DoDeactivate()
    {
    if ( iContainer )
        {
        AppUi()->RemoveFromStack( iContainer );
        delete iContainer;
        iContainer = NULL;
        }
    }

// ---------------------------------------------------------------------------
// CTe2HelpView::HandleClientRectChange
// ---------------------------------------------------------------------------
//
void CTe2HelpView::HandleClientRectChange()
    {
    if ( iContainer )
        {
        iContainer->SetRect( ClientRect() );
        }
    }

// ---------------------------------------------------------------------------
// CTe2HelpView::ChangeMenuBarL
// ---------------------------------------------------------------------------
//
void CTe2HelpView::ChangeMenuBarL( TInt aMenuTitleResourceId )
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
            case R_TETRIS2_SOFTKEYS_EMPTYBACK:
                {
                resourceData = resourceFile.AllocReadLC ( R_TETRIS2_SOFTKEYS_EMPTYBACK );
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
// CTe2HelpView::StoreL
// ---------------------------------------------------------------------------
//
void CTe2HelpView::StoreL()
    {
    if ( iContainer )
        {
        iContainer->StoreL();
        }
    }

// ---------------------------------------------------------------------------
// CTe2HelpView::CTe2HelpView
// ---------------------------------------------------------------------------
//
CTe2HelpView::CTe2HelpView( CTe2AppUi* aAppUi )
    : iAppUi( aAppUi )
    { 
    // No implementation required
    }

// ---------------------------------------------------------------------------
// CTe2HelpView::ConstructL
// ---------------------------------------------------------------------------
//
void CTe2HelpView::ConstructL()
    {
    BaseConstructL( R_TETRIS2_HELP_VIEW );
    //CEikCba* eikCba = static_cast< CEikCba* >( Cba()->ButtonGroup() );
    //eikCba->MakeVisible( EFalse );
    }

// ---------------------------------------------------------------------------
// CTe2HelpView::InitContainerL
// ---------------------------------------------------------------------------
//
void CTe2HelpView::InitContainerL()
    {
    if ( iMenuTitleResourceId )
        {
        ChangeMenuBarL( iMenuTitleResourceId );
        iMenuTitleResourceId = 0;
        }

    HBufC* contentsText = NULL;
    if ( EQhdPortrait == CTe2LayoutManager::Resolution() )
        {
        contentsText = StringLoader::LoadLC( R_TETRIS2_HELP_CONTENTS_TOUCH );
        }
    else
        {
        contentsText = StringLoader::LoadLC( R_TETRIS2_HELP_CONTENTS );
        }

    HBufC* aboutText = StringLoader::LoadLC( R_TETRIS2_HELP_ABOUT );
    
    HBufC* helpText = HBufC::NewLC( contentsText->Length() + aboutText->Length() + 1 );
    TPtr hlpPtr( helpText->Des() );
    hlpPtr.Append( *contentsText );
    hlpPtr.Append( *aboutText );
    
    iContainer->AddTextItemL( *helpText );

    CleanupStack::PopAndDestroy(); // helpText
    CleanupStack::PopAndDestroy(); // aboutText
    CleanupStack::PopAndDestroy(); // contentsText
    }

// End of File
