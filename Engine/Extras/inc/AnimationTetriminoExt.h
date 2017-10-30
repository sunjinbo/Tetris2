/* ====================================================================
 * File: AnimationTetriminoExt.h
 * Created: 01/04/09
 * Modified:
 * Author: Sun Jinbo
 * Copyright (c): Tieto, All rights reserved
 * ==================================================================== */

#ifndef C_ANIMATIONTETRIMINOEXT_H
#define C_ANIMATIONTETRIMINOEXT_H

// INCLUDE FILES
#include "AnimationTetrimino.h"

// FORWARD DECLARATIONS
class CPeriodic;

// CLASS DECLARATION
/**
* CAnimationTetriminoExt class
*
* @since S60 3.1
*/
NONSHARABLE_CLASS( CAnimationTetriminoExt ) : public CAnimationTetrimino
    {
public:// New Functions
    static CAnimationTetriminoExt* NewL( 
        CMatrix& aMatrix, MTetriminoObserver& aObserver );
    virtual ~CAnimationTetriminoExt();
protected:
    // C++ Constructor
    CAnimationTetriminoExt( 
        CMatrix& aMatrix, MTetriminoObserver& aObserver );
    // Completes construction
    virtual void ConstructL();
    };

#endif // C_ANIMATIONTETRIMINOEXT_H

// End of File
