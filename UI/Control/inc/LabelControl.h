/* ====================================================================
 * File: LabelControl.h
 * Created: 06/06/10
 * Modified: 06/06/10
 * Author: Sun Jinbo
 * Copyright (c): Tieto, All rights reserved
 * ==================================================================== */

#ifndef C_LABELCONTROL_H
#define C_LABELCONTROL_H

// INCLUDE FILES
#include <e32std.h>
#include <e32base.h>
#include <coecntrl.h>

// CONSTANTS

// FORWARD DECLARATIONS
class CFbsBitGc;

// CLASS DECLARATION
/**
* CLabelControl class
*
* @since S60 3.1
*/
class CLabelControl : public CCoeControl
    {
public: // Constructor and Destructor
    static CLabelControl* NewL( const TRect& aRect, CFbsBitGc*& aBgc );
    virtual ~CLabelControl();

public: // from base class CCoeControl
    void Draw(const TRect& aRect) const;

public: // New function
    void SetTextL( const TDesC& aText );
    void SetColor( TRgb aColor );
    void SetFont( const CFont* aFont );
    void SetTextAlign( CGraphicsContext::TTextAlign aAlign );
    void Draw();

protected: // Constructor
    CLabelControl( CFbsBitGc*& aBgc );
    void ConstructL( const TRect& aRect );

protected: // Data members
    CFbsBitGc*& iBgc;
    HBufC* iText;
    TRgb iColor;
    const CFont* iFont;
    CGraphicsContext::TTextAlign iAlign;
    };

#endif // C_LABELCONTROL_H

// End of File
