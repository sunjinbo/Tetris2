/* ====================================================================
 * File: EngineAction.h
 * Created: 01/17/09
 * Modified:01/17/09
 * Author: Sun Jinbo
 * Copyright (c): Tieto, All rights reserved
 * ==================================================================== */

#ifndef C_ENGINEACTION_H
#define C_ENGINEACTION_H

// INCLUDE FILES
#include "Block.h"

// ENUMERATES
// for reporting to observer what kinds of actions
// would be done in matrix.
enum TMatrixActionType
    {
    EMatrixActionCreated = 0x00FF,
    EMatrixActionMoved,
    EMatrixActionUpdated,
    EMatrixActionRemoved,
    EMatrixActionReleased,
#ifdef RD_TE2_ADVANCE_FUNC
    EMatrixActionExploded,
    EMatrixActionChanged
#endif // RD_TE2_ADVANCE_FUNC
    };

// for reporting to observer what kinds of actions
// would be done in scorekeeper.
enum TScorekeeperActionType
    {
    EScorekeeperActionLevel = 0xFF00,
    EScorekeeperActionLine,
    EScorekeeperActionScore,
    EScorekeeperActionLife
    };

// DEFINITIONS
// type definition for Engine interface.
typedef CArrayFixFlat<TBlock>* TBlockArrayPointer;

// CLASS DECLARATION
/**
* TEngineAction class
*
* @since S60 3.1
*/
class TEngineAction
    {
public: // New Functions
    inline TEngineAction();
    inline TEngineAction( TInt aType, TInt aValue );
    inline TEngineAction( TInt aType, const TBlockArrayPointer& aValue );

    inline TInt Type() const;
    inline void Get( TInt32& aValue ) const;
    inline void Get( TBlockArrayPointer& aValue) const;
    inline void SetType( TInt aType );
    inline void Set( const TInt& aValue );
    inline void Set( const TBlockArrayPointer& aValue );

private: // Member data
    /// Type of the data this action is holding
    TInt iType;

    // Data value stored in this variant.
    // One of these are effective at a time depending on the iType
    //
    union UData
        {
        TInt32 iInt32;      // 32-bit integer
        TBlockArrayPointer iArray;
        };
    UData iValue;
    };
// INLINE FUNCTIONS
#include "EngineAction.inl"


#endif // C_ENGINEACTION_H

// End of File
