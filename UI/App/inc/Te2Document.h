/* ====================================================================
 * File: Te2Document.h
 * Created: 01/14/09
 * Modified:
 * Author: Sun Jinbo
 * Copyright (c): Tieto, All rights reserved
 * ==================================================================== */

#ifndef C_TE2DOCUMENT_H
#define C_TE2DOCUMENT_H

// INCLUDE FILES
#include <akndoc.h>

// FORWARD REFERENCES
class CTe2AppUi;
class CAknApplication;


// CLASS DECLARATION
/**
* CTe2Document class
*
* Discussion An instance of class CTe2Document is the Document part of
* the AVKON application framework for the Tetris example application.
*
* @since S60 3.1
*/
class CTe2Document : public CAknDocument
    {
public: // Constructor and destructor

    static CTe2Document* NewL( CEikApplication& aApp );
    static CTe2Document* NewLC( CEikApplication& aApp );
    ~CTe2Document();

public: // from base class CAknDocument
    CEikAppUi* CreateAppUiL();

private: // Constructor
    void ConstructL();
    CTe2Document( CEikApplication& aApp );
    };


#endif // C_TE2DOCUMENT_H

// End of File
