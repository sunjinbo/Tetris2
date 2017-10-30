/* ====================================================================
 * File: Te2RankView.cpp
 * Created: 06/06/10
 * Modified: 06/06/10
 * Author: Sun Jinbo
 * Copyright (c): Tieto, All rights reserved
 * ==================================================================== */

// INCLUDE FILES
#include <aknviewappui.h>
#include <aknconsts.h>
#include <akndef.h>
#include <aknutils.h>
#include <akntitle.h> 
#include <barsread.h>
#include <bautils.h>
#include <Tetris2.rsg>
#include <RankContainer.h>
#include "Te2RankView.h"
#include "Te2AppUi.h"
#include "Tetris2.hrh"
#include "Tetris2ViewIds.h"

// CONSTANTS
_LIT( KResourceFileName, "\\resource\\apps\\Tetris2.rsc" );

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CTe2RankView::NewL
// ---------------------------------------------------------------------------
// 
CTe2RankView* CTe2RankView::NewL( CTe2AppUi* aAppUi )
    {
    CTe2RankView* self = new (ELeave) CTe2RankView( aAppUi );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// CTe2RankView::~CTe2RankView
// ---------------------------------------------------------------------------
// 
CTe2RankView::~CTe2RankView()
    { // No implementation required
    }

// ---------------------------------------------------------------------------
// CTe2RankView::Id
// ---------------------------------------------------------------------------
// 
TUid CTe2RankView::Id() const
    {
    return KTe2RankViewId;
    }

// ---------------------------------------------------------------------------
// CTe2RankView::HandleCommandL
// ---------------------------------------------------------------------------
// 
void CTe2RankView::HandleCommandL( TInt aCommand )
    {
    switch ( aCommand )
        {
        case ETetris2CmdClear:
            {
            iContainer->CleanRankL();
            }
            break;
        default:
            break;
        }
    }

// ---------------------------------------------------------------------------
// CTe2RankView::DoActivateL
// ---------------------------------------------------------------------------
// 
void CTe2RankView::DoActivateL( const TVwsViewId& /*aPrevViewId*/,
    TUid /*aCustomMessageId*/, const TDesC8& /*aCustomMessage*/ )
    {
    iContainer = CRankContainer::NewL( iAppUi->ClientRect(), *iAppUi );
    InitContainerL();
    AppUi()->AddToViewStackL( *this, iContainer );
    }

// ---------------------------------------------------------------------------
// CTe2RankView::DoDeactivate
// ---------------------------------------------------------------------------
//
void CTe2RankView::DoDeactivate()
    {
    if ( iContainer )
        {
        AppUi()->RemoveFromStack( iContainer );
        delete iContainer;
        iContainer = NULL;
        }
    }

// ---------------------------------------------------------------------------
// CTe2RankView::HandleClientRectChange
// ---------------------------------------------------------------------------
//
void CTe2RankView::HandleClientRectChange()
    {
    if ( iContainer )
        {
        iContainer->SetRect( ClientRect() );
        }
    }

// ---------------------------------------------------------------------------
// CTe2RankView::ChangeMenuBarL
// ---------------------------------------------------------------------------
//
void CTe2RankView::ChangeMenuBarL( TInt aMenuTitleResourceId )
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
            case R_TETRIS2_SOFTKEYS_CLEARBACK:
                {
                resourceData = resourceFile.AllocReadLC ( R_TETRIS2_SOFTKEYS_CLEARBACK );
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
// CTe2RankView::StoreL
// ---------------------------------------------------------------------------
//
void CTe2RankView::StoreL()
    {
    if ( iContainer )
        {
        iContainer->StoreL();
        }
    }

// ---------------------------------------------------------------------------
// CTe2RankView::CTe2RankView
// ---------------------------------------------------------------------------
//
CTe2RankView::CTe2RankView( CTe2AppUi* aAppUi )
    : iAppUi( aAppUi )
    { // No implementation required
    }

// ---------------------------------------------------------------------------
// CTe2RankView::ConstructL
// ---------------------------------------------------------------------------
//
void CTe2RankView::ConstructL()
    {
    BaseConstructL( R_TETRIS2_RANK_VIEW );
    }

// ---------------------------------------------------------------------------
// CTe2RankView::InitContainerL
// ---------------------------------------------------------------------------
//
void CTe2RankView::InitContainerL()
    {
    if ( iMenuTitleResourceId )
        {
        ChangeMenuBarL( iMenuTitleResourceId );
        iMenuTitleResourceId = 0;
        }
    }

// End of File
