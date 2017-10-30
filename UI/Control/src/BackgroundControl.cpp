/* ====================================================================
 * File: BackgroundControl.cpp
 * Created: 03/15/09
 * Modified: 03/17/09
 * Author: Sun Jinbo
 * Copyright (c): Tieto, All rights reserved
 * ==================================================================== */

// INCLUDE FILES
#include <e32math.h>
#include <fbs.h>
#include <eikenv.h>
#include <e32property.h>
#include <background.mbg>
#include <SvgUtils.h>
#include <LayoutManager.h>
#include "BackgroundControl.h"
#include "BackgroundPSKeys.h"
#include "BackgroundDefines.h"

// CONSTANTS
const TInt KAnimalDelayTimeout = 0;
const TInt KAnimalFrameUpdateTimeout = 100000;
#if defined( __WINS__ )
_LIT( KBackgroundSvgFileName, "Z:\\resource\\apps\\background.mbm" );
#else
_LIT( KBackgroundSvgFileName, "\\resource\\apps\\background.mbm" );
#endif // __WINS__

// ======== MEMBER FUNCTIONS ========

// ----------------------------------------------------------------------------
// CBackgroundControl::NewL
// ----------------------------------------------------------------------------
//
CBackgroundControl* CBackgroundControl::NewL( const TRect& aRect, CFbsBitGc*& aBgc )
    {
    CBackgroundControl* self = new (ELeave) CBackgroundControl( aBgc );
    CleanupStack::PushL (self );
    self->ConstructL ( aRect );
    CleanupStack::Pop();
    return self;
    }

// ----------------------------------------------------------------------------
// CBackgroundControl::~CBackgroundControl
// ----------------------------------------------------------------------------
//
CBackgroundControl::~CBackgroundControl()
    {
    Stop();
    delete iSvgUtils;
    }

// ----------------------------------------------------------------------------
// CBackgroundControl::Draw
// ----------------------------------------------------------------------------
//
void CBackgroundControl::Draw(const TRect& /*aRect*/) const
    { 
    // No implementation required
    }

// ----------------------------------------------------------------------------
// CBackgroundControl::SizeChanged
// ----------------------------------------------------------------------------
//
void CBackgroundControl::SizeChanged()
    {
    TRect rect( Rect() );
    InitializeAnimalTl();
    iRuralTl.iX = rect.iBr.iX - iRuralSize.iWidth;
    iRuralTl.iY = rect.iBr.iY - iRuralSize.iHeight;
    iSunTl.iX = rect.iBr.iX - iSunSize.iWidth;
    iSunTl.iY = rect.iTl.iY;

    // Changes clouds postion accroding to background revolution
    if ( EQhdPortrait == CTe2LayoutManager::Resolution() )
        {
        iCloud0Tl = rect.iTl;
        iCloud1Tl.iX = rect.iBr.iX - iSunSize.iWidth / 1.5;
        iCloud1Tl.iY = rect.iTl.iY + iSunSize.iHeight / 2;
        iCloud2Tl.iX = rect.iTl.iX + iCloud2Size.iWidth / 3;
        iCloud2Tl.iY = rect.iTl.iY + iCloud2Size.iHeight / 2;
        }
    else
        {
        iCloud0Tl.iX = rect.iTl.iX + iCloud0Size.iWidth / 2;
        iCloud0Tl.iY = rect.iTl.iY;
        iCloud1Tl.iX = rect.iBr.iX - iSunSize.iWidth / 2;
        iCloud1Tl.iY = rect.iTl.iY + iSunSize.iHeight / 2;
        iCloud2Tl.iX = rect.iTl.iX + iCloud2Size.iWidth / 2;
        iCloud2Tl.iY = rect.iTl.iY + iCloud2Size.iHeight / 2;
        }
    }

// ----------------------------------------------------------------------------
// CBackgroundControl::StoreL
// ----------------------------------------------------------------------------
//
void CBackgroundControl::StoreL()
    {
    User::LeaveIfError( RProperty::Set( KPSUidTe2Ctrl, KTe2CtrlAnimal, (TInt)iAnimal ) );
    User::LeaveIfError( RProperty::Set( KPSUidTe2Ctrl, KTe2CtrlXAxis, iAnimalTl.iX ) );
    User::LeaveIfError( RProperty::Set( KPSUidTe2Ctrl, KTe2CtrlYAxis, iAnimalTl.iY ) );
    }

// ----------------------------------------------------------------------------
// CBackgroundControl::Draw
// ----------------------------------------------------------------------------
//
void CBackgroundControl::Draw()
    {
    // Background color
    iBgc->SetBrushColor( KRgbBakColor );
    iBgc->SetBrushStyle( CGraphicsContext::ESolidBrush );
    iBgc->SetPenColor( KRgbBakColor );
    iBgc->SetPenStyle( CGraphicsContext::ESolidPen );
    iBgc->DrawRect( Rect() );
    iBgc->SetBrushStyle( CGraphicsContext::ENullBrush );
    iBgc->SetPenStyle( CGraphicsContext::ENullPen );

    // Sun
    iBgc->BitBlt( iSunTl,
            iSvgUtils->Bitmap( EMbmBackgroundSun ),
            TRect( TPoint( 0, 0 ), iSunSize ) );

    // Rural
    iBgc->BitBlt( iRuralTl,
            iSvgUtils->Bitmap( EMbmBackgroundRural ),
            TRect( TPoint( 0, 0 ), iRuralSize ) );

    // Clouds 0, 1, 2
    iBgc->BitBltMasked( iCloud0Tl,
            iSvgUtils->Bitmap( EMbmBackgroundCloud0 ),
            TRect( TPoint( 0, 0 ), iCloud0Size ),
            iSvgUtils->Bitmap( EMbmBackgroundCloud0mask ),
            EFalse );

    iBgc->BitBltMasked( iCloud1Tl,
            iSvgUtils->Bitmap( EMbmBackgroundCloud1 ),
            TRect( TPoint( 0, 0 ), iCloud1Size ),
            iSvgUtils->Bitmap( EMbmBackgroundCloud1mask ),
            EFalse );

    iBgc->BitBltMasked( iCloud2Tl,
            iSvgUtils->Bitmap( EMbmBackgroundCloud2 ),
            TRect( TPoint( 0, 0 ), iCloud2Size ),
            iSvgUtils->Bitmap( EMbmBackgroundCloud2mask ),
            EFalse );

    // Animals
    switch ( iAnimal )
        {
        case EFish:
            iBgc->BitBltMasked( iAnimalTl,
                    iSvgUtils->Bitmap( EMbmBackgroundFish ),
                    TRect( TPoint( 0, 0 ), iFishSize ),
                    iSvgUtils->Bitmap( EMbmBackgroundFishmask ),
                    EFalse );
            break;
        case ETortoise:
            iBgc->BitBltMasked( iAnimalTl,
                    iSvgUtils->Bitmap( EMbmBackgroundTortoise ),
                    TRect( TPoint( 0, 0 ), iTortoiseSize ),
                    iSvgUtils->Bitmap( EMbmBackgroundTortoisemask ),
                    EFalse );
            break;
        default:
            break;
        }
    }

// ----------------------------------------------------------------------------
// CBackgroundControl::CBackgroundControl
// ----------------------------------------------------------------------------
//
CBackgroundControl::CBackgroundControl( CFbsBitGc*& aBgc )
    : iBgc( aBgc ),iAnimal( EFish )
    { 
    // No implementation required.
    }

// ----------------------------------------------------------------------------
// CBackgroundControl::ConstructL
// ----------------------------------------------------------------------------
//
void CBackgroundControl::ConstructL( const TRect& aRect )
    {
    // Create a window for this application view
    CreateWindowL();

    InitializeSize();

    // Set the windows size
    SetRect( aRect ); // --> call SizeChanged()

    iSvgUtils = CSvgUtils::NewL( KBackgroundSvgFileName,
                EMbmBackgroundSun,
                EMbmBackgroundLastElement );
    iSvgUtils->SetSize( EMbmBackgroundSun, iSunSize );
    iSvgUtils->SetSize( EMbmBackgroundSunmask, iSunSize );
    iSvgUtils->SetSize( EMbmBackgroundCloud0, iCloud0Size );
    iSvgUtils->SetSize( EMbmBackgroundCloud0mask, iCloud0Size );
    iSvgUtils->SetSize( EMbmBackgroundCloud1, iCloud1Size );
    iSvgUtils->SetSize( EMbmBackgroundCloud1mask, iCloud1Size );
    iSvgUtils->SetSize( EMbmBackgroundCloud2, iCloud2Size );
    iSvgUtils->SetSize( EMbmBackgroundCloud2mask, iCloud2Size );
    iSvgUtils->SetSize( EMbmBackgroundFish, iFishSize );
    iSvgUtils->SetSize( EMbmBackgroundFishmask, iFishSize );
    iSvgUtils->SetSize( EMbmBackgroundTortoise, iTortoiseSize );
    iSvgUtils->SetSize( EMbmBackgroundTortoisemask, iTortoiseSize );
    iSvgUtils->SetSize( EMbmBackgroundRural, iRuralSize );
    iSvgUtils->SetSize( EMbmBackgroundRuralmask, iRuralSize );

    RProperty property;
    TInt value (0);
    TInt err = KErrNone;
    err = property.Define(
            KPSUidTe2Ctrl, KTe2CtrlAnimal, 
            RProperty::EInt );
    
    if ( err == KErrAlreadyExists )
        {
        err = RProperty::Get( KPSUidTe2Ctrl, KTe2CtrlAnimal, value );
        User::LeaveIfError( err );
        iAnimal = (TAnimal)value;
        }
    else if ( err == KErrNone )
        {
        // do nothing
        }
    else
        {
        User::LeaveIfError( err );
        }

    err = property.Define(
            KPSUidTe2Ctrl, KTe2CtrlXAxis, 
            RProperty::EInt );

    if ( err == KErrAlreadyExists )
        {
        err = RProperty::Get( KPSUidTe2Ctrl, KTe2CtrlXAxis, value );
        User::LeaveIfError( err );
        if ( iAnimal == EFish )
            {
            ++value;    
            }
        else
            {
            --value;
            }
        iAnimalTl.iX = value;
        }
    else if ( err == KErrNone )
        {
        // Do nothing
        }
    else
        {
        User::LeaveIfError( err );
        }
    
    err = property.Define(
            KPSUidTe2Ctrl, KTe2CtrlYAxis, 
            RProperty::EInt );

    if ( err == KErrAlreadyExists )
        {
        err = RProperty::Get( KPSUidTe2Ctrl, KTe2CtrlYAxis, value );
        User::LeaveIfError( err );
        if ( value >= iSunSize.iHeight
            && value <= aRect.Height() - iRuralSize.iHeight )
            {
            iAnimalTl.iY = value;    
            }
        else
            {
            InitializeAnimalTl();
            }
        }
    else if ( err == KErrNone )
        {
        // Do nothing
        }
    else
        {
        User::LeaveIfError( err );
        }

    iAnimalTimer = CPeriodic::NewL( CActive::EPriorityStandard );
    iAnimalTimer->Cancel();
    iAnimalTimer->Start(
                    KAnimalDelayTimeout,
                    KAnimalFrameUpdateTimeout,
                    TCallBack( TimerCallback, this ) );
    // Activate the window, which makes it ready to be drawn
    ActivateL();
    }

// ----------------------------------------------------------------------------
// CBackgroundControl::Stop
// ----------------------------------------------------------------------------
//
void CBackgroundControl::Stop()
    {
    if ( iAnimalTimer )
        {  
        iAnimalTimer->Cancel();
        delete iAnimalTimer;
        iAnimalTimer = NULL;   
        }
    }

// ----------------------------------------------------------------------------
// CBackgroundControl::DoAnimal
// ----------------------------------------------------------------------------
//
void CBackgroundControl::DoAnimal()
    {
    TRect rect( Rect() );
    switch ( iAnimal )
        {
        case EFish:
            {
            if ( iAnimalTl.iX >= rect.Width() )
                {
                iAnimal = ETortoise;
                InitializeAnimalTl();
                }
            iAnimalTl.iX += 1; // To right
            }
            break;
        case ETortoise:
            {
            if ( iAnimalTl.iX <= rect.iTl.iX - iTortoiseSize.iWidth )
                {
                iAnimal = EFish;
                InitializeAnimalTl();
                }
            iAnimalTl.iX -= 1; // To left
            }
            break;
        default:
            break;
        }
    }

// ----------------------------------------------------------------------------
// CBackgroundControl::TimerCallback
// ----------------------------------------------------------------------------
//
TInt CBackgroundControl::TimerCallback( TAny* aPtr )
    {
    CBackgroundControl* self = static_cast<CBackgroundControl*> ( aPtr );
    self->DoAnimal();
    return 0;
    }

// ----------------------------------------------------------------------------
// CBackgroundControl::Rand
// ----------------------------------------------------------------------------
//
TInt CBackgroundControl::Rand( TInt aRange ) const
    {
    return Math::Random() % aRange;
    }

// ----------------------------------------------------------------------------
// CBackgroundControl::InitializeAnimalTl
// ----------------------------------------------------------------------------
//
void CBackgroundControl::InitializeAnimalTl()
    {
    TRect rect( Rect() );
    switch ( iAnimal )
        {
        case EFish:
            iAnimalTl.iX = rect.iTl.iX - iFishSize.iWidth;
            break;
        case ETortoise:
            iAnimalTl.iX = rect.Width();
            break;
        default:
            break;
        }

    TInt range = rect.Height() - iRuralSize.iHeight - iSunSize.iHeight;
    TInt rand = Rand( range );
    iAnimalTl.iY = iSunSize.iHeight + rand;
    }

// ----------------------------------------------------------------------------
// CBackgroundControl::InitializeSize
// ----------------------------------------------------------------------------
//
void CBackgroundControl::InitializeSize()
    {
    if ( EQhdPortrait == CTe2LayoutManager::Resolution() )
        {
        iSunSize = KQhdSunSize;
        iCloud0Size = KQhdCloud0Size;
        iCloud1Size = KQhdCloud1Size;
        iCloud2Size = KQhdCloud2Size;
        iFishSize = KQhdFishSize;
        iTortoiseSize = KQhdTortoiseSize;
        iRuralSize = KQhdRuralSize;
        }
    else
        {
        iSunSize = KQvgaSunSize;
        iCloud0Size = KQvgaCloud0Size;
        iCloud1Size = KQvgaCloud1Size;
        iCloud2Size = KQvgaCloud2Size;
        iFishSize = KQvgaFishSize;
        iTortoiseSize = KQvgaTortoiseSize;
        iRuralSize = KQvgaRuralSize;
        }
    }

// End of File
