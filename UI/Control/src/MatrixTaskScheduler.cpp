/* ====================================================================
 * File: MatrixTaskScheduler.cpp
 * Created: 04/19/09
 * Modified: 04/19/09
 * Author: Sun Jinbo
 * Copyright (c): Tieto, All rights reserved
 * ==================================================================== */

// INCLUDE FILES
#include "MatrixTaskScheduler.h"
#include "MatrixTask.h"
#include "Logger.h"

// CONSTANTS
const TInt KMatrixTaskListCranularity = 5;
const TInt KMatrixTaskListHeadIndex = 0;

// ======== MEMBER FUNCTIONS ========

// ----------------------------------------------------------------------------
// CMatrixTaskScheduler::NewL
// ----------------------------------------------------------------------------
//
CMatrixTaskScheduler* CMatrixTaskScheduler::NewL()
    {
    CMatrixTaskScheduler* self = new (ELeave) CMatrixTaskScheduler;
    CleanupStack::PushL (self );
    self->ConstructL ();
    CleanupStack::Pop();
    return self;
    }

// ----------------------------------------------------------------------------
// CMatrixTaskScheduler::~CMatrixTaskScheduler
// ----------------------------------------------------------------------------
//
CMatrixTaskScheduler::~CMatrixTaskScheduler()
    {
    iTaskList.ResetAndDestroy();
    iTaskList.Close();
    }

// ----------------------------------------------------------------------------
// CMatrixTaskScheduler::TaskCompleted
// ----------------------------------------------------------------------------
//
void CMatrixTaskScheduler::TaskCompleted()
    {
    CMatrixTask* task( iTaskList[ KMatrixTaskListHeadIndex ] );
    delete task;
    task = NULL;
    iTaskList.Remove( KMatrixTaskListHeadIndex );
    iTaskList.Compress();
    iRunning = EFalse;
    if ( iTaskList.Count() )
        {
        iTaskList[ KMatrixTaskListHeadIndex ]->Start();
        }
    }

// ----------------------------------------------------------------------------
// CMatrixTaskScheduler::AddTask
// ----------------------------------------------------------------------------
//
void CMatrixTaskScheduler::AddTask( CMatrixTask* aTask )
    {
    iTaskList.Append( aTask );
    if ( ! iRunning )
        {
        iRunning = ETrue;
        iTaskList[ KMatrixTaskListHeadIndex ]->Start();
        }
    }

// ----------------------------------------------------------------------------
// CMatrixTaskScheduler::Clear
// ----------------------------------------------------------------------------
//
void CMatrixTaskScheduler::Clear()
    {
    iTaskList.ResetAndDestroy();
    }

// ----------------------------------------------------------------------------
// CMatrixTaskScheduler::CMatrixTaskScheduler
// ----------------------------------------------------------------------------
//
CMatrixTaskScheduler::CMatrixTaskScheduler()
    : iTaskList( KMatrixTaskListCranularity ),  iRunning( EFalse )
    { // No implementation required.
    }

// ----------------------------------------------------------------------------
// CMatrixTaskScheduler::CMatrixTaskScheduler
// ----------------------------------------------------------------------------
//
void CMatrixTaskScheduler::ConstructL()
    { // No implementation required.
    }

// End of File
