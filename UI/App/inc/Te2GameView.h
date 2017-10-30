/* ====================================================================
 * File: Te2GameView.h
 * Created: 01/14/09
 * Modified: 04/14/09
 * Author: Sun Jinbo
 * Copyright (c): Tieto, All rights reserved
 * ==================================================================== */

#ifndef C_TE2GAMEVIEW_H
#define C_TE2GAMEVIEW_H

// INCLUDE FILES
#include <aknview.h>
#include "Tetris2.hrh"

// FORWARD DECLARATIONS
class CGameContainer;
class CTe2AppUi;

/**
* CTe2GameView class
*
* @since S60 3.1
*/
class CTe2GameView : public CAknView
    {
public: // Constructor and destructor
    static CTe2GameView* NewL( CTe2AppUi* aAppUi );
    virtual ~CTe2GameView();

public: // from base class CAknView
    /**
    * Id
    * @return Id Uid value
    */
    TUid Id() const;
    
    /**
    * HandleCommandL
    * From CAknView, takes care of command handling.
    * @param aCommand Command to be handled
    */
    void HandleCommandL( TInt aCommand );

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
    void SetAudio( TBool aAudio );
    void StoreL();
    
public: // Constructors and destructor

    /**
    * CTe2GameView.
    * C++ default constructor.
    */
    CTe2GameView( CTe2AppUi* aAppUi );

    /**
    * ConstructL.
    * 2nd phase constructor.
    */
    void ConstructL();

private: // New function
    void InitContainerL();
    void DoStartL();

private: // Data
    
    CTe2AppUi* iAppUi;
    
    /**
    * iContainer container for this view
    * owned by CTe2GameView object.
    */
    CGameContainer* iContainer;
    
    TInt iMenuTitleResourceId;
    
    TTe2Mode iModeType;
    
    TBool iAudio;
    };

#endif // C_TE2GAMEVIEW_H

// End of File
