/* ====================================================================
 * File: Te2Settings.h
 * Created: 09/17/10
 * Modifed: 09/17/10
 * Author: Sun Jinbo
 * Copyright (c): Tieto, All rights reserved
 * ==================================================================== */

#ifndef T_TE2SETTINGS_H
#define T_TE2SETTINGS_H

// INCLUDE
#include <e32def.h>
#include "Tetris2.hrh"

// FORWARD DECLARATION
class RFs;

// CLASS DECLARATION

/**
 *  TTe2Settings class
 */
class TTe2Settings
    {
public: // Default C++ constructor
	TTe2Settings();
	TTe2Settings( const TTe2Settings& aSettings );
	TTe2Settings& operator=( const TTe2Settings& aSettings );

public: // New methods
    void InternalizedL();
    void ExternalizedL();

private: // New methods
    void CreateDefaultSettingsL();
    TInt GetFileName( TFileName& aFileName );
    TInt GetPrivatePath( TFileName& aPrivatePath );

public: // Data
    RFs& iFs;
    TTe2Mode iMode;
    TTe2Audio iAudio;
    };

#endif // T_TE2SETTINGS_H

// End Of File
