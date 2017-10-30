/* ====================================================================
 * File: FixedTetriminoExt.cpp
 * Created: 01/04/09
 * Modified:
 * Author: Sun Jinbo
 * Copyright (c): Tieto, All rights reserved
 * ==================================================================== */

// INCLUDE FILES
#include <e32base.h> // CPeriodic
#include <e32std.h> // TTime
#include <e32math.h> // Math arithmetic
#include "FixedTetriminoExt.h"
#include "TetriminoObserver.h"
#include "EngineCommon.h"
#include "Te2Engine.hrh"

// CONSTANTS
const TInt KAutoGrowDelayTimeout = 45; // 45 secs
const TInt KAutoGrowBaseTimeout = 80;  // 80 secs
const TInt KAutoGrowExtraTimeout = 25;  // 25 secs
const TInt KAtuoGrowTypeProbability = 100; // 100%
const TInt KAtuoGrowTypeIncLife = 5; // 5%
const TInt KAtuoGrowTypeRemoveLine4 = 5 + KAtuoGrowTypeIncLife; // 5%
const TInt KAtuoGrowTypeRemoveLine3 = 10 + KAtuoGrowTypeRemoveLine4; // 10%
const TInt KAtuoGrowTypeRemoveLine2 = 10 + KAtuoGrowTypeRemoveLine3; // 10%
const TInt KAtuoGrowTypeRemoveLine1 = 20 + KAtuoGrowTypeRemoveLine2; // 20%
const TInt KAtuoGrowTypeIncScore400 = 20 + KAtuoGrowTypeRemoveLine1; // 20%
const TInt KAtuoGrowTypeIncScore200 = 30 + KAtuoGrowTypeIncScore400; // 30%

// ======== MEMBER FUNCTIONS ========

// ----------------------------------------------------------------------------
// CFixedTetriminoExt::NewL
// ----------------------------------------------------------------------------
//
CFixedTetriminoExt* CFixedTetriminoExt::NewL( 
    CMatrix& aMatrix, MTetriminoObserver& aObserver )
    {
    CFixedTetriminoExt* self = 
        new( ELeave ) CFixedTetriminoExt( aMatrix, aObserver );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// ----------------------------------------------------------------------------
// CFixedTetriminoExt::~CFixedTetriminoExt
// ----------------------------------------------------------------------------
//
CFixedTetriminoExt::~CFixedTetriminoExt()
    {
    delete iTicks;
    }

// ----------------------------------------------------------------------------
// CFixedTetriminoExt::MergeL
// ----------------------------------------------------------------------------
//
void CFixedTetriminoExt::MergeL( const CTetriminoBase& aTetrimino )
    {
    /// Tetrimino bomb only has one block.
    TInt count( aTetrimino.iList->Count() );
    if ( --count )
        {
        CFixedTetrimino::MergeL( aTetrimino );
        }
    else // It's a bomb!!!
        {
        TBlock bomb( aTetrimino.iList->At( 0 ) );
        bomb.iX += aTetrimino.iOffsetX;
        bomb.iY += aTetrimino.iOffsetY;
        BombL( bomb );

        iObserver.StateChangedL( EFixedTetriminoMerged );
        }
    }

// ----------------------------------------------------------------------------
// CFixedTetriminoExt::RemoveL
// ----------------------------------------------------------------------------
//
void CFixedTetriminoExt::RemoveL()
    {
    CFixedTetrimino::RemoveL();
    if ( iGrowState )
        {
        iGrowState = EFalse;
        AutoGrowL();
        }
    }

// ----------------------------------------------------------------------------
// CFixedTetriminoExt::Reset()
// ----------------------------------------------------------------------------
//
void CFixedTetriminoExt::Reset()
    {
	CTetriminoBase::Reset();
	iGrowState = EFalse;
	iTicks->Stop();
    }

// ----------------------------------------------------------------------------
// CFixedTetriminoExt::Start()
// ----------------------------------------------------------------------------
//
void CFixedTetriminoExt::Start()
    {
    iTicks->Start( KAutoGrowDelayTimeout );
    }

// ----------------------------------------------------------------------------
// CFixedTetriminoExt::Stop()
// ----------------------------------------------------------------------------
//
void CFixedTetriminoExt::Stop()
    {
    iTicks->Pause();
    }

// ----------------------------------------------------------------------------
// CFixedTetriminoExt::Resume()
// ----------------------------------------------------------------------------
//
void CFixedTetriminoExt::Resume()
    {
    iTicks->Resume();
    }

// ----------------------------------------------------------------------------
// CFixedTetriminoExt::TicksCompletedL
// ----------------------------------------------------------------------------
//
void CFixedTetriminoExt::TicksCompletedL()
    {
    iGrowState = ETrue;
    
    TInt extraTimeout = Math::Random() % KAutoGrowExtraTimeout;
    
    iTicks->Start( KAutoGrowBaseTimeout + extraTimeout );
    }

// ----------------------------------------------------------------------------
// CFixedTetriminoExt::AutoGrowL
// ----------------------------------------------------------------------------
//
void CFixedTetriminoExt::AutoGrowL()
    {
    TTime time;
    time.UniversalTime();
    TInt64 seed( time.Int64() );
    TInt line( Math::Rand( seed ) % ETetris + 1 );
    DoAutoGrowL( line );
    EndTestL();
    }

// ----------------------------------------------------------------------------
// CFixedTetriminoExt::DoAutoGrowL
// ----------------------------------------------------------------------------
//
void CFixedTetriminoExt::DoAutoGrowL( TInt aLine )
    {
    NotifyAllChangeL( EMatrixActionRemoved );
    TBlock* block = NULL;
    for ( TInt i = 0; i < iList->Count(); ++i )
        {
        block = &iList->At( i );
        block->iY += aLine;
        }
    NotifyAllChangeL( EMatrixActionMoved );

    for ( TInt height = aLine - 1; height >= 0; height-- )
        {
        RArray<TBlock> array;
        CleanupClosePushL( array );
        
        for ( TInt width = 0; width < KPrimaryMatrixWidth; width++ )
            {
            if ( iBlank )
                {
                TBlock comer;
                comer.SetXY( width, height );
                comer.SetType( EGray );
                
                array.Append( comer );
                
                iBlank = EFalse;
                }
            else
                {
                iBlank = ETrue;
                }
            }
        
        TInt rand( Math::Random() % array.Count() );

        for ( TInt i = 0; i < array.Count(); i++ )
            {
            TBlock theBlock = array[i];
            if ( i == rand )
                {
                theBlock.SetType( AutoGrowType() );
                }
            iList->AppendL( theBlock );
            }

        NotifyAllChangeL( EMatrixActionMoved );

        array.Close();
        CleanupStack::PopAndDestroy( &array );

        iBlank = !iBlank;
        }

    iObserver.StateChangedL( EFixedTetriminoUpdated );
    }

// ----------------------------------------------------------------------------
// CFixedTetriminoExt::AutoGrowType
// ----------------------------------------------------------------------------
//
TType CFixedTetriminoExt::AutoGrowType() const
    {
    TType type;
    TInt rand( Math::Random() % KAtuoGrowTypeProbability );

    if ( rand < KAtuoGrowTypeIncScore200
        && rand >= KAtuoGrowTypeIncScore400 )
        {
        type = EIncScore200;
        }
    else if ( KAtuoGrowTypeRemoveLine1 <= rand 
        && rand < KAtuoGrowTypeIncScore400 )
        {
        type = EIncScore400;
        }
    else if ( KAtuoGrowTypeRemoveLine2 <= rand 
        && rand < KAtuoGrowTypeRemoveLine1 )
        {
        type = ERemoveLine1;
        }
    else if ( KAtuoGrowTypeRemoveLine3 <= rand 
        && rand < KAtuoGrowTypeRemoveLine2 )
        {
        type = ERemoveLine2;
        }
    else if ( KAtuoGrowTypeRemoveLine4 <= rand 
        && rand < KAtuoGrowTypeRemoveLine3 )
        {
        type = ERemoveLine3;
        }
    else if ( KAtuoGrowTypeIncLife <= rand && 
        rand < KAtuoGrowTypeRemoveLine4 )
        {
        type = ERemoveLine4;
        }
    else {
        type = EIncLife;
        }
    return type;
    }

// ----------------------------------------------------------------------------
// CFixedTetriminoExt::BombL
// ----------------------------------------------------------------------------
//
void CFixedTetriminoExt::BombL( const TBlock& aBomb )
    {
    NotifyAllReset();
    NotifyChangeL( EMatrixActionExploded, aBomb );
    
    TInt i =0;
    for ( i = 0; i < iList->Count(); i++ )
        {
        TBlock block = iList->At( i );
        if ( ( block.iX + 1 == aBomb.iX && block.iY == aBomb.iY ) // Right
            || ( block.iX - 1 == aBomb.iX && block.iY == aBomb.iY ) // Left
            || ( block.iX == aBomb.iX && block.iY + 1 == aBomb.iY ) // Top
            || ( block.iX - 1 == aBomb.iX && block.iY + 1 == aBomb.iY ) // Top left
            || ( block.iX + 1 == aBomb.iX && block.iY + 1 == aBomb.iY ) // Top right
            || ( block.iX == aBomb.iX && block.iY - 1 == aBomb.iY ) // Bottom
            || ( block.iX + 1 == aBomb.iX && block.iY - 1 == aBomb.iY ) // Bottom right
            || ( block.iX - 1 == aBomb.iX && block.iY - 1 == aBomb.iY ) )// Bottom left
            {
            NotifyChangeL( EMatrixActionExploded, block );
            iList->Delete( i );
            iList->Compress();
            --i;
            }
        }
/*
    // Removes impending blocks
    for ( i = 0; i < iList->Count(); i++ )
        {
        TBlock block = iList->At( i );
        if ( block.iY == 0
            || block.iX == 0
            || block.iX == KPrimaryMatrixWidth - 1
            || HasBlock( block.iX - 1, block.iY - 1 ) // Top left
            || HasBlock( block.iX, block.iY - 1 ) // Top
            || HasBlock( block.iX + 1, block.iY - 1 ) // Top right
            || HasBlock( block.iX - 1, block.iY ) // Left
            || HasBlock( block.iX + 1, block.iY ) // Right
            || HasBlock( block.iX - 1, block.iY + 1 ) // Bottom left
            || HasBlock( block.iX, block.iY + 1 ) // Bottom
            || HasBlock( block.iX + 1, block.iY + 1 ) )// Bottom right 
            {
            continue;
            }
        else
            {
            NotifyChangeL( EMatrixActionExploded, block );
            iList->Delete( i );
            iList->Compress();
            --i;
            }
        }
*/
    iObserver.StateChangedL( EFixedTetriminoUpdated );
    }

// ----------------------------------------------------------------------------
// CFixedTetriminoExt::HasBlock
// ----------------------------------------------------------------------------
//
TBool CFixedTetriminoExt::HasBlock( TInt aX, TInt aY ) const
    {
    TBool retVal = EFalse;
    for ( TInt i = 0; i < iList->Count(); i++ )
        {
        TBlock block = iList->At( i );
        if ( block.iX == aX && block.iY == aY )
            {
            retVal = ETrue;
            break;
            }
        }
    return retVal;
    }

// ----------------------------------------------------------------------------
// CFixedTetriminoExt::CFixedTetriminoExt
// ----------------------------------------------------------------------------
//
CFixedTetriminoExt::CFixedTetriminoExt( 
    CMatrix& aMatrix, MTetriminoObserver& aObserver )
: CFixedTetrimino( aMatrix, aObserver ), iGrowState( EFalse )
    { 
    // no implementation required
    }

// ----------------------------------------------------------------------------
// CFixedTetriminoExt::ConstructL
// ----------------------------------------------------------------------------
//
void CFixedTetriminoExt::ConstructL()
    {
    CFixedTetrimino::ConstructL();

    iTicks = CTicks::NewL( *this );
    }

// End of File
