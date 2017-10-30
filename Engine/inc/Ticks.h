/* ====================================================================
 * File: Ticks.h
 * Created: 09/17/10
 * Modifed: 09/17/10
 * Author: Sun Jinbo
 * Copyright (c): Tieto, All rights reserved
 * ==================================================================== */

#ifndef C_TICKS_H
#define C_TICKS_H

// INCLUDE
#include <e32base.h>

// FORWARD DECLARATION

// CONSTANTS

// CLASS DECLARATION

/**
 *  MTicksCallback class
 */
class MTicksCallback
    {
public: // callback function
    virtual void TicksCompletedL() = 0;
    };

/**
 *  CTicks class
 */
class CTicks : public CActive
    {
public: // Symbian 2nd constructor and destructor
    static CTicks* NewL( MTicksCallback& aCallback );
    virtual ~CTicks();

public: // New methods
    void Start( TInt aSeconds );
    void Pause();
    void Resume();
    void Stop();

public: // from CActive
    void RunL();
    void DoCancel();

private: // Default C++ constructor and 2nd constructor
    CTicks( MTicksCallback& aCallback );
    void ConstructL();

private: // Data
    MTicksCallback& iCallback;
    RTimer iTimer;
    TUint iTickCount;
    TUint iInitTickCount;
    };

#endif // C_TICKS_H

// End of File
