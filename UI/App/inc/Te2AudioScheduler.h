/* ====================================================================
 * File: Te2AudioScheduler.h
 * Created: 04/19/09
 * Modified: 04/19/09
 * Author: Sun Jinbo
 * Copyright (c): Tieto, All rights reserved
 * ==================================================================== */

#ifndef C_TE2AUDIOSCHEDULER_H
#define C_TE2AUDIOSCHEDULER_H

// INCLUDE FILES
#include <e32std.h>
#include <e32base.h>
#include "Te2AudioTask.h"

// FORWARD DECLARATIONS
class CTe2AudioTask;

// CLASS DECLARATION
/**
* CTe2AudioScheduler class
*
* @since S60 3.1
*/
class CTe2AudioScheduler : public CBase, public MAudioTaskObserver
    {
public: // Constructor and Destructor
    static CTe2AudioScheduler* NewL();
    virtual ~CTe2AudioScheduler();

public: // from base class MMatrixTaskObserver
    virtual void TaskCompleted();

public: // New functions
    void AddTask( CTe2AudioTask* aTask );
    void Clear();

private: // Constructor
    CTe2AudioScheduler();
    void ConstructL();

private: // New functions
    void Sort();

private: // Data members
    RPointerArray<CTe2AudioTask> iTaskList;
    TBool iRunning;
    };

#endif // C_TE2AUDIOSCHEDULER_H

// End of File
