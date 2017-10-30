/* ====================================================================
 * File: MatrixControl.cpp
 * Created: 03/15/09
 * Modified: 04/19/09
 * Author: Sun Jinbo
 * Copyright (c): Tieto, All rights reserved
 * ==================================================================== */

// INCLUDE FILES
#include <fbs.h>
#include <eikenv.h>
#include <EngineAction.h> // TEngineAction
#include <Block.h>
#include <matrix_svg.mbg>
#include <SvgUtils.h>
#include "MatrixControl.h"
#include "MatrixTaskScheduler.h"
#include "MatrixTask.h"
#include "MatrixObserver.h"
#include "LayoutManager.h"
#include "Logger.h"

// CONSTANTS
const TInt KMatrixInvalidWidth = 0;
const TInt KMatrixInvalidHeight = 0;    
const TInt KMatrixTipsDelayTimeout = 0;
const TInt KMatrixTipsUpdateTimeout = 150000;
const TInt KMatrixTipsTimer = 8;
const TInt KMatrixIndiTimer = 4;
const TInt KMatrixIndiDelayTimeout = 0;
const TInt KMatrixIndiUpdateTimeout = 100000;
const TInt KQhdBorderPenSize = 3;
const TInt KQvgaBorderPenSize = 1;
const TInt KDefaultDragCount = 0;
const TInt KMaxDragCount = 3;
const TInt KMatrixMargin = 5;
const TInt KDefaultRotateOffset = 0;
const TInt KMaxRotateOffset = 3;


#if defined( __WINS__ )
_LIT( KMatrixSvgFileName, "Z:\\resource\\apps\\matrix_svg.mbm" );
#else
_LIT( KMatrixSvgFileName, "\\resource\\apps\\matrix_svg.mbm" );
#endif // __WINS__

// ======== MEMBER FUNCTIONS ========

// ----------------------------------------------------------------------------
// CMatrixControl::NewL
// ----------------------------------------------------------------------------
//
CMatrixControl* CMatrixControl::NewL( 
    const TRect& aRect, CFbsBitGc*& aBgc, const TSize& aMatrixSize )
    {
    CMatrixControl* self = new (ELeave) CMatrixControl( aBgc );
    CleanupStack::PushL (self );
    self->ConstructL ( aRect, aMatrixSize );
    CleanupStack::Pop();
    return self;
    }

// ----------------------------------------------------------------------------
// CMatrixControl::~CMatrixControl
// ----------------------------------------------------------------------------
//
CMatrixControl::~CMatrixControl()
    {
    delete iScheduler;
    delete iSvgUtils;
    delete iTipsText;
    StopTips(); // iPeriodic
    if ( iIndiPeriodic )
        {  
        iIndiPeriodic->Cancel();
        delete iIndiPeriodic;  
        }
    ReleaseBmpBuffer();

    iRemovableRegion.Close();
    iEikonEnv->ScreenDevice()->ReleaseFont( iTipsFont );
    }

// ----------------------------------------------------------------------------
// CMatrixControl::Draw
// ----------------------------------------------------------------------------
//
void CMatrixControl::Draw(const TRect& /*aRect*/) const
    { 
    // no implementation required
    }

// ----------------------------------------------------------------------------
// CMatrixControl::SizeChanged
// ----------------------------------------------------------------------------
//
void CMatrixControl::SizeChanged()
    {
    if ( iMatrixWidth != KMatrixInvalidWidth 
        && iMatrixHeight != KMatrixInvalidHeight
        && iSvgUtils )
        {
        TInt width = Rect().Width() / iMatrixWidth;
        TInt height = Rect().Height() / iMatrixHeight;
        TInt size = ( width > height ) ? height : width;
        iBlockSize = TSize( size, size );
        iMatrixSize = TSize( size * iMatrixWidth, size * iMatrixHeight );
        iTipsPos.iX = iMatrixSize.iWidth / 2;
        iTipsPos.iY = iMatrixSize.iHeight / 2;
        iTipsSize.iWidth = width * 3;
        iTipsSize.iHeight = height;
        iIndiSize = TSize( iBlockSize.iWidth / 2, iBlockSize.iHeight / 2 );

        for ( TInt i = EMbmMatrix_svgRed; i < EMbmMatrix_svgLeft; i += 2 )
            {
            iSvgUtils->SetSize( i, iBlockSize );
            }

        for ( TInt i = EMbmMatrix_svgLeft; i < EMbmMatrix_svgLastElement; i += 2 )
            {
            iSvgUtils->SetSize( i, iIndiSize );
            }

        TRAP_IGNORE( CreateBmpBufferL() );
        }
    }

// ----------------------------------------------------------------------------
// CMatrixControl::HandlePointerEventL
// ----------------------------------------------------------------------------
//
void CMatrixControl::HandlePointerEventL( const TPointerEvent& aPointerEvent )
    {
	TRect theRect( RealRect() );
    theRect.Grow( TSize( KMatrixMargin, KMatrixMargin ) );
    if ( iObserver && theRect.Contains( aPointerEvent.iPosition ) )
        {
        switch ( aPointerEvent.iType )
            {
            case TPointerEvent::EButton1Down:
                {
				iKeyLongPressed = EFalse;
				iDragCount = KDefaultDragCount;
				iStartPoint = aPointerEvent.iPosition;
                }
                break;

            case TPointerEvent::EButton1Up:
                {
                if ( iKeyLongPressed )
                    {
                    TGestureArea gesture 
                        = GestureArea( iStartPoint, aPointerEvent.iPosition );
                    HandleGestureAreaEventL( gesture );
                    }
                else
                    {
					TRect r = iRemovableRegion.BoundingRect();
					TDirection d = Direction( r, aPointerEvent.iPosition );
					HandleDirectionEventL( d );
                    }
                }
                break;

            case TPointerEvent::EDrag:
                {
                ++iDragCount;
                if ( iDragCount > KMaxDragCount )
                    {
                    iKeyLongPressed = ETrue;
                    }
                }
                break;

            default:
                break;
            }
        }
    }

// ----------------------------------------------------------------------------
// CMatrixControl::SetObserver
// ----------------------------------------------------------------------------
//
void CMatrixControl::SetObserver( MMatrixObserver* aObserver )
    {
    iObserver = aObserver;
    }

// ----------------------------------------------------------------------------
// CMatrixControl::SetBorder
// ----------------------------------------------------------------------------
//
void CMatrixControl::SetBorder( TBool aBorder )
    {
    iBorder = aBorder;
    }

// ----------------------------------------------------------------------------
// CMatrixControl::SetMatrixSize
// ----------------------------------------------------------------------------
//
void CMatrixControl::SetMatrixSize( TInt aWidth, TInt aHeight )
    {
    iMatrixWidth = aWidth;
    iMatrixHeight = aHeight;
    }

// ----------------------------------------------------------------------------
// CMatrixControl::UpdateMatrixL
// ----------------------------------------------------------------------------
//
void CMatrixControl::UpdateMatrixL( const TEngineAction& aAction )
    {
    CArrayFixFlat<TBlock>* array( NULL );
    CMatrixTask* task = CMatrixTask::NewL( *this, *iScheduler );
    
    // [1]. Apends these blocks onto the task->
    aAction.Get( array );
    for ( TInt i = 0; array != NULL && i < array->Count(); ++i )
        {
        if ( array->At( i ).iY < iMatrixHeight )
            {
            task->Append( array->At( i ) );
            }
        }

    // [2]. Sets the task's property.
    switch( aAction.Type() )
        {
        case EMatrixActionCreated:
            task->SetTaskType( EMatrixTaskDarken );
            break;
        case EMatrixActionMoved:
        	GetRemovableRegion( array );
        case EMatrixActionUpdated:
            task->SetTaskType( EMatrixTaskDrawing );
            break;
        case EMatrixActionRemoved:
            task->SetTaskType( EMatrixTaskClear );
            break;
        case EMatrixActionReleased:
            task->SetTaskType( EMatrixTaskLighten );
            break;
#ifdef RD_TE2_ADVANCE_FUNC
        case EMatrixActionExploded:
            task->SetTaskType( EMatrixTaskClear );
            break;
        case EMatrixActionChanged:
            break;
#endif // RD_TE2_ADVANCE_FUNC
        default:
            break;
        }

    // [3]. Add this task into the schedule
    // Ownship was transfer to scheduler 
    iScheduler->AddTask( task );
    }

// ----------------------------------------------------------------------------
// CMatrixControl::SetTipsL
// ----------------------------------------------------------------------------
//
void CMatrixControl::SetTipsL( const TDesC& aText )
    {
    if ( iTipsText )
        {
        delete iTipsText;
        iTipsText = NULL;
        }
    iTipsText =  HBufC::NewL( aText.Length() );
    TPtr text( iTipsText->Des() );
    text.Copy( aText );
    
    // Get started to draw tips.
    DrawTips();
    StartTipsL();
    }

// ----------------------------------------------------------------------------
// CMatrixControl::StartIndicatorL
// ----------------------------------------------------------------------------
//
void CMatrixControl::StartIndicatorL( TIndicatorType aIndicator )
    {
    iIndicator = aIndicator;

    DrawIndicator();

    if ( EQhdPortrait == CTe2LayoutManager::Resolution() )
        {
        TRect theRect( RealRect() );
        theRect.Shrink( KMatrixMargin, KMatrixMargin );
        if ( theRect.Contains( iStartPoint ) )
            {
            iIndiPos = iStartPoint;
            }
        else
            {
            iIndiPos = theRect.Center();
            }
        }
    else
        {
        iIndiPos = iRemovableRegion.BoundingRect().Center();
        }

    iIndiTimer = KMatrixIndiTimer;
    iIndiRotateOffset = KDefaultRotateOffset;
    
    iIndiPeriodic->Cancel();
    iIndiPeriodic->Start(
                KMatrixIndiDelayTimeout,
                KMatrixIndiUpdateTimeout,
                TCallBack( IndiTimerCallback, this ) );
    }

// ----------------------------------------------------------------------------
// CMatrixControl::Clear
// ----------------------------------------------------------------------------
//
void CMatrixControl::Clear()
    {
    iScheduler->Clear();
    }

// ----------------------------------------------------------------------------
// CMatrixControl::Draw
// ----------------------------------------------------------------------------
//
void CMatrixControl::Draw()
    {
    // Draw border
    if ( iBorder ) 
        {
        DrawBorder();
        }
    // Draw matrix
    if ( iBitmap && iMask )
        {
        iBgc->BitBltMasked( Rect().iTl, iBitmap, 
                TRect( TPoint( 0, 0 ), iMatrixSize ), iMask, EFalse );
        }

    // Draw tips
    if ( iTipsText && iTipsBitmap && iTipsMask )
        {
        iBgc->BitBltMasked( iTipsPos, iTipsBitmap, 
                TRect( TPoint( 0, 0 ), iTipsSize ), iTipsMask, EFalse );
        }
        
    // Draw indicator
    if ( iIndicator != EIndicatorNone && iTipsBitmap && iTipsMask )
        {
        iBgc->BitBltMasked( iIndiPos, iIndicatorBitmap, 
                TRect( TPoint( 0, 0 ), iIndiSize ), iIndicatorMask, ETrue );
        }
    }

// ----------------------------------------------------------------------------
// CMatrixControl::CMatrixControl
// ----------------------------------------------------------------------------
//
CMatrixControl::CMatrixControl( CFbsBitGc*& aBgc )
: iBgc( aBgc ), iBorder( EFalse )
    { 
    // No implementation required.
    }

// ----------------------------------------------------------------------------
// CMatrixControl::ConstructL
// ----------------------------------------------------------------------------
//
void CMatrixControl::ConstructL( const TRect& aRect, const TSize& aMatrixSize )
    {
	// Create a window for this application view
    CreateWindowL();

    iSvgUtils = CSvgUtils::NewL( KMatrixSvgFileName,
            EMbmMatrix_svgRed,
            EMbmMatrix_svgLastElement );

    // Set the windows size
    SetMatrixSize( aMatrixSize.iWidth, aMatrixSize.iHeight );
    SetRect( aRect );

    TFontSpec fontSpec = iEikonEnv->LegendFont()->FontSpecInTwips();
    fontSpec.iFontStyle.SetBitmapType( EMonochromeGlyphBitmap );

    if ( EQhdPortrait == CTe2LayoutManager::Resolution() )
        {
        fontSpec.iHeight  -= fontSpec.iHeight / 6;
        iBorderPenSize = KQhdBorderPenSize;
        }
    else
        {
        fontSpec.iHeight  -= fontSpec.iHeight / 4;
        iBorderPenSize = KQvgaBorderPenSize;
        }

    User::LeaveIfError( 
    		iEikonEnv->ScreenDevice()->GetNearestFontInTwips( iTipsFont, fontSpec ) 
    		);

    iScheduler = CMatrixTaskScheduler::NewL();

    iIndiPeriodic = CPeriodic::NewL( CActive::EPriorityStandard );
    iIndiPeriodic->Cancel();

    if ( EQhdPortrait == CTe2LayoutManager::Resolution() )
        {
        // Enables handling of drag events
        EnableDragEvents();
        }

    // Activate the window, which makes it ready to be drawn
    ActivateL();
    }

// ----------------------------------------------------------------------------
// CMatrixControl::DrawBorder
// ----------------------------------------------------------------------------
//
void CMatrixControl::DrawBorder() const
    {
    TPoint tl = TPoint( 0, 0 );
    TPoint br;
    br.iX = tl.iX + iBlockSize.iWidth * iMatrixWidth - 1;
    br.iY = tl.iY + iBlockSize.iHeight * iMatrixHeight - 1;
    TPoint tr = TPoint( br.iX, tl.iY );
    TPoint bl = TPoint( tl.iX, br.iY );

    iBitmapGc->SetPenColor( KRgbBlack );
    iBitmapGc->SetPenStyle( CGraphicsContext::ESolidPen );
    iBitmapGc->SetPenSize( TSize( iBorderPenSize, iBorderPenSize ) );
    iMaskGc->SetPenColor( KRgbGray );
    iMaskGc->SetPenStyle( CGraphicsContext::ESolidPen );
    iMaskGc->SetPenSize( TSize( iBorderPenSize, iBorderPenSize ) );

    // From topleft to bottomleft.
    iBitmapGc->DrawLine( tl, bl );
    iMaskGc->DrawLine( tl, bl );
    // From topleft to topright
    iBitmapGc->DrawLine( tl, tr );
    iMaskGc->DrawLine( tl, tr );
    // From topright to bottomright
    iBitmapGc->DrawLine( tr, br );
    iMaskGc->DrawLine( tr, br );
    // From bottomleft to bottomright
    iBitmapGc->DrawLine( bl, br );
    iMaskGc->DrawLine( bl, br );
    }

// ----------------------------------------------------------------------------
// CMatrixControl::DrawTips
// ----------------------------------------------------------------------------
//
void CMatrixControl::DrawTips() const
    {
    iTipsBitmapGc->SetBrushStyle( CGraphicsContext::ESolidBrush );
    iTipsBitmapGc->SetBrushColor( KRgbBlack );
    iTipsBitmapGc->SetPenStyle( CGraphicsContext::ESolidPen );
    iTipsBitmapGc->SetPenColor( KRgbRed );
    iTipsBitmapGc->DrawRect( TRect( TPoint( 0, 0 ), iTipsSize ) );
    iTipsMaskGc->SetBrushStyle( CGraphicsContext::ESolidBrush );
    iTipsMaskGc->SetBrushColor( KRgbBlack );
    iTipsMaskGc->SetPenStyle( CGraphicsContext::ESolidPen );
    iTipsMaskGc->SetPenColor( KRgbWhite );
    iTipsMaskGc->Clear( TRect( TPoint( 0, 0 ), iTipsSize ) );

    TRect tipsRect( TPoint( 0, 0 ), iTipsSize );
    TInt baseline = 
        ( tipsRect.Height() + iTipsFont->HeightInPixels() ) / 2;

    iTipsBitmapGc->UseFont( iTipsFont );
    iTipsBitmapGc->DrawText( *iTipsText, tipsRect, baseline, CGraphicsContext::ELeft, 0 );
    iTipsBitmapGc->DiscardFont();
    iTipsMaskGc->UseFont( iTipsFont );
    iTipsMaskGc->DrawText( *iTipsText, tipsRect, baseline, CGraphicsContext::ELeft, 0 );
    iTipsMaskGc->DiscardFont();
    }

// ----------------------------------------------------------------------------
// CMatrixControl::DrawIndicator
// ----------------------------------------------------------------------------
//
void CMatrixControl::DrawIndicator() const
    {
    iIndicatorBitmapGc->SetBrushStyle( CGraphicsContext::ESolidBrush );
    iIndicatorBitmapGc->SetBrushColor( KRgbBlack );
    iIndicatorBitmapGc->Clear( TRect( TPoint( 0, 0 ), iIndiSize ) );
    iIndicatorMaskGc->SetBrushStyle( CGraphicsContext::ESolidBrush );
    iIndicatorMaskGc->SetBrushColor( KRgbBlack );
    iIndicatorMaskGc->Clear( TRect( TPoint( 0, 0 ), iIndiSize ) );

    switch ( iIndicator )
        {
        case EIndicatorNone:
            break;
        case EIndicatorLeft:
            iIndicatorBitmapGc->BitBlt( 
                    TPoint( 0, 0 ),
                    iSvgUtils->Bitmap( EMbmMatrix_svgLeft ) );
            iIndicatorMaskGc->BitBlt( 
                    TPoint( 0, 0 ),
                    iSvgUtils->Bitmap( EMbmMatrix_svgLeft_mask ) );
            break;
        case EIndicatorRight:
            iIndicatorBitmapGc->BitBlt( 
                    TPoint( 0, 0 ),
                    iSvgUtils->Bitmap( EMbmMatrix_svgRight ) );
            iIndicatorMaskGc->BitBlt( 
                    TPoint( 0, 0 ),
                    iSvgUtils->Bitmap( EMbmMatrix_svgRight_mask ) );
            break;
        case EIndicatorDescend:
            iIndicatorBitmapGc->BitBlt( 
                    TPoint( 0, 0 ),
                    iSvgUtils->Bitmap( EMbmMatrix_svgDescend ) );
            iIndicatorMaskGc->BitBlt( 
                    TPoint( 0, 0 ),
                    iSvgUtils->Bitmap( EMbmMatrix_svgDescend_mask ) );
            break;
        case EIndicatorRotate:
            iIndicatorBitmapGc->BitBlt( 
                    TPoint( 0, 0 ),
                    iSvgUtils->Bitmap( EMbmMatrix_svgRotate0 
                    + iIndiRotateOffset * 4 ) );
            iIndicatorMaskGc->BitBlt( 
                    TPoint( 0, 0 ),
                    iSvgUtils->Bitmap( EMbmMatrix_svgRotate0_mask 
                    + iIndiRotateOffset * 4 ) );
            break;
        default:
            break;
        }
    }

// ----------------------------------------------------------------------------
// CMatrixControl::CreateBmpBufferL
// ----------------------------------------------------------------------------
//
void CMatrixControl::CreateBmpBufferL()
    {
    // If double buffer resouces has been 
    // created, release them at first.
    ReleaseBmpBuffer();

    // For Matrix
    // Create double buffer bitmap
    iBitmap = new (ELeave) CFbsBitmap;
    User::LeaveIfError( iBitmap->Create( Size(), EColor16MAP ) );

    // Create double buffer graphics context
    iBitmapDevice = CFbsBitmapDevice::NewL( iBitmap );
    User::LeaveIfError( iBitmapDevice->CreateContext( iBitmapGc ) );

    // Create double buffer mask
    iMask = new (ELeave) CFbsBitmap;
    User::LeaveIfError( iMask->Create( Size(), EColor16MAP ) );

    // Create double buffer graphics context
    iMaskDevice = CFbsBitmapDevice::NewL( iMask );
    User::LeaveIfError( iMaskDevice->CreateContext( iMaskGc ) );

    iMaskGc->SetPenStyle( CGraphicsContext::ESolidPen );
    iMaskGc->SetPenColor( KRgbBlack );
    iMaskGc->SetBrushStyle( CGraphicsContext::ESolidBrush );
    iMaskGc->SetBrushColor( KRgbBlack );
    iMaskGc->DrawRect( TRect( TPoint( 0,0 ), Rect().Size() ) );


    // For Tips
    // Create double buffer bitmap
    iTipsBitmap = new (ELeave) CFbsBitmap;
    User::LeaveIfError( iTipsBitmap->Create( iTipsSize, EColor16MAP ) );

    // Create double buffer graphics context
    iTipsBitmapDevice = CFbsBitmapDevice::NewL( iTipsBitmap );
    User::LeaveIfError( iTipsBitmapDevice->CreateContext( iTipsBitmapGc ) );

    // Create double buffer mask
    iTipsMask = new (ELeave) CFbsBitmap;
    User::LeaveIfError( iTipsMask->Create( iTipsSize, EColor16MAP ) );

    // Create double buffer graphics context
    iTipsMaskDevice = CFbsBitmapDevice::NewL( iTipsMask );
    User::LeaveIfError( iTipsMaskDevice->CreateContext( iTipsMaskGc ) );


    // For Indicator
    // Create double buffer bitmap
    iIndicatorBitmap = new (ELeave) CFbsBitmap;
    User::LeaveIfError( iIndicatorBitmap->Create( iIndiSize, EColor16MAP ) );

    // Create double buffer graphics context
    iIndicatorBitmapDevice = CFbsBitmapDevice::NewL( iIndicatorBitmap );
    User::LeaveIfError( iIndicatorBitmapDevice->CreateContext( iIndicatorBitmapGc ) );

    // Create double buffer mask
    iIndicatorMask = new (ELeave) CFbsBitmap;
    User::LeaveIfError( iIndicatorMask->Create( iIndiSize, EColor16MAP ) );

    // Create double buffer graphics context
    iIndicatorMaskDevice = CFbsBitmapDevice::NewL( iIndicatorMask );
    User::LeaveIfError( iIndicatorMaskDevice->CreateContext( iIndicatorMaskGc ) );
    }

// ----------------------------------------------------------------------------
// CMatrixControl::ReleaseBmpBuffer
// ----------------------------------------------------------------------------
//
void CMatrixControl::ReleaseBmpBuffer()
    {
    // For Matrix
    if ( iBitmapGc )
        {
        delete iBitmapGc;
        iBitmapGc = NULL;
        }
    if ( iBitmapDevice )
        {
        delete iBitmapDevice;
        iBitmapDevice = NULL;
        }
    if ( iBitmap )
        {
        delete iBitmap;
        iBitmap = NULL;
        }
    if ( iMaskGc )
        {
        delete iMaskGc;
        iMaskGc = NULL;
        }
    if ( iMaskDevice )
        {
        delete iMaskDevice;
        iMaskDevice = NULL;
        }
    if ( iMask )
        {
        delete iMask;
        iMask = NULL;
        }

    // For Tips
    if ( iTipsBitmapGc )
        {
        delete iTipsBitmapGc;
        iTipsBitmapGc = NULL;
        }
    if ( iTipsBitmapDevice )
        {
        delete iTipsBitmapDevice;
        iTipsBitmapDevice = NULL;
        }
    if ( iTipsBitmap )
        {
        delete iTipsBitmap;
        iTipsBitmap = NULL;
        }
    if ( iTipsMaskGc )
        {
        delete iTipsMaskGc;
        iTipsMaskGc = NULL;
        }
    if ( iTipsMaskDevice )
        {
        delete iTipsMaskDevice;
        iTipsMaskDevice = NULL;
        }
    if ( iTipsMask )
        {
        delete iTipsMask;
        iTipsMask = NULL;
        }
        
    // For Indicator
    if ( iIndicatorBitmapGc )
        {
        delete iIndicatorBitmapGc;
        iIndicatorBitmapGc = NULL;
        }
    if ( iIndicatorBitmapDevice )
        {
        delete iIndicatorBitmapDevice;
        iIndicatorBitmapDevice = NULL;
        }
    if ( iIndicatorBitmap )
        {
        delete iIndicatorBitmap;
        iIndicatorBitmap = NULL;
        }
    if ( iIndicatorMaskGc )
        {
        delete iIndicatorMaskGc;
        iIndicatorMaskGc = NULL;
        }
    if ( iIndicatorMaskDevice )
        {
        delete iIndicatorMaskDevice;
        iIndicatorMaskDevice = NULL;
        }
    if ( iIndicatorMask )
        {
        delete iIndicatorMask;
        iIndicatorMask = NULL;
        }
    }

// ----------------------------------------------------------------------------
// CMatrixControl::TimerCallback
// ----------------------------------------------------------------------------
//
TInt CMatrixControl::TimerCallback( TAny* aPtr )
    {
    CMatrixControl* self = static_cast<CMatrixControl*> ( aPtr );
    self->MoveTips();
    return 0;
    }

// ----------------------------------------------------------------------------
// CMatrixControl::IndiTimerCallback
// ----------------------------------------------------------------------------
//
TInt CMatrixControl::IndiTimerCallback( TAny* aPtr )
    {
    CMatrixControl* self = static_cast<CMatrixControl*> ( aPtr );
    self->MoveIndicator();
    return 0;
    }

// ----------------------------------------------------------------------------
// CMatrixControl::MoveTips
// ----------------------------------------------------------------------------
//
void CMatrixControl::MoveTips()
    {
    if ( --iTipsTimer )
        {
        iTipsPos.iX += 1;
        iTipsPos.iY -= 1;
        }
    else
        {
        StopTips();
        delete iTipsText;
        iTipsText = NULL;
        }
    }

// ----------------------------------------------------------------------------
// CMatrixControl::MoveIndicator
// ----------------------------------------------------------------------------
//
void CMatrixControl::MoveIndicator()
    {
    if ( --iIndiTimer )
        {
        switch ( iIndicator )
            {
            case EIndicatorNone:
                break;
            case EIndicatorLeft:
                iIndiPos.iX -= 1;
                break;
            case EIndicatorRight:
                iIndiPos.iX += 1;
                break;
            case EIndicatorDescend:
                iIndiPos.iY += 1;
                break;
            case EIndicatorRotate:
                iIndiRotateOffset += 1;
                if ( iIndiRotateOffset > KMaxRotateOffset )
                    {
                    iIndiRotateOffset = KDefaultRotateOffset;
                    }
                DrawIndicator();
                break;
            default:
                break;
            }
        }
    else
        {
        iIndiPeriodic->Cancel();
        iIndicator = EIndicatorNone;
        }
    }

// ----------------------------------------------------------------------------
// CMatrixControl::StartTipsL
// ----------------------------------------------------------------------------
//
void CMatrixControl::StartTipsL()
    {
    StopTips();
    StopIndicator();

    iTipsPos.iX = iMatrixSize.iWidth / 2;
    iTipsPos.iY = iMatrixSize.iHeight / 2;
    iTipsTimer = KMatrixTipsTimer;

    iPeriodic = CPeriodic::NewL( CActive::EPriorityStandard );
    iPeriodic->Cancel();
    iPeriodic->Start(
                KMatrixTipsDelayTimeout,
                KMatrixTipsUpdateTimeout,
                TCallBack( TimerCallback, this ) );
    }

// ----------------------------------------------------------------------------
// CMatrixControl::StopTips
// ----------------------------------------------------------------------------
//
void CMatrixControl::StopTips()
    {
    if ( iPeriodic )
        {  
        iPeriodic->Cancel();
        delete iPeriodic;
        iPeriodic = NULL;   
        }
    }

// ----------------------------------------------------------------------------
// CMatrixControl::StopIndicator
// ----------------------------------------------------------------------------
//
void CMatrixControl::StopIndicator()
    {
    if ( iIndicator != EIndicatorNone && iIndiPeriodic )
        {  
        iIndiPeriodic->Cancel();
        iIndicator = EIndicatorNone;
        }
    }

// ----------------------------------------------------------------------------
// CMatrixControl::GetRemovableRegion
// ----------------------------------------------------------------------------
//
void CMatrixControl::GetRemovableRegion( const CArrayFixFlat<TBlock>* aArray )
    {
    iRemovableRegion.Clear();
    if ( aArray )
        {
        TRect rect( Rect() );
        for ( TInt i = 0; i < aArray->Count(); i++ )
            {
            TBlock block = aArray->At( i );
            TInt x = rect.iTl.iX 
                + block.iX * iBlockSize.iWidth;
            TInt y = rect.iTl.iY 
                + ( iMatrixHeight - block.iY - 1 ) * iBlockSize.iHeight;
            iRemovableRegion.AddRect( TRect( TPoint( x, y ), iBlockSize ) );
            }
        }
    }

// ----------------------------------------------------------------------------
// CMatrixControl::Direction
// ----------------------------------------------------------------------------
//
CMatrixControl::TDirection CMatrixControl::Direction( const TRect& aRect, 
    const TPoint& aPos ) const
    {
    TDirection direction;
    if ( ( aRect.iTl.iX < aPos.iX ) 
        && ( aRect.iBr.iX > aPos.iX ) 
        && ( aRect.iTl.iY > aPos.iY ) )
        {
        direction = ETop;
        }
    else if ( ( aRect.iTl.iX < aPos.iX ) 
        && ( aRect.iBr.iX > aPos.iX ) 
        && ( aRect.iBr.iY < aPos.iY ) )
        {
        direction = EBottom;
        }
    else if ( ( aRect.iTl.iX > aPos.iX ) 
        && ( aRect.iTl.iY < aPos.iY ) 
        && ( aRect.iBr.iY > aPos.iY ) )
        {
        direction = ELeft;
        }
    else if ( ( aRect.iBr.iX < aPos.iX ) 
        && ( aRect.iTl.iY < aPos.iY ) 
        && ( aRect.iBr.iY > aPos.iY ) )
        {
        direction = ERight;
        }
    else if ( ( aRect.iTl.iX > aPos.iX ) 
        && ( aRect.iTl.iY > aPos.iY ) )
        {
        direction = ETopLeft;
        }
    else if ( ( aRect.iBr.iX < aPos.iX ) 
        && ( aRect.iTl.iY > aPos.iY ) )
        {
        direction = ETopRight;
        }
    else if ( ( aRect.iTl.iX > aPos.iX ) 
        && ( aRect.iBr.iY < aPos.iY ) )
        {
        direction = EBottomLeft;
        }
    else if ( ( aRect.iBr.iX < aPos.iX ) 
        && ( aRect.iBr.iY < aPos.iY )  )
        {
        direction = EBottomRight;
        }
    else
        {
        direction = ECenter;
        }

    return direction;
    }

// ----------------------------------------------------------------------------
// CMatrixControl::GestureArea
// ----------------------------------------------------------------------------
//
CMatrixControl::TGestureArea CMatrixControl::GestureArea( 
        const TPoint& aStartPoint, const TPoint& aEndPoint ) const
    {
    if ( aStartPoint.iY >= aEndPoint.iY
        && ( Abs( aEndPoint.iX - aStartPoint.iX ) 
            < Abs( aEndPoint.iY - aStartPoint.iY ) ) )
        {
        return EGestureAreaA;
        }
    
    if ( aStartPoint.iX > aEndPoint.iX
        && ( Abs( aEndPoint.iX - aStartPoint.iX ) 
            > Abs( aEndPoint.iY - aStartPoint.iY ) ) )
        {
        return EGestureAreaB;
        }
    
    if ( aStartPoint.iY <= aEndPoint.iY
        && ( Abs( aEndPoint.iX - aStartPoint.iX ) 
            < Abs( aEndPoint.iY - aStartPoint.iY ) ) )
        {
        return EGestureAreaC;
        }

    if ( aStartPoint.iX < aEndPoint.iX
        && ( Abs( aEndPoint.iX - aStartPoint.iX ) 
            > Abs( aEndPoint.iY - aStartPoint.iY ) ) )
        {
        return EGestureAreaD;
        }

    return EGestureAreaA;
    }

// ----------------------------------------------------------------------------
// CMatrixControl::HandleDirectionEventL
// ----------------------------------------------------------------------------
//
void CMatrixControl::HandleDirectionEventL( TDirection aDirection )
    {
    switch ( aDirection )
        {
        case EBottom:
            iObserver->NotifyL( EMatrixDescend );
            break;

        case ELeft:
        case ETopLeft:
        case EBottomLeft:
            iObserver->NotifyL( EMatrixMoveLeft );
            break;

        case ERight:
        case ETopRight:
        case EBottomRight:
            iObserver->NotifyL( EMatrixMoveRight );
            break;

        case ETop:
        case ECenter:
            iObserver->NotifyL( EMatrixRotate );
            break;

        default:
            break;
        }
    }

// ----------------------------------------------------------------------------
// CMatrixControl::HandleGestureAreaEventL
// ----------------------------------------------------------------------------
//
void CMatrixControl::HandleGestureAreaEventL( TGestureArea aGestureArea )
    {
    switch ( aGestureArea )
        {
        case EGestureAreaA:
            StartIndicatorL( EIndicatorRotate );
            iObserver->NotifyL( EMatrixRotate );
            break;
        case EGestureAreaB:
            StartIndicatorL( EIndicatorLeft );
            iObserver->NotifyL( EMatrixMoveLeft );
            break;
        case EGestureAreaC:
            StartIndicatorL( EIndicatorDescend );
            iObserver->NotifyL( EMatrixDoulbeDescend );
            break;
        case EGestureAreaD:
            StartIndicatorL( EIndicatorRight );
            iObserver->NotifyL( EMatrixMoveRight );
            break;
        default:
            break;
        }
    }

// ----------------------------------------------------------------------------
// CMatrixControl::RealRect
// ----------------------------------------------------------------------------
//
TRect CMatrixControl::RealRect() const
    {
    TRect theRect;
    TPoint br;
    br.iX = Rect().iTl.iX + iBlockSize.iWidth * iMatrixWidth - 1;
    br.iY = Rect().iTl.iY + iBlockSize.iHeight * iMatrixHeight - 1;
    theRect.iTl = Rect().iTl;
    theRect.iBr = br;

    return theRect;
    }

// End of File
