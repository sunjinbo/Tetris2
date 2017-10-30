/* ====================================================================
 * File: GameContainer.h
 * Created: 03/15/09
 * Modified: 04/14/09
 * Author: Sun Jinbo
 * Copyright (c): Tieto, All rights reserved
 * ==================================================================== */

#ifndef C_GAMECONTAINER_H
#define C_GAMECONTAINER_H

// INCLUDE FILES
#include <EngineObserver.h>
#include <RegistrationTimeout.h>

#ifdef RD_TE2_VIBRA_FUNC
#include <hwrmvibra.h>
#endif // RD_TE2_VIBRA_FUNC
#include "BaseContainer.h"
#include "MatrixObserver.h"

// FORWARD DECLARATIONS
class CMatrixControl;
class CScorekeeperControl;
class CTe2Engine;

// Enums
enum TModeType
    {
    EModeCommon,
    EModeExtra
    };

// CLASS DECLARATION

/**
* CGameContainer class
*
* @since S60 3.1
*/
class CGameContainer : public CBaseContainer, public MEngineObserver
  ,public MTimeoutCallback ,public MMatrixObserver
	{
public: // Constructor and Destructor
    IMPORT_C static CGameContainer* NewL( const TRect& aRect, MModelObserver& aObserver );
    IMPORT_C virtual ~CGameContainer();

public: // from base class CBaseControl
    void Draw( const TRect& aRect ) const; 
    void SizeChanged();
    void HandleResourceChange( TInt aType );
    TKeyResponse OfferKeyEventL( const TKeyEvent& aKeyEvent, TEventCode aType );
    void HandlePointerEventL(const TPointerEvent& aPointerEvent);

public: // from base class MEngineObserver
    void StateChangedL( TEngineState aNewState, TEngineAction aAction );

public: // from base class MTimeoutCallback
    void HandleTimeoutL();

public: // from base class MMatrixObserver
    void NotifyL( TMatrixEvent aEvent );

public: // New functions
    IMPORT_C void StartL( TModeType aType );
    IMPORT_C void Stop();
    IMPORT_C void Pause();
    IMPORT_C void Resume();
    IMPORT_C void Restart();
    IMPORT_C void SetAudio( TBool aAudio );
    void Draw();

private: // Constructor
    CGameContainer( MModelObserver& aObserver );
    void ConstructL( const TRect& aRect );

private: // New functions
    void UpdateScorekeeperL( TEngineAction aAction );
    void HandleOrientationChanged();
    TBool TouchTriggered() const;
#ifdef RD_TE2_VIBRA_FUNC
    void VibraL();
#endif // RD_TE2_VIBRA_FUNC
    void HandleCommandL( TInt aCommand );
    TKeyResponse HandleSideslipKeyEventL( 
        const TKeyEvent& aKeyEvent, TEventCode aType );

protected: // Data members
    CTe2Engine* iGameEngine; // own
    CMatrixControl* iPrimaryMatrix; // own
    CMatrixControl* iSecondaryMatrix; // own
    CScorekeeperControl* iScorekeeper; // own
    CRegistrationTimeout* iTimeout; // own
    HBufC* iGameOverText; // own
#ifdef RD_TE2_VIBRA_FUNC
    CHWRMVibra* iVibra; // own
#endif // RD_TE2_VIBRA_FUNC
    TBool iPrevKeyLongPressed;
    TBool iAudio;
    TInt iLine;
    TInt iScore;
    TInt iLife;
    TInt iResOffset;
    TInt iAlpha;
    enum TGameState
    	{
    	EPlaying,
    	EPause,
    	EAnimation,
    	EGameOver
    	};
    TGameState iState;
    };

#endif // C_GAMECONTAINER_H

// End of File
