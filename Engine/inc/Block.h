/* ====================================================================
 * File: Block.h
 * Created: 01/17/09
 * Modified:01/17/09
 * Author: Sun Jinbo
 * Copyright (c): Tieto, All rights reserved
 * ==================================================================== */

#ifndef T_BLOCK_H
#define T_BLOCK_H

// INCLUDE FILES
#include <e32cmn.h>

#ifdef RD_TE2_ADVANCE_FUNC
// Enumeration for specifying, which all kinds of type in block.
enum TType
    {
    EUnknown = -1,
    ERed, // Normal I tetrimino
    EMagenta, // Normal J tetrimino
    EYellow, // Normal L tetrimino
    ECyan, // Normal O tetrimino
    EBlue, // Normal S tetrimino
    EDarkBlue, // Normal T tetrimino
    EGreen, // Normal Z tetrimino
    EOlive, // For KK tetrimino
    EPurple, // For SS tetrimino
    EOrange, // For ZZ tetrimino
    EBrown, // For CC tetrimino
    ELightGrey, // For TT tetrimino
    EGray, // For Grown block
    EBomb,  // For Bomb block
    EIncScore200, // Be equal to 100 score
    EIncScore400, // Be equal to 400 score
    ERemoveLine1, // Remove one line
    ERemoveLine2, // Remove two lines
    ERemoveLine3, // Remove three lines
    ERemoveLine4, // Remove four lines
    EIncLife // Increase one life
    };
#endif // RD_TE2_ADVANCE_FUNC

// CLASS DECLARATION
/**
* TBlock class
*
* @since S60 3.1
*/
class TBlock
    {
public:
    enum { EUninit = -1 };
    /**
    Constructs default point, initialising its iX and iY members to -1.
    */
    inline TBlock();
    inline TBlock( TInt aX, TInt aY );
#ifdef RD_TE2_ADVANCE_FUNC
    inline TBlock( TInt aX, TInt aY, TType aType );
#endif // RD_TE2_ADVANCE_FUNC
    inline TBlock( const TBlock& aBlock );
    inline TBool operator == ( const TBlock& aBlock ) const;
    inline TBool operator != ( const TBlock& aBlock ) const;
    inline void SetXY( TInt aX, TInt aY );
#ifdef RD_TE2_ADVANCE_FUNC
    inline void SetType( const TType& aType );
#endif // RD_TE2_ADVANCE_FUNC
    inline TBool IsNull();

public: // Member data
    /**
    The x co-ordinate.
    */
    TInt iX;

    /**
    The y co-ordinate.
    */
    TInt iY;

#ifdef RD_TE2_ADVANCE_FUNC
    /**
    The block's type.
    */
    TType iType;
#endif // RD_TE2_ADVANCE_FUNC
    };
#include "Block.inl"

#endif // T_BLOCK_H

// End of File
