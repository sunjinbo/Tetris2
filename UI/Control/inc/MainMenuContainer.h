/* ====================================================================
 * File: MainMenuContainer.h
 * Created: 03/15/09
 * Modified: 04/29/09
 * Author: Sun Jinbo
 * Copyright (c): Tieto, All rights reserved
 * ==================================================================== */

#ifndef C_MAINMENUCONTAINER_H
#define C_MAINMENUCONTAINER_H

// INCLUDE FILES
#include <bitstd.h> // CFbsBitGc
#include "BaseContainer.h"

// FORWARD DECLARATIONS
class CFbsBitGc;
class CFont;

// CLASS DECLARATION
/**
* CMainMenuContainer class
*
* @since S60 3.1
*/
class CMainMenuContainer : public CBaseContainer
    {
public: // Constructor and Destructor
    IMPORT_C static CMainMenuContainer* NewL( const TRect& aRect, MModelObserver& aObserver );
    IMPORT_C virtual ~CMainMenuContainer();

public: // from base class CBaseContainer
    void Draw( const TRect& aRect ) const; 
    void SizeChanged();
    TKeyResponse OfferKeyEventL( const TKeyEvent& aKeyEvent, TEventCode aType );
    void HandlePointerEventL(const TPointerEvent& aPointerEvent);

public: // New function
    IMPORT_C void AddMenuItemL( const TDesC& aText, TInt aId, TBool aActive = ETrue );
    IMPORT_C void RemoveMenuItem( TInt aId );
    IMPORT_C void DisableMenuItem( TInt aId );
    IMPORT_C void EnableMenuItem( TInt aId );
    IMPORT_C void SetSelectedItem( TInt aId );

    void Draw();

private: // Constructor
    CMainMenuContainer( MModelObserver& aObserver );
    void ConstructL( const TRect& aRect );

private: // New fucntion
    TInt NextSelectedItem();
    TInt PrevSelectedItem();
    TInt ActiveItemCount() const;
    TBool ClickMainMenuL( const TPoint& aPos );
    TKeyResponse HandleSideslipKeyEventL( 
        const TKeyEvent& aKeyEvent, TEventCode aType );

private: // Data members
    RPointerArray<HBufC> iItemTextArray;
    RArray<TInt> iItemIdArray;
    RArray<TBool> iItemActiveArray;
    TInt iSelectedItemId;
    TRect iMenuRect;
    CFont* iFont;
    };

#endif // C_MAINMENUCONTAINER_H

// End of File
