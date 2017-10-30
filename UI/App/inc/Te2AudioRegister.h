/* ====================================================================
 * File: Te2AudioRegister.h
 * Created: 06/17/10
 * Author: Sun Jinbo
 * Copyright (c): Tieto, All rights reserved
 * ==================================================================== */

#ifndef C_TE2AUDIOTHREADREGISTER_H
#define C_TE2AUDIOTHREADREGISTER_H

// INCLUDE FILES
#include <e32base.h>
#include <AudioUtilsObserver.h>
#include "Te2AudioDefines.h"

// FORWARD REFERENCES
class CEikonEnv;
class MAudioRegisterObserver;

// ENUMS
enum TRegisterState
    {
    ERegisterOpeningSuccess,
    ERegisterOpeningFailed,
    ERegisterAllClipSuccess,
    ERegisterAllClipFailed
    };

// CLASS DECLARATION

/**
* MAudioRegisterObserver class
*
* @since S60 3.1
*/
class MAudioRegisterObserver
    {
public: // callback method
    virtual void StateChangedL( TInt aNewState ) = 0;
    };

/**
* CTe2AudioRegister class
*
* @since S60 3.1
*/
class CTe2AudioRegister : public CBase, public MAudioInitObserver
    {
public: // Constructor and destructor
    static CTe2AudioRegister* NewL( 
            CEikonEnv& aCoeEnv, MAudioRegisterObserver& aObserver );
    virtual ~CTe2AudioRegister();

public: // New methods
    CAudioUtils* AudioUtils( TAudioSourceId aSourceId  );
    void Stop();

public: // from MAudioInitObserver
    void AudioInitComplete( CAudioUtils* aUtils, TInt aError );

private: // Symbian 2nd Constructors
    CTe2AudioRegister( CEikonEnv& aCoeEnv, MAudioRegisterObserver& aObserver );
    void ConstructL();

private: // new methods
    TInt GetPrivatePath( TFileName& aPrivatePath );
    TInt GetFileName( TFileName& aFileName, TAudioSourceId aId );
    TInt Rand( TInt aRange ) const;

private: // data
    CEikonEnv& iCoeEnv;
    MAudioRegisterObserver& iObserver;
    RPointerArray<CAudioUtils> iAudioUtilsList; // owned
    TInt iRegistionCount;
    };

#endif // C_TE2AUDIOTHREADREGISTER_H

// End of File
