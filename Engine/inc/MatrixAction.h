/* ====================================================================
 * File: MatrixAction.h
 * Created: 01/17/09
 * Modified:01/17/09
 * Author: Sun Jinbo
 * Copyright (c): Tieto, All rights reserved
 * ==================================================================== */

#ifndef C_MATRIXACTION_H
#define C_MATRIXACTION_H

// INCLUDE FILES
#include <e32base.h>
#include <e32cmn.h>
#include <e32def.h>
#include "Block.h"
#include "EngineAction.h"

// DEFINITIONS
// type definition for matrix action interface.
typedef TEngineAction TMatrixAction;

// CLASS DECLARATION
/**
* CMatrixAction class
*
* @since S60 3.1
*/
NONSHARABLE_CLASS( CMatrixAction ) : public CBase
    {
public: // Constructor and destructor
    static CMatrixAction* NewL();
    ~CMatrixAction();

public:  // New functions
    TInt Count( TMatrixActionType aType ) const;
    TBlock At( TMatrixActionType aType, TInt aIndex ) const;
    CArrayFixFlat<TBlock>* At( TMatrixActionType aType ) const;
    void AppendL( TMatrixActionType aType, const TBlock& aBlock );
    void Reset( TMatrixActionType aType );
    void Reset();

private: // Constructor
    CMatrixAction();
    void ConstructL();

private: // Member data
    RArray<TMatrixActionType> iType;
    RArray<CArrayFixFlat<TBlock>*> iData;
    };

#endif // C_MATRIXACTION_H

// End of File
