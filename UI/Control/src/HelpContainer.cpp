/* ====================================================================
 * File: HelpContainer.cpp
 * Created: 03/15/09
 * Modified: 05/05/09
 * Author: Sun Jinbo
 * Copyright (c): Tieto, All rights reserved
 * ==================================================================== */

// INCLUDE FILES
#include <akndef.h>
#include <aknutils.h>
#include <coecntrl.h>
#include "HelpContainer.h"
#include "ScrollControl.h"
#include "LayoutManager.h"

// CONSTANTS

// ======== MEMBER FUNCTIONS ========

// ----------------------------------------------------------------------------
// CHelpContainer::~CHelpContainer
// ----------------------------------------------------------------------------
//
EXPORT_C CHelpContainer* CHelpContainer::NewL( const TRect& aRect, MModelObserver& aObserver )
    {
    CHelpContainer* self = new (ELeave) CHelpContainer( aObserver );
    CleanupStack::PushL ( self );
    self->ConstructL ( aRect );
    CleanupStack::Pop();
    return self;
    }

// ----------------------------------------------------------------------------
// CHelpContainer::~CHelpContainer
// ----------------------------------------------------------------------------
//
EXPORT_C CHelpContainer::~CHelpContainer()
    { // No implementation required
    }

// ----------------------------------------------------------------------------
// CHelpContainer::Draw
// ----------------------------------------------------------------------------
//
void CHelpContainer::Draw( const TRect& aRect ) const
    {    
    CBaseContainer::Draw( aRect );
    }

// ----------------------------------------------------------------------------
// CHelpContainer::SizeChanged
// ----------------------------------------------------------------------------
//
void CHelpContainer::SizeChanged()
    {
    CBaseContainer::SizeChanged();
    if ( iScroll )
        {
        iScroll->SetRect( iLayoutMgr->LayoutRect( EScroller ) );
        }
    }

// ----------------------------------------------------------------------------
// CHelpContainer::OfferKeyEventL
// ----------------------------------------------------------------------------
//
TKeyResponse CHelpContainer::OfferKeyEventL( const TKeyEvent& aKeyEvent, 
    TEventCode aType )
    {
    return CBaseContainer::OfferKeyEventL( aKeyEvent, aType );
    }

// ----------------------------------------------------------------------------
// CHelpContainer::AddTextItemL
// ----------------------------------------------------------------------------
//
EXPORT_C void CHelpContainer::AddTextItemL( const TDesC &  aText )
    {
    iScroll->AppendTextL( aText );
    }

// ----------------------------------------------------------------------------
// CHelpContainer::StartL
// ----------------------------------------------------------------------------
//
EXPORT_C void CHelpContainer::Start()
    {
    iScroll->Start();
    }

// ----------------------------------------------------------------------------
// CHelpContainer::Draw
// ----------------------------------------------------------------------------
//
void CHelpContainer::Draw()
    {
    CBaseContainer::Draw();
    iScroll->Draw();
    }

// ----------------------------------------------------------------------------
// CHelpContainer::CHelpContainer
// ----------------------------------------------------------------------------
//
CHelpContainer::CHelpContainer( MModelObserver& aObserver )
: CBaseContainer( aObserver )
    { 
    // No implementation required
    }

// ----------------------------------------------------------------------------
// CHelpContainer::ConstructL
// ----------------------------------------------------------------------------
//
void CHelpContainer::ConstructL( const TRect& aRect )
    {
    CBaseContainer::ConstructL( aRect );
    TRect rect( iLayoutMgr->LayoutRect( EScroller ) );
    iScroll = CScrollControl::NewL( rect, iBmpBufferContext );
    iScroll->SetContainerWindowL( *this );
    Components().AppendLC( iScroll );
    CleanupStack::Pop( iScroll );
    ActivateL();
    }

// End of File
