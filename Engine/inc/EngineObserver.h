/* ====================================================================
 * File: EngineObserver.h
 * Created: 01/17/09
 * Modified: 01/19/09
 * Author: Sun Jinbo
 * Copyright (c): Tieto, All rights reserved
 * ==================================================================== */

#ifndef M_ENGINEOBSERVER_H
#define M_ENGINEOBSERVER_H

// INCLUDE FILES
#include <e32cmn.h>
#include "EngineAction.h"

// Enumeration for specifying
enum TEngineState // for reporting to observer
    {
    EPrimaryMatirxUpdated = 0,
    ESecondaryMatrixUpdated,
    EScorekeeperUpdated,
    EAnimationStarted,
    EAnimationEnded,
    EEngGameOver
    };

// CLASS DECLARATION
/**
* MEngineObserver class
*
* @since S60 3.1
*/
class MEngineObserver
    {
public:
    // Called when game engine state changes and observer needs to be notified.
    virtual void StateChangedL( TEngineState aNewState, TEngineAction aAction ) = 0;
    };

#endif // M_ENGINEOBSERVER_H

// End of File
