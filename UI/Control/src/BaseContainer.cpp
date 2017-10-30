/* ====================================================================
 * File: BaseContainer.cpp
 * Created: 03/15/09
 * Modified: 05/09/09
 * Author: Sun Jinbo
 * Copyright (c): Tieto, All rights reserved
 * ==================================================================== */

// INCLUDE FILES
#include <akndef.h>
#include <aknutils.h>
#include "BaseContainer.h"
#include "BackgroundControl.h"
#include "MenubarControl.h"
#include "ModelObserver.h"

#include "Logger.h"

// CONSTANTS
const TInt KDoDelayTimeout = 0;
const TInt KDoFrameUpdateTimeout = 100000;

// ======== MEMBER FUNCTIONS ========

// ----------------------------------------------------------------------------
// CBaseContainer::~CBaseContainer
// ----------------------------------------------------------------------------
//
CBaseContainer::~CBaseContainer()
    {
    Stop();
    ReleaseBmpBuffer();
    delete iLayoutMgr;
    }

// ----------------------------------------------------------------------------
// CBaseContainer::Draw
// ----------------------------------------------------------------------------
//
void CBaseContainer::Draw( const TRect& /*aRect*/ ) const
    { 
    // No implementation required
    }

// ----------------------------------------------------------------------------
// CBaseContainer::SizeChanged
// ----------------------------------------------------------------------------
//
void CBaseContainer::SizeChanged()
    {
    TRAP_IGNORE( CreateBmpBufferL() );

    // FOR BACKGROUND/MENUBAR Control
    if( iBackground && 
        iMenubar1 && iMenubar2 )
        {
        TRect rect;
        AknLayoutUtils::LayoutMetricsRect( AknLayoutUtils::EScreen, rect );
        iBackground->SetRect( rect );

        LayoutSoftkeys();
        }
    }

// ----------------------------------------------------------------------------
// CBaseContainer::HandleResourceChange
// ----------------------------------------------------------------------------
//
void CBaseContainer::HandleResourceChange( TInt aType )
    {
    CCoeControl::HandleResourceChange( aType );
    if ( aType == KEikDynamicLayoutVariantSwitch )
        {
        iLayoutMgr->ResolutionChanged();
        SetExtentToWholeScreen();
        }
    }

// ----------------------------------------------------------------------------
// CBaseContainer::OfferKeyEventL
// ----------------------------------------------------------------------------
//
TKeyResponse CBaseContainer::OfferKeyEventL( const TKeyEvent& aKeyEvent, 
    TEventCode aType )
    {
    TKeyResponse response( EKeyWasNotConsumed );
    if ( aType == EEventKeyUp )
        {
        switch ( aKeyEvent.iScanCode )
            {
            case EStdKeyDevice0:
                {
                iObserver.MenuItemSelectedL( iSoftkey1Id );
                response = EKeyWasConsumed;
                }
                break;
            case EStdKeyDevice1:
                {
                iObserver.MenuItemSelectedL( iSoftkey2Id );
                response = EKeyWasConsumed;
                }
                break;
            default:
                break;
            }
        }
    return response;
    }

// ----------------------------------------------------------------------------
// CBaseContainer::HandlePointerEventL
// ----------------------------------------------------------------------------
//
void CBaseContainer::HandlePointerEventL(const TPointerEvent& aPointerEvent)
    {
    if ( aPointerEvent.iType == TPointerEvent::EButton1Up )
        {
        if ( iMenubar1->Rect().Contains( aPointerEvent.iPosition ) )
            {
            iObserver.MenuItemSelectedL( iSoftkey1Id );
            }
            
        if ( iMenubar2->Rect().Contains( aPointerEvent.iPosition ) )
            {
            iObserver.MenuItemSelectedL( iSoftkey2Id );
            }
        }
    }

// ----------------------------------------------------------------------------
// CBaseContainer::SetMenuBar1L
// ----------------------------------------------------------------------------
//
EXPORT_C void CBaseContainer::SetMenuBar1L( const TDesC& aLabel, TInt aId )
    {
    iMenubar1->SetTitleL( aLabel );
    iSoftkey1Id = aId;
    }

// ----------------------------------------------------------------------------
// CBaseContainer::SetMenuBar2L
// ----------------------------------------------------------------------------
//
EXPORT_C void CBaseContainer::SetMenuBar2L( const TDesC& aLabel, TInt aId )
    {
    iMenubar2->SetTitleL( aLabel );
    iSoftkey2Id = aId;
    }

// ----------------------------------------------------------------------------
// CBaseContainer::StoreL
// ----------------------------------------------------------------------------
//
EXPORT_C void CBaseContainer::StoreL()
    {
    iBackground->StoreL();
    }

// ----------------------------------------------------------------------------
// CBaseContainer::Draw
// ----------------------------------------------------------------------------
//
void CBaseContainer::Draw()
    {
    iBackground->Draw();
    iMenubar1->Draw();
    iMenubar2->Draw();
    }

// ----------------------------------------------------------------------------
// CBaseContainer::CBaseContainer
// ----------------------------------------------------------------------------
//
CBaseContainer::CBaseContainer( MModelObserver& aObserver )
    : iObserver( aObserver )
    { // No implementation required
    }

// ----------------------------------------------------------------------------
// CBaseContainer::ConstructL
// ----------------------------------------------------------------------------
//
void CBaseContainer::ConstructL( const TRect& aRect )
    {
    // Creates a control's window.
    // The created window is the child of the application's window group.
    CreateWindowL();

    // Sets the control's extent ¡ª specifying a TRect.
    // Calling this function results in a call to SizeChanged.
    SetRect( aRect ); // --> call SizeChanged

    SetExtentToWholeScreen();

    // For sub-CCoeControl
    iLayoutMgr = CTe2LayoutManager::NewL();
    iLayoutMgr->ResolutionChanged();
    iResolution = iLayoutMgr->Resolution();
    
    InitComponentArrayL();
    TRect rect = iLayoutMgr->LayoutRect( EBackground );
    iBackground = CBackgroundControl::NewL( rect, iBmpBufferContext );
    iBackground->SetContainerWindowL( *this );
    Components().AppendLC( iBackground );
    CleanupStack::Pop( iBackground );

    rect = iLayoutMgr->LayoutRect( ESoftkey1 );
    iMenubar1 = CMenubarControl::NewL( rect, iBmpBufferContext );
    iMenubar1->SetContainerWindowL( *this );
    Components().AppendLC( iMenubar1 );
    CleanupStack::Pop( iMenubar1 );
    
    rect = iLayoutMgr->LayoutRect( ESoftkey2 );
    iMenubar2 = CMenubarControl::NewL( rect, iBmpBufferContext );
    iMenubar2->SetContainerWindowL( *this );
    Components().AppendLC( iMenubar2 );
    CleanupStack::Pop( iMenubar2 );
    
    LayoutSoftkeys();

    iPeriodic = CPeriodic::NewL( CActive::EPriorityLow );
    iPeriodic->Cancel();
    iPeriodic->Start(
            KDoDelayTimeout,
            KDoFrameUpdateTimeout,
            TCallBack( TimerCallback, this ) );

    if ( CTe2LayoutManager::Resolution() == EQhdPortrait )
        {
        // Enables handling of drag events
        EnableDragEvents();
        }

    // Sets control as ready to be drawn.
    ActivateL();
    }

// ----------------------------------------------------------------------------
// CBaseContainer::CreateBmpBufferL
// ----------------------------------------------------------------------------
//
void CBaseContainer::CreateBmpBufferL()
    {
    // If double buffer resouces has been created, 
    // release them at first.
    ReleaseBmpBuffer();

    // Create double buffer bitmap
    iBmpBuffer = new (ELeave) CFbsBitmap;
    User::LeaveIfError( iBmpBuffer->Create( Size(), EColor16MAP ) );

    // Create double buffer graphics context
    iBmpBufferDevice = CFbsBitmapDevice::NewL( iBmpBuffer );
    User::LeaveIfError( iBmpBufferDevice->CreateContext( iBmpBufferContext ) );
    iBmpBufferContext->SetPenStyle( CGraphicsContext::ESolidPen );
    }

// ----------------------------------------------------------------------------
// CBaseContainer::ReleaseBmpBuffer
// ----------------------------------------------------------------------------
//
void CBaseContainer::ReleaseBmpBuffer()
    {
    delete iBmpBufferContext;
    iBmpBufferContext = NULL;
    delete iBmpBufferDevice;
    iBmpBufferDevice = NULL;
    delete iBmpBuffer;
    iBmpBuffer = NULL;
    }

// ----------------------------------------------------------------------------
// CBaseContainer::Stop
// ----------------------------------------------------------------------------
//
void CBaseContainer::Stop()
    {
    if ( iPeriodic )
        {  
        iPeriodic->Cancel();
        delete iPeriodic;
        iPeriodic = NULL;   
        }
    }

// ----------------------------------------------------------------------------
// CBaseContainer::DoFrame
// ----------------------------------------------------------------------------
//
void CBaseContainer::DoFrame()
    {
    Window().Invalidate( Rect() );
    ActivateGc();
    Window().BeginRedraw( Rect() );
    Draw();
    CWindowGc& gc = SystemGc();
    gc.BitBlt( TPoint( 0, 0 ), iBmpBuffer );
    Window().EndRedraw();
    DeactivateGc();
    }

// ----------------------------------------------------------------------------
// CBaseContainer::Stop
// ----------------------------------------------------------------------------
//
TInt CBaseContainer::TimerCallback( TAny* aPtr )
    {
    CBaseContainer* self = static_cast<CBaseContainer*> ( aPtr );
    self->DoFrame();
    return 0;
    }

// ----------------------------------------------------------------------------
// CBaseContainer::LayoutSoftkeys
// ----------------------------------------------------------------------------
//
void CBaseContainer::LayoutSoftkeys()
    {
    _LOG_WRITE( "CBaseContainer::LayoutSoftkeys()" )
    TRect rect;
    switch ( AknLayoutUtils::CbaLocation() )
        {
        case AknLayoutUtils::EAknCbaLocationBottom:
            { 
            // Landscape and Portrait
            _LOG_WRITE( "AknLayoutUtils::EAknCbaLocationBottom" )
            iMenubar1->SetAlignment( CGraphicsContext::ELeft );
            iMenubar1->SetRect( iLayoutMgr->LayoutRect( ESoftkey1 ) );
            iMenubar2->SetAlignment( CGraphicsContext::ERight );
            iMenubar2->SetRect( iLayoutMgr->LayoutRect( ESoftkey2 ) );
            }
            break;
        case AknLayoutUtils::EAknCbaLocationRight:
            {
            // Only Landscape
            _LOG_WRITE( "AknLayoutUtils::EAknCbaLocationRight" )
            AknLayoutUtils::LayoutMetricsRect( AknLayoutUtils::EControlPaneBottom, rect );
            iMenubar1->SetAlignment( CGraphicsContext::ERight );
            iMenubar1->SetRect( rect );
            AknLayoutUtils::LayoutMetricsRect( AknLayoutUtils::EControlPaneTop, rect );
            iMenubar2->SetAlignment( CGraphicsContext::ERight );
            iMenubar2->SetRect( rect );
            }
            break;
        case AknLayoutUtils::EAknCbaLocationLeft:
            { 
            // Only landscape
            _LOG_WRITE( "AknLayoutUtils::EAknCbaLocationLeft" )
            AknLayoutUtils::LayoutMetricsRect( AknLayoutUtils::EControlPaneTop, rect );
            iMenubar1->SetAlignment( CGraphicsContext::ELeft );
            iMenubar1->SetRect( rect );
            AknLayoutUtils::LayoutMetricsRect( AknLayoutUtils::EControlPaneBottom, rect );
            iMenubar2->SetAlignment( CGraphicsContext::ELeft );
            iMenubar2->SetRect( rect );
            }
            break;
        default:
            break;
        }
    }

// End of File
