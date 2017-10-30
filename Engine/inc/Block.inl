/* ====================================================================
 * File: Block.inl
 * Created: 01/17/09
 * Modified:01/17/09
 * Author: Sun Jinbo
 * Copyright (c): Tieto, All rights reserved
 * ==================================================================== */


// ======== MEMBER FUNCTIONS ========

// ----------------------------------------------------------------------------
// TBlock::TBlock()
// ----------------------------------------------------------------------------
//
inline TBlock::TBlock()
#ifndef RD_TE2_ADVANCE_FUNC
    : iX( EUninit ), iY( EUninit )
#else
    : iX( EUninit ), iY( EUninit ), iType( EUnknown )
#endif // RD_TE2_ADVANCE_FUNC
    {
    }

// ----------------------------------------------------------------------------
// TBlock::TBlock( TInt aX, TInt aY )
// ----------------------------------------------------------------------------
//
inline TBlock::TBlock( TInt aX, TInt aY )
    : iX( aX ), iY( aY )
    {
    }

#ifdef RD_TE2_ADVANCE_FUNC
// ----------------------------------------------------------------------------
// TBlock::TBlock( TInt aX, TInt aY, TType aType )
// ----------------------------------------------------------------------------
//
inline TBlock::TBlock( TInt aX, TInt aY, TType aType )
    : iX( aX ), iY( aY ), iType( aType )
    {
    }
#endif // RD_TE2_ADVANCE_FUNC

// ----------------------------------------------------------------------------
// TBlock::TBlock( const TBlock& aBlock )
// ----------------------------------------------------------------------------
//
inline TBlock::TBlock( const TBlock& aBlock )
#ifndef RD_TE2_ADVANCE_FUNC
    : iX( aBlock.iX ), iY( aBlock.iY )
#else
    : iX( aBlock.iX ), iY( aBlock.iY ), iType( aBlock.iType )
#endif // RD_TE2_ADVANCE_FUNC
    {
    }

// ----------------------------------------------------------------------------
// TBlock::operator==( const TBlock& aBlock ) const
// ----------------------------------------------------------------------------
//
inline TBool TBlock::operator==( const TBlock& aBlock ) const
    {
    if ( ( iX == aBlock.iX ) 
        && ( iX == aBlock.iY )
#ifdef RD_TE2_ADVANCE_FUNC
        && ( iType == aBlock.iType )
#endif // RD_TE2_ADVANCE_FUNC
       )
        {
        return ETrue;
        }
    return EFalse;
    }

// ----------------------------------------------------------------------------
// TBlock::operator !=( const TBlock& aBlock ) const
// ----------------------------------------------------------------------------
//
inline TBool TBlock::operator!=( const TBlock& aBlock ) const
    {
    return ( *this == aBlock ) ? ETrue : EFalse ;
    }

// ----------------------------------------------------------------------------
// TBlock::SetXY( TInt aX, TInt aY )
// ----------------------------------------------------------------------------
//
inline void TBlock::SetXY( TInt aX, TInt aY )
    {
    iX = aX;
    iY = aY;
    }


#ifdef RD_TE2_ADVANCE_FUNC
// ----------------------------------------------------------------------------
// TBlock::SetType( const TType& aType )
// ----------------------------------------------------------------------------
//
inline void TBlock::SetType( const TType& aType )
    {
    iType = aType;
    }
#endif // RD_TE2_ADVANCE_FUNC

// ----------------------------------------------------------------------------
// TBlock::IsNull
// ----------------------------------------------------------------------------
//
inline TBool TBlock::IsNull()
    {
    if ( ( iX == EUninit ) 
        || ( iY == EUninit )
#ifdef RD_TE2_ADVANCE_FUNC
        || ( iType == EUnknown )
#endif // RD_TE2_ADVANCE_FUNC
       )
        {
        return ETrue;
        }
    return EFalse;
    }

// End of File
