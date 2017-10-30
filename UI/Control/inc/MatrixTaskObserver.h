/* ====================================================================
 * File: MatrixTaskObserver.h
 * Created: 04/19/09
 * Modified: 04/19/09
 * Author: Sun Jinbo
 * Copyright (c): Tieto, All rights reserved
 * ==================================================================== */

#ifndef M_MATRIXTASKOBSERVER_H
#define M_MATRIXTASKOBSERVER_H


// CLASS DECLARATION
/**
* MMatrixTaskObserver class
*
* @since S60 3.1
*/
class MMatrixTaskObserver
    {
public:
    // Called when matrix task completes and observer needs to be notified.
    virtual void TaskCompleted() = 0;
    };

#endif // M_MATRIXTASKOBSERVER_H

// End of File
