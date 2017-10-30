/* ====================================================================
 * File: Te2AppUi.h
 * Created: 01/14/09
 * Modified: 03/14/09
 * Author: Sun Jinbo
 * Copyright (c): Tieto, All rights reserved
 * ==================================================================== */

#ifndef C_TE2APPUI_H
#define C_TE2APPUI_H

// INCLUDE FILES
#include <aknViewAppUi.h>
#include <ModelObserver.h>
#include <PropertyObserver.h>
#include "Te2StateObserver.h"
#include "Te2Settings.h"

// FORWARD DECLARATIONS
class CTe2MenuView;
class CTe2HelpView;
class CTe2GameView;
class CTe2RankView;
class CTe2State;
class CPropertySubscriber;
class CScreenSaverBlocker;

// CLASS DECLARATION
/**
* CTe2AppUi class
*
* @since S60 3.1
*/
class CTe2AppUi : public CAknViewAppUi,
public MTe2StateObserver, 
public MModelObserver, 
public MPropertyObserver
    {
public: // 2nd phase constructor.
    void ConstructL();
    virtual ~CTe2AppUi();

public: // from class MTe2StateObserver
    virtual void ChangeModeL( const TTe2Mode& aNewType );
    virtual void ChangeAudioL( const TTe2Audio& aNewType );
    virtual void ChangeStateL( const TTe2State& aNewState );
    virtual void ChangeEngineL( TInt aCommand );
    virtual void ChangeViewL( TInt aViewId );
    virtual void ChangeMenubarL( TInt aMenuTitleResourceId );
    virtual void CleanRankL();
    
private: // from class MModelObserver
    virtual void MenuItemSelectedL( TInt aMenuItem );
    virtual void GameOverNotifiedL();

private: // from class MPropertyObserver
    void HandlePropertyChangedL( 
                const TUid& aCategory, 
                TInt aKey );

private: // from class CAknAppUi
    void HandleResourceChangeL( TInt aType );
    void HandleCommandL( TInt aCommand );
    void HandleWsEventL( const TWsEvent& aEvent,
        CCoeControl* aDestination );
    TKeyResponse HandleKeyEventL( const TKeyEvent& /*aKeyEvent*/,
        TEventCode /*aType*/ );

private: // new methods
    void StartAudioThreadL();
    static TInt AudioThreadEntryPoint( TAny* aParam );
    void RunAudioThreadL();
    void StoreL();

private: // Member data
    CTe2MenuView* iMenuView; // owned
    CTe2HelpView* iHelpView; // owned
    CTe2GameView* iGameView; // owned
    CTe2RankView* iRankView; // owned
    CTe2State* iState; // owned
    CPropertySubscriber* iProperty; // owned
    CScreenSaverBlocker* iBlocker; // owned
    TTe2Settings iSettings;
    TInt iCurrentView;
    };

#endif // C_TE2APPUI_H

// End of File
