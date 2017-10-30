/* ====================================================================
 * File: AnimationTetrimino.h
 * Created: 01/11/09
 * Author: Sun Jinbo
 * Copyright (c): Tieto, All rights reserved
 * ==================================================================== */

#ifndef C_ANIMATIONTETRIMINO_H
#define C_ANIMATIONTETRIMINO_H

// INCLUDES
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
NONSHARABLE_CLASS( CAnimationTetrimino ) : public CTetriminoBase
    {
public:// Constructor and destructor
    static CAnimationTetrimino* NewL( CMatrix& aMatrix, MTetriminoObserver& aObserver );
    virtual ~CAnimationTetrimino();

public: // new function
    void PlayL( CTetriminoBase& aTetrimino );
    void Stop();
    void Resume();
    TBool Animation() const;

public: // from base class CTetriminoBase
    virtual void Reset();
    void UpdateAllL();

protected: // Constructor and new functions
    // C++ Constructor
    CAnimationTetrimino( CMatrix& aMatrix, MTetriminoObserver& aObserver ); 
    void ConstructL();

    // Handles an active object's request completion event.
    void RunL();
    TInt Find( TInt aX, TInt aY  );

    // Callback for put-down updates.
    static TInt TimerCallbackL( TAny* aPtr );

private: // Members
    enum TAnimationStatus
    	{
        EAnimationNone = 0,
        EAnimationAscend,
        EAnimationDescend
    	};
    // Current Status 
    TAnimationStatus iStatus;

    // Current Line
    TInt iLine;

    // Periodic timer active object, owned
    CPeriodic* iPeriodic;

    };

#endif // C_ANIMATIONTETRIMINO_H

// End of File
