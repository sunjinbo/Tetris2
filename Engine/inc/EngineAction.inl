/* ====================================================================
 * File: MatrixAction.cpp
 * Created: 01/17/09
 * Modified: 01/17/09
 * Author: Sun Jinbo
 * Copyright (c): Tieto, All rights reserved
 * ==================================================================== */

// ======== INLINE FUNCTIONS ========

// ----------------------------------------------------------------------------
// TEngineAction::TEngineAction
// ----------------------------------------------------------------------------
//
inline TEngineAction::TEngineAction()
    {
    }

// ----------------------------------------------------------------------------
// TEngineAction::TEngineAction
// ----------------------------------------------------------------------------
//
inline TEngineAction::TEngineAction( TInt aType, TInt aValue )
    : iType( aType )
    {
    iValue.iInt32 = aValue;
    }

// ----------------------------------------------------------------------------
// TEngineAction::TEngineAction
// ----------------------------------------------------------------------------
//
inline TEngineAction::TEngineAction( TInt aType, const TBlockArrayPointer& aValue )
    : iType( aType )
    {
    iValue.iArray = aValue;
    }

// ----------------------------------------------------------------------------
// TEngineAction::Type
// ----------------------------------------------------------------------------
//
inline TInt TEngineAction::Type() const
    {
    return iType;
    }

// ----------------------------------------------------------------------------
// TEngineAction::Get
// ----------------------------------------------------------------------------
//
inline void TEngineAction::Get( TInt32& aValue ) const
    {
    aValue = iValue.iInt32;
    }
    
// ----------------------------------------------------------------------------
// TEngineAction::Get
// ----------------------------------------------------------------------------
//
inline void TEngineAction::Get( TBlockArrayPointer& aValue) const
    {
    aValue = iValue.iArray;
    }

// ----------------------------------------------------------------------------
// TEngineAction::SetType
// ----------------------------------------------------------------------------
//
inline void TEngineAction::SetType( TInt aType )
    {
    iType = aType;
    }

// ----------------------------------------------------------------------------
// TEngineAction::Set
// ----------------------------------------------------------------------------
//
inline void TEngineAction::Set( const TInt& aValue )
    {
    iValue.iInt32 = aValue;
    }

// ----------------------------------------------------------------------------
// TEngineAction::Set
// ----------------------------------------------------------------------------
//
inline void TEngineAction::Set( const TBlockArrayPointer& aValue )
    {
    iValue.iArray = aValue;
    }

// End of File
