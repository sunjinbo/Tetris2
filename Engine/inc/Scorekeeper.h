/* ====================================================================
 * File: Scorekeeper.h
 * Created: 01/17/09
 * Modifed: 01/17/09
 * Author: Sun Jinbo
 * Copyright (c): Tieto, All rights reserved
 * ==================================================================== */

#ifndef C_SCOREKEEPER_H
#define C_SCOREKEEPER_H

// INCLUDE FILES
#include <e32def.h>
#include <e32base.h>
#include "ScorekeeperObserver.h"

// FORWARD DECLARATIONS

// CLASS DECLARATION
class CRankList;

/**
* CScorekeeper class
*
* @since S60 3.1
*/
NONSHARABLE_CLASS( CScorekeeper ) : public CBase
    {
public:
    // Creates a new Scorekeeper object, 
    // and leaves if there is insufficient memory to create it.
    static CScorekeeper* NewL( MScorekeeperObserver& aObserver );
    virtual ~CScorekeeper();
    
    inline TInt Line() const;
    inline TInt Score() const;
    inline TInt Level() const;
    inline TInt Life() const;
    
    void StartL();
    void RemoveLineL( TInt aLine );
    void KillL();
    void Reset();

public: // new method for rank
    TInt GetRankItem( TInt aRank, TTime& aTime, TInt& aScore  );
    void CleanRankL();
    void SaveRankL();

private: // Constractor
    CScorekeeper( MScorekeeperObserver& aObserver );
    void ConstructL();

private: // Member data
    MScorekeeperObserver& iObserver;
    CRankList* iRankList;  // owned
    TInt iLine;
    TInt iScore;
    TInt iLevel;
    TInt iLife;
    };

// ----------------------------------------------------------------------------
// CScorekeeper::Line
// ----------------------------------------------------------------------------
//
inline TInt CScorekeeper::Line() const
    {
    return iLine;
    }

// ----------------------------------------------------------------------------
// CScorekeeper::Line
// ----------------------------------------------------------------------------
//
inline TInt CScorekeeper::Score() const
    {
    return iScore;
    }

// ----------------------------------------------------------------------------
// CScorekeeper::Line
// ----------------------------------------------------------------------------
//
inline TInt CScorekeeper::Level() const
    {
    return iLevel;
    }

// ----------------------------------------------------------------------------
// CScorekeeper::Line
// ----------------------------------------------------------------------------
//
inline TInt CScorekeeper::Life() const
    {
    return iLife;
    }

#endif // C_SCOREKEEPER_H

// End of File
