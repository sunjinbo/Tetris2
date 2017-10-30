/* ====================================================================
 * File: MenubarControl.h
 * Created: 03/15/09
 * Modified: 03/15/09
 * Author: Sun Jinbo
 * Copyright (c): Tieto, All rights reserved
 * ==================================================================== */

#ifndef C_MENUBARCONTROL_H
#define C_MENUBARCONTROL_H

// INCLUDE FILES
#include <e32std.h>
#include <e32base.h>
#include <gdi.h> // CGraphicsContext::TTextAlign
#include <coecntrl.h>

// FORWARD DECLARATIONS
class CFbsBitGc;
class CFont;

// CLASS DECLARATION
/**
* CMenubarControl class
*
* @since S60 3.1
*/
class CMenubarControl : public CCoeControl
    {
public: // Constructor and Destructor
    static CMenubarControl* NewL( const TRect& aRect, CFbsBitGc*& aBgc );
    virtual ~CMenubarControl();

public: // from base class CCoeControl
    void Draw(const TRect& aRect) const;
    void SizeChanged();

public: // New function
    void SetTitleL( const TDesC& aText );
    void SetAlignment( CGraphicsContext::TTextAlign aAlign );

    void Draw();

protected: // Constructor
    CMenubarControl( CFbsBitGc*& aBgc );
    void ConstructL( const TRect& aRect );

protected: // Data members
    CFbsBitGc*& iBgc;
    HBufC* iMenuText; // owned
    CGraphicsContext::TTextAlign iAlign;
    };

#endif // C_MENUBARCONTROL_H

// End of File
