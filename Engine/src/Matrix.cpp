/* ====================================================================
 * File: Matrix.cpp
 * Created: 01/13/09
 * Modified:02/03/09
 * Author: Sun Jinbo
 * Copyright (c): Tieto, All rights reserved
 * ==================================================================== */

// INCLUDE FILES
#include "Matrix.h"
#include "TetriminoBase.h"
#include "MatrixAction.h"

// ======== MEMBER FUNCTIONS ========

// ----------------------------------------------------------------------------
// CMatrix::NewL
// ----------------------------------------------------------------------------
//
CMatrix* CMatrix::NewL()
    {
    CMatrix* self = new( ELeave ) CMatrix;
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// ----------------------------------------------------------------------------
// CMatrix::~CMatrix
// ----------------------------------------------------------------------------
//
CMatrix::~CMatrix()
    {
    iList.Close();
    delete iMatrixAction;
    }

// ----------------------------------------------------------------------------
// CMatrix::Append
// ----------------------------------------------------------------------------
//
void CMatrix::Append( CTetriminoBase* aTetrimino )
    {
    iList.Append( aTetrimino );
    }

// ----------------------------------------------------------------------------
// CMatrix::Count
// ----------------------------------------------------------------------------
//
TInt CMatrix::Count() const
    {
    return iList.Count();
    }

// ----------------------------------------------------------------------------
// CMatrix::At
// ----------------------------------------------------------------------------
//
CTetriminoBase* CMatrix::At( TInt aIndex ) const
    {
    return iList[aIndex];
    }

// ----------------------------------------------------------------------------
// CMatrix::SceneAction
// ----------------------------------------------------------------------------
//
CMatrixAction* CMatrix::MatrixAction()
    {
    return iMatrixAction;
    }

// ----------------------------------------------------------------------------
// CMatrix::CMatrix
// ----------------------------------------------------------------------------
//
CMatrix::CMatrix()
    {  
    // no implementation required 
    }

// ----------------------------------------------------------------------------
// CMatrix::ConstructL
// ----------------------------------------------------------------------------
//
void CMatrix::ConstructL()
    {
    iList.Reset();
    iMatrixAction = CMatrixAction::NewL();
    }

// End of File
