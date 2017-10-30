/* ====================================================================
 * File: LabelControl.cpp
 * Created: 06/06/10
 * Modified: 06/06/10
 * Author: Sun Jinbo
 * Copyright (c): Tieto, All rights reserved
 * ==================================================================== */

// INCLUDE FILES
#include <fbs.h>
#include <eikenv.h>
#include "LabelControl.h"

// CONSTANTS

// ======== MEMBER FUNCTIONS ========

// ----------------------------------------------------------------------------
// CLabelControl::NewL
// ----------------------------------------------------------------------------
//
CLabelControl* CLabelControl::NewL( const TRect& aRect, CFbsBitGc*& aBgc )
    {
    CLabelControl* self = new (ELeave) CLabelControl( aBgc );
    CleanupStack::PushL (self );
    self->ConstructL (aRect);
    CleanupStack::Pop();
    return self;
    }

// ----------------------------------------------------------------------------
// CLabelControl::~CLabelControl
// ----------------------------------------------------------------------------
//
CLabelControl::~CLabelControl()
    {
    delete iText;
    }

// ----------------------------------------------------------------------------
// CLabelControl::Draw
// ----------------------------------------------------------------------------
//
void CLabelControl::Draw(const TRect& /*aRect*/) const
    { // No implementation required
    }

// ----------------------------------------------------------------------------
// CLabelControl::SetTextL
// ----------------------------------------------------------------------------
//
void CLabelControl::SetTextL( const TDesC& aText )
    {
    if ( iText )
        {
        delete iText;
        iText = NULL;
        }
    iText =  HBufC::NewL( aText.Length() );
    TPtr text( iText->Des() );
    text.Copy( aText );
    }

// ----------------------------------------------------------------------------
// CLabelControl::SetColor
// ----------------------------------------------------------------------------
//
void CLabelControl::SetColor( TRgb aColor )
    {
    iColor = aColor;
    }

// ----------------------------------------------------------------------------
// CLabelControl::SetFont
// ----------------------------------------------------------------------------
//
void CLabelControl::SetFont( const CFont* aFont )
    {
    iFont = aFont;
    }

// ----------------------------------------------------------------------------
// CLabelControl::SetTextAlign
// ----------------------------------------------------------------------------
//
void CLabelControl::SetTextAlign( CGraphicsContext::TTextAlign aAlign )
    {
    iAlign = aAlign;
    }

// ----------------------------------------------------------------------------
// CLabelControl::Draw
// ----------------------------------------------------------------------------
//
void CLabelControl::Draw()
    {
    TRgb itemColor( KRgbBlack );
    iBgc->SetPenStyle( CGraphicsContext::ESolidPen );
    iBgc->SetPenColor( iColor );
    if ( iText && iFont )
        {
        iBgc->UseFont( iFont );
        
        TPoint origin;
        TSize wh;
        TInt borderWidth;
        TInt borderHeight;
        
        borderWidth = iFont->TextWidthInPixels( *iText );
        borderHeight = iFont->HeightInPixels();
        
        wh.SetSize( borderWidth, borderHeight );
        origin.iX = ( Rect().Width() - borderWidth ) / 2;
        origin.iY = ( Rect().Height() - borderHeight ) / 2;
        
        TRect textRect( origin, wh );
        
        TInt baseline = 
            ( textRect.Height() + iFont->HeightInPixels() ) / 2;
        
        iBgc->DrawText( *iText, Rect(), baseline, iAlign, 0 );
        iBgc->DiscardFont();
        }
    }

// ----------------------------------------------------------------------------
// CLabelControl::CLabelControl
// ----------------------------------------------------------------------------
//
CLabelControl::CLabelControl( CFbsBitGc*& aBgc )
: iBgc( aBgc )
    { 
    // No implementation required.
    }

// ----------------------------------------------------------------------------
// CLabelControl::ConstructL
// ----------------------------------------------------------------------------
//
void CLabelControl::ConstructL( const TRect& aRect )
    {
    // Create a window for this application view
    CreateWindowL();

    // Set the windows size
    SetRect(aRect);

    // Activate the window, which makes it ready to be drawn
    ActivateL();
    }

// End of File
