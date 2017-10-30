/* ====================================================================
 * File: TetriminoObserver.h
 * Created: 01/17/09
 * Author: Sun Jinbo
 * Copyright (c): Tieto, All rights reserved
 * ==================================================================== */

#ifndef M_TETRIMINOOBSERVER_H
#define M_TETRIMINOOBSERVER_H

// Enumeration for specifying, which tetrimino state chaged.
enum TTetriminoState // for reporting to observer
    {
    EFixedTetriminoSingleRemoved = 1,
    EFixedTetriminoDoubleRemoved = 2,
    EFixedTetriminoTripleRemoved = 3,
    EFixedTetriminoTetrisRemoved = 4,
    EFixedTetriminoUpdated,
    EFixedTetriminoAllUpdated,
    EFixedTetriminoMerged,
    EFixedTetriminoEnded,
    ENextRemovableTetriminoRemoved,
    ENextRemovableTetriminoCreated,
    ENextRemovableTetriminoAllUpdated,
    ERemovableTetriminoLeft,
    ERemovableTetriminoRight,
    ERemovableTetriminoRotated,
    ERemovableTetriminoDescend,
    ERemovableTetriminoFastDescend,
    ERemovableTetriminoUpdated,
    ERemovableTetriminoAllUpdated,
    ERemovableTetriminoDescendEnd,
    ERemovableTetriminoFastDescendEnd,
    EAnimationTetriminoStarted,
    EAnimationTetriminoUpdated,
    EAnimationTetriminoAllUpdated,
    EAnimationTetriminoEnded
    };

// CLASS DECLARATION
/**
* MTetriminoObserver class
*
* @since S60 3.1
*/
class MTetriminoObserver
    {
public:
    // Called when tetrimino state changes and observer needs to be notified.
    virtual void StateChangedL( TTetriminoState aNewState ) = 0;
    };

#endif // M_TETRIMINOOBSERVER_H

// End of File
