/* ====================================================================
 * File: Te2HelpView.h
 * Created: 01/14/09
 * Modified: 03/14/09
 * Author: Sun Jinbo
 * Copyright (c): Tieto, All rights reserved
 * ==================================================================== */

#ifndef C_TE2HELPVIEW_H
#define C_TE2HELPVIEW_H

// INCLUDE FILES
#include <aknview.h>

// FORWARD DECLARATIONS
class CHelpContainer;
class CTe2AppUi;

/**
* CTe2HelpView class
*
* @since S60 3.1
*/
class CTe2HelpView : public CAknView
    {
public: // Constructor and destructor
    static CTe2HelpView* NewL( CTe2AppUi* aAppUi );
    virtual ~CTe2HelpView();

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
    void StoreL();
    
public: // Constructors

    /**
    * CTe2HelpView.
    * C++ default constructor.
    */
    CTe2HelpView( CTe2AppUi* aAppUi );

    /**
    * ConstructL.
    * 2nd phase constructor.
    */
    void ConstructL();

private: // New function
    void InitContainerL();

private: // Data
    CTe2AppUi* iAppUi;

    /**
    * iContainer container for this view
    * owned by CTe2HelpView object.
    */
    CHelpContainer* iContainer;
    
    TInt iMenuTitleResourceId;
    };

#endif // C_TE2HELPVIEW_H

// End of File
