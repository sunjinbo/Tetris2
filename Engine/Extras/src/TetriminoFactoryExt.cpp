/* ====================================================================
 * File: TetriminoFactoryExt.cpp
 * Created: 01/05/09
 * Modified:
 * Author: Sun Jinbo
 * Copyright (c): Tieto, All rights reserved
 * ==================================================================== */

//  INCLUDE FILES
#include "TetriminoFactoryExt.h"
#include "FixedTetriminoExt.h"
#include "NextRemovableTetriminoExt.h"
#include "RemovableTetriminoExt.h"
#include "AnimationTetriminoExt.h"
#include "Matrix.h"
#include "TetriminoObserver.h"

// ======== MEMBER FUNCTIONS ========

// ----------------------------------------------------------------------------
// CTetriminoFactoryExt::NewL
// ----------------------------------------------------------------------------
//
CTetriminoFactoryExt* CTetriminoFactoryExt::NewL()
    {
    CTetriminoFactoryExt* self = new( ELeave ) CTetriminoFactoryExt();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// ----------------------------------------------------------------------------
// CTetriminoFactoryExt::~CTetriminoFactoryExt
// ----------------------------------------------------------------------------
//
CTetriminoFactoryExt::~CTetriminoFactoryExt()
    { // no implementation required 
    }

// ----------------------------------------------------------------------------
// CTetriminoFactoryExt::CreateFixedTetriminoL
// ----------------------------------------------------------------------------
//
CFixedTetrimino* CTetriminoFactoryExt::CreateFixedTetriminoL( 
    CMatrix& aMatrix, MTetriminoObserver& aObserver )
    {
    return CFixedTetriminoExt::NewL( aMatrix, aObserver );
    }

// ----------------------------------------------------------------------------
// CTetriminoFactoryExt::CreateNextRemovableTetriminoL
// ----------------------------------------------------------------------------
//
CNextRemovableTetrimino* CTetriminoFactoryExt::CreateNextRemovableTetriminoL( 
    CMatrix& aMatrix, MTetriminoObserver& aObserver )
    {
    return CNextRemovableTetriminoExt::NewL( aMatrix, aObserver );
    }

// ----------------------------------------------------------------------------
// CTetriminoFactoryExt::CreateRemovableTetriminoL
// ----------------------------------------------------------------------------
//
CRemovableTetrimino* CTetriminoFactoryExt::CreateRemovableTetriminoL(
    CMatrix& aMatrix, MTetriminoObserver& aObserver )
    {
    return CRemovableTetriminoExt::NewL( aMatrix, aObserver );
    }

// ----------------------------------------------------------------------------
// CTetriminoFactoryExt::CreateAnimationTetriminoL
// ----------------------------------------------------------------------------
//
CAnimationTetrimino* CTetriminoFactoryExt::CreateAnimationTetriminoL( 
    CMatrix& aMatrix, MTetriminoObserver& aObserver )
    {
    return CAnimationTetriminoExt::NewL( aMatrix, aObserver );
    }

// ----------------------------------------------------------------------------
// CTetriminoFactoryExt::CTetriminoFactoryExt
// ----------------------------------------------------------------------------
//
CTetriminoFactoryExt::CTetriminoFactoryExt()
    { 
    // no implementation required 
    }

// ----------------------------------------------------------------------------
// CTetriminoFactoryExt::ConstructL
// ----------------------------------------------------------------------------
//
void CTetriminoFactoryExt::ConstructL()
    { 
    // no implementation required 
    }

// End of File
