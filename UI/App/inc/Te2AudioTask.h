/* ====================================================================
 * File: Te2AudioTask.h
 * Created: 04/19/09
 * Modified: 04/19/09
 * Author: Sun Jinbo
 * Copyright (c): Tieto, All rights reserved
 * ==================================================================== */

#ifndef C_TE2AUDIOTASK_H
#define C_TE2AUDIOTASK_H

// INCLUDE FILES
#include <e32base.h>
#include <AudioUtilsObserver.h>
#include "Te2AudioDefines.h"

// FORWARD DECLARATIONS

// CLASS DECLARATION

/**
* MAudioTaskObserver class
*
* @since S60 3.1
*/
class MAudioTaskObserver
    {
public: // callback function
    virtual void TaskCompleted() = 0;
    };

/**
* CTe2AudioTask class
*
* @since S60 3.1
*/
class CTe2AudioTask : public CBase, public MAudioPlayObserver
    {
public: // Constructor and Destructor
    static CTe2AudioTask* NewL( MAudioTaskObserver& aObserver );
    virtual ~CTe2AudioTask();

public: // New functions
    void SetRequest( const TAudioRequest& aRequest );
    TAudioRequest AudioRequest() const;
    void Start();
    static TInt Compare( const CTe2AudioTask& aFirst, const CTe2AudioTask& aSecond );

public: // from MAudioPlayObserver
    void AudioPlayComplete( TInt aError );
    
private: // Constructor
    CTe2AudioTask( MAudioTaskObserver& aObserver );
    void ConstructL();

private: // data
    MAudioTaskObserver& iObserver;
    TAudioRequest iRequest;
    };

#endif // C_TE2AUDIOTASK_H

// End of File
