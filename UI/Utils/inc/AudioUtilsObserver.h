/* ====================================================================
 * File: AudioUtilsObserver.h
 * Created: 01/18/09
 * Modified: 03/15/09
 * Author: Sun Jinbo
 * Copyright (c): Tieto, All rights reserved
 * ==================================================================== */

#ifndef M_AUDIOUTILSOBSERVER_H
#define M_AUDIOUTILSOBSERVER_H

// INCLUDE FILES

// FORWARD DECLARATIONS
class CAudioUtils;

// CLASS DECLARATION

/**
* MAudioInitObserver class
*/
class MAudioInitObserver
    {
public: // Interface function
    virtual void AudioInitComplete( CAudioUtils* aUtils, TInt aError ) = 0;
    };

/**
* MAudioPlayObserver class
*/
class MAudioPlayObserver
    {
public: // Interface function
    virtual void AudioPlayComplete( TInt aError ) = 0;
    };

#endif // M_AUDIOUTILSOBSERVER_H

// End of File
