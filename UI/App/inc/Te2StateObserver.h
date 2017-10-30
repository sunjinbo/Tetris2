/* ====================================================================
 * File: Te2StateObserver.h
 * Created: 01/18/09
 * Modified: 03/15/09
 * Author: Sun Jinbo
 * Copyright (c): Tieto, All rights reserved
 * ==================================================================== */

#ifndef C_TE2STATEOBSERVER_H
#define C_TE2STATEOBSERVER_H

// INCLUDE FILES
#include <e32std.h>
#include "Tetris2.hrh"

// Enumeration for specifying, which is for reporting to observer
enum TTe2State // for the game state of Tetris
    {
    ETe2MenuState,
    ETe2PlayState,
    ETe2PauseState,
    ETe2StopState,
    ETe2HelpState,
    ETe2RankState
    };

// CLASS DECLARATION
/**
* MTe2StateObserver class
*
* @since S60 3.1
*/
class MTe2StateObserver
    {
public: // Interface function
    virtual void ChangeModeL( const TTe2Mode& aNewType ) = 0;
    virtual void ChangeAudioL( const TTe2Audio& aNewType ) = 0;
    virtual void ChangeStateL( const TTe2State& aNewState ) = 0;
    virtual void ChangeEngineL( TInt aCommand ) = 0;
    virtual void ChangeViewL( TInt aViewId ) = 0;
    virtual void ChangeMenubarL( TInt aMenuTitleResourceId ) = 0;
    virtual void CleanRankL() = 0;
    };

#endif // C_TE2STATEOBSERVER_H

// End of File
