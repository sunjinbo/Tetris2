/* ====================================================================
 * File: ScorekeeperObserver.h
 * Created: 01/17/09
 * Author: Sun Jinbo
 * Copyright (c): Tieto, All rights reserved
 * ==================================================================== */

#ifndef C_SCOREKEEPEROBSERVER_H
#define C_SCOREKEEPEROBSERVER_H

#include <e32def.h>
#include <e32base.h>

// Enumeration for specifying, which tetrimino state chaged.
enum TScorekeeperState // for reporting to observer
    {
    EScorekeeperLevelUpdated = 0,
    EScorekeeperLineUpdated,
    EScorekeeperScoreUpdated,
    EScorekeeperLifeUpdated,
    EScorekeeperLifeEnded
    };

// CLASS DECLARATION
/**
* MScorekeeperObserver class
*
* @since S60 3.1
*/
class MScorekeeperObserver
    {
public:
    // Called when tetrimino state changes and observer needs to be notified.
    virtual void StateChangedL( TScorekeeperState aNewState ) = 0;
    };

#endif // C_SCOREKEEPEROBSERVER_H

// End of file
