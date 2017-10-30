/* ====================================================================
 * File: Te2AppUi.cpp
 * Created: 01/14/09
 * Modified: 01/14/09
 * Author: Sun Jinbo
 * Copyright (c): Tieto, All rights reserved
 * ==================================================================== */

// INCLUDE FILES
#include <avkon.hrh>
#include <eikenv.h>
#include <aknnotewrappers.h> 
#include <Tetris2.rsg>
#include <e32def.h>
#include <Logger.h>
#include <RepositorySubscriber.h>
#include <PropertySubscriber.h>
#include <ScreensaverBlocker.h>
#include <LayoutManager.h>
#include "Te2AppUi.h"
#include "Te2MenuView.h"
#include "Te2HelpView.h"
#include "Te2GameView.h"
#include "Te2RankView.h"
#include "Te2State.h"
#include "Tetris2.hrh"
#include "Tetris2ViewIds.h"
#include "Te2PSKeys.h"
#include "Te2AudioHandler.h"

// CONSTANTS

// ======== MEMBER FUNCTIONS ========
// ---------------------------------------------------------------------------
// CTe2AppUi::~CTe2AppUi
// ---------------------------------------------------------------------------
// 
CTe2AppUi::~CTe2AppUi()
    {
    delete iState;
    delete iProperty;
    delete iBlocker;
    }

// ---------------------------------------------------------------------------
// CTe2AppUi::ConstructL
// ---------------------------------------------------------------------------
// 
void CTe2AppUi::ConstructL()
    {
    _LOG_WRITE( "CTe2AppUi::ConstructL" );
    BaseConstructL();

    if ( EQhdPortrait == CTe2LayoutManager::Resolution() )
        {
        SetOrientationL( CAknAppUiBase::EAppUiOrientationPortrait );
        }

    RProperty property;
    TInt err = KErrNone;
    err = property.Define(
            KPSUidTetris2, 
            KTetris2AudioCommand, 
            RProperty::EInt );
    if( err == KErrAlreadyExists || err == KErrNone )
        {
        User::LeaveIfError( property.Set( 
                KPSUidTetris2, 
                KTetris2AudioCommand, 
                EAudioNotSet ) );
        }

    err = property.Define(
            KPSUidTetris2, 
            KTetris2AudioFeedback, 
            RProperty::EInt );

    if( err == KErrAlreadyExists || err == KErrNone )
        {
        User::LeaveIfError( property.Set( 
                KPSUidTetris2, 
                KTetris2AudioFeedback, 
                EUninitialized ) );
        }

    iProperty = CPropertySubscriber::NewL( this, 
                KPSUidTetris2, 
                KTetris2AudioFeedback );
    iProperty->Subscribe();
 
    StartAudioThreadL();

    TRect rect( ClientRect() );

    iMenuView = CTe2MenuView::NewL( this );
    iHelpView = CTe2HelpView::NewL( this );
    iGameView = CTe2GameView::NewL( this );
    iRankView = CTe2RankView::NewL( this );

    // Transfer ownership to base class
    AddViewL( iMenuView );
    AddViewL( iHelpView );
    AddViewL( iGameView );
    AddViewL( iRankView );

    iSettings.InternalizedL();
    iMenuView->SetMode( iSettings.iMode );
    iMenuView->SetAudio( iSettings.iAudio );
    iGameView->SetMode( iSettings.iMode );

    switch ( iSettings.iAudio )
        {
        case ETe2AudioOn:
            iGameView->SetAudio( ETrue );
            break;
        case ETe2AudioOff:
            iGameView->SetAudio( EFalse );
            break;
        default:
            break;
        }
    
    iCurrentView = KTe2MainViewIdValue;
    SetDefaultViewL( *iMenuView );
    iState = CMenuState::NewL( *this );
    
    iBlocker = CScreenSaverBlocker::NewL();
    iBlocker->DeactivateLightTimeoutAndScreenSaver();
    }

// ---------------------------------------------------------------------------
// CTe2AppUi::ChangeModeL
// ---------------------------------------------------------------------------
//
void CTe2AppUi::ChangeModeL( const TTe2Mode& aNewType )
    {
    iSettings.iMode = aNewType;
    iSettings.ExternalizedL();

    iMenuView->SetMode( aNewType );
    iGameView->SetMode( aNewType );
    }

// ---------------------------------------------------------------------------
// CTe2AppUi::ChangeAudioL
// ---------------------------------------------------------------------------
// 
void CTe2AppUi::ChangeAudioL( const TTe2Audio& aNewType )
    {
    iSettings.iAudio = aNewType;
    iSettings.ExternalizedL();

    iMenuView->SetAudio( aNewType );

    switch ( aNewType )
        {
        case ETe2AudioOn:
            User::LeaveIfError( RProperty::Set( 
                    KPSUidTetris2, 
                    KTetris2AudioCommand,
                    EAudioOn ) );
            iGameView->SetAudio( ETrue );
            break;
        case ETe2AudioOff:
            User::LeaveIfError( RProperty::Set( 
                    KPSUidTetris2, 
                    KTetris2AudioCommand,
                    EAudioOff ) );
            iGameView->SetAudio( EFalse );
            break;
        default:
            break;
        }
    }

// ---------------------------------------------------------------------------
// CTe2AppUi::ChangeStateL
// ---------------------------------------------------------------------------
// 
void CTe2AppUi::ChangeStateL( const TTe2State& aNewState )
    {
    CTe2State* oldState( iState );
    switch ( aNewState )
        {
        case ETe2MenuState:
            {
            iState = CMenuState::NewL( *this, oldState );
            }
            break;
        case ETe2PlayState:
            {
            iState = CPlayState::NewL( *this, oldState );
            }
            break;
        case ETe2PauseState:
            {
            iState = CPauseState::NewL( *this, oldState );
            }
            break;
        case ETe2StopState:
            {
            iState = CStopState::NewL( *this, oldState );
            }
            break;
        case ETe2HelpState:
            {
            iState = CHelpState::NewL( *this, oldState );
            }
            break;
        case ETe2RankState:
            {
            iState = CRankState::NewL( *this, oldState );
            }
            break;
        default:
            break;
        }
    delete oldState;
    }

// ---------------------------------------------------------------------------
// CTe2AppUi::ChangeEngineL
// ---------------------------------------------------------------------------
// 
void CTe2AppUi::ChangeEngineL( TInt aCommand )
    {
    switch ( aCommand )
        {
        case ETetris2CmdStart:
        case ETetris2CmdRestart:
        case ETetris2CmdStop:
        case ETetris2CmdPause:
        case ETetris2CmdResume:
            {
            iGameView->HandleCommandL( aCommand );
            }
            break;
        default:
            break;
        }
    }

// ---------------------------------------------------------------------------
// CTe2AppUi::ChangeViewL
// ---------------------------------------------------------------------------
// 
void CTe2AppUi::ChangeViewL( TInt aViewId )
    {
    TUid commandId( TUid::Uid(0) );
    StoreL(); // store background status of previous view
    iCurrentView = aViewId;
    switch ( aViewId )
        {
        case KTe2MainViewIdValue:
            {
            commandId  = KTe2MainViewId;
            }
            break;
        case KTe2HelpViewIdValue:
            {
            commandId  = KTe2HelpViewId;
            }
            break;
        case KTe2GameViewIdValue:
            {
            commandId = KTe2GameViewId;
            }
            break;
        case KTe2RankViewIdValue:
            {
            commandId = KTe2RankViewId;
            }
            break;
        default:
            break;
        }

    ActivateLocalViewL( commandId );
    }

// ---------------------------------------------------------------------------
// CTe2AppUi::ChangeMenubarL
// ---------------------------------------------------------------------------
// 
void CTe2AppUi::ChangeMenubarL( TInt aMenuTitleResourceId )
    {
    switch ( iCurrentView )
        {
        case KTe2MainViewIdValue:
            {
            iMenuView->ChangeMenuBarL( aMenuTitleResourceId );
            }
            break;
        case KTe2HelpViewIdValue:
            {
            iHelpView->ChangeMenuBarL( aMenuTitleResourceId );
            }
            break;
        case KTe2GameViewIdValue:
            {
            iGameView->ChangeMenuBarL( aMenuTitleResourceId );
            }
            break;
        case KTe2RankViewIdValue:
            {
            iRankView->ChangeMenuBarL( aMenuTitleResourceId );
            }
            break;
        default:
            break;
        }
    }

// ---------------------------------------------------------------------------
// CTe2AppUi::CleanRankL
// ---------------------------------------------------------------------------
//
void CTe2AppUi::CleanRankL()
    {
    iRankView->HandleCommandL( ETetris2CmdClear );
    }

// ---------------------------------------------------------------------------
// CTe2AppUi::MenuItemSelectedL
// ---------------------------------------------------------------------------
//
void CTe2AppUi::MenuItemSelectedL( TInt aMenuItem )
    {
    //__ASSERT_DEBUG( iState == NULL, Panic( ENullPointer ) );
    switch ( aMenuItem )
        {
        case ETetris2CmdStart:
            {
            iState->StartL();
            }
            break;
        case ETetris2CmdNormal:
        case ETetris2CmdAdvanced:
            {
            iState->ModeL();
            }
            break;
        case ETetris2CmdAudioOn:
        case ETetris2CmdAudioOff:
            {
            iState->AudioL();
            }
            break;
        case ETetris2CmdRank:
            {
            iState->RankL();
            }
            break;
        case ETetris2CmdHelp:
            {
            iState->HelpL();
            }
            break;
        case ETetris2CmdRestart:
            {
            iState->RestartL();
            }
            break;
        case ETetris2CmdStop:
            {
            iState->StopL();
            }
            break;
        case ETetris2CmdPause:
            {
            iState->PauseL();
            }
            break;
        case ETetris2CmdResume:
            {
            iState->ResumeL();
            }
            break;
        case ETetris2CmdBack:
            {
            iState->BackL();
            }
            break;
        case ETetris2CmdQuit:
            {
            iState->QuitL();
            }
            break;
        case ETetris2CmdClear:
            {
            iState->ClearL();
            }
            break;
        default:
            break;
        }
    }

// ---------------------------------------------------------------------------
// CTe2AppUi::GameOverNotifiedL
// ---------------------------------------------------------------------------
//
void CTe2AppUi::GameOverNotifiedL()
    {
    ChangeStateL( ETe2StopState );
    }

// ---------------------------------------------------------------------------
// CTe2AppUi::HandlePropertyChangedL
// ---------------------------------------------------------------------------
//
void CTe2AppUi::HandlePropertyChangedL( const TUid& aCategory, 
                TInt aKey )
    {
    TInt value(0);
    TInt err = RProperty::Get( aCategory, aKey, value );
    User::LeaveIfError( err );
    
    switch( value )
        {
        case EUninitialized:
            break;
            
        case EPreparedCompleted:
            {
            if ( iSettings.iAudio == ETe2AudioOn )
                {
                err = RProperty::Set( 
                        KPSUidTetris2, 
                        KTetris2AudioCommand,
                        EAudioOn 
                        );
                User::LeaveIfError( err );
                }
            }
            break;

        case EPreparedFailed:
            {
            // Kill audio thread
            err = RProperty::Set( 
                KPSUidTetris2, 
                KTetris2AudioCommand, 
                EAudioExit 
                );
            User::LeaveIfError( err );
            }
            break;
            
        case EExitCompleted:
            {
            // Exit the whole Tetris2
            Exit();    
            }
            break;
        default:
            break;
        }
    }

// ---------------------------------------------------------------------------
// CTe2AppUi::HandleResourceChangeL
// ---------------------------------------------------------------------------
//
void CTe2AppUi::HandleResourceChangeL( TInt aType )
    {
    CAknAppUi::HandleResourceChangeL( aType );    
    }

// --------------------------------------------------------------------------
// CTe2AppUi::HandleCommandL(TInt aCommand)
// Takes care of command handling
// --------------------------------------------------------------------------
//
void CTe2AppUi::HandleCommandL( TInt aCommand )
    {
    switch ( aCommand )
        {           
        case EAknSoftkeyExit:
        case EEikCmdExit:
            {
            TInt err =RProperty::Set( 
                KPSUidTetris2, 
                KTetris2AudioCommand, 
                EAudioExit 
                );
            User::LeaveIfError( err );
            break;
            }
        default:
            {
            break;
            }
        }
    }

// --------------------------------------------------------------------------
// CTe2AppUi::HandleWsEventL
// --------------------------------------------------------------------------
//
void CTe2AppUi::HandleWsEventL( const TWsEvent& aEvent,
    CCoeControl* aDestination )
    {
    CAknViewAppUi::HandleWsEventL( aEvent, aDestination );
    }

// --------------------------------------------------------------------------
// CTe2AppUi::HandleKeyEventL
// --------------------------------------------------------------------------
//
TKeyResponse CTe2AppUi::HandleKeyEventL( const TKeyEvent& /*aKeyEvent*/,
    TEventCode /*aType*/ )
    {
    return EKeyWasNotConsumed;
    }

// --------------------------------------------------------------------------
// CTe2AppUi::StartAudioThreadL
// --------------------------------------------------------------------------
//
void CTe2AppUi::StartAudioThreadL()
    {
    RThread thread;
    TInt stackSize = 0x8000; //Set the stack size for this thread as 8K
    TInt err = thread.Create( 
        _L( "AudioThread" ), 
        AudioThreadEntryPoint, 
        stackSize, 
        NULL, 
        this
        );

    User::LeaveIfError( err );

    // give thread more priority 
    thread.SetPriority( EPriorityMore ) ;

    // resume thread (wake it up sometime after this function returns)
    thread.Resume();
    }

// --------------------------------------------------------------------------
// CTe2AppUi::AudioThreadEntryPoint
// --------------------------------------------------------------------------
//
TInt CTe2AppUi::AudioThreadEntryPoint( TAny* aParam )
	{
    // Create cleanup stack
    __UHEAP_MARK;
    CTrapCleanup* cleanup = CTrapCleanup::New();
    TInt err = KErrNone;
    if( cleanup )
        {
        CTe2AppUi* self 
            = static_cast<CTe2AppUi*> ( aParam );
        TRAP_IGNORE( self->RunAudioThreadL() );
        delete cleanup;
        }
    else
    	{
        err = KErrNoMemory;
        }
    __UHEAP_MARKEND;
    return err;
	}

// --------------------------------------------------------------------------
// CTe2AppUi::RunAudioThreadL
// --------------------------------------------------------------------------
//
void CTe2AppUi::RunAudioThreadL()
    {
    // Create active scheduler (to run active objects)
    CActiveScheduler* scheduler = new (ELeave) CActiveScheduler();
    CleanupStack::PushL( scheduler );
    CActiveScheduler::Install( scheduler );

    CTe2AudioHandler* handler = CTe2AudioHandler::NewLC( *iEikonEnv );

    handler->StartL();

    CActiveScheduler::Start(); // Start scheduling

    // Cleanup the handler and scheduler
    CleanupStack::PopAndDestroy( handler );
    CleanupStack::PopAndDestroy( scheduler );
    }

// --------------------------------------------------------------------------
// CTe2AppUi::StoreL
// --------------------------------------------------------------------------
//
void CTe2AppUi::StoreL()
    {
    switch ( iCurrentView )
        {
        case KTe2MainViewIdValue:
            {
            iMenuView->StoreL();
            }
            break;
        case KTe2HelpViewIdValue:
            {
            iHelpView->StoreL();
            }
            break;
        case KTe2GameViewIdValue:
            {
            iGameView->StoreL();
            }
            break;
        case KTe2RankViewIdValue:
            {
            iRankView->StoreL();
            }
            break;
        default:
            break;
        }
    }

// End of File
