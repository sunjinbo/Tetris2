/* ====================================================================
 * File: ModelObserver.h
 * Created: 01/16/09
 * Modified: 03/31/09
 * Author: Sun Jinbo
 * Copyright (c): Tieto, All rights reserved
 * ==================================================================== */

#ifndef M_MODELOBSERVER_H
#define M_MODELOBSERVER_H


// CLASS DECLARATION
/**
* MModelObserver class
*
* @since S60 3.1
*/
class MModelObserver
    {
public:
    // Called when model state changes and observer needs to be notified.
    virtual void MenuItemSelectedL( TInt aMenuItem ) = 0;
    virtual void GameOverNotifiedL() = 0;
    };

#endif // M_MODELOBSERVER_H

// End of File
