/* ====================================================================
 * File: LayoutManager.h
 * Created: 03/15/09
 * Modified: 03/30/09
 * Author: Sun Jinbo
 * Copyright (c): Tieto, All rights reserved
 * ==================================================================== */

#ifndef C_TE2LAYOUTMANAGER_H
#define C_TE2LAYOUTMANAGER_H

// INCLUDE FILES
#include <e32base.h>
#include "LayoutManagerValues.h"


// FORWARD DECLARATIONS
class TAknLayoutRect;

// CLASS DECLARATION
/**
* CBackgroundUi class
*
* @since S60 3.1
*/
class CTe2LayoutManager : public CBase
    {
public: // Constructor and Destructor
    IMPORT_C static CTe2LayoutManager* NewL();
    IMPORT_C virtual ~CTe2LayoutManager();    

public: // New functions
    IMPORT_C void ResolutionChanged( TOrientation aOrientation = EAutomatic );
    IMPORT_C TRect LayoutRect( TTe2Component aComponent ) const;
    IMPORT_C static TResolution Resolution();

private: // Constructor
    CTe2LayoutManager();
    void ConstructL();

private: // Data members
    TResolution iResolution;
    };

#endif // C_TE2LAYOUTMANAGER_H

// End of File
