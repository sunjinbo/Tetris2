/* ====================================================================
 * File: Te2RankView.h
 * Created: 06/06/10
 * Modified: 06/06/10
 * Author: Sun Jinbo
 * Copyright (c): Tieto, All rights reserved
 * ==================================================================== */

#ifndef C_TE2RANKVIEW_H
#define C_TE2RANKVIEW_H

// INCLUDE FILES
#include <aknview.h>

// FORWARD DECLARATIONS
class CRankContainer;
class CTe2AppUi;

/**
* CTe2RankView class
*
* @since S60 3.1
*/
class CTe2RankView : public CAknView
    {
public: // Constructor and destructor
    static CTe2RankView* NewL( CTe2AppUi* aAppUi );
    virtual ~CTe2RankView();

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
    
public: // Constructors and destructor

    /**
    * CTe2RankView.
    * C++ default constructor.
    */
    CTe2RankView( CTe2AppUi* aAppUi );

    /**
    * ConstructL.
    * 2nd phase constructor.
    */
    void ConstructL();

private: // New function
    void InitContainerL();
    void TextItemLoadL( TInt aResourceId );

private: // Data
    CTe2AppUi* iAppUi;

    /**
    * iContainer container for this view
    * owned by CTe2RankView object.
    */
    CRankContainer* iContainer;
    
    TInt iMenuTitleResourceId;
    };

#endif // C_TE2RANKVIEW_H

// End of File
