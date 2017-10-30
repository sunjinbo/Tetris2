/* ====================================================================
 * File: TetriminoBase.cpp
 * Created: 01/03/09
 * Modified:01/03/09
 * Author: Sun Jinbo
 * Copyright (c): Tieto, All rights reserved
 * ==================================================================== */

// INCLUDE FILES
#include "TetriminoBase.h"
#include "TetriminOobserver.h"
#include "Matrix.h"
#include "MatrixAction.h"

// CONSTANTS
const TInt KTetriminoDefautOffsetX = 0;
const TInt KTetriminoDefautOffsetY = 0;
const TInt KTetriminoListCranularity = 4;


// ======== MEMBER FUNCTIONS ========

// ----------------------------------------------------------------------------
// CTetriminoBase::~CTetriminoBase
// ----------------------------------------------------------------------------
//
CTetriminoBase::~CTetriminoBase()
    {
    delete iList;
    }

// ----------------------------------------------------------------------------
// CTetriminoBase::CloneL
// ----------------------------------------------------------------------------
//
void CTetriminoBase::CloneL( const CTetriminoBase& aTetrimino )
    {
    Reset();
    
    iOffsetX = aTetrimino.iOffsetX;
    iOffsetY = aTetrimino.iOffsetY;
    TInt count( aTetrimino.iList->Count() );
    for ( TInt i = 0; i < count; ++i )
        {
        iList->AppendL( aTetrimino.iList->At( i ) );
        }
    }

// ----------------------------------------------------------------------------
// CTetriminoBase::Reset
// ----------------------------------------------------------------------------
//
void CTetriminoBase::Reset()
    {
    iOffsetX = KTetriminoDefautOffsetX;
    iOffsetY = KTetriminoDefautOffsetY;
    iList->Reset();
    }

// ----------------------------------------------------------------------------
// CTetriminoBase::CTetriminoBase
// ----------------------------------------------------------------------------
//
CTetriminoBase::CTetriminoBase( CMatrix& aMatrix, MTetriminoObserver& aObserver )
    : iMatrix( aMatrix ), iObserver( aObserver )
    {
    iMatrix.Append( this );
    }

// ----------------------------------------------------------------------------
// CTetriminoBase::ConstructL
// ----------------------------------------------------------------------------
//
void CTetriminoBase::ConstructL()
    {
    iList = new( ELeave ) CArrayFixFlat<TBlock> ( KTetriminoListCranularity );
    }

// ----------------------------------------------------------------------------
// CTetriminoBase::NotifyChangeL
// ----------------------------------------------------------------------------
//
void CTetriminoBase::NotifyChangeL( TMatrixActionType aType, TBlock aBlock )
    {
    CMatrixAction* matrixAction( iMatrix.MatrixAction() );
    TBlock block( aBlock );
    block.iX += iOffsetX;
    block.iY += iOffsetY;
    matrixAction->AppendL( aType, block );
    }

// ----------------------------------------------------------------------------
// CTetriminoBase::NotifyChangeL
// ----------------------------------------------------------------------------
//
void CTetriminoBase::NotifyAllChangeL( TMatrixActionType aType )
    {
    iMatrix.MatrixAction()->Reset( aType );

    TInt count( iList->Count() );
    TBlock block;
    for ( TInt i = 0; i < count; i++ )
        {
        block = iList->At( i );
        NotifyChangeL( aType, block );
        }
    }

// ----------------------------------------------------------------------------
// CTetriminoBase::NotifyAllReset
// ----------------------------------------------------------------------------
//
void CTetriminoBase::NotifyAllReset()
    {
    iMatrix.MatrixAction()->Reset();
    }

// End of File
