/* ====================================================================
 * File: MatrixAction.cpp
 * Created: 01/13/09
 * Modified:01/23/09
 * Author: Sun Jinbo
 * Copyright (c): Tieto, All rights reserved
 * ==================================================================== */

// INCLUDE FILES
#include "MatrixAction.h"
#include "EnginePanic.h"

// CONSTANTS
const TInt KActionListCranularity = 4;


// ======== MEMBER FUNCTIONS ========

// ----------------------------------------------------------------------------
// CMatrixAction::NewL
// ----------------------------------------------------------------------------
//
CMatrixAction* CMatrixAction::NewL()
    {
    CMatrixAction* self = new( ELeave ) CMatrixAction;
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// ----------------------------------------------------------------------------
// CMatrixAction:::~CMatrixAction
// ----------------------------------------------------------------------------
//
CMatrixAction::~CMatrixAction()
    {
    iType.Close();
    
    for ( TInt i = 0; i < iData.Count(); ++i )
        {
        delete iData[i];
        }
    iData.Close();
    }

// ----------------------------------------------------------------------------
// CMatrixAction:::Count
// ----------------------------------------------------------------------------
//
TInt CMatrixAction::Count( TMatrixActionType aType ) const
    {
    TInt index( iType.Find( aType ) );
    if ( index != KErrNotFound )
        {
        return iData[index]->Count();
        }
    return KErrNotFound;
    }

// ----------------------------------------------------------------------------
// CMatrixAction:::At
// ----------------------------------------------------------------------------
//
TBlock CMatrixAction::At( TMatrixActionType aType, TInt aIndex ) const
    {
    TBlock ret;
    TInt index( iType.Find( aType ) );
    if ( index != KErrNotFound )
        {
        CArrayFixFlat<TBlock>* array = iData[index];
        ret = array->At( aIndex );
        }
    return ret;
    }

// ----------------------------------------------------------------------------
// CMatrixAction:::At
// ----------------------------------------------------------------------------
//
CArrayFixFlat<TBlock>* CMatrixAction::At( TMatrixActionType aType ) const
    {
    CArrayFixFlat<TBlock>* ret = NULL;
    TInt index( iType.Find( aType ) );
    if ( index != KErrNotFound )
        {
        ret = iData[index];
        }
    return ret;
    }

// ----------------------------------------------------------------------------
// CMatrixAction:::AppendL
// ----------------------------------------------------------------------------
//
void CMatrixAction::AppendL( TMatrixActionType aType, const TBlock& aBlock )
    {
    TInt index( iType.Find( aType ) );
    if ( index != KErrNotFound )
        {
        iData[index]->AppendL( aBlock );
        }
    else
        {
        CArrayFixFlat<TBlock>* array = 
            new(ELeave) CArrayFixFlat<TBlock>( KActionListCranularity );
        CleanupStack::PushL( array );
        array->AppendL( aBlock );
        CleanupStack::Pop( array );
        iType.Append( aType );
        iData.Append( array );
        }
    }

// ----------------------------------------------------------------------------
// CMatrixAction:::Reset
// ----------------------------------------------------------------------------
//
void CMatrixAction::Reset( TMatrixActionType aType )
    {
    TInt index( iType.Find( aType ) );
    if ( index != KErrNotFound )
        {
        iData[index]->Reset();
        }
    }

// ----------------------------------------------------------------------------
// CMatrixAction:::Reset
// ----------------------------------------------------------------------------
//
void CMatrixAction::Reset()
    {
    for ( TInt i = 0; i < iType.Count(); ++i )
        {
        iData[i]->Reset();
        }
    }

// ----------------------------------------------------------------------------
// CMatrixAction:::CMatrixAction
// ----------------------------------------------------------------------------
//
CMatrixAction::CMatrixAction()
    { // no implementation required 
    }

// ----------------------------------------------------------------------------
// CMatrixAction:::ConstructL
// ----------------------------------------------------------------------------
//
void CMatrixAction::ConstructL()
    {  // no implementation required 
    }

// End of File
