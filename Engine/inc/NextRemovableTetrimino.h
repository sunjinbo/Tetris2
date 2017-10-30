/* ====================================================================
 * File: NextRemovableTetrimino.h
 * Created: 01/17/09
 * Author: Sun Jinbo
 * Copyright (c): Tieto, All rights reserved
 * ==================================================================== */

#ifndef C_NEXTREMOVABLETETRIMINO_H
#define C_NEXTREMOVABLETETRIMINO_H

// INCLUDE FILES
#include <e32def.h>
#include <e32base.h>
#include "TetriminoBase.h"

// CLASS DECLARATION
/**
* CNextRemovableTetrimino class
*
* @since S60 3.1
*/
NONSHARABLE_CLASS( CNextRemovableTetrimino ) : public CTetriminoBase
    {
public: // Constructor and destructor
    static CNextRemovableTetrimino* NewL( 
        CMatrix& aMatrix, MTetriminoObserver& aObserver );
    virtual ~CNextRemovableTetrimino();

public: // New function
    void NextL();
    void UpdateAllL();

protected: /// Constructor
    // C++ Constructor
    CNextRemovableTetrimino( CMatrix& aMatrix, MTetriminoObserver& aObserver );
    // Completes construction
    void ConstructL();
    virtual void RandomDirection(); 
    virtual void RandomTetriminoL();

#ifdef RD_TE2_ADVANCE_FUNC
    void DoTetriminoL( const TInt* aTetrimino, TType aType );
#else
    void DoTetriminoL( const TInt* aTetrimino );
#endif // RD_TE2_ADVANCE_FUNC
    };

#endif // C_NEXTREMOVABLETETRIMINO_H

// End of File
