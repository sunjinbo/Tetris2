/* ====================================================================
 * File: MatrixObserver.h
 * Created: 06/14/10
 * Author: Sun Jinbo
 * Copyright (c): Tieto, All rights reserved
 * ==================================================================== */

#ifndef C_MATRIXOBSERVER_H
#define C_MATRIXOBSERVER_H

#include <e32def.h>
#include <e32base.h>

// Enumeration for specifying, which matrix state chaged.
enum TMatrixEvent // for reporting to observer
    {
    EMatrixRotate = 0,
    EMatrixMoveLeft,
    EMatrixMoveRight,
    EMatrixDescend,
    EMatrixFastDescendOn,
    EMatrixFastDescendOff,
    EMatrixDoulbeDescend
    };

// CLASS DECLARATION
/**
* MMatrixObserver class
*
* @since S60 3.1
*/
class MMatrixObserver
    {
public:
    virtual void NotifyL( TMatrixEvent aEvent ) = 0;
    };

#endif // C_MATRIXOBSERVER_H

// End of file
