/* ====================================================================
 * File: RemovableTetriminoExt.cpp
 * Created: 01/04/09
 * Modifed: 01/06/09
 * Author: Sun Jinbo
 * Copyright (c): Tieto, All rights reserved
 * ==================================================================== */

// INCLUDE FILES
#include "RemovableTetriminoExt.h"

// ======== MEMBER FUNCTIONS ========

// ----------------------------------------------------------------------------
// CRemovableTetriminoExt::NewL
// ----------------------------------------------------------------------------
//
CRemovableTetriminoExt* CRemovableTetriminoExt::NewL( 
    CMatrix& aMatrix, MTetriminoObserver& aObserver )
    {
    CRemovableTetriminoExt* self = 
        new( ELeave ) CRemovableTetriminoExt( aMatrix, aObserver );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// ----------------------------------------------------------------------------
// CRemovableTetriminoExt::~CRemovableTetriminoExt
// ----------------------------------------------------------------------------
//
CRemovableTetriminoExt::~CRemovableTetriminoExt()
    { 
    // no implementation required  
    }

// ----------------------------------------------------------------------------
// CRemovableTetriminoExt::RotateL
// ----------------------------------------------------------------------------
//
void CRemovableTetriminoExt::RotateL()
    {
    // This is for fix a error.
    // When the Tetrimino is a bomb,
    // rotating it could cause some exceptional action.
    TInt count( iList->Count() );
    if ( --count )
        {
        CRemovableTetrimino::RotateL();
        }
    }

// ----------------------------------------------------------------------------
// CRemovableTetriminoExt::CRemovableTetriminoExt
// ----------------------------------------------------------------------------
//
CRemovableTetriminoExt::CRemovableTetriminoExt( 
    CMatrix& aMatrix, MTetriminoObserver& aObserver )
    : CRemovableTetrimino( aMatrix, aObserver )
    { // no implementation required 
    }

// ----------------------------------------------------------------------------
// CRemovableTetriminoExt::ConstructL
// ----------------------------------------------------------------------------
//
void CRemovableTetriminoExt::ConstructL()
    {
    CRemovableTetrimino::ConstructL();
    }

// End of File
