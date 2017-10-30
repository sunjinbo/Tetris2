/* ====================================================================
 * File: RemovableTetrimino.h
 * Created: 01/08/09
 * Author: Sun Jinbo
 * Copyright (c): Tieto, All rights reserved
 * ==================================================================== */

#ifndef C_REMOVABLETETRIMINO_H
#define C_REMOVABLETETRIMINO_H

// INCLUDE FILES
#include <e32def.h>
#include "TetriminoBase.h"

// FORWARD DECLARATIONS
class CPeriodic;


// CLASS DECLARATION
/**
* CRemovableTetrimino class
*
* @since S60 3.1
*/
NONSHARABLE_CLASS( CRemovableTetrimino ) : public CTetriminoBase
    {
public:// New Functions
    static CRemovableTetrimino* NewL( CMatrix& aMatrix, MTetriminoObserver& aObserver );
    ~CRemovableTetrimino();
    
    virtual void CloneL( const CTetriminoBase& aTetrimino );
    // Rotate( by 90 degrees clockwise )
    virtual void RotateL();
    void LeftL();
    void RightL();
    void DescendL();
    void FastDescendL( TBool aState );
    void DoubleDescendL();
    void StartL( TInt aLevel );
    void Stop();
    void Resume();
    void UpdateAllL();

public: // from base class CTetriminoBase
    virtual void Reset();

protected: // Constructor
    // C++ Constructor
    CRemovableTetrimino( CMatrix& aMatrix, MTetriminoObserver& aObserver );
    // Completes construction
    virtual void ConstructL();

protected: // New functions
    void SetLevel( TInt aLevel );
    void DoStartL();
    TBool HitTest( TInt aX, TInt aY );
    TBool EdgeTest( TInt aX, TInt aY );

    // Callback for drop-down updates.
    static TInt TimerCallbackL( TAny* aPtr );

protected: // Members data
    // Level
    TInt iLevel;
    TBool iFastState;
    // The timer for put-down updates
    CPeriodic* iPeriodic;

    // Drop speed
    TTimeIntervalMicroSeconds32 iDescendSpeed;
    };

#endif // C_REMOVABLETETRIMINO_H

// End of File
