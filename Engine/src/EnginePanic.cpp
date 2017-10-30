/* ====================================================================
 * File: EnginePanic.h
 * Created: 01/17/09
 * Modified: 01/25/09
 * Author: Sun Jinbo
 * Copyright (c): Tieto, All rights reserved
 * ==================================================================== */

// INCLUDE FILES
#include "e32def.h"
#include "EnginePanic.h"

// ======== GLOBAL FUNCTIONS ========

// ----------------------------------------------------------------------------
// EnginePanic
// ----------------------------------------------------------------------------
//
GLDEF_C void EnginePanic( TEnginePanic aPanic )
    {
#ifdef RD_TE2_USING_PANIC
    _LIT( KEnginePanic, "TE2-ENGINE-PANIC" );
    User::Panic( KEnginePanic, aPanic );
#else
    ( void )aPanic; // Do nothing, it just for avoid warning.
#endif // RD_TE2_USING_PANIC
    }

// End of File
