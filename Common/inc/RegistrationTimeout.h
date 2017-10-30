/* ====================================================================
 * File: RegistrationTimeout.h
 * Created: 07/06/10
 * Modified: 07/06/10
 * Author: Sun Jinbo
 * Copyright (c): Tieto, All rights reserved
 * ==================================================================== */

#ifndef C_REGISTRATIONTIMEOUT_H
#define C_REGISTRATIONTIMEOUT_H

// INCLUDE FILES
#include <e32base.h>

// FORWARD DECLARATIONS

// CLASS DECLARATION

/**
 *  MTimeoutCallback
 */
class MTimeoutCallback
	{
public:
	virtual void HandleTimeoutL() = 0;
	};

/**
 *  CRegistrationTimeout
 */
class CRegistrationTimeout : public CTimer
	{
public:
	IMPORT_C static CRegistrationTimeout* NewL( MTimeoutCallback& aCallback );
	IMPORT_C virtual ~CRegistrationTimeout();

    /**
     * By default the timeout is 0
     * but in the function body it will be adapted to 
     * a preset positive value KTimeout.
     */
	IMPORT_C void Start( TInt aTimeoutInMicroSeconds = 0 );
    
    IMPORT_C void Stop();

private:
    CRegistrationTimeout( MTimeoutCallback& aCallback );

private: // from CTimer
    void RunL();
    TInt RunError( TInt /*aError*/ );

private:
    MTimeoutCallback& iCallback;
    };

#endif // C_REGISTRATIONTIMEOUT_H
