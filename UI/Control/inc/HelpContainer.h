/* ====================================================================
 * File: HelpContainer.h
 * Created: 03/15/09
 * Modified: 03/30/09
 * Author: Sun Jinbo
 * Copyright (c): Tieto, All rights reserved
 * ==================================================================== */

#ifndef C_HELPCONTAINER_H
#define C_HELPCONTAINER_H

// INCLUDE FILES
#include "BaseContainer.h"

// FORWARD DECLARATIONS
class CScrollControl;


// CLASS DECLARATION
/**
* CHelpContainer class
*
* @since S60 3.1
*/
class CHelpContainer : public CBaseContainer
    {
public: // Constructor and Destructor
    IMPORT_C static CHelpContainer* NewL( const TRect& aRect, MModelObserver& aObserver );
    IMPORT_C virtual ~CHelpContainer();

public: // from base class CBaseControl
    void Draw( const TRect& aRect ) const; 
    void SizeChanged();
    TKeyResponse OfferKeyEventL( const TKeyEvent& aKeyEvent, TEventCode aType );

public: // New functions
    IMPORT_C void AddTextItemL( const TDesC &  aText );
    IMPORT_C void Start();

    void Draw();

protected: // Constructor
    CHelpContainer( MModelObserver& aObserver );
    void ConstructL( const TRect& aRect );

protected: // Data members
    CScrollControl* iScroll;
    };

#endif // C_HELPCONTAINER_H

// End of File
