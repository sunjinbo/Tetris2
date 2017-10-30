/* ====================================================================
 * File: AudioUtils.h
 * Created: 05/22/09
 * Modified: 05/22/09
 * Author: Sun Jinbo
 * Copyright (c): Tieto, All rights reserved
 * ==================================================================== */

#ifndef C_AUTILSUTILS_H
#define C_AUTILSUTILS_H

// INCLUDE FILES
#include <e32std.h>
#include <MdaAudioSamplePlayer.h>

// FORWARD DECLARATIONS
class MAudioInitObserver;
class MAudioPlayObserver;

// CLASS DECLARATION

/**
* CAudioUtils class
*
* @since S60 3.1
*/
class CAudioUtils : public CBase, MMdaAudioPlayerCallback
    {
public: // Constructor and destructor
    IMPORT_C static CAudioUtils* NewL();
    IMPORT_C virtual ~CAudioUtils();

public: // New functions
    IMPORT_C void Play();
    IMPORT_C void Stop();
    IMPORT_C void OpenFileL( const TDesC& aFileName, TInt aUniqueId );
    IMPORT_C TInt UniqueId() const;
    IMPORT_C void SetAudioInitObserver( MAudioInitObserver* aObserver );
    IMPORT_C void SetAudioPlayObserver( MAudioPlayObserver* aObserver );

private: // From base class MMdaAudioToneObserver
    void MapcInitComplete( 
        TInt aError, const TTimeIntervalMicroSeconds& /*aDuration*/ );
    void MapcPlayComplete( TInt aError );

private: // Constructor
    CAudioUtils();
    void ConstructL();

private: // Data member
    MAudioInitObserver* iInitObserver; // not owned
    MAudioPlayObserver* iPlayObserver; // not owned
    CMdaAudioPlayerUtility* iPlayer; // owned
    TInt iUniqueId;
    TBool iReadyToPlay;
    };

#endif // C_AUTILSUTILS_H

// End of File
