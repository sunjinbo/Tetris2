/* ====================================================================
 * File: SvgUtils.h
 * Created: 03/11/09
 * Modified: 05/08/09
 * Author: Sun Jinbo
 * Copyright (c): Tieto, All rights reserved
 * ==================================================================== */

#ifndef C_SVGUTILS_H
#define C_SVGUTILS_H

// INCLUDE FILES
#include "BmpUtils.h"


// CLASS DECLARATION
/**
* CSvgUtils class
*
* For handles creation and drawing of .SVG file.
*
* @since S60 3.1
*/
class CSvgUtils : public CBmpUtils
    {
public: // Two-phased constructor and Destructor
    IMPORT_C static CSvgUtils* NewL( const TDesC& aFileName, TInt aFrom, TInt aTo );
    IMPORT_C virtual ~CSvgUtils();

public: // New functions
    IMPORT_C void SetSize( TInt aIndex, TSize aSize );
    IMPORT_C void SetSizeAndRotation( TInt aIndex, TSize aSize, TInt aAngle );

    IMPORT_C CFbsBitmap* Bitmap( TInt aIndex ) const;

protected: // Default destructor
    CSvgUtils( TInt aFrom, TInt aTo );
    void ConstructL( const TDesC& aFileName );

    virtual void LoadBitmapL ();

    };

#endif // C_SVGUTILS_H

// End of File
