/* ====================================================================
 * File: EnginePanic.h
 * Created: 01/17/09
 * Modified: 02/25/09
 * Author: Sun Jinbo
 * Copyright (c): Tieto, All rights reserved
 * ==================================================================== */

#ifndef C_TE2ENGINEPANIC_H
#define C_TE2ENGINEPANIC_H

// INCLUDE FILES
#include <e32std.h>

/** Panics in Te2Engine.
*  @lib Te2Engine
*  @since 3.1
*/
enum TEnginePanic
    {
    EEnginePanicIndexOutOfRange = 0x0F0F,
    EEnginePanicValueOutOfRange,
    EEnginePanicInvalidArguments,
    EEnginePanicRequestNotHandled,
    EEnginePanicListEmpty,
    EEnginePanicNotInitialised,
    EEnginePanicResourceNotOpened,
    EEnginePanicResourceAlreadyOpened,
    EEnginePanicMemoryLeak,
    EEnginePanicNullPointer,
    };

/** Te2Engine panic.
*
*  @lib Te2Engine
*  @since 3.1
*/
GLDEF_C void EnginePanic( TEnginePanic aPanic );

#endif // C_TE2ENGINEPANIC_H

// End of File
