/* ====================================================================
 * File: MainMenuContainer.cpp
 * Created: 03/15/09
 * Modified: 04/29/09
 * Author: Sun Jinbo
 * Copyright (c): Tieto, All rights reserved
 * ==================================================================== */

// INCLUDE FILES
#include <akndef.h>
#include <aknutils.h>
#include "MainMenuContainer.h"
#include "ModelObserver.h"
#include "LayoutManager.h"
#include "MenubarControl.h"

// CONSTANTS
const TInt KItemArrayCranularity = 8;
const TInt KUnavailableIndex = -1;
const TInt KUnavailabeItemId = -1;
const TInt KActiveItemCountZero = 0;

// ======== MEMBER FUNCTIONS ========

// ----------------------------------------------------------------------------
// CMainMenuContainer::~CMainMenuContainer
// ----------------------------------------------------------------------------
//
EXPORT_C CMainMenuContainer* CMainMenuContainer::NewL( 
    const TRect& aRect, MModelObserver& aObserver )
    {
    CMainMenuContainer* self = new (ELeave) CMainMenuContainer( aObserver );
    CleanupStack::PushL ( self );
    self->ConstructL ( aRect );
    CleanupStack::Pop();
    return self;
    }

// ----------------------------------------------------------------------------
// CMainMenuContainer::~CMainMenuContainer
// ----------------------------------------------------------------------------
//
EXPORT_C CMainMenuContainer::~CMainMenuContainer()
    {
    iItemTextArray.ResetAndDestroy();
    iItemTextArray.Close();
    iItemIdArray.Close();
    iItemActiveArray.Close();
    iEikonEnv->ScreenDevice()->ReleaseFont( iFont );
    }

// ----------------------------------------------------------------------------
// CMainMenuContainer::Draw
// ----------------------------------------------------------------------------
//
void CMainMenuContainer::Draw( const TRect& /*aRect*/ ) const
    {  // No implementation required
    }

// ----------------------------------------------------------------------------
// CMainMenuContainer::SizeChanged
// ----------------------------------------------------------------------------
//
void CMainMenuContainer::SizeChanged()
    {
    CBaseContainer::SizeChanged();
    if ( iLayoutMgr )
        {
        iMenuRect = iLayoutMgr->LayoutRect( EMainMenu );
        }
    }

// ----------------------------------------------------------------------------
// CMainMenuContainer::OfferKeyEventL
// ----------------------------------------------------------------------------
//
TKeyResponse CMainMenuContainer::OfferKeyEventL( 
                        const TKeyEvent& aKeyEvent, TEventCode aType )
    {
    TKeyResponse response( EKeyWasNotConsumed );
    
    if ( iResolution == EQhdPortrait )
        {
        response = HandleSideslipKeyEventL( aKeyEvent, aType );
        }
    else
        {
        if ( aType == EEventKeyUp )
            {
            switch ( aKeyEvent.iScanCode )
                {
                case EStdKeyDevice0: // Softkey1
                case EStdKeyDevice3: // Enter key
                    {
                    iObserver.MenuItemSelectedL( iSelectedItemId );
                    }
                    break;
                case EStdKeyDevice1: // Softkey2
                    {
                    iObserver.MenuItemSelectedL( iSoftkey2Id );
                    }
                    break;
                case EStdKeyUpArrow: // Up arrow key
                    {
                    SetSelectedItem( PrevSelectedItem() );
                    response = EKeyWasConsumed;
                    }
                    break;
                case EStdKeyDownArrow: // Down arrow key
                    {
                    SetSelectedItem( NextSelectedItem() );
                    response = EKeyWasConsumed;
                    }
                    break;
                default: // Other keys
                    {
                    response = CBaseContainer::OfferKeyEventL( aKeyEvent, aType );
                    }
                    break;
                }
            }
        }

    return response;
    }

// ----------------------------------------------------------------------------
// CMainMenuContainer::HandlePointerEventL
// ----------------------------------------------------------------------------
//
void CMainMenuContainer::HandlePointerEventL(const TPointerEvent& aPointerEvent)
    {
    if ( aPointerEvent.iType == TPointerEvent::EButton1Up )
        {
        if ( iMenubar1->Rect().Contains( aPointerEvent.iPosition ) )
            {
            iObserver.MenuItemSelectedL( iSelectedItemId );
            }
            
        if ( iMenubar2->Rect().Contains( aPointerEvent.iPosition ) )
            {
            iObserver.MenuItemSelectedL( iSoftkey2Id );
            }
        
        if ( iMenuRect.Contains( aPointerEvent.iPosition ) )
            {
            if ( ClickMainMenuL( aPointerEvent.iPosition ) )
                {
                iObserver.MenuItemSelectedL( iSelectedItemId );
                }
            }
        }
    }

// ----------------------------------------------------------------------------
// CMainMenuContainer::AddMenuItemL
// ----------------------------------------------------------------------------
//
EXPORT_C void CMainMenuContainer::AddMenuItemL( const TDesC& aText, 
    TInt aId, TBool aActive )
    {
    TInt index( KUnavailableIndex );
    HBufC* textBuf =  HBufC::NewL( aText.Length() );
    TPtr textItem( textBuf->Des() );
    textItem.Copy( aText );
    User::LeaveIfError( iItemIdArray.InsertInOrder( aId ) );
    if ( iItemIdArray.FindInOrder( aId, index ) == KErrNone )
        {
        User::LeaveIfError( iItemTextArray.Insert( textBuf, index ) );
        User::LeaveIfError( iItemActiveArray.Insert( aActive, index ) );
        }
    }

// ----------------------------------------------------------------------------
// CMainMenuContainer::RemoveMenuItem
// ----------------------------------------------------------------------------
//
EXPORT_C void CMainMenuContainer::RemoveMenuItem( TInt aId )
    {
    TInt index( KUnavailableIndex );
    if ( iItemIdArray.FindInOrder( aId, index ) == KErrNone )
        {
        iItemTextArray.Remove( index );
        iItemIdArray.Remove( index );
        iItemActiveArray.Remove( index );
        }
    }

// ----------------------------------------------------------------------------
// CMainMenuContainer::DisableMenuItem
// ----------------------------------------------------------------------------
//
EXPORT_C void CMainMenuContainer::DisableMenuItem( TInt aId )
    {
    TInt index( KUnavailableIndex );
    if ( iItemIdArray.FindInOrder( aId, index ) == KErrNone )
        {
        iItemActiveArray[ index ] = EFalse;
        }
    }

// ----------------------------------------------------------------------------
// CMainMenuContainer::EnableMenuItem
// ----------------------------------------------------------------------------
//
EXPORT_C void CMainMenuContainer::EnableMenuItem( TInt aId )
    {
    TInt index( KUnavailableIndex );
    if ( iItemIdArray.FindInOrder( aId, index ) == KErrNone )
        {
        iItemActiveArray[ index ] = ETrue;
        }
    }

// ----------------------------------------------------------------------------
// CMainMenuContainer::SetSelectedItem
// ----------------------------------------------------------------------------
//
EXPORT_C void CMainMenuContainer::SetSelectedItem( TInt aId )
    {
    iSelectedItemId = aId;
    }

// ----------------------------------------------------------------------------
// CMainMenuContainer::Draw
// ----------------------------------------------------------------------------
//
void CMainMenuContainer::Draw()
    {
    CBaseContainer::Draw();
    
    TInt count = ActiveItemCount();
    if ( KActiveItemCountZero != count )
        {
        TRgb itemColor( KRgbBlack );
        TRgb selectedItemColor( KRgbRed );
        itemColor.SetAlpha( 200 );
        selectedItemColor.SetAlpha( 200 );
        iBmpBufferContext->SetPenStyle( CGraphicsContext::ESolidPen );
        iBmpBufferContext->UseFont( iFont );
        TSize menuItemSize;
        menuItemSize.iHeight = iMenuRect.Height() / count;
        menuItemSize.iWidth = iMenuRect.Width();
        TPoint menuItemTl( iMenuRect.iTl );
        for ( TInt i = 0; i < iItemIdArray.Count(); ++i )
            {
            TRect rect( menuItemTl, menuItemSize );
            if ( iItemActiveArray[ i ] )
                {
                if ( iSelectedItemId == iItemIdArray[ i ] )
                    {
                    iBmpBufferContext->SetPenColor( selectedItemColor );
                    }
                else
                    {
                    iBmpBufferContext->SetPenColor( itemColor );
                    }
                    
                TPoint origin;
                TSize wh;
                TInt borderWidth;
                TInt borderHeight;

                borderWidth = iFont->TextWidthInPixels( *iItemTextArray[ i ] );
                borderHeight = iFont->HeightInPixels();

                wh.SetSize( borderWidth, borderHeight );
                origin.iX = ( rect.Width() - borderWidth ) / 2;
                origin.iY = ( rect.Height() - borderHeight ) / 2;

                TRect textRect( origin, wh );
                
                TInt baseline = 
                    ( textRect.Height() + iFont->HeightInPixels() ) / 2;
                
                iBmpBufferContext->DrawText( 
                    *iItemTextArray[ i ], rect, baseline, CGraphicsContext::ECenter, 0 );

                menuItemTl.iY += menuItemSize.iHeight;
                }
            }
        iBmpBufferContext->DiscardFont();
        }
    
    }

// ----------------------------------------------------------------------------
// CMainMenuContainer::CMainMenuContainer
// ----------------------------------------------------------------------------
//
CMainMenuContainer::CMainMenuContainer( MModelObserver& aObserver )
    : CBaseContainer( aObserver ),
    iItemTextArray( KItemArrayCranularity ),
    iItemIdArray( KItemArrayCranularity ),
    iItemActiveArray( KItemArrayCranularity ),
    iSelectedItemId( KUnavailabeItemId )
    { // No implementation required
    }

// ----------------------------------------------------------------------------
// CMainMenuContainer::ConstructL
// ----------------------------------------------------------------------------
//
void CMainMenuContainer::ConstructL( const TRect& aRect )
    {
    CBaseContainer::ConstructL( aRect );
    
    iMenuRect = iLayoutMgr->LayoutRect( EMainMenu );
    TFontSpec fontSpec = iEikonEnv->LegendFont()->FontSpecInTwips();
    
    if ( EQhdPortrait == CTe2LayoutManager::Resolution() )
        {
        fontSpec.iHeight  += fontSpec.iHeight / 4;
        }
    else
        {
        fontSpec.iHeight  -= fontSpec.iHeight / 4;
        }

    fontSpec.iFontStyle.SetBitmapType( EMonochromeGlyphBitmap );
    User::LeaveIfError( iEikonEnv->ScreenDevice()->GetNearestFontInTwips( iFont, fontSpec ) );
    
    // Sets control as ready to be drawn.
    ActivateL();
    }

// ----------------------------------------------------------------------------
// CMainMenuContainer::NextSelectedItem
// ----------------------------------------------------------------------------
//
TInt CMainMenuContainer::NextSelectedItem()
    {
    TInt id( KUnavailabeItemId );
    TInt index( KUnavailableIndex );
    if ( iItemIdArray.FindInOrder( iSelectedItemId, index ) == KErrNone )
        {
        for ( TInt i = 0; i < iItemIdArray.Count(); ++i )
            {
            if ( ++index == iItemIdArray.Count() )
                {
                index = 0;
                }

            if ( iItemActiveArray[ index ] )
                {
                id = iItemIdArray[ index ];
                break;
                }
            }
        }
    return id;
    }

// ----------------------------------------------------------------------------
// CMainMenuContainer::PrevSelectedItem
// ----------------------------------------------------------------------------
//
TInt CMainMenuContainer::PrevSelectedItem()
    {
    TInt id( KUnavailabeItemId );
    TInt index( KUnavailableIndex );
    if ( iItemIdArray.FindInOrder( iSelectedItemId, index ) == KErrNone )
        {
        for ( TInt i = 0; i < iItemIdArray.Count(); ++i )
            {
            if ( --index + 1 == 0 )
                {
                index = iItemIdArray.Count() - 1;
                }

            if ( iItemActiveArray[ index ] )
                {
                id = iItemIdArray[ index ];
                break;
                }
            }
        }
    return id;
    }

// ----------------------------------------------------------------------------
// CMainMenuContainer::ActiveItemCount
// ----------------------------------------------------------------------------
//
TInt CMainMenuContainer::ActiveItemCount() const
    {
    TInt count( 0 );
    for ( TInt i = 0; i < iItemActiveArray.Count(); i++ )
        {
        if ( iItemActiveArray[ i ] )
            {
            count++;
            }
        }
    return count;
    }

// ----------------------------------------------------------------------------
// CMainMenuContainer::ClickMainMenuL
// ----------------------------------------------------------------------------
//
TBool CMainMenuContainer::ClickMainMenuL( const TPoint& aPos )
    {
    TInt retVal( ETrue );
    TInt id ( KUnavailabeItemId );
    TInt count( ActiveItemCount() );
    if ( count > 0 )
        {
        TInt ix = ( aPos.iY - iMenuRect.iTl.iY ) / ( iMenuRect.Height() / count );
        for ( TInt i = 0; i < iItemActiveArray.Count(); i++ )
            {
            if ( iItemActiveArray[ i ] )
                {
                --ix;
                if ( ix < 0 )
                    {
                    id = iItemIdArray[ i ];
                    break;
                    }
                }
            }
        }
    
    if ( iSelectedItemId != id )
        {
        iSelectedItemId = id;
        retVal = EFalse;
        }
    return retVal;
    }

// ----------------------------------------------------------------------------
// CMainMenuContainer::ClickMainMenuL
// ----------------------------------------------------------------------------
//
TKeyResponse CMainMenuContainer::HandleSideslipKeyEventL( 
        const TKeyEvent& aKeyEvent, TEventCode aType )
    {
    TKeyResponse response( EKeyWasNotConsumed );
    if ( aType == EEventKeyUp )
        {
        switch ( aKeyEvent.iScanCode )
            {
            case EStdKeyDevice0: // Softkey1
            case EStdKeyDevice3: // Enter key
                {
                iObserver.MenuItemSelectedL( iSelectedItemId );
                }
                break;
            case EStdKeyDevice1: // Softkey2
                {
                iObserver.MenuItemSelectedL( iSoftkey2Id );
                }
                break;
            case EStdKeyLeftArrow: // Up arrow key
                {
                SetSelectedItem( PrevSelectedItem() );
                response = EKeyWasConsumed;
                }
                break;
            case EStdKeyRightArrow: // Down arrow key
                {
                SetSelectedItem( NextSelectedItem() );
                response = EKeyWasConsumed;
                }
                break;
            default: // Other keys
                {
                response = CBaseContainer::OfferKeyEventL( aKeyEvent, aType );
                }
                break;
            }
        }
    return response;
    }

// End of File
