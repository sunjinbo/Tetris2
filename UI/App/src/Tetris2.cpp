/* ====================================================================
 * File: Tetris2.cpp
 * Created: 01/14/09
 * Modified: 01/14/09
 * Author: Sun Jinbo
 * Copyright (c): Tieto, All rights reserved
 * ==================================================================== */

// INCLUDE FILES
#include <eikstart.h> // EikStart
#include "Te2Application.h"


// ======== GLOBAL FUNCTIONS ========

// ---------------------------------------------------------------------------
// Create an application, and return a pointer to it .
// ---------------------------------------------------------------------------
//
CApaApplication* NewApplication()
    {
    return new CTe2Application;
    }

// ---------------------------------------------------------------------------
// Main function of the application executable.
// ---------------------------------------------------------------------------
// 
GLDEF_C TInt E32Main()
    {
    return EikStart::RunApplication( NewApplication );
    }

// End of File
