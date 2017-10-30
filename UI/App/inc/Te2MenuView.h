/* ====================================================================
 * File: Te2MenuView.h
 * Created: 01/14/09
 * Modified: 03/27/09
 * Author: Sun Jinbo
 * Copyright (c): Tieto, All rights reserved
 * ==================================================================== */

#ifndef C_TE2MENUVIEW_H
#define C_TE2MENUVIEW_H

// INCLUDE FILES
#include <aknview.h>
#include "Tetris2.hrh"

// FORWARD DECLARATIONS
class CMainMenuContainer;
class CTe2AppUi;

/**
* CTe2MenuView class
*
* @since S60 3.1
*/
class CTe2MenuView : public CAknView
    {
public: // Constructor and destructor
    static CTe2MenuView* NewL( CTe2AppUi* aAppUi );
    virtual ~CTe2MenuView();

public: // from base class CAknView
    /**
    * Id
    * @return Id Uid value
    */
    TUid Id() const;

    /**
    * DoActivateL
    * From CAknExView, activate an AknView.
    * @param aPrevViewId The id of the previous view
    * @param aCustomMessageId message identifier
    * @param aCustomMessage custom message provided when the view is changed
    */
    void DoActivateL( const TVwsViewId& aPrevViewId,
                      TUid aCustomMessageId,
                      const TDesC8& aCustomMessage );

    /**
    * DoDeactivate
    * From AknView, deactivate an AknView
    * Remove the container class instance from the App UI's stack and
    * deletes the instance
    */
    void DoDeactivate();

    /**
    * HandleClientRectChange
    * From CAknView, Handles the clientrect.
    */
    void HandleClientRectChange();

public: // New functions
    void ChangeMenuBarL( TInt aMenuTitleResourceId = 0 );
    void SetMode( TTe2Mode aType );
    void SetAudio( TTe2Audio aType );
    void StoreL();
    
public: // Constructors and destructor

    /**
    * C++ default constructor.
    */
    CTe2MenuView( CTe2AppUi* aAppUi );

    /**
    * 2nd phase constructor.
    */
    void ConstructL();

protected: // New function
    void InitContainerL();
    TBool IsSelectedItem( TInt32 aFlag ) const;
    TBool IsEnableItem( TInt32 aFlag ) const;

private: // Data
    CTe2AppUi* iAppUi;
    CMainMenuContainer* iContainer;
    TTe2Mode iModeType;
    TTe2Audio iAudioType;
    };

#endif // C_TE2MENUVIEW_H

// End of File
