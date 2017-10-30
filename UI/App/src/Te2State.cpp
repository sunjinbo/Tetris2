/* ====================================================================
 * File: Te2State.cpp
 * Created: 01/17/09
 * Modified: 03/30/09
 * Author: Sun Jinbo
 * Copyright (c): Tieto, All rights reserved
 * ==================================================================== */

// INCLUDE FILES
#include <e32std.h>
#include <Tetris2.rsg>
#include "Te2State.h"
#include "Tetris2.hrh"
#include "Tetris2ViewIds.h"

// ======== MEMBER FUNCTIONS ========

// -----------------------------------------------------------------------------
// CTe2State::~CTe2State
// Destructor
// -----------------------------------------------------------------------------
//
CTe2State::~CTe2State()
    {  
    // no implementation required
    }

// -----------------------------------------------------------------------------
// CTe2State::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CTe2State::ConstructL()
    {  
    // no implementation required
    }

// -----------------------------------------------------------------------------
// CTe2State::CTe2State
// C++ default constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
CTe2State::CTe2State( MTe2StateObserver& aObserver, CTe2State* aState )
    : iObserver( aObserver )
    {
    if ( aState )
        {
        // Use privious configuration
        iModeType = aState->iModeType;
        iAudioType = aState->iAudioType;
        }
    else
        {
        // Use default configuration.
        iModeType = ETe2ModeNormal;
        iAudioType = ETe2AudioOn;
        }
    }

//*******************************************************//
//                 CTe2State Methods
//
// state sub-classes over ride methods that they need to 
// over ride, otherwise they just inherit CTe2State
// class's implementation.
//*******************************************************//

// ---------------------------------------------------------------------------
// CTe2State::StartL
// ---------------------------------------------------------------------------
//
void CTe2State::StartL()
    { 
    // no implementation required
    }

// ---------------------------------------------------------------------------
// CTe2State::PauseL
// ---------------------------------------------------------------------------
//
void CTe2State::PauseL()
    { 
    // no implementation required
    }
    
// ---------------------------------------------------------------------------
// CTe2State::ResumeL
// ---------------------------------------------------------------------------
//
void CTe2State::ResumeL()
    { 
    // no implementation required
    }

// ---------------------------------------------------------------------------
// CTe2State::StopL
// ---------------------------------------------------------------------------
//
void CTe2State::StopL()
    { 
    // no implementation required
    }

// ---------------------------------------------------------------------------
// CTe2State::RestartL
// ---------------------------------------------------------------------------
//
void CTe2State::RestartL()
    { 
    // no implementation required
    }

// ---------------------------------------------------------------------------
// CTe2State::HelpL
// ---------------------------------------------------------------------------
//
void CTe2State::HelpL()
    { 
    // no implementation required
    }

// ---------------------------------------------------------------------------
// CTe2State::BackL
// ---------------------------------------------------------------------------
//
void CTe2State::BackL()
    { 
    // no implementation required
    }

// ---------------------------------------------------------------------------
// CTe2State::AudioL
// ---------------------------------------------------------------------------
//
void CTe2State::AudioL()
    { 
    // no implementation required
    }

// ---------------------------------------------------------------------------
// CTe2State::ModeL
// ---------------------------------------------------------------------------
//
void CTe2State::ModeL()
    { 
    // no implementation required
    }

// ---------------------------------------------------------------------------
// CTe2State::RankL
// ---------------------------------------------------------------------------
//
void CTe2State::RankL()
    { 
    // no implementation required
    }

// ---------------------------------------------------------------------------
// CTe2State::QuitL
// ---------------------------------------------------------------------------
//
void CTe2State::QuitL()
    { 
    // no implementation required
    }

// ---------------------------------------------------------------------------
// CTe2State::ClearL
// ---------------------------------------------------------------------------
//
void CTe2State::ClearL()
    { 
    // no implementation required
    }

//*******************************************************//
//                 STATE SUB-CLASSES
// not all methods are over riddnen by these sub-state
// classes, they only implement methods where their 
// behavior differs
//*******************************************************//

///////////////////////////////////////
///     CMenuState
///////////////////////////////////////

// ---------------------------------------------------------------------------
// CMenuState::NewL
// ---------------------------------------------------------------------------
//
CTe2State* CMenuState::NewL( MTe2StateObserver& aObserver, CTe2State* aState )
    {
    CMenuState* self = new(ELeave) CMenuState( aObserver, aState );   
    CleanupStack::PushL( self );  
    self->ConstructL();
    CleanupStack::Pop();
    return self;  
    }

// ---------------------------------------------------------------------------
// CMenuState::~CMenuState
// ---------------------------------------------------------------------------
//
CMenuState::~CMenuState()
    {
    // no implementation required
    }

// ---------------------------------------------------------------------------
// CMenuState::StartL
// ---------------------------------------------------------------------------
//
void CMenuState::StartL()
    {
    iObserver.ChangeViewL( KTe2GameViewIdValue );
    iObserver.ChangeMenubarL( R_TETRIS2_SOFTKEYS_PAUSESTOP );
    iObserver.ChangeEngineL( ETetris2CmdStart );
    iObserver.ChangeStateL( ETe2PlayState );
    }

// ---------------------------------------------------------------------------
// CMenuState::ModeL
// ---------------------------------------------------------------------------
//
void CMenuState::ModeL()
    {
    switch( iModeType )
        {
        case ETe2ModeNormal:
            iModeType = ETe2ModeAdvanced;
            break;
        case ETe2ModeAdvanced:
            iModeType = ETe2ModeNormal;
            break;
        default:
            break;
        }
    iObserver.ChangeModeL( iModeType );
    }

// ---------------------------------------------------------------------------
// CMenuState::AudioL
// ---------------------------------------------------------------------------
//
void CMenuState::AudioL()
    {
    switch( iAudioType )
        {
        case ETe2AudioOn:
            iAudioType = ETe2AudioOff;
            break;
        case ETe2AudioOff:
            iAudioType = ETe2AudioOn;
            break;
        default:
            break;
        }
    iObserver.ChangeAudioL( iAudioType );
    }

// ---------------------------------------------------------------------------
// CMenuState::RankL
// ---------------------------------------------------------------------------
//
void CMenuState::RankL()
    {
    iObserver.ChangeViewL( KTe2RankViewIdValue );
    iObserver.ChangeMenubarL( R_TETRIS2_SOFTKEYS_CLEARBACK );
    iObserver.ChangeStateL( ETe2RankState );
    }

// ---------------------------------------------------------------------------
// CMenuState::HelpL
// ---------------------------------------------------------------------------
//
void CMenuState::HelpL()
    {
    iObserver.ChangeViewL( KTe2HelpViewIdValue );
    iObserver.ChangeMenubarL( R_TETRIS2_SOFTKEYS_EMPTYBACK );
    iObserver.ChangeStateL( ETe2HelpState );
    }

// ---------------------------------------------------------------------------
// CMenuState::QuitL
// ---------------------------------------------------------------------------
//
void CMenuState::QuitL()
    {
    User::Exit( 0 );//???
    }

// -----------------------------------------------------------------------------
// CMenuState::CMenuState
// C++ default constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
CMenuState::CMenuState( MTe2StateObserver& aObserver, CTe2State* aState )
    : CTe2State( aObserver, aState )
    {
    // no implementation required
    }

// ---------------------------------------------------------------------------
// CMenuState::ConstructL
// ---------------------------------------------------------------------------
//
void CMenuState::ConstructL()
    {
    iObserver.ChangeViewL( KTe2MainViewIdValue );
    iObserver.ChangeMenubarL( R_TETRIS2_SOFTKEYS_SELECTQUIT );
    }

///////////////////////////////////////
///     CPlayState
///////////////////////////////////////

// ---------------------------------------------------------------------------
// CPlayState::NewL
// ---------------------------------------------------------------------------
//
CTe2State* CPlayState::NewL( MTe2StateObserver& aObserver, CTe2State* aState )
    {
    CPlayState* self = new(ELeave) CPlayState( aObserver, aState );   
    CleanupStack::PushL( self );  
    self->ConstructL();
    CleanupStack::Pop();
    return self;  
    }

// ---------------------------------------------------------------------------
// CPlayState::~CPlayState
// ---------------------------------------------------------------------------
//
CPlayState::~CPlayState()
    {
    // no implementation required
    }

// ---------------------------------------------------------------------------
// CPlayState::PauseL
// ---------------------------------------------------------------------------
//
void CPlayState::PauseL()
    {
    iObserver.ChangeMenubarL( R_TETRIS2_SOFTKEYS_RESUMESTOP );
    iObserver.ChangeEngineL( ETetris2CmdPause );
    iObserver.ChangeStateL( ETe2PauseState );
    }

// ---------------------------------------------------------------------------
// CPlayState::StopL
// ---------------------------------------------------------------------------
//
void CPlayState::StopL()
    {
    iObserver.ChangeViewL( KTe2MainViewIdValue );
    iObserver.ChangeMenubarL( R_TETRIS2_SOFTKEYS_SELECTQUIT );
    iObserver.ChangeEngineL( ETetris2CmdStop );
    iObserver.ChangeStateL( ETe2MenuState );
    }

// -----------------------------------------------------------------------------
// CPlayState::CPlayState
// C++ default constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
CPlayState::CPlayState( MTe2StateObserver& aObserver, CTe2State* aState )
    : CTe2State( aObserver, aState )
    {
    // no implementation required
    }

///////////////////////////////////////
///     CPauseState
///////////////////////////////////////

// ---------------------------------------------------------------------------
// CPauseState::NewL
// ---------------------------------------------------------------------------
//
CTe2State* CPauseState::NewL( MTe2StateObserver& aObserver, CTe2State* aState )
    {
    CPauseState* self = new(ELeave) CPauseState( aObserver, aState );   
    CleanupStack::PushL( self );  
    self->ConstructL(); //self->ConstructL(); 
    CleanupStack::Pop();
    return self;  
    }

// ---------------------------------------------------------------------------
// CPauseState::~CPauseState
// ---------------------------------------------------------------------------
//
CPauseState::~CPauseState()
    {
    // no implementation required
    }


// ---------------------------------------------------------------------------
// CPauseState::ResumeL
// ---------------------------------------------------------------------------
//
void CPauseState::ResumeL()
    {
    iObserver.ChangeMenubarL( R_TETRIS2_SOFTKEYS_PAUSESTOP );
    iObserver.ChangeEngineL( ETetris2CmdResume );
    iObserver.ChangeStateL( ETe2PlayState );
    }

// ---------------------------------------------------------------------------
// CPauseState::StopL
// ---------------------------------------------------------------------------
//
void CPauseState::StopL()
    {
    iObserver.ChangeViewL( KTe2MainViewIdValue );
    iObserver.ChangeMenubarL( R_TETRIS2_SOFTKEYS_SELECTQUIT );
    iObserver.ChangeEngineL( ETetris2CmdStop );
    iObserver.ChangeStateL( ETe2MenuState );
    }

// -----------------------------------------------------------------------------
// CPauseState::CPauseState
// C++ default constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
CPauseState::CPauseState( MTe2StateObserver& aObserver, CTe2State* aState )
    : CTe2State( aObserver, aState )
    {
    // no implementation required
    }

///////////////////////////////////////
///     CStopState
///////////////////////////////////////

// ---------------------------------------------------------------------------
// CStopState::NewL
// ---------------------------------------------------------------------------
//
CTe2State* CStopState::NewL( MTe2StateObserver& aObserver, CTe2State* aState )
    {
    CStopState* self = new(ELeave) CStopState( aObserver, aState );   
    CleanupStack::PushL( self );  
    self->ConstructL(); //self->ConstructL(); 
    CleanupStack::Pop();
    return self;  
    }

// ---------------------------------------------------------------------------
// CStopState::~CStopState
// ---------------------------------------------------------------------------
//
CStopState::~CStopState()
    {
    // no implementation required
    }

// ---------------------------------------------------------------------------
// CStopState::RestartL
// ---------------------------------------------------------------------------
//
void CStopState::RestartL()
    {
    iObserver.ChangeMenubarL( R_TETRIS2_SOFTKEYS_PAUSESTOP );
    iObserver.ChangeEngineL( ETetris2CmdRestart );
    iObserver.ChangeStateL( ETe2PlayState );
    }

// ---------------------------------------------------------------------------
// CStopState::BackL
// ---------------------------------------------------------------------------
//
void CStopState::BackL()
    {
    iObserver.ChangeViewL( KTe2MainViewIdValue );
    iObserver.ChangeMenubarL( R_TETRIS2_SOFTKEYS_SELECTQUIT );
    iObserver.ChangeStateL( ETe2MenuState );
    }

// -----------------------------------------------------------------------------
// CStopState::CStopState
// C++ default constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
CStopState::CStopState( MTe2StateObserver& aObserver, CTe2State* aState )
    : CTe2State( aObserver, aState )
    {
    // no implementation required
    }

// ---------------------------------------------------------------------------
// CStopState::ConstructL
// ---------------------------------------------------------------------------
//
void CStopState::ConstructL()
    {
    iObserver.ChangeMenubarL( R_TETRIS2_SOFTKEYS_RESTARTBACK );
    iObserver.ChangeEngineL( ETetris2CmdStop );
    }

///////////////////////////////////////
///     CHelpState
///////////////////////////////////////

// ---------------------------------------------------------------------------
// CHelpState::NewL
// ---------------------------------------------------------------------------
//
CTe2State* CHelpState::NewL( MTe2StateObserver& aObserver, CTe2State* aState )
    {
    CHelpState* self = new(ELeave) CHelpState( aObserver, aState );   
    CleanupStack::PushL( self );  
    self->ConstructL(); 
    CleanupStack::Pop();
    return self;  
    }

// ---------------------------------------------------------------------------
// CHelpState::~CHelpState
// ---------------------------------------------------------------------------
//
CHelpState::~CHelpState()
    {
    // no implementation required
    }

// ---------------------------------------------------------------------------
// CHelpState::BackL
// ---------------------------------------------------------------------------
//
void CHelpState::BackL()
    {
    iObserver.ChangeViewL( KTe2MainViewIdValue );
    iObserver.ChangeMenubarL( R_TETRIS2_SOFTKEYS_SELECTQUIT );
    iObserver.ChangeStateL( ETe2MenuState );
    }

// -----------------------------------------------------------------------------
// CHelpState::CHelpState
// C++ default constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
CHelpState::CHelpState( MTe2StateObserver& aObserver, CTe2State* aState )
    : CTe2State( aObserver, aState )
    {
    // no implementation required
    }


///////////////////////////////////////
///     CRankState
///////////////////////////////////////

// ---------------------------------------------------------------------------
// CRankState::NewL
// ---------------------------------------------------------------------------
//
CTe2State* CRankState::NewL( MTe2StateObserver& aObserver, CTe2State* aState )
    {
    CRankState* self = new(ELeave) CRankState( aObserver, aState );   
    CleanupStack::PushL( self );  
    self->ConstructL(); 
    CleanupStack::Pop();
    return self;  
    }

// ---------------------------------------------------------------------------
// CRankState::~CRankState
// ---------------------------------------------------------------------------
//
CRankState::~CRankState()
    {
    // no implementation required
    }

// ---------------------------------------------------------------------------
// CRankState::BackL
// ---------------------------------------------------------------------------
//
void CRankState::BackL()
    {
    iObserver.ChangeViewL( KTe2MainViewIdValue );
    iObserver.ChangeMenubarL( R_TETRIS2_SOFTKEYS_SELECTQUIT );
    iObserver.ChangeStateL( ETe2MenuState );
    }

// ---------------------------------------------------------------------------
// CRankState::ClearL
// ---------------------------------------------------------------------------
//
void CRankState::ClearL()
    {
    iObserver.CleanRankL();
    }

// -----------------------------------------------------------------------------
// CRankState::CRankState
// C++ default constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
CRankState::CRankState( MTe2StateObserver& aObserver, CTe2State* aState )
    : CTe2State( aObserver, aState )
    {  
    // no implementation required
    }

// End of File
