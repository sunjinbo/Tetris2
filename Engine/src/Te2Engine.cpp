/* ====================================================================
 * File: Te2Engine.cpp
 * Created: 01/06/09
 * Modified: 05/04/09
 * Author: Sun Jinbo
 * Copyright (c): Tieto, All rights reserved
 * ==================================================================== */

// INCLUDE FILES
#include "Te2Engine.h"
#include "FixedTetrimino.h"
#include "NextRemovableTetrimino.h"
#include "RemovableTetrimino.h"
#include "AnimationTetrimino.h"
#include "Matrix.h"
#include "MatrixAction.h"
#include "Scorekeeper.h"
#include "EngineObserver.h"
#include "TetriminoFactory.h"
#ifdef RD_TE2_ADVANCE_FUNC
#include "TetriminoFactoryExt.h"
#endif // RD_TE2_ADVANCE_FUNC


// ======== MEMBER FUNCTIONS ========

// ----------------------------------------------------------------------------
// CTe2Engine::NewL
// ----------------------------------------------------------------------------
//
EXPORT_C CTe2Engine* CTe2Engine::NewL( MEngineObserver& aObserver, TEngineType aType )
    {
    CTe2Engine* self = new (ELeave) CTe2Engine( aObserver );
    CleanupStack::PushL( self );
    self->ConstructL( aType );
    CleanupStack::Pop( self );
    return self;
    }

// ----------------------------------------------------------------------------
// CTe2Engine::~CTe2Engine
// ----------------------------------------------------------------------------
//
EXPORT_C CTe2Engine::~CTe2Engine()
    {
    delete iTetriminoFactory;
    delete iScorekeeper;
    delete iPrimaryMatrix;
    delete iSecondaryMatrix;
    delete iRemovableTetrimino;
    delete iNextRemovableTetrimino;
    delete iAnimationTetrimino;
    delete iFixedTetrimino;
    }

// ----------------------------------------------------------------------------
// CTe2Engine::HandleCommandL
// Handle all kinds of commands from AppUi
// ----------------------------------------------------------------------------
//
EXPORT_C TInt CTe2Engine::HandleCommandL( TEngineCmd aCmd )
    {
    TInt retVal( KErrNone );

    switch( aCmd )
        {
        case EEngineStart:
            {
            StartL();    
            }
            break;

        case EEngineStop:
            {
            iPauseState = EFalse;
            iScorekeeper->SaveRankL();
            Stop();
            }
            break;

        case EEngineRestart:
            {
            iScorekeeper->SaveRankL();
            Stop();
            StartL(); 
            }
            break;

        case EEnginePause:
            {
            iPauseState = ETrue;
            iRemovableTetrimino->Stop();
            iFixedTetrimino->Stop();
            iAnimationTetrimino->Stop();
            }
            break;

        case EEngineResume:
            {
            iPauseState = EFalse;
            iRemovableTetrimino->Resume();
            iFixedTetrimino->Resume();
            iAnimationTetrimino->Resume();
            }
            break;

        case EEngineLeft:
            {
            if ( ! iPauseState )
                {
                iRemovableTetrimino->LeftL();    
                }
            }
            break;

        case EEngineRight:
            {
            if ( ! iPauseState )
                {
                iRemovableTetrimino->RightL();
                }
            }
            break;

        case EEngineRotate:
            {
            if ( ! iPauseState )
                {
                iRemovableTetrimino->RotateL();
                }
            }
            break;

        case EEngineDescend:
            {
            if ( ! iPauseState )
                {
                iRemovableTetrimino->DescendL();
                }    
            }
            break;

        case EEngineFastDescendOn:
            {
            if ( ! iPauseState )
                {
                iRemovableTetrimino->FastDescendL( ETrue );
                }
            }
            break;

        case EEngineFastDescendOff:
            {
            if ( ! iPauseState )
                {
                iRemovableTetrimino->FastDescendL( EFalse );
                }
            }
            break;

        case EEngineDoubleDescend:
            {
            if ( ! iPauseState )
                {
                iRemovableTetrimino->DoubleDescendL();
                }
            }
            break;

        case EEngineUpdateAll:
            {
            iRemovableTetrimino->UpdateAllL();
            iNextRemovableTetrimino->UpdateAllL();
            iFixedTetrimino->UpdateAllL();
            iAnimationTetrimino->UpdateAllL();;
            }
            break;

        case EEngineCleanRank:
            {
            iScorekeeper->CleanRankL();
            }
            break;

        default:
            {
            retVal = KErrArgument;
            }
            break;
        }
    return retVal;
    }

// ----------------------------------------------------------------------------
// CTe2Engine::GetRankItem
// ----------------------------------------------------------------------------
//
EXPORT_C TInt CTe2Engine::GetRankItem( TInt aRank, TTime& aTime, TInt& aScore  )
    {
    return iScorekeeper->GetRankItem( aRank, aTime, aScore );
    }

// ----------------------------------------------------------------------------
// CTe2Engine::StateChangedL
// From base class MTetriminoObserver
// Handle all kinds of notification from tetrimino
// ----------------------------------------------------------------------------
//
void CTe2Engine::StateChangedL( TTetriminoState aNewState )
    {
    TEngineAction action;

    switch( aNewState )
        {
        case EFixedTetriminoSingleRemoved:  // fall through
        case EFixedTetriminoDoubleRemoved:  // fall through
        case EFixedTetriminoTripleRemoved:  // fall through
        case EFixedTetriminoTetrisRemoved:  // fall through
            {
            action.SetType( EMatrixActionReleased );
            action.Set( iPrimaryMatrix->MatrixAction()->At( EMatrixActionReleased ) );
            iObserver.StateChangedL( EPrimaryMatirxUpdated, action );
            action.SetType( EMatrixActionRemoved );
            action.Set( iPrimaryMatrix->MatrixAction()->At( EMatrixActionRemoved ) );
            iObserver.StateChangedL( EPrimaryMatirxUpdated, action );
            action.SetType( EMatrixActionMoved );
            action.Set( iPrimaryMatrix->MatrixAction()->At( EMatrixActionMoved ) );
            iObserver.StateChangedL( EPrimaryMatirxUpdated, action );
            iScorekeeper->RemoveLineL( aNewState );
            }
            break;
            
        case EFixedTetriminoUpdated:
            {
#ifdef RD_TE2_ADVANCE_FUNC
            action.SetType( EMatrixActionExploded );
            action.Set( iPrimaryMatrix->MatrixAction()->At( EMatrixActionExploded ) );
            iObserver.StateChangedL( EPrimaryMatirxUpdated, action );
#endif // RD_TE2_ADVANCE_FUNC
            action.SetType( EMatrixActionRemoved );
            action.Set( iPrimaryMatrix->MatrixAction()->At( EMatrixActionRemoved ) );
            iObserver.StateChangedL( EPrimaryMatirxUpdated, action );
            action.SetType( EMatrixActionMoved );
            action.Set( iPrimaryMatrix->MatrixAction()->At( EMatrixActionMoved ) );
            iObserver.StateChangedL( EPrimaryMatirxUpdated, action );  
            }
            break;
        
        case EFixedTetriminoAllUpdated:
            {
            action.SetType( EMatrixActionUpdated );
            action.Set( iPrimaryMatrix->MatrixAction()->At( EMatrixActionUpdated ) );
            iObserver.StateChangedL( EPrimaryMatirxUpdated, action );  
            }
            break;
            
        case EFixedTetriminoMerged:
            {
            iRemovableTetrimino->Stop();
            iRemovableTetrimino->CloneL( *iNextRemovableTetrimino );
            iRemovableTetrimino->StartL( iScorekeeper->Level() );
            iNextRemovableTetrimino->NextL();
            }
            break;

        case EFixedTetriminoEnded:
            {
            iRemovableTetrimino->Stop();
            iAnimationTetrimino->PlayL( *iFixedTetrimino );
            }
            break;

        case ENextRemovableTetriminoRemoved:
            {
            action.SetType( EMatrixActionRemoved );
            action.Set( iSecondaryMatrix->MatrixAction()->At( EMatrixActionRemoved ) );
            iObserver.StateChangedL( ESecondaryMatrixUpdated, action );
            }
            break;

        case ENextRemovableTetriminoCreated:
            {
            if ( iFirstTetrimino )
                {
                iFirstTetrimino = EFalse;
                }
            else
                {
                action.SetType( EMatrixActionCreated );
                action.Set( iSecondaryMatrix->MatrixAction()->At( EMatrixActionCreated ) );
                iObserver.StateChangedL( ESecondaryMatrixUpdated, action );
                }
            }
            break;
 
        case ENextRemovableTetriminoAllUpdated:
            {
            action.SetType( EMatrixActionUpdated );
            action.Set( iSecondaryMatrix->MatrixAction()->At( EMatrixActionUpdated ) );
            iObserver.StateChangedL( ESecondaryMatrixUpdated, action );
            }    
            break;

        case ERemovableTetriminoLeft: // fall through
        case ERemovableTetriminoRight: // fall through
        case ERemovableTetriminoRotated: // fall through
        case ERemovableTetriminoDescend: // fall through
        case ERemovableTetriminoFastDescend: // fall through
        case ERemovableTetriminoUpdated: // fall through
            {
            action.SetType( EMatrixActionRemoved );
            action.Set( iPrimaryMatrix->MatrixAction()->At( EMatrixActionRemoved ) );
            iObserver.StateChangedL( EPrimaryMatirxUpdated, action );
            action.SetType( EMatrixActionMoved );
            action.Set( iPrimaryMatrix->MatrixAction()->At( EMatrixActionMoved ) );
            iObserver.StateChangedL( EPrimaryMatirxUpdated, action );
            }
            break;
        
        case ERemovableTetriminoAllUpdated:
            {
            action.SetType( EMatrixActionUpdated );
            action.Set( iPrimaryMatrix->MatrixAction()->At( EMatrixActionUpdated ) );
            iObserver.StateChangedL( EPrimaryMatirxUpdated, action );          
            }
            break;

        case ERemovableTetriminoDescendEnd:
        case ERemovableTetriminoFastDescendEnd:
            {
            iFixedTetrimino->MergeL( *iRemovableTetrimino );
            }
            break;

        case EAnimationTetriminoStarted:
            {
            TEngineAction action;
            iObserver.StateChangedL( EAnimationStarted, action );
            iFixedTetrimino->Stop();
            }
            break;

        case EAnimationTetriminoUpdated:
            {
            action.SetType( EMatrixActionRemoved );
            action.Set( iPrimaryMatrix->MatrixAction()->At( EMatrixActionRemoved ) );
            iObserver.StateChangedL( EPrimaryMatirxUpdated, action );
            action.SetType( EMatrixActionUpdated );
            action.Set( iPrimaryMatrix->MatrixAction()->At( EMatrixActionUpdated ) );
            iObserver.StateChangedL( EPrimaryMatirxUpdated, action );
            }
            break;

        case EAnimationTetriminoAllUpdated:
            {
            action.SetType( EMatrixActionUpdated );
            action.Set( iPrimaryMatrix->MatrixAction()->At( EMatrixActionUpdated ) );
            iObserver.StateChangedL( EPrimaryMatirxUpdated, action );
            }
            break;

        case EAnimationTetriminoEnded:
            {
            TEngineAction action;
            iObserver.StateChangedL( EAnimationEnded, action );
            iScorekeeper->KillL();
            }
            break;

        default:
            break;
        }
    }


// ----------------------------------------------------------------------------
// CTe2Engine::StateChangedL
// From base class MScorekeeperObserver
// Handle all kinds of notification from Scorekeeper
// ----------------------------------------------------------------------------
//
void CTe2Engine::StateChangedL( TScorekeeperState aNewState )
    {
    TEngineAction action;
    switch( aNewState )
        {
        case EScorekeeperLevelUpdated:
            {
            action.SetType( EScorekeeperActionLevel );
            action.Set( iScorekeeper->Level() );
            iObserver.StateChangedL( EScorekeeperUpdated, action );
            }
            break;

        case EScorekeeperLineUpdated:
            {
            action.SetType( EScorekeeperActionLine );
            action.Set( iScorekeeper->Line() );
            iObserver.StateChangedL( EScorekeeperUpdated, action );
            }
            break;
            
        case EScorekeeperScoreUpdated:
            {
            action.SetType( EScorekeeperActionScore );
            action.Set( iScorekeeper->Score() );
            iObserver.StateChangedL( EScorekeeperUpdated, action );
            }
            break;

        case EScorekeeperLifeUpdated:
            {
            action.SetType( EScorekeeperActionLife );
            action.Set( iScorekeeper->Life() );
            iObserver.StateChangedL( EScorekeeperUpdated, action );
            if ( iScorekeeper->Life() )
                {
                iFixedTetrimino->Reset();
                iRemovableTetrimino->Stop();
                iRemovableTetrimino->CloneL( *iNextRemovableTetrimino );
                iRemovableTetrimino->StartL( iScorekeeper->Level() );
                iNextRemovableTetrimino->NextL();
                iFixedTetrimino->Start();
                }
            }
            break;

        case EScorekeeperLifeEnded:
            {
            Stop();
            iObserver.StateChangedL( EEngGameOver, action );
            }
            break;

        default:
            break;
        }
    }

// ----------------------------------------------------------------------------
// CTe2Engine::CTe2Engine
// Constructor
// ----------------------------------------------------------------------------
//
CTe2Engine::CTe2Engine( MEngineObserver& aObserver ) 
: iObserver( aObserver ), iPauseState( EFalse ), iFirstTetrimino( ETrue )
    { 
    // no implementation required       
    }

// ----------------------------------------------------------------------------
// CTe2Engine::CTe2Engine
// Completes construction
// ----------------------------------------------------------------------------
//
void CTe2Engine::ConstructL( TEngineType aType )
    {
    switch ( aType )
        {
        case EEngineNormal:
            iTetriminoFactory = CTetriminoFactory::NewL();
            break;
#ifdef RD_TE2_ADVANCE_FUNC
        case EEngineExtra:
            iTetriminoFactory = CTetriminoFactoryExt::NewL();
            break;
#endif // RD_TE2_ADVANCE_FUNC
        default:
            break;
        }
    iScorekeeper = CScorekeeper::NewL( *this );
    iPrimaryMatrix = CMatrix::NewL();
    iSecondaryMatrix = CMatrix::NewL();

    iFixedTetrimino = 
        iTetriminoFactory->CreateFixedTetriminoL( *iPrimaryMatrix, *this );
    iNextRemovableTetrimino = 
        iTetriminoFactory->CreateNextRemovableTetriminoL( *iSecondaryMatrix, *this );
    iRemovableTetrimino = 
        iTetriminoFactory->CreateRemovableTetriminoL( *iPrimaryMatrix, *this );
    iAnimationTetrimino = 
        iTetriminoFactory->CreateAnimationTetriminoL( *iPrimaryMatrix, *this );
    }

// ----------------------------------------------------------------------------
// CTe2Engine::StartL
// ----------------------------------------------------------------------------
//
void CTe2Engine::StartL()
    {
    iScorekeeper->StartL();
    iNextRemovableTetrimino->NextL();
    iRemovableTetrimino->CloneL( *iNextRemovableTetrimino );
    iRemovableTetrimino->StartL( iScorekeeper->Level() );
    iNextRemovableTetrimino->NextL();
    iFixedTetrimino->Start();
    }

// ----------------------------------------------------------------------------
// CTe2Engine::Stop
// ----------------------------------------------------------------------------
//
void CTe2Engine::Stop()
    {
    iFirstTetrimino = ETrue;
    //iNextRemovableTetrimino->Reset();
    iRemovableTetrimino->Reset();
    iFixedTetrimino->Reset();
    iAnimationTetrimino->Reset();
    iScorekeeper->Reset();
    }

// End of File
