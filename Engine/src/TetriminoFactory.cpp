/* ====================================================================
 * File: TetriminoFactory.cpp
 * Created: 01/05/09
 * Modified:
 * Author: Sun Jinbo
 * Copyright (c): Tieto, All rights reserved
 * ==================================================================== */

//  INCLUDE FILES
#include "TetriminoFactory.h"
#include "FixedTetrimino.h"
#include "NextRemovableTetrimino.h"
#include "RemovableTetrimino.h"
#include "AnimationTetrimino.h"
#include "Matrix.h"
#include "TetriminoObserver.h"

// ======== MEMBER FUNCTIONS ========

// ----------------------------------------------------------------------------
// CTetriminoFactory::NewL
// ----------------------------------------------------------------------------
//
CTetriminoFactory* CTetriminoFactory::NewL()
    {
    CTetriminoFactory* self = new( ELeave ) CTetriminoFactory();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// ----------------------------------------------------------------------------
// CTetriminoFactory::~CTetriminoFactory
// ----------------------------------------------------------------------------
//
CTetriminoFactory::~CTetriminoFactory()
    { // no implementation required 
    }

// ----------------------------------------------------------------------------
// CTetriminoFactory::CreateFixedTetriminoL
// ----------------------------------------------------------------------------
//
CFixedTetrimino* CTetriminoFactory::CreateFixedTetriminoL( 
    CMatrix& aMatrix, MTetriminoObserver& aObserver )
    {
    return CFixedTetrimino::NewL( aMatrix, aObserver );
    }

// ----------------------------------------------------------------------------
// CTetriminoFactory::CreateNextRemovableTetriminoL
// ----------------------------------------------------------------------------
//
CNextRemovableTetrimino* CTetriminoFactory::CreateNextRemovableTetriminoL( 
    CMatrix& aMatrix, MTetriminoObserver& aObserver )
    {
    return CNextRemovableTetrimino::NewL( aMatrix, aObserver );
    }

// ----------------------------------------------------------------------------
// CTetriminoFactory::CreateRemovableTetriminoL
// ----------------------------------------------------------------------------
//
CRemovableTetrimino* CTetriminoFactory::CreateRemovableTetriminoL( 
    CMatrix& aMatrix, MTetriminoObserver& aObserver )
    {
    return CRemovableTetrimino::NewL( aMatrix, aObserver );
    }

// ----------------------------------------------------------------------------
// CTetriminoFactory::CreateAnimationTetriminoL
// ----------------------------------------------------------------------------
//
CAnimationTetrimino* CTetriminoFactory::CreateAnimationTetriminoL( 
    CMatrix& aMatrix, MTetriminoObserver& aObserver )
    {
    return CAnimationTetrimino::NewL( aMatrix, aObserver );
    }

// ----------------------------------------------------------------------------
// CTetriminoFactory::CTetriminoFactory
// ----------------------------------------------------------------------------
//
CTetriminoFactory::CTetriminoFactory()
    { // no implementation required 
    }

// ----------------------------------------------------------------------------
// CTetriminoFactory::ConstructL
// ----------------------------------------------------------------------------
//
void CTetriminoFactory::ConstructL()
    { // no implementation required 
    }

// End of File
