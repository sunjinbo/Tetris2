/* ====================================================================
 * File: RankList.h
 * Created: 09/17/10
 * Modifed: 09/17/10
 * Author: Sun Jinbo
 * Copyright (c): Tieto, All rights reserved
 * ==================================================================== */

#ifndef C_RANKLIST_H
#define C_RANKLIST_H

// INCLUDE
#include <e32def.h>

// FORWARD DECLARATION
class RFs;

// CONSTANTS
const TInt KMaxRankItemNbr =0x5;

// CLASS DECLARATION

class TRankItem
    {
public: // New methods
	TRankItem(){}
    TRankItem( TTime aTime, TInt aScore )
        : iTime( aTime ), iScore( aScore ) {}

    static TInt Compare( const TRankItem& aFirst, const TRankItem& aSecond )
        {
         if ( aFirst.iScore < aSecond.iScore )
          return 1; 
         if ( aFirst.iScore > aSecond.iScore )
          return -1;
        return 0;
        }
public: // Data
    TTime iTime;
    TInt iScore;
    };

/**
 *  CRankList class
 */
class CRankList
    {
public: // Symbian 2nd constructor and destructor
	static CRankList* NewL();
	virtual ~CRankList();

public: // New methods
    void InternalizedL();
    void ExternalizedL();
    void ClearL();
    void Sort();
    TInt GetRankItem( TInt aRankIx, TRankItem& aRankItem  );
    void AppendRankItem( const TRankItem& aRankItem );
    void CreateDefaultRankL();

private: // Default C++ constructor and 2nd constructor
    CRankList();
    void ConstructL();

private: // New methods
    TInt GetFileName( TFileName& aFileName );
    TInt GetPrivatePath( TFileName& aPrivatePath );

private: // Data
    RFs& iFs;
    RArray<TRankItem> iRankArray;
    };

#endif // C_RANKLIST_H

// End of File
