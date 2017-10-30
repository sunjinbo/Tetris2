/* ====================================================================
 * File: Scorekeeper.cpp
 * Created: 01/06/09
 * Modified: 06/07/10
 * Author: Sun Jinbo
 * Copyright (c): Tieto, All rights reserved
 * ==================================================================== */

// INCLUDE FILES
#include "Scorekeeper.h"
#include "EngineCommon.h"
#include "EnginePanic.h"
#include "Te2Engine.hrh"
#include "RankList.h"

#include "Logger.h"

// CONSTANTS

// ======== MEMBER FUNCTIONS ========

// ----------------------------------------------------------------------------
// CScorekeeper::NewL
// ----------------------------------------------------------------------------
//
CScorekeeper* CScorekeeper::NewL( MScorekeeperObserver& aObserver )
    {
    CScorekeeper* self = new( ELeave ) CScorekeeper( aObserver );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// ----------------------------------------------------------------------------
// CScorekeeper::~CScorekeeper
// ----------------------------------------------------------------------------
//
CScorekeeper::~CScorekeeper()
    {
    // no implementation required
    }

// ----------------------------------------------------------------------------
// CScorekeeper::StartL
// ----------------------------------------------------------------------------
//
void CScorekeeper::StartL()
    {
    // Notify all of data to engine
    iObserver.StateChangedL( EScorekeeperLevelUpdated );
    iObserver.StateChangedL( EScorekeeperLineUpdated );
    iObserver.StateChangedL( EScorekeeperScoreUpdated );
    iObserver.StateChangedL( EScorekeeperLifeUpdated );
    }

// ----------------------------------------------------------------------------
// CScorekeeper::RemoveLineL
// ----------------------------------------------------------------------------
//
void CScorekeeper::RemoveLineL( TInt aLine )
    {
    // Update line
    iLine += aLine;
    iObserver.StateChangedL( EScorekeeperLineUpdated );

    // Update score
    switch ( aLine )
        {
        case ESingle:
            {
            iScore += ( KScorekeeperSinglePoint * iLevel );
            }
            break;
        case EDouble:
            {
            iScore += ( KScorekeeperDoublePoint * iLevel );
            }
            break;
        case ETriple:
            {
            iScore += ( KScorekeeperTriplePoint * iLevel );
            }
            break;
        case ETetris:
            {
            iScore += ( KScorekeeperTetrisPoint * iLevel );
            }
            break;
        default:
            {
            EnginePanic( EEnginePanicInvalidArguments );
            }
            break;
        }
    iObserver.StateChangedL( EScorekeeperScoreUpdated );

    // Update level
    TInt level = iScore / KScorekeeperLevelIncPoints
        + KScorekeeperDefaultLevel;
    if ( level != iLevel && level < ELevel5 )
        {
        iLevel = level;
        iObserver.StateChangedL( EScorekeeperLevelUpdated );
        }
    }

// ----------------------------------------------------------------------------
// CScorekeeper::KillL
// ----------------------------------------------------------------------------
//
void CScorekeeper::KillL()
    {
    --iLife;
    iObserver.StateChangedL( EScorekeeperLifeUpdated );
    
    if ( !iLife )
        {
        SaveRankL();

        iObserver.StateChangedL( EScorekeeperLifeEnded );
        }
    }

// ----------------------------------------------------------------------------
// CTe2Engine::Reset
// ----------------------------------------------------------------------------
//
void CScorekeeper::Reset()
    {
    iLevel = KScorekeeperDefaultLevel;
    iLine = KScorekeeperDefaultLine;
    iScore = KScorekeeperDefaultScore;
    iLife = KScorekeeperDefaultLife;
    }

// ----------------------------------------------------------------------------
// CScorekeeper::GetRankItem
// ----------------------------------------------------------------------------
//
TInt CScorekeeper::GetRankItem( TInt aRank, TTime& aTime, TInt& aScore  )
    {
	TInt err = KErrNone;
	TRankItem rankItem;
	err = iRankList->GetRankItem( aRank, rankItem );
	aTime = rankItem.iTime;
	aScore = rankItem.iScore;

    return err;
    }

// ----------------------------------------------------------------------------
// CScorekeeper::CleanRankL
// ----------------------------------------------------------------------------
//
void CScorekeeper::CleanRankL()
    {
    iRankList->ClearL();
    }

// ----------------------------------------------------------------------------
// CScorekeeper::SaveRankL
// ----------------------------------------------------------------------------
//
void CScorekeeper::SaveRankL()
	{
	if ( iScore > KScorekeeperDefaultScore )
	    {
        TTime now;
        now.HomeTime();

        iRankList->AppendRankItem( TRankItem( now, iScore ) );
        iRankList->ExternalizedL();
	    }
	}

// ----------------------------------------------------------------------------
// CScorekeeper::CScorekeeper
// Constructor.
// ----------------------------------------------------------------------------
//
CScorekeeper::CScorekeeper( MScorekeeperObserver& aObserver )
    : iObserver( aObserver )
    { // no implementation required   
    }

// ----------------------------------------------------------------------------
// CScorekeeper::ConstructL
// Completes construction.
// ----------------------------------------------------------------------------
//
void CScorekeeper::ConstructL()
    {
    Reset();
    
    iRankList = CRankList::NewL();
    }

// End of File
