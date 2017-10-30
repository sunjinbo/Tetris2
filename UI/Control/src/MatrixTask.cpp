/* ====================================================================
 * File: MatrixTask.cpp
 * Created: 04/19/09
 * Modified: 04/19/09
 * Author: Sun Jinbo
 * Copyright (c): Tieto, All rights reserved
 * ==================================================================== */

// INCLUDE FILES
#include <e32math.h>
#include <SvgUtils.h>
#include <matrix_svg.mbg>
#include "MatrixTask.h"
#include "MatrixControl.h"
#include "MatrixTaskObserver.h"

// CONSTANTS
const TInt KMatrixTaskArrayCranularity = 4;
const TInt KMatrixTaskGradientTimeOut = 100000;
const TInt KMatrixTaskCollapsedTimeOut = 200000;
const TInt KMatrixTaskGradientFrequency = 64;
const TInt KMatrixTaskFullTransparency = 0;
const TInt KMatrixTaskSemiTransparency = 180;
const TInt KMatrixTaskNonTransparency = 255;
const TInt KMatrixTaskCollapsedOffset = 8;

// ======== MEMBER FUNCTIONS ========

// ----------------------------------------------------------------------------
// CMatrixTask::NewL
// ----------------------------------------------------------------------------
//
CMatrixTask* CMatrixTask::NewL( CMatrixControl& aControl, 
    MMatrixTaskObserver& aObserver )
    {
    CMatrixTask* self = new (ELeave) CMatrixTask( aControl, aObserver );
    CleanupStack::PushL (self );
    self->ConstructL ();
    CleanupStack::Pop();
    return self;
    }

// ----------------------------------------------------------------------------
// CMatrixTask::~CMatrixTask
// ----------------------------------------------------------------------------
//
CMatrixTask::~CMatrixTask()
    {
    Cancel();
    iArray.Reset();
    iArray.Close();
    }

// ----------------------------------------------------------------------------
// CMatrixTask::Append
// ----------------------------------------------------------------------------
//
void CMatrixTask::Append( const TBlock& aBlock )
    {
    if ( ! IsActive() )
        {
        iArray.Append( aBlock );
        }
    }

// ----------------------------------------------------------------------------
// CMatrixTask::Start
// ----------------------------------------------------------------------------
//
void CMatrixTask::Start()
    {
    if ( ! IsActive() )
        {
        switch( iType )
            {
            case EMatrixTaskNone:
                {
                // In here, it should be a PANIC
                iObserver.TaskCompleted(); // for debug
                }
                break;
            case EMatrixTaskDarken:
                {
                iOpacity = KMatrixTaskFullTransparency;
                CTimer::After( KMatrixTaskGradientTimeOut );
                }
                break;
            case EMatrixTaskLighten:
                {
                iOpacity = KMatrixTaskNonTransparency;
                CTimer::After( KMatrixTaskGradientTimeOut );
                }
                break;
            case EMatrixTaskDrawing:
                {
                iOpacity = KMatrixTaskSemiTransparency;
                DrawMatrix();
                iObserver.TaskCompleted();
                }
                break;
            case EMatrixTaskClear:
                {
                iOpacity = KMatrixTaskFullTransparency;
                DrawMatrix();
                iObserver.TaskCompleted();
                }
                break;
            case EMatrixTaskBlasting:
                { 
                // Do nothing
                }
                break;
            case EMatrixTaskCollapsed:
                {
                iOpacity = KMatrixTaskFullTransparency;
                DrawMatrix();
                iCollapsed = ETrue;
                iOpacity = KMatrixTaskNonTransparency;
                CTimer::After( KMatrixTaskCollapsedTimeOut );
                }
                break;
            default:
                break;
            }
        }
    }

// ----------------------------------------------------------------------------
// CMatrixTask::SetTaskType
// ----------------------------------------------------------------------------
//
void CMatrixTask::SetTaskType( TMatrixTaskType aType )
    {
    iType = aType;
    }

// ----------------------------------------------------------------------------
// CMatrixTask::CMatrixTask
// ----------------------------------------------------------------------------
//
CMatrixTask::CMatrixTask( CMatrixControl& aControl, 
    MMatrixTaskObserver& aObserver )
    : CTimer( EPriorityStandard ),
    iObserver( aObserver ),
    iControl( aControl ),
    iType( EMatrixTaskNone ),
    iArray( KMatrixTaskArrayCranularity )
    {
    CActiveScheduler::Add( this );
    }

// ----------------------------------------------------------------------------
// CMatrixTask::ConstructL
// ----------------------------------------------------------------------------
//
void CMatrixTask::ConstructL()
    {
    CTimer::ConstructL();
    }

// ----------------------------------------------------------------------------
// CMatrixTask::RunL
// ----------------------------------------------------------------------------
//
void CMatrixTask::RunL()
    {
    if ( iStatus == KErrNone )
        {
        switch ( iType )
            {
            case EMatrixTaskDarken:
                {
                iOpacity += KMatrixTaskGradientFrequency;
                if ( iOpacity >=  KMatrixTaskNonTransparency )
                    {
                    iOpacity = KMatrixTaskNonTransparency;
                    DrawMatrix();
                    iObserver.TaskCompleted();
                    }
                else
                    {
                    DrawMatrix();    
                    CTimer::After( KMatrixTaskGradientTimeOut );
                    }
                }
                break;
           case EMatrixTaskLighten:
                {
                iOpacity -= KMatrixTaskGradientFrequency;
                if ( iOpacity <=  KMatrixTaskFullTransparency )
                    {
                    iOpacity = KMatrixTaskFullTransparency;
                    DrawMatrix();
                    iObserver.TaskCompleted();
                    }
                else
                    {
                    DrawMatrix();    
                    CTimer::After( KMatrixTaskGradientTimeOut );
                    }
                }
                break;
            case EMatrixTaskDrawing: // fall through
            case EMatrixTaskClear: // fall through
            case EMatrixTaskBlasting: // fall through
                break;
            case EMatrixTaskCollapsed:
                {
                iOpacity -= KMatrixTaskGradientFrequency;
                if ( iOpacity <=  KMatrixTaskFullTransparency )
                    {
                    iOpacity = KMatrixTaskFullTransparency;
                    DrawMatrix();
                    iCollapsed = EFalse;
                    DrawMatrix();
                    iObserver.TaskCompleted();
                    }
                else
                    {
                    DrawMatrix();
                    CTimer::After( KMatrixTaskCollapsedTimeOut );
                    }
                }
                break;
            default:
                break;
            }
        }
    }

// ----------------------------------------------------------------------------
// CMatrixTask::DrawMatrix
// ----------------------------------------------------------------------------
//
void CMatrixTask::DrawMatrix()
    {
    for ( TInt i = 0; i < iArray.Count(); ++i )
        {
        TInt offset(0);
        if ( iCollapsed )
            {
            offset = Math::Random() % KMatrixTaskCollapsedOffset;
            }
        DrawBlock( iArray[ i ], offset );
        DrawMask( iArray[ i ], offset );
        }
    }

// ----------------------------------------------------------------------------
// CMatrixTask::DrawBlock
// ----------------------------------------------------------------------------
//
void CMatrixTask::DrawBlock( const TBlock& aBlock, TInt aOffsetY )
    {
    TPoint topLeft;
    topLeft.iX = aBlock.iX * iControl.iBlockSize.iWidth; 
    topLeft.iY = ( iControl.iMatrixHeight - 1 - aBlock.iY ) * iControl.iBlockSize.iHeight + aOffsetY; 

#ifdef RD_TE2_ADVANCE_FUNC
    switch ( aBlock.iType )
        {
        case ERed:
            {
            iControl.iBitmapGc->BitBltMasked( topLeft,
                iControl.iSvgUtils->Bitmap( EMbmMatrix_svgRed ),
                TRect( TPoint( 0, 0 ), iControl.iBlockSize ),
                iControl.iSvgUtils->Bitmap( EMbmMatrix_svgRed_mask ),
                ETrue );
            }
            break;
        case EMagenta:
            {
            iControl.iBitmapGc->BitBltMasked( topLeft,
                iControl.iSvgUtils->Bitmap( EMbmMatrix_svgMagenta ),
                TRect( TPoint( 0, 0 ), iControl.iBlockSize ),
                iControl.iSvgUtils->Bitmap( EMbmMatrix_svgMagenta_mask ),
                ETrue );
            }
            break;
        case EYellow:
            {
            iControl.iBitmapGc->BitBltMasked( topLeft,
                iControl.iSvgUtils->Bitmap( EMbmMatrix_svgYellow ),
                TRect( TPoint( 0, 0 ), iControl.iBlockSize ),
                iControl.iSvgUtils->Bitmap( EMbmMatrix_svgYellow_mask ),
                ETrue );
            }
            break;
        case ECyan:
            {
            iControl.iBitmapGc->BitBltMasked( topLeft,
                iControl.iSvgUtils->Bitmap( EMbmMatrix_svgCyan ),
                TRect( TPoint( 0, 0 ), iControl.iBlockSize ),
                iControl.iSvgUtils->Bitmap( EMbmMatrix_svgCyan_mask ),
                ETrue );
            }
            break;
        case EBlue:
            {
            iControl.iBitmapGc->BitBltMasked( topLeft,
                iControl.iSvgUtils->Bitmap( EMbmMatrix_svgBlue ),
                TRect( TPoint( 0, 0 ), iControl.iBlockSize ),
                iControl.iSvgUtils->Bitmap( EMbmMatrix_svgBlue_mask ),
                ETrue );
            }
            break;
        case EDarkBlue:
            {
            iControl.iBitmapGc->BitBltMasked( topLeft,
                iControl.iSvgUtils->Bitmap( EMbmMatrix_svgDarkblue ),
                TRect( TPoint( 0, 0 ), iControl.iBlockSize ),
                iControl.iSvgUtils->Bitmap( EMbmMatrix_svgDarkblue_mask ),
                ETrue );
            }
            break;
        case EGreen:
            {
            iControl.iBitmapGc->BitBltMasked( topLeft,
                iControl.iSvgUtils->Bitmap( EMbmMatrix_svgGreen ),
                TRect( TPoint( 0, 0 ), iControl.iBlockSize ),
                iControl.iSvgUtils->Bitmap( EMbmMatrix_svgGreen_mask ),
                ETrue );
            }
            break;
        case EOlive:
            {
            iControl.iBitmapGc->BitBltMasked( topLeft,
                iControl.iSvgUtils->Bitmap( EMbmMatrix_svgOlive ),
                TRect( TPoint( 0, 0 ), iControl.iBlockSize ),
                iControl.iSvgUtils->Bitmap( EMbmMatrix_svgOlive_mask ),
                ETrue );
            }
            break;
        case EPurple:
            {
            iControl.iBitmapGc->BitBltMasked( topLeft,
                iControl.iSvgUtils->Bitmap( EMbmMatrix_svgPurple ),
                TRect( TPoint( 0, 0 ), iControl.iBlockSize ),
                iControl.iSvgUtils->Bitmap( EMbmMatrix_svgPurple_mask ),
                ETrue );
            }
            break;
        case EOrange:
            {
            iControl.iBitmapGc->BitBltMasked( topLeft,
                iControl.iSvgUtils->Bitmap( EMbmMatrix_svgOrange ),
                TRect( TPoint( 0, 0 ), iControl.iBlockSize ),
                iControl.iSvgUtils->Bitmap( EMbmMatrix_svgOrange_mask ),
                ETrue );
            }
            break;
        case EBrown:
            {
            iControl.iBitmapGc->BitBltMasked( topLeft,
                iControl.iSvgUtils->Bitmap( EMbmMatrix_svgBrown ),
                TRect( TPoint( 0, 0 ), iControl.iBlockSize ),
                iControl.iSvgUtils->Bitmap( EMbmMatrix_svgBrown_mask ),
                ETrue );
            }
            break;
        case ELightGrey:
            {
            iControl.iBitmapGc->BitBltMasked( topLeft,
                iControl.iSvgUtils->Bitmap( EMbmMatrix_svgLightgrey ),
                TRect( TPoint( 0, 0 ), iControl.iBlockSize ),
                iControl.iSvgUtils->Bitmap( EMbmMatrix_svgLightgrey_mask ),
                ETrue );
            }
            break;
        case EGray:
            {
            iControl.iBitmapGc->BitBltMasked( topLeft,
                iControl.iSvgUtils->Bitmap( EMbmMatrix_svgGray ),
                TRect( TPoint( 0, 0 ), iControl.iBlockSize ),
                iControl.iSvgUtils->Bitmap( EMbmMatrix_svgGray_mask ),
                ETrue );
            }
            break;
        case EBomb:
            {
            iControl.iBitmapGc->BitBltMasked( topLeft,
                iControl.iSvgUtils->Bitmap( EMbmMatrix_svgBomb ),
                TRect( TPoint( 0, 0 ), iControl.iBlockSize ),
                iControl.iSvgUtils->Bitmap( EMbmMatrix_svgBomb_mask ),
                ETrue );
            }
            break;
        case EIncScore200:
            {
            iControl.iBitmapGc->BitBltMasked( topLeft,
                iControl.iSvgUtils->Bitmap( EMbmMatrix_svgIncscore200 ),
                TRect( TPoint( 0, 0 ), iControl.iBlockSize ),
                iControl.iSvgUtils->Bitmap( EMbmMatrix_svgIncscore200_mask ),
                ETrue );
            }
            break;
        case EIncScore400:
            {
            iControl.iBitmapGc->BitBltMasked( topLeft,
                iControl.iSvgUtils->Bitmap( EMbmMatrix_svgIncscore400 ),
                TRect( TPoint( 0, 0 ), iControl.iBlockSize ),
                iControl.iSvgUtils->Bitmap( EMbmMatrix_svgIncscore400_mask ),
                ETrue );
            }
            break;
        case ERemoveLine1:
            {
            iControl.iBitmapGc->BitBltMasked( topLeft,
                iControl.iSvgUtils->Bitmap( EMbmMatrix_svgRemoveline1 ),
                TRect( TPoint( 0, 0 ), iControl.iBlockSize ),
                iControl.iSvgUtils->Bitmap( EMbmMatrix_svgRemoveline1_mask ),
                ETrue );
            }
            break;
        case ERemoveLine2:
            {
            iControl.iBitmapGc->BitBltMasked( topLeft,
                iControl.iSvgUtils->Bitmap( EMbmMatrix_svgRemoveline2 ),
                TRect( TPoint( 0, 0 ), iControl.iBlockSize ),
                iControl.iSvgUtils->Bitmap( EMbmMatrix_svgRemoveline2_mask ),
                ETrue );
            }
            break;
        case ERemoveLine3:
            {
            iControl.iBitmapGc->BitBltMasked( topLeft,
                iControl.iSvgUtils->Bitmap( EMbmMatrix_svgRemoveline3 ),
                TRect( TPoint( 0, 0 ), iControl.iBlockSize ),
                iControl.iSvgUtils->Bitmap( EMbmMatrix_svgRemoveline3_mask ),
                ETrue );
            }
            break;
        case ERemoveLine4:
            {
            iControl.iBitmapGc->BitBltMasked( topLeft,
                iControl.iSvgUtils->Bitmap( EMbmMatrix_svgRemoveline4 ),
                TRect( TPoint( 0, 0 ), iControl.iBlockSize ),
                iControl.iSvgUtils->Bitmap( EMbmMatrix_svgRemoveline4_mask ),
                ETrue );
            }
            break;
        case EIncLife:
            {
            iControl.iBitmapGc->BitBltMasked( topLeft,
                iControl.iSvgUtils->Bitmap( EMbmMatrix_svgInclife ),
                TRect( TPoint( 0, 0 ), iControl.iBlockSize ),
                iControl.iSvgUtils->Bitmap( EMbmMatrix_svgInclife_mask ),
                ETrue );
            }
            break;
        default:
            break;
        }
#else

#endif // RD_TE2_ADVANCE_FUNC
    }

// ----------------------------------------------------------------------------
// CMatrixTask::DrawMask
// ----------------------------------------------------------------------------
//
void CMatrixTask::DrawMask( const TBlock& aMask, TInt aOffsetY )
    {
    TPoint topLeft( aMask.iX * iControl.iBlockSize.iWidth,
            ( iControl.iMatrixHeight - 1 - aMask.iY ) * iControl.iBlockSize.iHeight + aOffsetY );
    TRgb color( iOpacity, iOpacity, iOpacity );
    iControl.iMaskGc->SetPenStyle( CGraphicsContext::ENullPen );
    iControl.iMaskGc->SetBrushColor( color );
    iControl.iMaskGc->SetBrushStyle( CGraphicsContext::ESolidBrush );
    iControl.iMaskGc->DrawRect( TRect( topLeft, iControl.iBlockSize ) );  
    }

// End of File
