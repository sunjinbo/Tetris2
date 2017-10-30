/* ====================================================================
 * File: NextRemovableTetriminoExt.h
 * Created: 01/04/09
 * Modifed:
 * Author: Sun Jinbo
 * Copyright (c): Tieto, All rights reserved
 * ==================================================================== */

#ifndef C_NEXTREMOVABLETETRIMINOEXT_H
#define C_NEXTREMOVABLETETRIMINOEXT_H

// INCLUDE FILES
#include <e32def.h>
#include <e32base.h>
#include "NextRemovableTetrimino.h"


// CLASS DECLARATION
/**
* CNextRemovableTetriminoExt class
*
* @since S60 3.1
*/
NONSHARABLE_CLASS( CNextRemovableTetriminoExt ) : public CNextRemovableTetrimino
    {
public: // Constructor and destructor
    static CNextRemovableTetriminoExt* NewL( 
        CMatrix& aMatrix, MTetriminoObserver& aObserver );
    virtual ~CNextRemovableTetriminoExt();

protected: /// Constructor
    // C++ Constructor
    CNextRemovableTetriminoExt( 
        CMatrix& aMatrix, MTetriminoObserver& aObserver );
    // Completes construction
    virtual void ConstructL();
    virtual void RandomTetriminoL();
    };

#endif // C_NEXTREMOVABLETETRIMINOEXT_H

// End of File
