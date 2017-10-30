/* ====================================================================
 * File: ScorekeeperControl.cpp
 * Created: 03/15/09
 * Modified: 03/31/09
 * Author: Sun Jinbo
 * Copyright (c): Tieto, All rights reserved
 * ==================================================================== */

// INCLUDE FILES
#include <fbs.h>
#include <eikenv.h>
#include <barsread.h>
#include <bautils.h>
#include <AknUtils.h>
#include <StringLoader.h>
#include <Te2Control.rsg>
#include <EngineAction.h> // TEngineAction
#include <Logger.h>
#include "ScorekeeperControl.h"
#include "LayoutManager.h"

// CONSTANTS
_LIT( KResourceFileName, "\\resource\\apps\\Te2Control.rsc" );

const TInt KMaxBuffer = 32;
const TInt KMarginOffset = 24;

// ======== MEMBER FUNCTIONS ========

// ----------------------------------------------------------------------------
// CScorekeeperControl::NewL
// ----------------------------------------------------------------------------
//
CScorekeeperControl* CScorekeeperControl::NewL( const TRect& aRect, CFbsBitGc*& aBgc )
    {
    CScorekeeperControl* self = new (ELeave) CScorekeeperControl( aBgc );
    CleanupStack::PushL (self );
    self->ConstructL (aRect);
    CleanupStack::Pop();
    return self;
    }

// ----------------------------------------------------------------------------
// CScorekeeperControl::~CScorekeeperControl
// ----------------------------------------------------------------------------
//
CScorekeeperControl::~CScorekeeperControl()
    {
    CEikonEnv::Static()->DeleteResourceFile( iResOffset );
    iEikonEnv->ScreenDevice()->ReleaseFont( iTitleFont );
    iEikonEnv->ScreenDevice()->ReleaseFont( iValueFont );
    delete iLevelText;
    delete iLineText;
    delete iScoreText;
    delete iLifeText;
    }

// ----------------------------------------------------------------------------
// CScorekeeperControl::Draw
// ----------------------------------------------------------------------------
//
void CScorekeeperControl::Draw(const TRect& /*aRect*/) const
    {
    // No implementation required.
    }

// ----------------------------------------------------------------------------
// CScorekeeperControl::UpdateScorekeeper
// ----------------------------------------------------------------------------
//
void CScorekeeperControl::UpdateScorekeeper( const TEngineAction& aAction )
    {
    TInt32 value;
    aAction.Get( value );
    switch ( aAction.Type() )
        {
        case EScorekeeperActionLevel:
            iLevel = value;
            break;
        case EScorekeeperActionLine:
            iLine = value;
            break;
        case EScorekeeperActionScore:
            iScore = value;
            break;
        case EScorekeeperActionLife:
            iLife = ( --value >= 0 ) ? value : 0;
            break;
        default:
            break;
        }
    DrawNow();
    }

// ----------------------------------------------------------------------------
// CScorekeeperControl::Draw
// ----------------------------------------------------------------------------
//
void CScorekeeperControl::Draw()
    {
    iBgc->SetPenStyle( CGraphicsContext::ESolidPen );
    iBgc->SetBrushStyle( CGraphicsContext::ENullBrush );

    TInt height = Rect().Height() / 6;
    TInt width = Rect().Width();
    TPoint topLeft( Rect().iTl );
    TRect rect( topLeft, TSize( width, height ) );

    TInt titleBaseline = 
        ( height + iTitleFont->HeightInPixels() ) / 2;
    
    TInt valueBaseline = 
        ( height + iValueFont->HeightInPixels() ) / 2;
    
    //
    // Draw level
    //
    iBgc->SetPenColor( KRgbMagenta );
    iBgc->UseFont( iTitleFont );
    iBgc->DrawText( *iLevelText, rect, titleBaseline, CGraphicsContext::ELeft, 0 );
    iBgc->DiscardFont();

    topLeft.iY += height;
    rect.SetRect( 
				TPoint( topLeft.iX + KMarginOffset, topLeft.iY ), 
				TSize( width, height ) );

    TBuf<KMaxBuffer> levelText;
    levelText.AppendNum( iLevel );
    iBgc->SetPenColor( KRgbWhite );
    iBgc->UseFont( iValueFont );
    iBgc->DrawText( levelText, rect, valueBaseline, CGraphicsContext::ELeft, 0 );
    iBgc->DiscardFont();

    topLeft.iY += height;
    rect.SetRect( topLeft, TSize( width, height ) );

    //
    // Draw score
    //
    iBgc->SetPenColor( KRgbMagenta );
    iBgc->UseFont( iTitleFont );
    iBgc->DrawText( *iScoreText, rect, titleBaseline, CGraphicsContext::ELeft, 0 );
    iBgc->DiscardFont();

    topLeft.iY += height;
    rect.SetRect( 
				TPoint( topLeft.iX + KMarginOffset, topLeft.iY ), 
				TSize( width, height ) );

    TBuf<KMaxBuffer> scoreText;
    scoreText.AppendNum( iScore );
    iBgc->SetPenColor( KRgbWhite );
    iBgc->UseFont( iValueFont );
    iBgc->DrawText( scoreText, rect, valueBaseline, CGraphicsContext::ELeft, 0 );
    iBgc->DiscardFont();

    topLeft.iY += height;
    rect.SetRect( topLeft, TSize( width, height ) );

    //
    // Draw life
    //
    iBgc->SetPenColor( KRgbMagenta );
    iBgc->UseFont( iTitleFont );
    iBgc->DrawText( *iLifeText, rect, titleBaseline, CGraphicsContext::ELeft, 0 );
    iBgc->DiscardFont();

    topLeft.iY += height;
    rect.SetRect( 
				TPoint( topLeft.iX + KMarginOffset, topLeft.iY ), 
				TSize( width, height ) );

    TBuf<KMaxBuffer> lifeText;
    lifeText.AppendNum( iLife );
    iBgc->SetPenColor( KRgbWhite );
    iBgc->UseFont( iValueFont );
    iBgc->DrawText( lifeText, rect, valueBaseline, CGraphicsContext::ELeft, 0 );
    iBgc->DiscardFont();

    // Discards a selected device font.
    iBgc->DiscardFont();
    }

// ----------------------------------------------------------------------------
// CScorekeeperControl::CScorekeeperControl
// ----------------------------------------------------------------------------
//
CScorekeeperControl::CScorekeeperControl( CFbsBitGc*& aBgc )
: iBgc( aBgc )
    {
    // No implementation required.
    }

// ----------------------------------------------------------------------------
// CScorekeeperControl::ConstructL
// ----------------------------------------------------------------------------
//
void CScorekeeperControl::ConstructL( const TRect& aRect )
    {
    // Create a window for this application view
    CreateWindowL();

    // Set the windows size
    SetRect( aRect );

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

    TFontSpec fontSpec = iEikonEnv->LegendFont()->FontSpecInTwips();
    fontSpec.iFontStyle.SetBitmapType( EMonochromeGlyphBitmap );
    
    if ( EQhdPortrait == CTe2LayoutManager::Resolution() )
        {
        fontSpec.iHeight  -= fontSpec.iHeight / 6;
        }
    else
        {
        fontSpec.iHeight  -= fontSpec.iHeight / 4;
        }

    User::LeaveIfError( iEikonEnv->ScreenDevice()->GetNearestFontInTwips( iTitleFont, fontSpec ) );

    fontSpec.iHeight  -= fontSpec.iHeight / 8;
    User::LeaveIfError( iEikonEnv->ScreenDevice()->GetNearestFontInTwips( iValueFont, fontSpec ) );

    iLevelText = StringLoader::LoadL( R_TETRIS2_TEXT_LEVEL );
    iLineText = StringLoader::LoadL( R_TETRIS2_TEXT_LINE );
    iScoreText = StringLoader::LoadL( R_TETRIS2_TEXT_SCORE );
    iLifeText = StringLoader::LoadL( R_TETRIS2_TEXT_LIFE );
    
    // Activate the window, which makes it ready to be drawn
    ActivateL();
    }

// End of File
