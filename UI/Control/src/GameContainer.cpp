/* ====================================================================
 * File: GameContainer.cpp
 * Created: 03/15/09
 * Modified: 04/19/09
 * Author: Sun Jinbo
 * Copyright (c): Tieto, All rights reserved
 * ==================================================================== */

// INCLUDE FILES
#include <akndef.h>
#include <aknutils.h>
#include <coecntrl.h>
#include <StringLoader.h>
#include <bautils.h>
#include <f32file.h>
#include <e32property.h>
#include <Te2Engine.h>
#include <EngineCommon.h>
#include <Te2Control.rsg>
#include <Logger.h>
#include <Te2PSKeys.h>
#include "GameContainer.h"
#include "MatrixControl.h"
#include "ScorekeeperControl.h"
#include "BackgroundControl.h"
#include "LayoutManager.h"
#include "ModelObserver.h"

//  CONSTANTS
const TInt KMaxTipsChars = 8;
const TInt KGameOverTimeOut = 200000;
const TInt KIncAlpha = 16;
const TInt KMaxAlpha = 255; 
const TInt KMinAlpha = 0;

_LIT( KTipsRemLine, "-%d" );
_LIT( KTipsAddScore, "+%d" );
_LIT( KTipsAddLife, "+%d" );

_LIT( KResourceFileName, "\\resource\\apps\\Te2Control.rsc" );

#if defined( __WINS__ )
_LIT( KGameOverAudioFileName, "c:\\private\\e01ff1c2\\gameover.wav" );
_LIT( KRemoveAudioFileName, "c:\\private\\e01ff1c2\\remove.wav" );
#else
_LIT( KGameOverAudioFileName, "\\private\\e01ff1c2\\gameover.wav" );
_LIT( KRemoveAudioFileName, "\\private\\e01ff1c2\\remove.wav" );
#endif // __WINS__

#ifdef RD_TE2_VIBRA_FUNC
const TUint16 KVibraTimeout = 1000;
#endif // RD_TE2_VIBRA_FUNC

// ======== MEMBER FUNCTIONS ========

// ----------------------------------------------------------------------------
// CGameContainer::~CGameContainer
// ----------------------------------------------------------------------------
//
EXPORT_C CGameContainer* CGameContainer::NewL( 
        const TRect& aRect, MModelObserver& aObserver )
    {
    CGameContainer* self = new (ELeave) CGameContainer( aObserver );
    CleanupStack::PushL ( self );
    self->ConstructL ( aRect );
    CleanupStack::Pop();
    return self;
    }

// ----------------------------------------------------------------------------
// CGameContainer::~CGameContainer
// ----------------------------------------------------------------------------
//
EXPORT_C CGameContainer::~CGameContainer()
    {
    delete iGameEngine;
    delete iTimeout;
    delete iGameOverText;
#ifdef RD_TE2_VIBRA_FUNC
    delete iVibra;
#endif // RD_TE2_VIBRA_FUNC
    }

// ----------------------------------------------------------------------------
// CGameContainer::Draw
// ----------------------------------------------------------------------------
//
void CGameContainer::Draw( const TRect& /*aRect*/ ) const
    { 
    // No implementation requried.
    }

// ----------------------------------------------------------------------------
// CGameContainer::SizeChanged
// ----------------------------------------------------------------------------
//
void CGameContainer::SizeChanged()
    {
    CBaseContainer::SizeChanged();
    if ( iPrimaryMatrix && iSecondaryMatrix && iScorekeeper )
        {
        iPrimaryMatrix->SetRect( iLayoutMgr->LayoutRect( EPrimaryMatrix ) );
        iSecondaryMatrix->SetRect( iLayoutMgr->LayoutRect( ESecondaryMatrix ) );
        iScorekeeper->SetRect( iLayoutMgr->LayoutRect( EScorekeeper ) );
        
        HandleOrientationChanged();
        }
    }

// ----------------------------------------------------------------------------
// CGameContainer::HandleResourceChange
// ----------------------------------------------------------------------------
//
void CGameContainer::HandleResourceChange( TInt aType )
    {
    CBaseContainer::HandleResourceChange( aType );
    }

// ----------------------------------------------------------------------------
// CGameContainer::OfferKeyEventL
// ----------------------------------------------------------------------------
//
TKeyResponse CGameContainer::OfferKeyEventL( const TKeyEvent& aKeyEvent, TEventCode aType )
    {
    TKeyResponse response( EKeyWasNotConsumed );
    
    if ( iResolution == EQhdPortrait )
        {
        response = HandleSideslipKeyEventL( aKeyEvent, aType );
        }
    else
        {
        if ( aType == EEventKey )
            {
            enum TKeyPressLength
                {
                EShortKeyPress,
                ELongKeyPress
                };
            TKeyPressLength length = aKeyEvent.iRepeats ? ELongKeyPress : EShortKeyPress;
            switch( aKeyEvent.iCode )
                {  
                case EKeyUpArrow: // ROTATE
                    {
                    HandleCommandL( EEngineRotate );
                    response = EKeyWasConsumed;
                    }
                    break;
                case EKeyDownArrow: // SPEEDUP DROP
                    {
                    if ( length == EShortKeyPress )
                        {
                        HandleCommandL( EEngineDescend );    
                        }
                    else
                        {
                        iPrevKeyLongPressed = ETrue;
                        HandleCommandL( EEngineFastDescendOn );
                        }
                    response = EKeyWasConsumed;
                    }
                    break;
                case EKeyLeftArrow: // TURN LEFT
                    {
                    HandleCommandL( EEngineLeft );
                    response = EKeyWasConsumed;
                    }
                    break;
                case EKeyRightArrow: // TURN RIGHT
                    {
                    HandleCommandL( EEngineRight );
                    response = EKeyWasConsumed;
                    }
                    break;
#ifdef __WINS__
                case EKeySpace:
#endif // __WINS__
                case EKeyDevice3: // DESCEND
                    {
                    HandleCommandL( EEngineDoubleDescend );
                    response = EKeyWasConsumed;
                    }
                    break;
                default:
                    {
                    response = CBaseContainer::OfferKeyEventL( aKeyEvent, aType );
                    }
                    break;
                }
            } 
        else if ( aType == EEventKeyUp )
            {
            switch( aKeyEvent.iScanCode )
                {
                case EKeyDownArrow:
                case EStdKeyDownArrow:
                    {
                    if( iPrevKeyLongPressed )
                        {
                        HandleCommandL( EEngineFastDescendOff );
                        iPrevKeyLongPressed = EFalse;
                        }
                    response = EKeyWasConsumed;
                    }
                    break;
                default:
                    {
                    response = CBaseContainer::OfferKeyEventL( aKeyEvent, aType );
                    }
                    break;
                }
            }
        }
    
    return response;
    }

// ----------------------------------------------------------------------------
// CGameContainer::HandlePointerEventL
// ----------------------------------------------------------------------------
//
void CGameContainer::HandlePointerEventL(const TPointerEvent& aPointerEvent)
    {
    CBaseContainer::HandlePointerEventL( aPointerEvent );

    if ( TouchTriggered() )
        {
        iPrimaryMatrix->HandlePointerEventL( aPointerEvent );
        }
    }

// ----------------------------------------------------------------------------
// CGameContainer::StateChangedL
// ----------------------------------------------------------------------------
//
void CGameContainer::StateChangedL( TEngineState aNewState, TEngineAction aAction )
    {
    switch ( aNewState )
        {
        case EPrimaryMatirxUpdated:
            {
            if ( aAction.Type() == EMatrixActionReleased )
                {
#ifdef RD_TE2_VIBRA_FUNC
                VibraL();
#endif // RD_TE2_VIBRA_FUNC
                if ( iAudio )
                    {
                    RProperty::Set( 
                        KPSUidTetris2, 
                        KTetris2AudioCommand, 
                        EAudioRewardSound );
                    }
                }
            iPrimaryMatrix->UpdateMatrixL( aAction );
            }
            break;
        case ESecondaryMatrixUpdated:
            {
            iSecondaryMatrix->UpdateMatrixL( aAction );
            }
            break;
        case EScorekeeperUpdated:
            {
            UpdateScorekeeperL( aAction );
            }
            break;
        case EAnimationStarted:
            {
            iState = EAnimation;
            }
            break;
        case EAnimationEnded:
            {
            iState = EPlaying;
            }
            break;
        case EEngGameOver:
            {
#ifdef RD_TE2_VIBRA_FUNC
            VibraL();
#endif // RD_TE2_VIBRA_FUNC
            if ( iAudio )
                {
                RProperty::Set( 
                    KPSUidTetris2, 
                    KTetris2AudioCommand, 
                    EAudioGameOverSound );
                }
            iAlpha = KMinAlpha;
            iState = EGameOver;
            iTimeout->Start( KGameOverTimeOut );
            iObserver.GameOverNotifiedL();
            }
            break;
        default:
            break;
        }
    }

// ----------------------------------------------------------------------------
// CGameContainer::HandleTimeoutL
// ----------------------------------------------------------------------------
//
void CGameContainer::HandleTimeoutL()
    {
    iAlpha += KIncAlpha;
    if ( iAlpha <= KMaxAlpha )
        {
        iTimeout->Start( KGameOverTimeOut );
        }
    else
        {
        iAlpha = KMaxAlpha;
        }
    }

// ----------------------------------------------------------------------------
// CGameContainer::NotifyL
// ----------------------------------------------------------------------------
//
void CGameContainer::NotifyL( TMatrixEvent aEvent )
    {
    switch ( aEvent )
        {
        case EMatrixRotate:
            HandleCommandL( EEngineRotate );
            break;
        case EMatrixMoveLeft:
            HandleCommandL( EEngineLeft );
            break;
        case EMatrixMoveRight:
            HandleCommandL( EEngineRight );
            break;
        case EMatrixDescend:
            HandleCommandL( EEngineDescend );
            break;
        case EMatrixFastDescendOn: // fall through
        case EMatrixFastDescendOff: // fall through
            // 5.0 doesn't have this feature
            break;
        case EMatrixDoulbeDescend:
            HandleCommandL( EEngineDoubleDescend );
            break;
        default:
            break;
        }
    }

// ----------------------------------------------------------------------------
// CGameContainer::Start
// ----------------------------------------------------------------------------
//
EXPORT_C void CGameContainer::StartL( TModeType aType )
    {
    if ( !iGameEngine )
        {
        switch ( aType )
            {
            case EModeCommon:
                {
                iGameEngine = CTe2Engine::NewL( *this, EEngineNormal );
                }
                break;
            case EModeExtra:
                {
#ifdef RD_TE2_ADVANCE_FUNC
                iGameEngine = CTe2Engine::NewL( *this, EEngineExtra );
#else // !RD_TE2_ADVANCE_FUNC
                iGameEngine = CTe2Engine::NewL( *this, EEngineNormal );
#endif // RD_TE2_ADVANCE_FUNC
                }
                break;
            default:
                break;
            }
        }
    HandleCommandL( EEngineStart );
    }

// ----------------------------------------------------------------------------
// CGameContainer::Stop
// ----------------------------------------------------------------------------
//
EXPORT_C void CGameContainer::Stop()
    {
    if ( iGameEngine )
        {
        TRAP_IGNORE( HandleCommandL( EEngineStop ) );
        }
    }

// ----------------------------------------------------------------------------
// CGameContainer::Pause
// ----------------------------------------------------------------------------
//
EXPORT_C void CGameContainer::Pause()
    {
    if ( iGameEngine )
        {
        TRAP_IGNORE( HandleCommandL( EEnginePause ) );
        iState = EPause;
        }
    }

// ----------------------------------------------------------------------------
// CGameContainer::Resume
// ----------------------------------------------------------------------------
//
EXPORT_C void CGameContainer::Resume()
    {
    if ( iGameEngine )
        {
        TRAP_IGNORE( HandleCommandL( EEngineResume ) );
        iState = EPlaying;
        }
    }

// ----------------------------------------------------------------------------
// CGameContainer::Restart
// ----------------------------------------------------------------------------
//
EXPORT_C void CGameContainer::Restart()
    {
    if ( iGameEngine )
        {
        TRAP_IGNORE( HandleCommandL( EEngineRestart ) );
        iState = EPlaying;
        }
    }

// ----------------------------------------------------------------------------
// CGameContainer::SetAudio
// ----------------------------------------------------------------------------
//
EXPORT_C void CGameContainer::SetAudio( TBool aAudio )
    {
    iAudio = aAudio;
    }

// ----------------------------------------------------------------------------
// CGameContainer::Draw
// ----------------------------------------------------------------------------
//
void CGameContainer::Draw()
    {
    CBaseContainer::Draw();
    iPrimaryMatrix->Draw();
    iSecondaryMatrix->Draw();
    iScorekeeper->Draw();
    
    if ( iState == EGameOver )
        {
        TRgb color( KRgbDarkBlue );
        color.SetAlpha( iAlpha );
        iBmpBufferContext->SetPenColor( color );

        iBmpBufferContext->SetPenStyle( CGraphicsContext::ESolidPen );
        iBmpBufferContext->UseFont( iEikonEnv->TitleFont() );
        
        TPoint origin;
        TSize wh;
        TInt borderWidth;
        TInt borderHeight;
        
        borderWidth = iEikonEnv->TitleFont()->TextWidthInPixels( *iGameOverText );
        borderHeight = iEikonEnv->TitleFont()->HeightInPixels() * 1.3;
        
        wh.SetSize( borderWidth, borderHeight );
        origin.iX = ( iPrimaryMatrix->Rect().Width() - borderWidth ) / 2;
        origin.iY = ( iPrimaryMatrix->Rect().Height() - borderHeight ) / 2;
        
        TRect textRect( origin, wh );
        
        TInt baseline = 
            ( textRect.Height() + iEikonEnv->LegendFont()->HeightInPixels() ) / 2;
        
        iBmpBufferContext->DrawText( *iGameOverText, textRect, baseline );

        iBmpBufferContext->DiscardFont();
        }
    }

// ----------------------------------------------------------------------------
// CGameContainer::CGameContainer
// ----------------------------------------------------------------------------
//
CGameContainer::CGameContainer( MModelObserver& aObserver )
    : CBaseContainer( aObserver ), 
    iLine( KScorekeeperDefaultLine ),
    iScore( KScorekeeperDefaultScore ),
    iLife( KScorekeeperDefaultLife ),
    iState( EPlaying )
    { 
    // No implementation required
    }

// ----------------------------------------------------------------------------
// CGameContainer::ConstructL
// ----------------------------------------------------------------------------
//
void CGameContainer::ConstructL( const TRect& aRect )
    {
    CBaseContainer::ConstructL( aRect );

    // Adds the specified resource file to the list maintained by CCoeEnv.
    TFileName dll;
    Dll::FileName( dll );
    TPtrC drive = TParsePtrC( dll ).Drive();
    TFileName resourceFileName;
    resourceFileName.Copy( drive );
    resourceFileName.Append( KResourceFileName );
    TInt err = CompleteWithAppPath( resourceFileName );
    if ( err != KErrNotSupported )
        {
        User::LeaveIfError( err );
        }

    CCoeEnv* env = CCoeEnv::Static();
    BaflUtils::NearestLanguageFile( env->FsSession(), resourceFileName );    

    iResOffset = env->AddResourceFileL( resourceFileName );

    iGameOverText = StringLoader::LoadL( R_TETRIS2_TEXT_GAME_OVER );
    CEikonEnv::Static()->DeleteResourceFile( iResOffset );

    // primary matrix
    TRect rect( iLayoutMgr->LayoutRect( EPrimaryMatrix ) );
    TSize matrixSize( KPrimaryMatrixWidth, KPrimaryMatrixHeight );
    iPrimaryMatrix = CMatrixControl::NewL( rect, iBmpBufferContext, matrixSize );
    iPrimaryMatrix->SetContainerWindowL( *this );
    iPrimaryMatrix->SetObserver( this );
    iPrimaryMatrix->SetBorder( ETrue );
    Components().AppendLC( iPrimaryMatrix );
    CleanupStack::Pop( iPrimaryMatrix );

    // secondary matrix
    rect =  iLayoutMgr->LayoutRect( ESecondaryMatrix );
    matrixSize.iWidth = KSecondaryMatrixWidth;
    matrixSize.iHeight = KSecondaryMatrixHeight;
    iSecondaryMatrix = CMatrixControl::NewL( rect, iBmpBufferContext, matrixSize );
    iSecondaryMatrix->SetContainerWindowL( *this );
    Components().AppendLC( iSecondaryMatrix );
    CleanupStack::Pop( iSecondaryMatrix );

    // scorekeeper
    rect =  iLayoutMgr->LayoutRect( EScorekeeper );
    iScorekeeper = CScorekeeperControl::NewL( rect, iBmpBufferContext );
    iScorekeeper->SetContainerWindowL( *this );
    Components().AppendLC( iScorekeeper );
    CleanupStack::Pop( iScorekeeper );

    iTimeout = CRegistrationTimeout::NewL( *this );

#ifdef RD_TE2_VIBRA_FUNC
    iVibra = CHWRMVibra::NewL();
#endif // RD_TE2_VIBRA_FUNC

    ActivateL();
    }

// ----------------------------------------------------------------------------
// CGameContainer::UpdateScorekeeperL
// ----------------------------------------------------------------------------
//
void CGameContainer::UpdateScorekeeperL( TEngineAction aAction )
    {
    TBuf<KMaxTipsChars> tips;
    switch ( aAction.Type() )
        {
        case EScorekeeperActionLevel:
            break;
        case EScorekeeperActionLine:
            {
            TInt32 line;
            aAction.Get( line );
            if ( line )
                {
                tips.Format( KTipsRemLine, line );
                iPrimaryMatrix->SetTipsL( tips );
                }
            }
            break;
        case EScorekeeperActionScore:
            {
            TInt32 score;
            aAction.Get( score );
            if ( score > iScore )
                {
                tips.Format( KTipsAddScore, score - iScore );
                iPrimaryMatrix->SetTipsL( tips );
                iScore  = score;
                }
            }
            break;
        case EScorekeeperActionLife:
            {
            TInt32 life;
            aAction.Get( life );
            if ( life > iLife )
                {
                tips.Format( KTipsAddLife, life - iLife );
                iPrimaryMatrix->SetTipsL( tips );
                iLife  = life;
                }
            }
            break;
        default:
            break;
        }
    iScorekeeper->UpdateScorekeeper( aAction );
    }

// ----------------------------------------------------------------------------
// CGameContainer::HandleOrientationChanged
// ----------------------------------------------------------------------------
//
void CGameContainer::HandleOrientationChanged()
    {
    HandleCommandL( EEngineUpdateAll );
    }

// ----------------------------------------------------------------------------
// CGameContainer::TouchTriggered
// ----------------------------------------------------------------------------
//
TBool CGameContainer::TouchTriggered() const
    {
    return ( iState == EPlaying ) ? ETrue : EFalse;
    }

#ifdef RD_TE2_VIBRA_FUNC
// ----------------------------------------------------------------------------
// CGameContainer::VibraL
// ----------------------------------------------------------------------------
//
void CGameContainer::VibraL()
    {
    CHWRMVibra::TVibraModeState mode = iVibra->VibraSettings();
	CHWRMVibra::TVibraStatus status = iVibra->VibraStatus();

    if ( mode == CHWRMVibra::EVibraModeON 
    		&& status == CHWRMVibra::EVibraStatusOn )
        {
        iVibra->StartVibraL( KVibraTimeout );
        }
    }

#endif // RD_TE2_VIBRA_FUNC

// ----------------------------------------------------------------------------
// CGameContainer::HandleCommandL
// ----------------------------------------------------------------------------
//
void CGameContainer::HandleCommandL( TInt aCommand )
    {
    switch ( aCommand )
        {
        case EEngineLeft: // fall through
        case EEngineRight: // fall through
        case EEngineRotate: // fall through
        case EEngineDescend: // fall through
        case EEngineFastDescendOn: // fall through
        case EEngineFastDescendOff: // fall through
        case EEngineDoubleDescend: // fall through
            {
            // we only care direction keys in Playing state
            if ( iState == EPlaying )
                {
                iGameEngine->HandleCommandL( (TEngineCmd)aCommand );
                }
            }
            break;

        case EEngineStart: // fall through
        case EEngineStop: // fall through
        case EEngineRestart: // fall through
        case EEnginePause: // fall through
        case EEngineResume: // fall through
        case EEngineUpdateAll: // fall through
        case EEngineCleanRank: // fall through
        default:
            iGameEngine->HandleCommandL( (TEngineCmd)aCommand );
            break;
        }

    }

// ----------------------------------------------------------------------------
// CGameContainer::HandleSideslipKeyEventL
// ----------------------------------------------------------------------------
//
TKeyResponse CGameContainer::HandleSideslipKeyEventL( 
                        const TKeyEvent& aKeyEvent, TEventCode aType )
    {
    TKeyResponse response( EKeyWasNotConsumed );
    if ( aType == EEventKey )
        {
        enum TKeyPressLength
            {
            EShortKeyPress,
            ELongKeyPress
            };
        TKeyPressLength length = aKeyEvent.iRepeats ? ELongKeyPress : EShortKeyPress;
        switch( aKeyEvent.iCode )
            {  
            case EKeyLeftArrow: // ROTATE
                {
                HandleCommandL( EEngineRotate );
                response = EKeyWasConsumed;
                }
                break;
            case EKeyRightArrow: // SPEEDUP DROP
                {
                if ( length == EShortKeyPress )
                    {
                    HandleCommandL( EEngineDescend );    
                    }
                else
                    {
                    iPrevKeyLongPressed = ETrue;
                    HandleCommandL( EEngineFastDescendOn );
                    }
                response = EKeyWasConsumed;
                }
                break;
            case EKeyDownArrow: // TURN LEFT
                {
                HandleCommandL( EEngineLeft );
                response = EKeyWasConsumed;
                }
                break;
            case EKeyUpArrow: // TURN RIGHT
                {
                HandleCommandL( EEngineRight );
                response = EKeyWasConsumed;
                }
                break;
#ifdef __WINS__
            case EKeySpace:
#endif // __WINS__
            case EKeyDevice3: // FASTER DESCEND
                {
                HandleCommandL( EEngineDoubleDescend );
                response = EKeyWasConsumed;
                }
                break;
            default:
                {
                response = CBaseContainer::OfferKeyEventL( aKeyEvent, aType );
                }
                break;
            }
        } 
    else if ( aType == EEventKeyUp )
        {
        switch( aKeyEvent.iScanCode )
            {
            case EKeyRightArrow:
            case EStdKeyRightArrow:
                {
                if( iPrevKeyLongPressed )
                    {
                    HandleCommandL( EEngineFastDescendOff );
                    iPrevKeyLongPressed = EFalse;
                    }
                response = EKeyWasConsumed;
                }
                break;
            default:
                {
                response = CBaseContainer::OfferKeyEventL( aKeyEvent, aType );
                }
                break;
            }
        }
    return response;
    }

// End of File
