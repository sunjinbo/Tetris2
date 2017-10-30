/* ====================================================================
 * File: ScrollControl.cpp
 * Created: 03/15/09
 * Modified: 04/05/09
 * Author: Sun Jinbo
 * Copyright (c): Tieto, All rights reserved
 * ==================================================================== */

// INCLUDE FILES
#include <fbs.h>
#include <eikenv.h>
#include "ScrollControl.h"

// CONSTANTS
const TInt KScorllDelayTimeout = 0;
const TInt KScorllUpdateTimeout = 1000000;
const TInt KTextItemListCranularity = 10;
const TInt KScrollUnavailableIndex = -1;
const TInt KScrollUnavailableValue = -1;
const TInt KScrollinitializeValue = 0;

// ======== MEMBER FUNCTIONS ========

// ----------------------------------------------------------------------------
// CScrollControl::NewL
// ----------------------------------------------------------------------------
//
CScrollControl* CScrollControl::NewL( const TRect& aRect, CFbsBitGc*& aBgc )
    {
    CScrollControl* self = new (ELeave) CScrollControl( aBgc );
    CleanupStack::PushL (self );
    self->ConstructL (aRect);
    CleanupStack::Pop();
    return self;
    }

// ----------------------------------------------------------------------------
// CScrollControl::~CScrollControl
// ----------------------------------------------------------------------------
//
CScrollControl::~CScrollControl()
    {
    if ( iPeriodic )
        {
        iPeriodic->Cancel();
        delete iPeriodic;
        }
	//iTextArray.ResetAndDestroy();
    iTextArray.Close();
    iEikonEnv->ScreenDevice()->ReleaseFont( iFont );
    }

// ----------------------------------------------------------------------------
// CScrollControl::Draw
// ----------------------------------------------------------------------------
//
void CScrollControl::Draw(const TRect& /*aRect*/) const
    {
    // No implementation required
    }

// ----------------------------------------------------------------------------
// CScrollControl::AppendTextL
// ----------------------------------------------------------------------------
//
void CScrollControl::AppendTextL( const TDesC& aText )
    {
    AppendL( aText );
    // Start();
    }

// ----------------------------------------------------------------------------
// CRollpacker::Clear
// ----------------------------------------------------------------------------
//
void CScrollControl::Clear()
    {
    iTextArray.ResetAndDestroy();
    }

// ----------------------------------------------------------------------------
// CScrollControl::Start
// ----------------------------------------------------------------------------
//
void CScrollControl::Start()
    {
    if ( iTextArray.Count() )
        {
        InitializeScroll();
        iPeriodic->Cancel();
        iPeriodic->Start(
                KScorllDelayTimeout,
                KScorllUpdateTimeout,
                TCallBack( TimerCallback, this ) );
        }
    }

// ----------------------------------------------------------------------------
// CScrollControl::Draw
// ----------------------------------------------------------------------------
//
void CScrollControl::Draw()
    {
    TRgb itemColor( KRgbBlack );
    iBgc->SetPenStyle( CGraphicsContext::ESolidPen );
    iBgc->SetPenColor( itemColor );

    iBgc->UseFont( iFont );

    HBufC* textItem( NULL );
    TInt height = Rect().Height() / KScrollLineNumber;
    TInt width = Rect().Width();
    TPoint topLeft( Rect().iTl );
    for ( TInt i = 0; i < KScrollLineNumber; ++i )
        {
        if ( iScrollCabinet[ i ] != KScrollUnavailableIndex
            && iScrollCabinet[ i ] < iTextArray.Count() )
            {
            TRect rect( topLeft, TSize( width, height ) );
            textItem = iTextArray[ iScrollCabinet[ i ] ];
            
            TPoint origin;
            TSize wh;
            TInt borderWidth;
            TInt borderHeight;
            
            borderWidth = iFont->TextWidthInPixels( *textItem );
            borderHeight = iFont->HeightInPixels();
            
            wh.SetSize( borderWidth, borderHeight );
            origin.iX = ( rect.Width() - borderWidth ) / 2;
            origin.iY = ( rect.Height() - borderHeight ) / 2;
            
            TRect textRect( origin, wh );
            
            TInt baseline = 
                ( textRect.Height() + iFont->HeightInPixels() ) / 2;
            
            iBgc->DrawText( *textItem, rect, baseline, CGraphicsContext::ECenter, 0 );
            }
        topLeft.iY += height;
        }

    iBgc->DiscardFont();
    }

// ----------------------------------------------------------------------------
// CScrollControl::CScrollControl
// ----------------------------------------------------------------------------
//
CScrollControl::CScrollControl( CFbsBitGc*& aBgc )
    : iBgc( aBgc ), iTextArray( KTextItemListCranularity )
    { // No implementation required.
    }

// ----------------------------------------------------------------------------
// CScrollControl::ConstructL
// ----------------------------------------------------------------------------
//
void CScrollControl::ConstructL( const TRect& aRect )
    {
    // Create a window for this application view
    CreateWindowL();

    // Set the windows size
    SetRect(aRect);
    
    iPeriodic = CPeriodic::NewL( CActive::EPriorityStandard );

    TFontSpec fontSpec = iEikonEnv->SymbolFont()->FontSpecInTwips();
    fontSpec.iHeight  -= fontSpec.iHeight / 3;
    fontSpec.iFontStyle.SetBitmapType( EMonochromeGlyphBitmap );
    User::LeaveIfError( iEikonEnv->ScreenDevice()->GetNearestFontInTwips( iFont, fontSpec ) );

    // Activate the window, which makes it ready to be drawn
    ActivateL();
    }

// ----------------------------------------------------------------------------
// CScrollControl::Stop
// ----------------------------------------------------------------------------
//
void CScrollControl::Stop()
    {
    if ( iPeriodic )
        {  
        iPeriodic->Cancel();
        }
    }

// ----------------------------------------------------------------------------
// CScrollControl::DoScroll
// ----------------------------------------------------------------------------
//
void CScrollControl::DoScroll()
    {
    if ( iHeadIndex == KScrollUnavailableIndex
        && iTailIndex == KScrollUnavailableIndex
        && iHeadValue == KScrollUnavailableValue )
        { 
        // It's the first step of scrolling text item.
        iHeadIndex = KScrollLineNumber - 1;
        iTailIndex = KScrollLineNumber - 1;
        iHeadValue = KScrollinitializeValue;
        }
    else
        { 
        // other scroll steps
        if ( --iScrollStep )
            {
            if ( --iCabinetStep )
                {
                iHeadIndex--;
                }
            else
                {
                iHeadValue++;
                }

            if ( KScrollLineNumber  >= iScrollStep )
                {
                iTailIndex--;
                }
            }
        else
            { // Running out of scroll steps, restart again.
            Start();
            }
        }
    UpdateCabinetArray();
    }

// ----------------------------------------------------------------------------
// CScrollControl::TimerCallback
// ----------------------------------------------------------------------------
//
TInt CScrollControl::TimerCallback( TAny* aPtr )
    {
    CScrollControl* self = static_cast<CScrollControl*> ( aPtr );
    self->DoScroll();
    return 0;
    }

// ----------------------------------------------------------------------------
// CScrollControl::InitializeScroll
// ----------------------------------------------------------------------------
//
void CScrollControl::InitializeScroll()
    {
    iHeadIndex = KScrollUnavailableIndex;
    iTailIndex =  KScrollUnavailableIndex;
    iHeadValue = KScrollUnavailableValue;
    iScrollStep = KScrollLineNumber + iTextArray.Count();
    iCabinetStep = KScrollLineNumber;
    UpdateCabinetArray();
    }

// ----------------------------------------------------------------------------
// CScrollControl::UpdateCabinetArray
// ----------------------------------------------------------------------------
//
void CScrollControl::UpdateCabinetArray()
    {
    for ( TInt i = 0; i < KScrollLineNumber; ++i )
        {
        if ( ( i >= iHeadIndex ) && ( i <= iTailIndex ) )
            {
            iScrollCabinet[ i ] = iHeadValue + i - iHeadIndex;    
            }
        else
            {
            iScrollCabinet[ i ] = KScrollUnavailableIndex;    
            }
        }
    }

// ----------------------------------------------------------------------------
// CScrollControl::AppendL
// ----------------------------------------------------------------------------
//
void CScrollControl::AppendL( const TDesC& aText )
    {
    TPtrC ptr( aText );
    const TChar KLf( '\n' );
    const TInt pos = aText.Locate( KLf );

    HBufC* txt( NULL );
    if ( pos != KErrNotFound )
        {
        ptr.Set( aText.Ptr(), pos );
        txt = HBufC::NewL( ptr.Length() );
        TPtr txtPtr( txt->Des() );
        txtPtr.Copy( ptr );
        iTextArray.Append( txt );
        }
    else
        {
        if ( aText.Length() != 0 )
            {
            txt = HBufC::NewL( aText.Length() );
            TPtr txtPtr( txt->Des() );
            txtPtr.Copy( aText );
            iTextArray.Append( txt );
            }
        }

    if ( pos != KErrNotFound )
        {
        ptr.Set( &aText.Ptr()[ pos + 1 ], aText.Length() - pos - 1 );

        AppendL( ptr ); // Recursive call
        }
    }

// End of File
