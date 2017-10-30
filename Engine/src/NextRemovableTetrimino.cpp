/* ====================================================================
 * File: NextRemovableTetrimino.cpp
 * Created: 01/13/09
 * Author: Sun Jinbo
 * Copyright (c): Tieto, All rights reserved
 * ==================================================================== */

// INCLUDE FILES
#include <e32base.h>
#include <e32std.h> // TTime
#include <e32math.h> // Math arithmetic
#include "NextRemovableTetrimino.h"
#include "Matrix.h"
#ifndef RD_TE2_ADVANCE_FUNC
#include "Block.h"
#endif // RD_TE2_ADVANCE_FUNC
#include "EngineCommon.h"
#include "TetriminoObserver.h"
#include "Te2Engine.hrh"

// CONSTANTS
/**
 * I Type
 * J Type
 * L Type
 * O Type
 * S Type
 * T Type
 * Z Type
**/
static const TInt KTetriminoTotalNumber = 7;

/**
 * North
 * East
 * South
 * West
**/
static const TInt KTetriminoDirection = 4;

#ifdef RD_TE2_ADVANCE_FUNC
/**
 * I == Red
 * J == Magenta
 * L == Yellow
 * O == Cyan
 * S == Blue
 * T == Dark Blue
 * Z == Green 
**/
static const TInt KTetriminoColorNumber = KTetriminoTotalNumber;

static const TType KTetriminoColorType[KTetriminoColorNumber] =
    {
    ERed,
    EMagenta,
    EYellow,
    ECyan,
    EBlue,
    EDarkBlue,
    EGreen
    };
#endif // RD_TE2_ADVANCE_FUNC

// There are 7 differently shaped Tetriminos that fall into a rectangular Matrix.
// A group of four Blocks (Minos) joined together to form one of the 7 shapes, 
// namely, the O-, I-, T-, L-, J-, S-, and Z-Tetriminos. 
// Their template as follows,
// >> I Tetrimino Template
static const TInt KTetriminoI[4][4] =
    {
        { 0, 1, 0, 0 },
        { 0, 1, 0, 0 },
        { 0, 1, 0, 0 },
        { 0, 1, 0, 0 }
    };

// >> J Tetrimino Template
static const TInt KTetriminoJ[4][4] =
    {
        { 0, 0, 1, 0 },
        { 0, 0, 1, 0 },
        { 0, 1, 1, 0 },
        { 0, 0, 0, 0 }
    };
 
// >> L Tetrimino Template
static const TInt KTetriminoL[4][4] =
    {
        { 0, 1, 0, 0 },
        { 0, 1, 0, 0 },
        { 0, 1, 1, 0 },
        { 0, 0, 0, 0 }
    };

// >> O Tetrimino Template
static const TInt KTetriminoO[4][4] =
    {
        { 0, 0, 0, 0 },
        { 0, 1, 1, 0 },
        { 0, 1, 1, 0 },
        { 0, 0, 0, 0 }
    };

// >> S Tetrimino Template
static const TInt KTetriminoS[4][4] =
    {
        { 0, 0, 0, 0 },
        { 0, 0, 1, 1 },
        { 0, 1, 1, 0 },
        { 0, 0, 0, 0 }
    };

// >> T Tetrimino Template
static const TInt KTetriminoT[4][4] =
    {
        { 0, 0, 1, 0 },
        { 0, 1, 1, 0 },
        { 0, 0, 1, 0 },
        { 0, 0, 0, 0 }
    };

// >> Z Tetrimino Template
static const TInt KTetriminoZ[4][4] =
    {
        { 0, 0, 0, 0 },
        { 0, 1, 1, 0 },
        { 0, 0, 1, 1 },
        { 0, 0, 0, 0 }
    };


// ======== MEMBER FUNCTIONS ========

// ----------------------------------------------------------------------------
// CNextRemovableTetrimino::NewL
// ----------------------------------------------------------------------------
//
CNextRemovableTetrimino* CNextRemovableTetrimino::NewL( 
    CMatrix& aMatrix, MTetriminoObserver& aObserver )
    {
    CNextRemovableTetrimino* self = 
        new( ELeave ) CNextRemovableTetrimino( aMatrix, aObserver );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// ----------------------------------------------------------------------------
// CNextRemovableTetrimino::~CNextRemovableTetrimino
// ----------------------------------------------------------------------------
//
CNextRemovableTetrimino::~CNextRemovableTetrimino()
    { // no implementation required 
    }

// ----------------------------------------------------------------------------
// CNextRemovableTetrimino::Next
// ----------------------------------------------------------------------------
//
void CNextRemovableTetrimino::NextL()
    {
    // Remove previous blocks in next removable tetrimino.
    NotifyAllChangeL( EMatrixActionRemoved );
    iObserver.StateChangedL( ENextRemovableTetriminoRemoved );
    // Create a group of blocks in next removable tetrimino.
    RandomTetriminoL();
    RandomDirection();
    NotifyAllChangeL( EMatrixActionCreated );
    iObserver.StateChangedL( ENextRemovableTetriminoCreated );
    }

// ----------------------------------------------------------------------------
// CNextRemovableTetrimino::UpdateAllL
// ----------------------------------------------------------------------------
//
void CNextRemovableTetrimino::UpdateAllL()
    {
    NotifyAllChangeL( EMatrixActionUpdated );
    iObserver.StateChangedL( ENextRemovableTetriminoAllUpdated );
    }

// ----------------------------------------------------------------------------
// CNextRemovableTetrimino::CNextRemovableTetrimino
// ----------------------------------------------------------------------------
//
CNextRemovableTetrimino::CNextRemovableTetrimino( 
    CMatrix& aMatrix, MTetriminoObserver& aObserver )
    : CTetriminoBase( aMatrix, aObserver )
    { // no implementation required 
    }

// ----------------------------------------------------------------------------
// CNextRemovableTetrimino::ConstructL
// ----------------------------------------------------------------------------
//
void CNextRemovableTetrimino::ConstructL()
    {
    CTetriminoBase::ConstructL();
    }

// ----------------------------------------------------------------------------
// CNextRemovableTetrimino::RandomTetriminoL
// ----------------------------------------------------------------------------
//
void CNextRemovableTetrimino::RandomTetriminoL()
    {
    TTime time;
    TInt rand( Math::Random() % KTetriminoTotalNumber );

    // Gets Tetrimino type according to the random value.
#ifdef RD_TE2_ADVANCE_FUNC
    TType type = KTetriminoColorType[ rand ];
#endif // RD_TE2_ADVANCE_FUNC

    switch( rand )
        {
#ifndef RD_TE2_ADVANCE_FUNC
        case ETetriminoI:
            DoTetriminoL( &KTetriminoI[0][0] );
            break;
        case ETetriminoJ:
            DoTetriminoL( &KTetriminoJ[0][0] );
            break;
        case ETetriminoL:
        	DoTetriminoL( &KTetriminoL[0][0] );
            break;
        case ETetriminoO:
        	DoTetriminoL( &KTetriminoO[0][0] );
            break;
        case ETetriminoS:
        	DoTetriminoL( &KTetriminoS[0][0] );
            break;
        case ETetriminoT:
        	DoTetriminoL( &KTetriminoT[0][0] );
            break;
        case ETetriminoZ:
        	DoTetriminoL( &KTetriminoZ[0][0] );
            break;
#else // RD_TE2_ADVANCE_FUNC
        case ETetriminoI:
        	DoTetriminoL( &KTetriminoI[0][0], type );
            break;
        case ETetriminoJ:
        	DoTetriminoL( &KTetriminoJ[0][0], type );
            break;
        case ETetriminoL:
        	DoTetriminoL( &KTetriminoL[0][0], type );
            break;
        case ETetriminoO:
        	DoTetriminoL( &KTetriminoO[0][0], type );
            break;
        case ETetriminoS:
        	DoTetriminoL( &KTetriminoS[0][0], type );
            break;
        case ETetriminoT:
        	DoTetriminoL( &KTetriminoT[0][0], type );
            break;
        case ETetriminoZ:
        	DoTetriminoL( &KTetriminoZ[0][0], type );
            break;
#endif // RD_TE2_ADVANCE_FUNC
        default:
            break;
        }
    }

// ----------------------------------------------------------------------------
// CNextRemovableTetrimino::DoTetriminoL
// ----------------------------------------------------------------------------
//
#ifdef RD_TE2_ADVANCE_FUNC
void CNextRemovableTetrimino::DoTetriminoL( const TInt* aTetrimino, TType aType )
#else
void CNextRemovableTetrimino::DoTetriminoL( const TInt* aTetrimino )
#endif // RD_TE2_ADVANCE_FUNC
    {
    iList->Reset();
  
    for ( TInt i = 0; i < KSecondaryMatrixWidth; ++i )
        for ( TInt j = 0; j < KSecondaryMatrixHeight; ++j )
            {
            if ( *( aTetrimino + KSecondaryMatrixWidth * i + j ) )
                {
#ifdef RD_TE2_ADVANCE_FUNC
                iList->AppendL( TBlock( i, j, aType ) );
#else
                iList->AppendL( TBlock( i, j ) );
#endif // RD_TE2_ADVANCE_FUNC
                }
            }
    }

// ----------------------------------------------------------------------------
// CNextRemovableTetrimino::RandomDirection
// ----------------------------------------------------------------------------
//
void CNextRemovableTetrimino::RandomDirection()
    {
    TTime time;
    time.UniversalTime();
    TInt64 seed( time.Int64() );
    TInt rand( Math::Rand( seed ) % KTetriminoDirection );
    
    // Random rotate
    for ( TInt i = 0; i < rand; ++i )
        {
        for ( TInt j = 0; j < iList->Count(); ++j )
            {
            TBlock* block( &iList->At( j ) );
            block->iY = block->iX + block->iY;
            block->iX = block->iY - block->iX;
            block->iY = KSecondaryMatrixHeight - 1 - block->iY + block->iX;
            }
        }
    }

// End of File
