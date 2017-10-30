/* ====================================================================
 * File: RemovableTetriminoExt.h
 * Created: 01/04/09
 * Modifed:
 * Author: Sun Jinbo
 * Copyright (c): Tieto, All rights reserved
 * ==================================================================== */

#ifndef C_REMOVABLETETRIMINOEXT_H
#define C_REMOVABLETETRIMINOEXT_H

// INCLUDE FILES
#include <e32def.h>
#include "RemovableTetrimino.h"

// CLASS DECLARATION
/**
* CRemovableTetriminoExt class
*
* @since S60 3.1
*/
NONSHARABLE_CLASS( CRemovableTetriminoExt ) : public CRemovableTetrimino
    {
public:// New Functions
    static CRemovableTetriminoExt* NewL( 
        CMatrix& aMatrix, MTetriminoObserver& aObserver );
    virtual ~CRemovableTetriminoExt();
    // Rotate( by 90 degrees clockwise )
    virtual void RotateL();
protected:
    // C++ Constructor
    CRemovableTetriminoExt( 
        CMatrix& aMatrix, MTetriminoObserver& aObserver );
    // Completes construction
    virtual void ConstructL();
    };

#endif // C_REMOVABLETETRIMINOEXT_H

// End of File
