/* ====================================================================
 * File: MatrixControl.h
 * Created: 03/15/09
 * Modified: 03/15/09
 * Author: Sun Jinbo
 * Copyright (c): Tieto, All rights reserved
 * ==================================================================== */

#ifndef C_MATRIXCONTROL_H
#define C_MATRIXCONTROL_H

// INCLUDE FILES
#include <e32std.h>
#include <e32base.h>
#include <coecntrl.h>

// FORWARD DECLARATIONS
class CFbsBitGc;
class TEngineAction;
class CMatrixTaskScheduler;
class CSvgUtils;
class CFont;
class MMatrixObserver;

// CLASS DECLARATION

/**
* CMatrixControl class
*
* @since S60 3.1
*/
class CMatrixControl : public CCoeControl
    {
public: // Enum

    enum TDirection
        {
        ETop,
        EBottom,
        ELeft,
        ERight,
        ETopLeft,
        ETopRight,
        EBottomLeft,
        EBottomRight,
        ECenter
        };
    
    enum TGestureArea
        {
        EGestureAreaA,
        EGestureAreaB,
        EGestureAreaC,
        EGestureAreaD
        };

    enum TIndicatorType
        {
        EIndicatorNone = 0,
        EIndicatorLeft,
        EIndicatorRight,
        EIndicatorDescend,
        EIndicatorRotate
        };

public: // friend class
    friend class CMatrixTask;

public: // Constructor and Destructor
    static CMatrixControl* NewL( 
        const TRect& aRect, 
        CFbsBitGc*& aBgc, 
        const TSize& aMatrixSize 
        );
    virtual ~CMatrixControl();

public: // from base class CCoeControl
    void Draw(const TRect& aRect) const;
    void SizeChanged();
    void HandlePointerEventL(const TPointerEvent& aPointerEvent);

public: // New functions
    void SetObserver( MMatrixObserver* aObserver );
    void SetBorder( TBool aBorder );
    void UpdateMatrixL( const TEngineAction& aAction );
    void SetTipsL( const TDesC& aText );
    void Clear();
    void Draw();

private: // Constructor
    CMatrixControl( CFbsBitGc*& aBgc );
    void ConstructL( const TRect& aRect, const TSize& aMatrixSize );

private: // New functions
    void SetMatrixSize( TInt aWidth, TInt aHeight );
    void DrawBorder() const;
    void DrawTips() const;
    void DrawIndicator() const;
    void CreateBmpBufferL();
    void ReleaseBmpBuffer();
    static TInt TimerCallback( TAny* aPtr );
    static TInt IndiTimerCallback( TAny* aPtr );
    void MoveTips();
    void MoveIndicator();
    void StartTipsL();
    void StartIndicatorL( TIndicatorType aIndicator );
    void StopTips();
    void StopIndicator();
    void GetRemovableRegion( const CArrayFixFlat<TBlock>* aArray );
    TDirection Direction( const TRect& aRect, const TPoint& aPos ) const;
    TGestureArea GestureArea( const TPoint& aStartPoint, const TPoint& aEndPoint ) const;
    void HandleDirectionEventL( TDirection aDirection );
    void HandleGestureAreaEventL( TGestureArea aGestureArea );
    TRect RealRect() const;

private: // Data members
    CFbsBitGc*& iBgc;
    MMatrixObserver* iObserver;
    CFbsBitmap* iBitmap;
    CFbsBitmapDevice* iBitmapDevice;
    CFbsBitGc* iBitmapGc;
    CFbsBitmap* iMask;
    CFbsBitmapDevice* iMaskDevice;
    CFbsBitGc* iMaskGc;

    CFbsBitmap* iTipsBitmap;
    CFbsBitmapDevice* iTipsBitmapDevice;
    CFbsBitGc* iTipsBitmapGc;
    CFbsBitmap* iTipsMask;
    CFbsBitmapDevice* iTipsMaskDevice;
    CFbsBitGc* iTipsMaskGc;

    CSvgUtils* iSvgUtils;
    CMatrixTaskScheduler* iScheduler;
    
    CPeriodic* iPeriodic;
    HBufC* iTipsText;
    CFont* iTipsFont;
    
    CFbsBitmap* iIndicatorBitmap;
    CFbsBitmapDevice* iIndicatorBitmapDevice;
    CFbsBitGc* iIndicatorBitmapGc;
    CFbsBitmap* iIndicatorMask;
    CFbsBitmapDevice* iIndicatorMaskDevice;
    CFbsBitGc* iIndicatorMaskGc;
    
    CPeriodic* iIndiPeriodic;
    TIndicatorType iIndicator;
    TPoint iIndiPos;
    TSize iIndiSize;
    TInt iIndiTimer;
    TInt iIndiRotateOffset;
    
    TPoint iTipsPos;
    TSize iTipsSize;
    TInt iTipsTimer;

    TSize iMatrixSize;
    TSize iBlockSize;
    TBool iBorder;
    TInt iMatrixWidth;
    TInt iMatrixHeight;
    TInt iOpacity;
    RRegion iRemovableRegion;
    TPoint iStartPoint;
    TBool iKeyLongPressed;
    TInt iDragCount;
    TInt iBorderPenSize;
    };

#endif // C_MATRIXCONTROL_H

// End of File
