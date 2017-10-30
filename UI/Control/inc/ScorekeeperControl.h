/* ====================================================================
 * File: ScorekeeperControl.h
 * Created: 03/15/09
 * Modified: 03/15/09
 * Author: Sun Jinbo
 * Copyright (c): Tieto, All rights reserved
 * ==================================================================== */

#ifndef C_SCOREKEEPERCONTROL_H
#define C_SCOREKEEPERCONTROL_H

// INCLUDE FILES
#include <e32std.h>
#include <e32base.h>
#include <coecntrl.h>

// FORWARD DECLARATIONS
class CFbsBitGc;

// CLASS DECLARATION
/**
* CScorekeeperControl class
*
* @since S60 3.1
*/
class CScorekeeperControl : public CCoeControl
    {
public: // Constructor and Destructor
    static CScorekeeperControl* NewL( const TRect& aRect, CFbsBitGc*& aBgc );
    virtual ~CScorekeeperControl();

public: // from base class CCoeControl
    void Draw(const TRect& aRect) const;

public: // New functions
    void UpdateScorekeeper( const TEngineAction& aAction );
    void Draw();
    
protected: // Constructor
    CScorekeeperControl( CFbsBitGc*& aBgc );
    void ConstructL( const TRect& aRect );

protected: // Data members
    CFbsBitGc*& iBgc;
    HBufC* iLevelText; // owned
    HBufC* iLineText; // owned
    HBufC* iScoreText; // owned
    HBufC* iLifeText; // owned
    TInt iLevel;
    TInt iLine;
    TInt iScore;
    TInt iLife;
    TInt iResOffset;
    CFont* iTitleFont;
    CFont* iValueFont;
    };

#endif // C_SCOREKEEPERCONTROL_H

// End of File
