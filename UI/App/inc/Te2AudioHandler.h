/* ====================================================================
 * File: Te2AudioHandler.h
 * Created: 06/17/10
 * Author: Sun Jinbo
 * Copyright (c): Tieto, All rights reserved
 * ==================================================================== */

#ifndef C_TE2AUDIOTHREADHANDLER_H
#define C_TE2AUDIOTHREADHANDLER_H

// INCLUDE FILES
#include <e32base.h>
#include <eikenv.h>
#include <PropertyListenerCallback.h>
#include <RegistrationTimeout.h>
#include "Te2AudioRegister.h"
#include "Te2AudioDefines.h"

// FORWARD REFERENCES
class CPropertyListener;
class CTe2AudioRegister;
class CTe2AudioScheduler;

// CLASS DECLARATION

/**
* CTe2AudioHandler class
*
* @since S60 3.1
*/
class CTe2AudioHandler : public CBase,
                        public MPropertyListenerCallBack,
                        public MTimeoutCallback,
                        public MAudioRegisterObserver
    {
public: // Constructor and destructor
    static CTe2AudioHandler* NewL( CEikonEnv& aCoeEnv );
    static CTe2AudioHandler* NewLC( CEikonEnv& aCoeEnv );
    virtual ~CTe2AudioHandler();

public: // New methods
    void StartL();

private: // from MPropertyListenerCallBack
    void HandlePropertyChangedL();

private: // from MTimeoutCallback
    void HandleTimeoutL();

private: // from MAudioRegisterObserver
    void StateChangedL( TInt aNewState );

private: // Symbian 2nd Constructors
    CTe2AudioHandler( CEikonEnv& aCoeEnv );
    void ConstructL();

private: // New methods
    void StartTimeout();

private: // data
    CEikonEnv& iCoeEnv;
    CPropertyListener* iListener; // owned
    CRegistrationTimeout* iTimeout; // owned
    CTe2AudioRegister* iRegister; // owned
    CTe2AudioScheduler* iScheduler; // owned
    };

#endif // C_TE2AUDIOTHREADHANDLER_H

// End of File
