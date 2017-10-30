/* ====================================================================
 * File: ScrollControl.h
 * Created: 03/15/09
 * Modified: 03/15/09
 * Author: Sun Jinbo
 * Copyright (c): Tieto, All rights reserved
 * ==================================================================== */

#ifndef C_SCROLLCONTROL_H
#define C_SCROLLCONTROL_H

// INCLUDE FILES
#include <e32std.h>
#include <e32base.h>
#include <coecntrl.h>

// CONSTANTS
const TInt KScrollLineNumber = 7;

// FORWARD DECLARATIONS
class CFbsBitGc;
class CPeriodic;

// CLASS DECLARATION
/**
* CScrollControl class
*
* @since S60 3.1
*/
class CScrollControl : public CCoeControl
    {
public: // Constructor and Destructor
    static CScrollControl* NewL( const TRect& aRect, CFbsBitGc*& aBgc );
    virtual ~CScrollControl();

public: // from base class CCoeControl
    void Draw(const TRect& aRect) const;

public: // New function
    void AppendTextL( const TDesC& aText );
    void Clear();
    void Start();
    void Draw();

protected: // Constructor
    CScrollControl( CFbsBitGc*& aBgc );
    void ConstructL( const TRect& aRect );

private: // New function
    void Stop();
    void DoScroll();
    static TInt TimerCallback( TAny* aPtr );
    void InitializeScroll();
    void UpdateCabinetArray();
    void AppendL( const TDesC& aText );

protected: // Data members
    CFbsBitGc*& iBgc;
    CFont* iFont;
    RPointerArray<HBufC> iTextArray;
    CPeriodic* iPeriodic;
    TFixedArray<TInt, KScrollLineNumber> iScrollCabinet;
    TInt iHeadIndex;
    TInt iTailIndex;
    TInt iHeadValue;
    TInt iScrollStep;
    TInt iCabinetStep;
    };

#endif // C_SCROLLCONTROL_H

// End of File
