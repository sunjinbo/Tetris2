/* ====================================================================
 * File: Te2State.cpp
 * Created: 01/17/09
 * Modified: 01/17/09
 * Author: Sun Jinbo
 * Copyright (c): Tieto, All rights reserved
 * ==================================================================== */

#ifndef C_TE2STATE_H
#define C_TE2STATE_H

// INCLUDE FILES
#include <e32base.h>
#include "Te2StateObserver.h"


// CLASS DECLARATION
/**
* CTe2State class
*
* @since S60 3.1
*/
class CTe2State : public CBase
    {
public:
    friend class MTeStateObserver;

public: // Constructors and destructor
    /**
    * Destructor.
    */
    virtual ~CTe2State();

public: // New funtions
    virtual void StartL();
    virtual void PauseL();
    virtual void ResumeL();
    virtual void StopL();
    virtual void RestartL();
    virtual void HelpL();
    virtual void BackL();
    virtual void AudioL();
    virtual void ModeL();
    virtual void RankL();
    virtual void QuitL();
    virtual void ClearL();

protected:
    /**
    * C++ default constructor.
    */
    CTe2State( MTe2StateObserver& aObserver, CTe2State* aState );

    /**
    * By default Symbian 2nd phase constructor is private.
    */
    virtual void ConstructL();

protected: // Member data
    MTe2StateObserver& iObserver;

    TTe2Mode iModeType;
    TTe2Audio iAudioType;
    };

/*******************************************************/
/*      SUB CLASS DECLARATIONS                         */
/*******************************************************/

////////////////////////////////////////////////////////
///     Declaration of CMenuState
////////////////////////////////////////////////////////
class CMenuState : public CTe2State
    {
public:
    static CTe2State* NewL( 
        MTe2StateObserver& aObserver, CTe2State* aState = NULL );
    virtual ~CMenuState();
public: // from base class CTe2State
    void StartL();
    void ModeL();
    void AudioL();
    void RankL();
    void HelpL();
    void QuitL();
protected:
    CMenuState( MTe2StateObserver& aObserver, CTe2State* aState = NULL  );
    void ConstructL();
    };

////////////////////////////////////////////////////////
///     Declaration of CPlayState
////////////////////////////////////////////////////////
class CPlayState : public CTe2State
    {
public:
    static CTe2State* NewL( 
        MTe2StateObserver& aObserver, CTe2State* aState = NULL );
    virtual ~CPlayState();
public: // from base class CTe2State
    void PauseL();
    void StopL();
protected:
    CPlayState( MTe2StateObserver& aObserver, CTe2State* aState = NULL );
    };

////////////////////////////////////////////////////////
///     Declaration of CPauseState
////////////////////////////////////////////////////////
class CPauseState : public CTe2State
    {
public:
    static CTe2State* NewL( 
        MTe2StateObserver& aObserver, CTe2State* aState = NULL );
    virtual ~CPauseState();
public: // from base class CTe2State
    void ResumeL();
    void StopL();
protected:
    CPauseState( MTe2StateObserver& aObserver, CTe2State* aState = NULL );
    };

////////////////////////////////////////////////////////
///     Declaration of CStopState
////////////////////////////////////////////////////////
class CStopState : public CTe2State
    {
public:
    static CTe2State* NewL( 
        MTe2StateObserver& aObserver, CTe2State* aState = NULL );
    virtual ~CStopState();
public: // from base class CTe2State
    void RestartL();
    void BackL();
protected:
    CStopState( MTe2StateObserver& aObserver, CTe2State* aState = NULL );
    void ConstructL();
    };

////////////////////////////////////////////////////////
///     Declaration of CHelpState
////////////////////////////////////////////////////////
class CHelpState : public CTe2State
    {
public:
    static CTe2State* NewL( 
        MTe2StateObserver& aObserver, CTe2State* aState = NULL );
    virtual ~CHelpState();
public: // from base class CTe2State
    void BackL();
protected:
    CHelpState( MTe2StateObserver& aObserver, CTe2State* aState = NULL );
    };

////////////////////////////////////////////////////////
///     Declaration of CRankState
////////////////////////////////////////////////////////
class CRankState : public CTe2State
    {
public:
    static CTe2State* NewL( 
        MTe2StateObserver& aObserver, CTe2State* aState = NULL );
    virtual ~CRankState();
public: // from base class CTe2State
    void BackL();
    void ClearL();
protected:
    CRankState( MTe2StateObserver& aObserver, CTe2State* aState = NULL );
    };

#endif // C_TE2STATE_H

// End of File
