/* ====================================================================
 * File: Te2Container.cpp
 * Created: 01/14/09
 * Modified: 03/30/09
 * Author: Sun Jinbo
 * Copyright (c): Tieto, All rights reserved
 * ==================================================================== */

// INCLUDE FILES
#include "Te2AppUi.h"
#include "Te2Document.h"
#include <Logger.h>

// CONSTANTS

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CTe2Document::NewL
// ---------------------------------------------------------------------------
// 
CTe2Document* CTe2Document::NewL( CEikApplication& aApp )
    {
    CTe2Document* self = NewLC( aApp );
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// CTe2Document::NewLC
// ---------------------------------------------------------------------------
//
CTe2Document* CTe2Document::NewLC( CEikApplication& aApp )
    {
    CTe2Document* self = new (ELeave) CTe2Document( aApp );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// ---------------------------------------------------------------------------
// CTe2Document::~CTe2Document
// ---------------------------------------------------------------------------
//
CTe2Document::~CTe2Document()
    {
    }

    
// ---------------------------------------------------------------------------
// CTe2Document::ConstructL
// ---------------------------------------------------------------------------
//
void CTe2Document::ConstructL()
    { 
    // no implementation required
    }   

// ---------------------------------------------------------------------------
// CTe2Document::CTe2Document
// ---------------------------------------------------------------------------
//
CTe2Document::CTe2Document( CEikApplication& aApp ) 
    : CAknDocument( aApp ) 
    {
    // no implementation required
    }

// ---------------------------------------------------------------------------
// CTe2Document::CreateAppUiL
// ---------------------------------------------------------------------------
//
CEikAppUi* CTe2Document::CreateAppUiL()
    {
    // Create the application user interface, and return a pointer to it,
    // the framework takes ownership of this object
    CEikAppUi* appUi = new (ELeave) CTe2AppUi;
    return appUi;
    }

// End of File
