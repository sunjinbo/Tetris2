/* ====================================================================
 * File: Te2Application.cpp
 * Created: 01/14/09
 * Modified: 03/30/09
 * Author: Sun Jinbo
 * Copyright (c): Tieto, All rights reserved
 * ==================================================================== */

// INCLUDE FILES
#include <Logger.h>
#include "Te2Application.h"
#include "Te2Document.h"
#include "Tetris2ViewIds.h"

// CONSTANTS

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CTe2Application::CreateDocumentL
// ---------------------------------------------------------------------------
//
CApaDocument* CTe2Application::CreateDocumentL()
    {
    _LOG_CREATE
    _LOG_WRITE( "CTe2Document::CreateDocumentL" );
    CApaDocument* document = CTe2Document::NewL( *this );
    return document;
    }

// ---------------------------------------------------------------------------
// CTe2Application::AppDllUid
// ---------------------------------------------------------------------------
//
TUid CTe2Application::AppDllUid() const
    {
    // Return the UID for the Tetris application.
    return KUidTetris2App;
    }

// End of File
