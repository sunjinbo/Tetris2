/* ====================================================================
 * File: Te2AudioScheduler.cpp
 * Created: 04/19/09
 * Modified: 04/19/09
 * Author: Sun Jinbo
 * Copyright (c): Tieto, All rights reserved
 * ==================================================================== */

// INCLUDE FILES
#include "Te2AudioScheduler.h"
#include "Logger.h"

// CONSTANTS
const TInt KAudioTaskListCranularity = 3;
const TInt KAudioTaskListHeadIndex = 0;

// ======== MEMBER FUNCTIONS ========

// ----------------------------------------------------------------------------
// CTe2AudioScheduler::NewL
// ----------------------------------------------------------------------------
//
CTe2AudioScheduler* CTe2AudioScheduler::NewL()
    {
    CTe2AudioScheduler* self = new (ELeave) CTe2AudioScheduler;
    CleanupStack::PushL (self );
    self->ConstructL ();
    CleanupStack::Pop();
    return self;
    }

// ----------------------------------------------------------------------------
// CTe2AudioScheduler::~CTe2AudioScheduler
// ----------------------------------------------------------------------------
//
CTe2AudioScheduler::~CTe2AudioScheduler()
    {
    iTaskList.ResetAndDestroy();
    iTaskList.Close();
    }

// ----------------------------------------------------------------------------
// CTe2AudioScheduler::TaskCompleted
// ----------------------------------------------------------------------------
//
void CTe2AudioScheduler::TaskCompleted()
    {
    CTe2AudioTask* task( iTaskList[ KAudioTaskListHeadIndex ] );
    delete task;
    task = NULL;

    iTaskList.Remove( KAudioTaskListHeadIndex );
    iTaskList.Compress();

    if ( iTaskList.Count() )
        {
        iTaskList[ KAudioTaskListHeadIndex ]->Start();
        }
    else
        {
        iRunning = EFalse;
        }
    }

// ----------------------------------------------------------------------------
// CTe2AudioScheduler::AddTask
// ----------------------------------------------------------------------------
//
void CTe2AudioScheduler::AddTask( CTe2AudioTask* aTask )
    {
    iTaskList.Append( aTask );
    
    Sort(); // sort task list, and put high priority task to the head of list.

    if ( ! iRunning )
        {
        iRunning = ETrue;
        iTaskList[ KAudioTaskListHeadIndex ]->Start();
        }
    }

// ----------------------------------------------------------------------------
// CTe2AudioScheduler::Clear
// ----------------------------------------------------------------------------
//
void CTe2AudioScheduler::Clear()
    {
    iTaskList.ResetAndDestroy();
    }

// ----------------------------------------------------------------------------
// CTe2AudioScheduler::CTe2AudioScheduler
// ----------------------------------------------------------------------------
//
CTe2AudioScheduler::CTe2AudioScheduler()
    : iTaskList( KAudioTaskListCranularity )
    { 
    // No implementation required.
    }

// ---------------------------------------------------------------------------
// CTe2AudioScheduler::ConstructL
// ---------------------------------------------------------------------------
//
void CTe2AudioScheduler::ConstructL()
    {
    // no implementation required
    }

// ---------------------------------------------------------------------------
// CTe2AudioScheduler::Sort()
// ---------------------------------------------------------------------------
//
void CTe2AudioScheduler::Sort()
    {
    TLinearOrder<CTe2AudioTask> order( CTe2AudioTask::Compare );
    iTaskList.Sort( order );
    }

// End of File
