/* ====================================================================
 * File: NextRemovableTetriminoExt.cpp
 * Created: 01/04/09
 * Modified:
 * Author: Sun Jinbo
 * Copyright (c): Tieto, All rights reserved
 * ==================================================================== */

// INCLUDE FILES
#include <e32std.h> // TTime
#include <e32math.h> // Math arithmetic
#include "NextRemovableTetriminoExt.h"
#include "Block.h"
#include "Te2Engine.hrh"

// CONSTANTS
/**
 * Normal Type Probability
 * Extras Type Probability
**/
static const TInt KEngineTypeNumber = 100; /// 100%
static const TInt KEngineNormalType = 0; /// 75%
static const TInt KEngineExtrasType = 75; // 25%

/**
 * OO Type
 * CC Type
 * ZZ Type
 * SS Type
 * KK Type
 * TT Type
**/
static const TInt KTetriminoExtTotalNumber = 6;

/**
 * OO == Bomb
 * CC == Olive
 * ZZ == Purple
 * SS == Orange
 * KK == Brown
 * TT == Light Grey
**/
static const TInt KTetriminoExtColorNumber = KTetriminoExtTotalNumber;

static const TType KTetriminoColorType[KTetriminoExtColorNumber] =
    {
    EBomb,
    EOlive,
    EPurple,
    EOrange,
    EBrown,
    ELightGrey
    };

// There are six differently shaped Tetriminos that fall into a rectangular Matrix.
// A group of four Blocks (Minos) joined together to form one of the 6 shapes, 
// namely, the OO-, CC-, ZZ-, SS-, KK-, and TT-Tetriminos. 
// Their template as follows,
// >> OO Tetrimino Template
static const TInt KTetriminoOO[4][4] =
    {
        { 0, 0, 0, 0 },
        { 0, 1, 0, 0 },
        { 0, 0, 0, 0 },
        { 0, 0, 0, 0 }
    };

// >> CC Tetrimino Template
static const TInt KTetriminoCC[4][4] =
    {
        { 1, 0, 1, 0 },
        { 1, 1, 1, 0 },
        { 0, 0, 0, 0 },
        { 0, 0, 0, 0 }
    };
 
// >> ZZ Tetrimino Template
static const TInt KTetriminoZZ[4][4] =
    {
        { 1, 1, 0, 0 },
        { 0, 1, 0, 0 },
        { 0, 1, 1, 0 },
        { 0, 0, 0, 0 }
    };

// >> SS Tetrimino Template
static const TInt KTetriminoSS[4][4] =
    {
        { 0, 1, 1, 0 },
        { 0, 1, 0, 0 },
        { 1, 1, 0, 0 },
        { 0, 0, 0, 0 }
    };

// >> KK Tetrimino Template
static const TInt KTetriminoKK[4][4] =
    {
        { 0, 1, 0, 0 },
        { 1, 1, 1, 0 },
        { 0, 1, 0, 0 },
        { 0, 0, 0, 0 }
    };

// >> TT Tetrimino Template
static const TInt KTetriminoTT[4][4] =
    {
        { 0, 0, 1, 0 },
        { 1, 1, 1, 0 },
        { 0, 0, 1, 0 },
        { 0, 0, 0, 0 }
    };


// ======== MEMBER FUNCTIONS ========

// ----------------------------------------------------------------------------
// CNextRemovableTetriminoExt::NewL
// ----------------------------------------------------------------------------
//
CNextRemovableTetriminoExt* CNextRemovableTetriminoExt::NewL( 
        CMatrix& aMatrix, MTetriminoObserver& aObserver )
    {
    CNextRemovableTetriminoExt* self = 
        new( ELeave ) CNextRemovableTetriminoExt( aMatrix, aObserver );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// ----------------------------------------------------------------------------
// CNextRemovableTetriminoExt::~CNextRemovableTetriminoExt
// ----------------------------------------------------------------------------
//
CNextRemovableTetriminoExt::~CNextRemovableTetriminoExt()
    { // no implementation required  
    }

// ----------------------------------------------------------------------------
// CNextRemovableTetriminoExt::CNextRemovableTetriminoExt
// ----------------------------------------------------------------------------
//
CNextRemovableTetriminoExt::CNextRemovableTetriminoExt( 
    CMatrix& aMatrix, MTetriminoObserver& aObserver )
    : CNextRemovableTetrimino( aMatrix, aObserver )
    { // no implementation required
    }

// ----------------------------------------------------------------------------
// CNextRemovableTetriminoExt::ConstructL
// ----------------------------------------------------------------------------
//
void CNextRemovableTetriminoExt::ConstructL()
    {
    CNextRemovableTetrimino::ConstructL();
    }

// ----------------------------------------------------------------------------
// CNextRemovableTetriminoExt::RandomTetriminoL
// ----------------------------------------------------------------------------
//
void CNextRemovableTetriminoExt::RandomTetriminoL()
    {
    TTime time;
    TInt rand( Math::Random() % KEngineTypeNumber );

    if ( rand >= KEngineNormalType && rand < KEngineExtrasType )
        { // for Normal.
        CNextRemovableTetrimino::RandomTetriminoL();
        }
    else // for Extras.
        {
        rand = Math::Random() % KTetriminoExtTotalNumber;
        TType type = KTetriminoColorType[rand];

        switch ( rand )
            {
        case ETetriminoOO:
            DoTetriminoL( &KTetriminoOO[0][0], type );
            break;
        case ETetriminoCC:
        	DoTetriminoL( &KTetriminoCC[0][0], type );
            break;
        case ETetriminoZZ:
        	DoTetriminoL( &KTetriminoZZ[0][0], type );
            break;
        case ETetriminoSS:
        	DoTetriminoL( &KTetriminoSS[0][0], type );
            break;
        case ETetriminoKK:
        	DoTetriminoL( &KTetriminoKK[0][0], type );
            break;
        case ETetriminoTT:
        	DoTetriminoL( &KTetriminoTT[0][0], type );
            break;
        default:
            break;
            }
        }
    }

// End of File
