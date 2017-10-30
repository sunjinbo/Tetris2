/* ====================================================================
 * File: MenubarControl.cpp
 * Created: 03/15/09
 * Modified: 03/17/09
 * Author: Sun Jinbo
 * Copyright (c): Tieto, All rights reserved
 * ==================================================================== */

// INCLUDE FILES
#include <fbs.h>
#include <eikenv.h>
#include "MenubarControl.h"

// ======== MEMBER FUNCTIONS ========

// ----------------------------------------------------------------------------
// CMenubarControl::NewL
// ----------------------------------------------------------------------------
//
CMenubarControl* CMenubarControl::NewL( const TRect& aRect, CFbsBitGc*& aBgc )
    {
    CMenubarControl* self = new (ELeave) CMenubarControl( aBgc );
    CleanupStack::PushL (self );
    self->ConstructL (aRect);
    CleanupStack::Pop();
    return self;
    }

// ----------------------------------------------------------------------------
// CMenubarControl::~CMenubarControl
// ----------------------------------------------------------------------------
//
CMenubarControl::~CMenubarControl()
    {
    delete iMenuText;
    }

// ----------------------------------------------------------------------------
// CMenubarControl::Draw
// ----------------------------------------------------------------------------
//
void CMenubarControl::Draw(const TRect& /*aRect*/) const
    {
    // No implementation required
    }

// ----------------------------------------------------------------------------
// CMenubarControl::SizeChanged
// ----------------------------------------------------------------------------
//
void CMenubarControl::SizeChanged()
    {
    // No implementation required
    }

// ----------------------------------------------------------------------------
// CMenubarControl::SetTitleL
// ----------------------------------------------------------------------------
//
void CMenubarControl::SetTitleL( const TDesC& aText )
    {
    if ( iMenuText )
        {
        delete iMenuText;
        iMenuText = NULL;
        }
    iMenuText =  HBufC::NewL( aText.Length() );
    TPtr text( iMenuText->Des() );
    text.Copy( aText );
    }

// ----------------------------------------------------------------------------
// CMenubarControl::SetAlignment
// ----------------------------------------------------------------------------
//
void CMenubarControl::SetAlignment( CGraphicsContext::TTextAlign aAlign )
    {
    iAlign = aAlign;
    }

// ----------------------------------------------------------------------------
// CMenubarControl::Draw
// ----------------------------------------------------------------------------
//
void CMenubarControl::Draw()
    {
    iBgc->SetBrushStyle( CGraphicsContext::ENullBrush );
    iBgc->SetPenColor( KRgbWhite );
    iBgc->SetPenStyle( CGraphicsContext::ESolidPen );
    
    CFont* font( NULL );
    TFontSpec fontSpec = iEikonEnv->LegendFont()->FontSpecInTwips();
    fontSpec.iHeight  += fontSpec.iHeight / 2;
    fontSpec.iFontStyle.SetBitmapType( EMonochromeGlyphBitmap );
    iEikonEnv->ScreenDevice()->GetNearestFontInTwips( font, fontSpec );
    iBgc->UseFont( font );

    if ( iMenuText )
        {
        TPoint origin;
        TSize wh;
        TInt borderWidth;
        TInt borderHeight;

        borderWidth = font->TextWidthInPixels( *iMenuText );
        borderHeight = font->HeightInPixels();

        wh.SetSize( borderWidth, borderHeight );
        origin.iX = ( Rect().Width() - borderWidth ) / 2;
        origin.iY = ( Rect().Height() - borderHeight ) / 2;

        TRect textRect( origin, wh );
        
        TInt baseline = 
            ( textRect.Height() + font->HeightInPixels() ) / 2;

        iBgc->DrawText( *iMenuText, Rect(), baseline, iAlign, 0 );
        }
    iBgc->DiscardFont();
    iEikonEnv->ScreenDevice()->ReleaseFont( font );
    }

// ----------------------------------------------------------------------------
// CMenubarControl::CMenubarControl
// ----------------------------------------------------------------------------
//
CMenubarControl::CMenubarControl( CFbsBitGc*& aBgc )
: iBgc( aBgc )
    {
    // No implementation required.
    }

// ----------------------------------------------------------------------------
// CMenubarControl::ConstructL
// ----------------------------------------------------------------------------
//
void CMenubarControl::ConstructL( const TRect& aRect )
    {
    // Create a window for this application view
    CreateWindowL();

    // Set the windows size
    SetRect( aRect );

    // Activate the window, which makes it ready to be drawn
    ActivateL();
    }

// End of File
