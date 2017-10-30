/* ====================================================================
 * File: Te2Application.h
 * Created: 01/14/09
 * Author: Sun Jinbo
 * Copyright (c): Tieto, All rights reserved
 * ==================================================================== */

#ifndef C_TE2APPLICATION_H
#define C_TE2APPLICATION_H

// INCLUDE FILES
#include <aknapp.h>

// CLASS DECLARATION
/**
* CTe2Application class
*
* Discussion An instance of CTetrisApplication is the application part of
* the AVKON application framework for the Tetris example application.
*
* @since S60 3.1
*/
class CTe2Application : public CAknApplication
    {
public: // from base class CAknApplication
    TUid AppDllUid() const;

protected: // from base class CAknApplication
    CApaDocument* CreateDocumentL();
    };

#endif // C_TE2APPLICATION_H

// End of File
