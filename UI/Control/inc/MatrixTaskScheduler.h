/* ====================================================================
 * File: MatrixTaskScheduler.h
 * Created: 04/19/09
 * Modified: 04/19/09
 * Author: Sun Jinbo
 * Copyright (c): Tieto, All rights reserved
 * ==================================================================== */

#ifndef C_MATRIXTASKSCHEDULER_H
#define C_MATRIXTASKSCHEDULER_H

// INCLUDE FILES
#include <e32std.h>
#include <e32base.h>
#include "MatrixTaskObserver.h"

// FORWARD DECLARATIONS
class CMatrixTask;

// CLASS DECLARATION
/**
* CMatrixTaskScheduler class
*
* @since S60 3.1
*/
class CMatrixTaskScheduler : public CBase, public MMatrixTaskObserver
    {
public: // Constructor and Destructor
    static CMatrixTaskScheduler* NewL();
    virtual ~CMatrixTaskScheduler();

public: // from base class MMatrixTaskObserver
    virtual void TaskCompleted();

public: // New functions
    void AddTask( CMatrixTask* aTask );
    void Clear();
    
protected: // Constructor
    CMatrixTaskScheduler();
    void ConstructL();

protected: // Data members
    RPointerArray<CMatrixTask> iTaskList;
    TBool iRunning;
    };

#endif // C_MATRIXTASKSCHEDULER_H

// End of File
