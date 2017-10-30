/* ====================================================================
 * File: RankContainer.h
 * Created: 06/06/10
 * Modified: 06/06/10
 * Author: Sun Jinbo
 * Copyright (c): Tieto, All rights reserved
 * ==================================================================== */

#ifndef C_RANKCONTAINER_H
#define C_RANKCONTAINER_H

// INCLUDE FILES
#include <EngineObserver.h>
#include "BaseContainer.h"

// FORWARD DECLARATIONS
class CLabelControl;
class CTe2Engine;

// CONSTANTS
const TInt KMaxRankingList = 5;

// CLASS DECLARATION

/**
* CRankContainer class
*
* @since S60 3.1
*/
class CRankContainer : public CBaseContainer, public MEngineObserver
    {
public: // Constructor and Destructor
    IMPORT_C static CRankContainer* NewL( const TRect& aRect, MModelObserver& aObserver );
    IMPORT_C virtual ~CRankContainer();

public: // New functions
    IMPORT_C void CleanRankL();

public: // from base class CBaseControl
    void Draw( const TRect& aRect ) const; 
    void SizeChanged();
    TKeyResponse OfferKeyEventL( const TKeyEvent& aKeyEvent, TEventCode aType );
    void Draw();

public: // from base class MEngineObserver
    void StateChangedL( TEngineState aNewState, TEngineAction aAction );


protected: // Constructor
    CRankContainer( MModelObserver& aObserver );
    void ConstructL( const TRect& aRect );

protected: // Data members
    CTe2Engine* iGameEngine;
    CLabelControl* iRankingTitleLabel;
    CLabelControl* iRankingListLabel[KMaxRankingList];
    struct TRankingListItem
        {
        TTime iTime;
        TInt iScore;
        };
    RArray<TRankingListItem> iRankingList;
    TInt iResOffset;
    };

#endif // C_RANKCONTAINER_H

// End of File
