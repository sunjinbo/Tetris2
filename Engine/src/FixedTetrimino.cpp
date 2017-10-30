/* ====================================================================
 * File: FixedTetrimino.cpp
 * Created: 01/13/09
 * Modified:01/17/09
 * Author: Sun Jinbo
 * Copyright (c): Tieto, All rights reserved
 * ==================================================================== */

// INCLUDE FILES
#include <e32math.h>
#include "FixedTetrimino.h"
#include "Matrix.h"
#include "MatrixAction.h"
#include "Block.h"
#include "TetriminoObserver.h"
#include "EngineCommon.h"

// ======== MEMBER FUNCTIONS ========

// ----------------------------------------------------------------------------
// CFixedTetrimino::NewL
// ----------------------------------------------------------------------------
//
CFixedTetrimino* CFixedTetrimino::NewL( CMatrix& aMatrix, MTetriminoObserver& aObserver )
    {
    CFixedTetrimino* self = new( ELeave ) CFixedTetrimino( aMatrix, aObserver );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// ----------------------------------------------------------------------------
// CFixedTetrimino::~CFixedTetrimino
// ----------------------------------------------------------------------------
//
CFixedTetrimino::~CFixedTetrimino()
    { // no implementation required     
    }

// ----------------------------------------------------------------------------
// CFixedTetrimino::MergeL
// ----------------------------------------------------------------------------
//
void CFixedTetrimino::MergeL( const CTetriminoBase& aTetrimino )
    {
    // Merge a removable tetrimino
    TInt count( aTetrimino.iList->Count() );
    for ( TInt i = 0; i < count; ++i )
        {
        TBlock block( aTetrimino.iList->At( i ) );
        block.iX += aTetrimino.iOffsetX;
        block.iY += aTetrimino.iOffsetY;
        iList->AppendL( block );
        }
    // Inform to engine
    iObserver.StateChangedL( EFixedTetriminoMerged );

    // Release full lines of the primary Matirx
    RemoveL();

    // Estimate whether the game is over or not.
    EndTestL();
    }

// ----------------------------------------------------------------------------
// CFixedTetrimino::UpdateAllL
// ----------------------------------------------------------------------------
//
void CFixedTetrimino::UpdateAllL()
    {
    NotifyAllChangeL( EMatrixActionUpdated );
    iObserver.StateChangedL( EFixedTetriminoAllUpdated );
    }

// ----------------------------------------------------------------------------
// CFixedTetrimino::CFixedTetrimino
// ----------------------------------------------------------------------------
//
CFixedTetrimino::CFixedTetrimino( CMatrix& aMatrix, MTetriminoObserver& aObserver )
    : CTetriminoBase( aMatrix, aObserver )
    { // no implementation required 
    }

// ----------------------------------------------------------------------------
// CFixedTetrimino::ConstructL
// ----------------------------------------------------------------------------
//
void CFixedTetrimino::ConstructL()
    {
    // Initialises this tetrimino with standard values.
    CTetriminoBase::ConstructL();
    }

// ----------------------------------------------------------------------------
// CFixedTetrimino::Remove
// ----------------------------------------------------------------------------
//
void CFixedTetrimino::RemoveL()
    {
    RArray<TInt> line;
    line.Reset();
    /// Finds which line should be removed within the array, 
    /// based on the matching of keys, using a sequential search.
    for ( TInt height = 0; height < KPrimaryMatrixHeight; ++height )
        {
        TInt count( 0 );
        for ( TInt i = 0; i < iList->Count(); ++i )
            {
            if ( iList->At( i ).iY == height )
                {
                count++;
                }
            
            if ( count == KPrimaryMatrixWidth )
                {
                line.Append( height );
                break;
                }
            }
        }
    
    // Implements the removal of the specified list element from the list.
    line.Sort();
    DoRemoveL( line );

    // Closes the array and frees all memory allocated to the array.
    line.Close();
    }

// ----------------------------------------------------------------------------
// CFixedTetrimino::DoRemove
// ----------------------------------------------------------------------------
//
void CFixedTetrimino::DoRemoveL( const RArray<TInt>& aLine )
    {
    /// Delete the line which was marked would be removed.
    NotifyAllReset();
    const TInt KReleasedBlockMark = -1;
    for ( TInt i = 0; i < iList->Count(); ++i )
        {
        TBlock* block( &iList->At( i ) );
        TInt count( 0 );
        for ( TInt j = 0; j < aLine.Count(); ++j )
            {
            if ( block->iY > aLine[j] )
                {
                count++;
                }
            else if ( block->iY == aLine[j] )
                {
                count = KReleasedBlockMark;
                break;
                }
            else // block->iY < aLine[j]
                {
                // Do nothing.
                }
            }

        if ( count > 0 )
            {
            NotifyChangeL( EMatrixActionRemoved, *block );
            block->iY -= count;
            NotifyChangeL( EMatrixActionMoved, *block );                
            }
        else if ( count == KReleasedBlockMark )
            {
            NotifyChangeL( EMatrixActionReleased, *block );
            iList->Delete( i );
            iList->Compress();
            i -= 1;
            }
        else // count == 0
            { 
            // Do nothing.
            }
        }

    // Notify to scorekeeper.
    switch( aLine.Count() )
        {
        case 1:
            iObserver.StateChangedL( EFixedTetriminoSingleRemoved );
            break;
        case 2:
            iObserver.StateChangedL( EFixedTetriminoDoubleRemoved);
            break;
        case 3:
            iObserver.StateChangedL( EFixedTetriminoTripleRemoved );
            break;
        case 4:
            iObserver.StateChangedL( EFixedTetriminoTetrisRemoved );
            break;
        default:
            break;
        }
    }

// ----------------------------------------------------------------------------
// CFixedTetrimino::EndTestL
// It uesd to evaluate whether tetrimino get to the top of its matrix.
// ----------------------------------------------------------------------------
//
void CFixedTetrimino::EndTestL()
    {
    TInt height( 0 );
    for ( TInt i = 0; i < iList->Count(); ++i )
        {
        if ( height < iList->At( i ).iY )
            {
            height = iList->At( i ).iY;
            }
        }

    if ( KPrimaryMatrixHeight - 1 <= height )
        {
        iObserver.StateChangedL( EFixedTetriminoEnded );
        }
    }

// End of File
