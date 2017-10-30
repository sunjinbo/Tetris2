/* ====================================================================
 * File: AnimationTetriminoExt.cpp
 * Created: 01/04/09
 * Modifed:
 * Author: Sun Jinbo
 * Copyright (c): Tieto, All rights reserved
 * ==================================================================== */

// INCLUDE FILES
#include "AnimationTetriminoExt.h"

// ======== MEMBER FUNCTIONS ========

// ----------------------------------------------------------------------------
// CAnimationTetriminoExt::NewL
// ----------------------------------------------------------------------------
//
CAnimationTetriminoExt* CAnimationTetriminoExt::NewL( 
    CMatrix& aMatrix, MTetriminoObserver& aObserver )
    {
    CAnimationTetriminoExt* self = 
        new(ELeave) CAnimationTetriminoExt( aMatrix, aObserver );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// ----------------------------------------------------------------------------
// CAnimationTetriminoExt::~CAnimationTetriminoExt
// ----------------------------------------------------------------------------
//
CAnimationTetriminoExt::~CAnimationTetriminoExt()
    { // no implementation required  
    }

// ----------------------------------------------------------------------------
// CAnimationTetriminoExt::CAnimationTetriminoExt
// ----------------------------------------------------------------------------
//
CAnimationTetriminoExt::CAnimationTetriminoExt( 
    CMatrix& aMatrix, MTetriminoObserver& aObserver )
    : CAnimationTetrimino( aMatrix, aObserver )
    { // no implementation required 
    }

// ----------------------------------------------------------------------------
// CAnimationTetriminoExt::ConstructL
// ----------------------------------------------------------------------------
//
void CAnimationTetriminoExt::ConstructL()
    {
    CAnimationTetrimino::ConstructL();
    }

// End of File
