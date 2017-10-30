/* ====================================================================
 * File: BackgroundControl.h
 * Created: 03/15/09
 * Modified: 05/03/09
 * Author: Sun Jinbo
 * Copyright (c): Tieto, All rights reserved
 * ==================================================================== */

#ifndef C_BACKGROUNDCONTROL_H
#define C_BACKGROUNDCONTROL_H

// INCLUDE FILES
#include <e32std.h>
#include <e32base.h>
#include <coecntrl.h>

// FORWARD DECLARATIONS
class CFbsBitGc;
class CPeriodic;
class CBmpUtils;
class CSvgUtils;

// CLASS DECLARATION
/**
* CBackgroundControl class
*
* @since S60 3.1
*/
class CBackgroundControl : public CCoeControl
    {
public: // Constructor and Destructor
    static CBackgroundControl* NewL( const TRect& aRect, CFbsBitGc*& aBgc );
    virtual ~CBackgroundControl();

public: // from base class CCoeControl
    void Draw(const TRect& aRect) const;
    void SizeChanged();

public: // New functions
    void StoreL();
    void Draw();

protected: // Constructor
    CBackgroundControl( CFbsBitGc*& aBgc );
    void ConstructL( const TRect& aRect );

protected: // New function
    void Stop();
    void DoAnimal();
    static TInt TimerCallback( TAny* aPtr );
    TInt Rand( TInt aRange ) const;
    void InitializeAnimalTl();
    void InitializeSize();

protected: // Data members
    CFbsBitGc*& iBgc;
    CSvgUtils* iSvgUtils;
    CPeriodic* iAnimalTimer;
    TPoint iRuralTl;
    TPoint iSunTl;
    TPoint iCloud0Tl;
    TPoint iCloud1Tl;
    TPoint iCloud2Tl;
    TPoint iAnimalTl;
    
    TSize iSunSize;
    TSize iCloud0Size;
    TSize iCloud1Size;
    TSize iCloud2Size;
    TSize iFishSize;
    TSize iTortoiseSize;
    TSize iRuralSize;
    
    enum TAnimal
        {
        EFish,
        ETortoise
        };
    TAnimal iAnimal;
    };

#endif // C_BACKGROUNDCONTROL_H

// End of File
