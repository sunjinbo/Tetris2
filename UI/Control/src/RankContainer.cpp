/* ====================================================================
 * File: RankContainer.cpp
 * Created: 06/06/10
 * Modified: 06/06/10
 * Author: Sun Jinbo
 * Copyright (c): Tieto, All rights reserved
 * ==================================================================== */

// INCLUDE FILES
#include <akndef.h>
#include <aknutils.h>
#include <coecntrl.h>
#include <bautils.h>
#include <StringLoader.h>
#include <Te2Engine.h>
#include <Te2Control.rsg>
#include "RankContainer.h"
#include "LabelControl.h"
#include "LayoutManager.h"

#include "Logger.h"

// CONSTANTS
const TInt KMaxRankLen = 0x100;
const TInt KUnusableScore = 0;
const TInt KNoRankScore = 0;
_LIT( KRankFormat, "%d.  %04d-%02d-%02d  %02d:%02d   +%d" );
_LIT( KResourceFileName, "\\resource\\apps\\Te2Control.rsc" );


// ======== MEMBER FUNCTIONS ========

// ----------------------------------------------------------------------------
// CRankContainer::~CRankContainer
// ----------------------------------------------------------------------------
//
EXPORT_C CRankContainer* CRankContainer::NewL( const TRect& aRect, MModelObserver& aObserver )
    {
    CRankContainer* self = new (ELeave) CRankContainer( aObserver );
    CleanupStack::PushL ( self );
    self->ConstructL ( aRect );
    CleanupStack::Pop();
    return self;
    }

// ----------------------------------------------------------------------------
// CRankContainer::~CRankContainer
// ----------------------------------------------------------------------------
//
EXPORT_C CRankContainer::~CRankContainer()
    {
    delete iGameEngine;
    CEikonEnv::Static()->DeleteResourceFile( iResOffset );
    }

// ----------------------------------------------------------------------------
// CRankContainer::CleanRankL
// ----------------------------------------------------------------------------
//
EXPORT_C void CRankContainer::CleanRankL()
    {
    iGameEngine->HandleCommandL( EEngineCleanRank );
    HBufC* titleBuf = NULL;
    titleBuf = StringLoader::LoadLC( R_TETRIS2_TEXT_NO_LOCAL_SCORE );
    iRankingTitleLabel->SetTextL( *titleBuf );
    iRankingTitleLabel->SetColor( KRgbBlack );
    iRankingTitleLabel->SetRect( iLayoutMgr->LayoutRect( ERankingList ) );
    CleanupStack::PopAndDestroy(); // titleBuf
    titleBuf = NULL;
    for ( TInt i = 0; i < KMaxRankingList && iRankingListLabel[ i ]; i++ )
        {
        iRankingListLabel[ i ]->SetTextL( KNullDesC );
        }
    }

// ----------------------------------------------------------------------------
// CRankContainer::Draw
// ----------------------------------------------------------------------------
//
void CRankContainer::Draw( const TRect& aRect ) const
    {    
    CBaseContainer::Draw( aRect );
    }

// ----------------------------------------------------------------------------
// CRankContainer::SizeChanged
// ----------------------------------------------------------------------------
//
void CRankContainer::SizeChanged()
    {
    CBaseContainer::SizeChanged();
    if ( iRankingTitleLabel  )
        {
        iRankingTitleLabel->SetRect( iLayoutMgr->LayoutRect( ERankingTitle ) );
        TRect rect (  iLayoutMgr->LayoutRect( ERankingList ) );
        TInt height( rect.Height() / KMaxRankingList );
        for ( TInt i = 0; i < KMaxRankingList && iRankingListLabel[ i ]; i++ )
            {
            TRect theRect;
            theRect.iTl = rect.iTl;
            theRect.iTl.iY += height * i;
            theRect.iBr = rect.iBr;
            theRect.iBr.iY -= ( height * ( KMaxRankingList - 1 - i ) );
            
            iRankingListLabel[ i ]->SetRect( theRect );
            }
        }
    }

// ----------------------------------------------------------------------------
// CRankContainer::OfferKeyEventL
// ----------------------------------------------------------------------------
//
TKeyResponse CRankContainer::OfferKeyEventL( const TKeyEvent& aKeyEvent, 
    TEventCode aType )
    {
    return CBaseContainer::OfferKeyEventL( aKeyEvent, aType );
    }

// ----------------------------------------------------------------------------
// CRankContainer::Draw
// ----------------------------------------------------------------------------
//
void CRankContainer::Draw()
    {
    CBaseContainer::Draw();
    iRankingTitleLabel->Draw();
    for ( TInt i = 0; i < KMaxRankingList && iRankingListLabel[ i ]; i++ )
        {
        iRankingListLabel[ i ]->Draw();
        }
    }

// ----------------------------------------------------------------------------
// CRankContainer::StateChangedL
// ----------------------------------------------------------------------------
//
void CRankContainer::StateChangedL( TEngineState /*aNewState*/, 
                                       TEngineAction /*aAction*/ )
    { 
    // No implementation required
    }

// ----------------------------------------------------------------------------
// CRankContainer::CRankContainer
// ----------------------------------------------------------------------------
//
CRankContainer::CRankContainer( MModelObserver& aObserver )
: CBaseContainer( aObserver )
    { 
    // No implementation required
    }

// ----------------------------------------------------------------------------
// CRankContainer::ConstructL
// ----------------------------------------------------------------------------
//
void CRankContainer::ConstructL( const TRect& aRect )
    {
    CBaseContainer::ConstructL( aRect );

    // Adds the specified resource file to the list maintained by CCoeEnv.
    TFileName dll;
    Dll::FileName( dll );
    TPtrC drive = TParsePtrC( dll ).Drive();
    TFileName resourceFileName;
    resourceFileName.Copy( drive );
    resourceFileName.Append( KResourceFileName );
    TInt err = CompleteWithAppPath( resourceFileName );
    if ( err != KErrNotSupported )
        {
        User::LeaveIfError( err );
        }

    CCoeEnv* env = CCoeEnv::Static();
    BaflUtils::NearestLanguageFile( env->FsSession(), resourceFileName );    
    iResOffset = env->AddResourceFileL( resourceFileName );

    iGameEngine = CTe2Engine::NewL( *this );

    TRect rect( iLayoutMgr->LayoutRect( ERankingTitle ) );

    iRankingTitleLabel = CLabelControl::NewL( rect, iBmpBufferContext );
    iRankingTitleLabel->SetContainerWindowL( *this );
    
    if ( EQhdPortrait == CTe2LayoutManager::Resolution() )
        {
        iRankingTitleLabel->SetFont( LatinBold19() );
        }
    else
        {
        iRankingTitleLabel->SetFont( LatinBold17() );
        }

    iRankingTitleLabel->SetTextAlign( CGraphicsContext::ECenter );
    Components().AppendLC( iRankingTitleLabel );
    CleanupStack::Pop( iRankingTitleLabel );

    rect = iLayoutMgr->LayoutRect( ERankingList );
    TInt height( rect.Height() / KMaxRankingList );
    TInt count( KNoRankScore );
    for ( TInt i = 0; i < KMaxRankingList; i++ )
        {
        TRect theRect;
        theRect.iTl = rect.iTl;
        theRect.iTl.iY += height * i;
        theRect.iBr = rect.iBr;
        theRect.iBr.iY -= ( height * ( KMaxRankingList - 1 - i ) );

        iRankingListLabel[ i ] = CLabelControl::NewL( theRect, iBmpBufferContext );
        iRankingListLabel[ i ]->SetContainerWindowL( *this );
        Components().AppendLC( iRankingListLabel[ i ] );
        CleanupStack::Pop( iRankingListLabel[ i ] );

        TTime time;
        TInt score;
        TInt err = iGameEngine->GetRankItem( i + 1, time, score );
        
        TDateTime dateTime = time.DateTime();
   
        if ( score > KUnusableScore && err == KErrNone )
            {
            TBuf<KMaxRankLen> rankBuffer;
            rankBuffer.Format( KRankFormat,
                    i + 1,
                    dateTime.Year(),
                    dateTime.Month() + 1,
                    dateTime.Day() + 1,
                    dateTime.Hour(),
                    dateTime.Minute(),
                    score
                    );

            iRankingListLabel[ i ]->SetTextL( rankBuffer );
            ++count;
            }
        else
            {
            break;
            }
        iRankingListLabel[ i ]->SetColor( KRgbWhite );
        
        if ( EQhdPortrait == CTe2LayoutManager::Resolution() )
            {
            iRankingListLabel[ i ]->SetFont( LatinBold16() );
            }
        else
            {
            iRankingListLabel[ i ]->SetFont( LatinBold13() );
            }

        iRankingListLabel[ i ]->SetTextAlign( CGraphicsContext::ELeft );

        }
    
    HBufC* titleBuf = NULL;
    if ( count > KNoRankScore )
        {
        titleBuf = StringLoader::LoadLC( R_TETRIS2_TEXT_RANK_TITLE );
        iRankingTitleLabel->SetColor( KRgbRed );
        }
    else
        {
        titleBuf = StringLoader::LoadLC( R_TETRIS2_TEXT_NO_LOCAL_SCORE );
        iRankingTitleLabel->SetColor( KRgbBlack );
        iRankingTitleLabel->SetRect( iLayoutMgr->LayoutRect( ERankingList ) );
        }
    iRankingTitleLabel->SetTextL( *titleBuf );
    CleanupStack::PopAndDestroy(); // titleBuf
    titleBuf = NULL;

    ActivateL();
    }

// End of File
