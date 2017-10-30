/* ====================================================================
 * File: AudioStreamEngine.cpp
 * Created: 06/16/10
 * Author: Sun Jinbo
 * Copyright (c): Tieto, All rights reserved
 * ==================================================================== */

// INCLUDE FILES
#include <eikenv.h>
#include <mda\common\audio.h>
#include <mmf\common\mmfutilities.h>
#include <MdaAudioInputStream.h> // audio input stream
#include <MdaAudioOutputStream.h> // audio output stream
#include <s32file.h> // RFileWriteStream and RFileReadStream
#include "AudioStreamEngine.h"

// CONSTANTS
const TInt KFrameSizePCM = 4096;
const TInt KFrameCountPCM = 10;
const TInt KAMRHeaderLength = 6;

// ======== GLOBAL FUNCTIONS ========

// ----------------------------------------------------------------------------
// PanicIfError
// ----------------------------------------------------------------------------
//
GLDEF_C void PanicIfError( TInt aError )
    {
#ifdef RD_TE2_USING_PANIC
    _LIT( KCommonPanic, "TE2-COMMON-PANIC" );
    User::Panic( KEnginePanic, aError );
#else
    (void)aError; // Do nothing, it just for avoid warning.
#endif // RD_TE2_USING_PANIC
    }

// ======== MEMBER FUNCTIONS ========

// ----------------------------------------------------------------------------
// CAudioStreamEngine::NewL
// ----------------------------------------------------------------------------
//
CAudioStreamEngine* CAudioStreamEngine::NewL( const TDesC& aFileName )
    {
    CAudioStreamEngine* self = CAudioStreamEngine::NewLC( aFileName );
    CleanupStack::Pop(self);
    return self;
    }

// ----------------------------------------------------------------------------
// CAudioStreamEngine::NewLC
// ----------------------------------------------------------------------------
//
CAudioStreamEngine* CAudioStreamEngine::NewLC( const TDesC& aFileName )
    {
    CAudioStreamEngine* self = new (ELeave) CAudioStreamEngine( aFileName );
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
    }

// ----------------------------------------------------------------------------
// CAudioStreamEngine::~CAudioStreamEngine()
//
// destructor
// ----------------------------------------------------------------------------
CAudioStreamEngine::~CAudioStreamEngine()
    { 
    if (iOutputStream)
        {
        Stop();
        delete iOutputStream;
        }

    delete iStreamBuffer;
    delete iIdle;
    }

// ----------------------------------------------------------------------------
// CAudioStreamEngine::Play()
//
// plays the audio data contained in the buffer
// ----------------------------------------------------------------------------
void CAudioStreamEngine::Play()
    {
    iIdle->Start( TCallBack( PlayCallBack, this ) );
    }

// ----------------------------------------------------------------------------
// CAudioStreamEngine::Stop()
//
// stops playing/recording
// ----------------------------------------------------------------------------
void CAudioStreamEngine::Stop()
    {   
    if ( iOutputStatus != ENotReady ) 
        {
        iIdle->Start( TCallBack( StopCallBack, this ) );
        }
    }


// MMdaAudioOutputStream callbacks (MMdaAudioOutputStreamCallBack)

// ----------------------------------------------------------------------------
// CAudioStreamEngine::MaoscOpenComplete(
//     TInt aError)
//
// called upon completion of CMdaAudioOutputStream::Open(),
// if the stream was opened succesfully (aError==KErrNone), it's ready for use.
// upon succesful open, the first audio data block will be written to the 
// output stream.
// ----------------------------------------------------------------------------
void CAudioStreamEngine::MaoscOpenComplete(TInt aError)
    {
    if (aError==KErrNone) 
        {
        // output stream opened succesfully, set status
        iOutputStatus = EOpen;
        // Set the data type (encoding). Should not fail, since we already
        // have tested support for this encoding in SetEncodingL with the 
        // corresponding input stream!
        TRAPD(error, iOutputStream->SetDataTypeL( iCurrentEncoding ) );
        PanicIfError(error);
        
        // set volume to 1/4th of stream max volume
        iOutputStream->SetVolume(iOutputStream->MaxVolume()/4);
        // set stream priority to normal and time sensitive
        iOutputStream->SetPriority(EPriorityNormal, 
            EMdaPriorityPreferenceTime);                

        if (iUseAMR)
            {
            // In case of AMR, the whole recorded/loaded buffer is played back at once, not frame by frame. 
            // The buffer might not be fully recorded, so we will only play back the part
            // that is filled with data.
            iStreamIdx = iStreamEnd;
            TRAPD( error2, iOutputStream->WriteL(GetPlaybackFrames(iStreamEnd)));
            PanicIfError( error2 );
            }
        else
            {
            // PCM needs to be played back frame by frame, otherwise some older devices might
            // run into buffer overflow situations.
            iStreamIdx = 0;
            TRAPD(error3, iOutputStream->WriteL(GetFrame(iStreamIdx)));
            PanicIfError(error3);
            }
        }
    else 
        {
        // output stream open failed
        iOutputStatus = ENotReady;
        }        
    }

// ----------------------------------------------------------------------------
// CAudioStreamEngine::MaoscBufferCopied(
//     TInt aError, const TDesC8& aBuffer)
//
// called when a block of audio data has been written to MMF. calls to WriteL() 
// will be issued until all blocks in the audio data buffer (iStreamBuffer) are 
// written.
// ----------------------------------------------------------------------------
void CAudioStreamEngine::MaoscBufferCopied(TInt aError, const TDesC8& /*aBuffer*/)
    {    
    if (aError==KErrNone) 
        {
        if ( iStreamIdx==iStreamEnd )
            {
            iOutputStatus = ENotReady;
            iOutputStream->Stop();
            }
        else 
            {
            iStreamIdx++;
            TRAPD( error, iOutputStream->WriteL( GetFrame(iStreamIdx ) ) );    
            PanicIfError( error );
            }
        }
    else if (aError==KErrAbort) 
        {
        // Playing was aborted, 
        // due to call to CMdaAudioOutputStream::Stop().
        // MaoscRecordComplete() will be called 
        // after exiting this method.
        iOutputStatus = ENotReady;
        }
    else 
        {       
        iOutputStatus = ENotReady;
        }
    }

// ----------------------------------------------------------------------------
// CAudioStreamEngine::MaoscPlayComplete(
//     TInt aError)
//
// called when output stream is closed by CMdaAudioOutputStream::Stop() or if 
// end of audio data has been reached, in this case KErrUnderflow will be 
// returned.
// ----------------------------------------------------------------------------
void CAudioStreamEngine::MaoscPlayComplete(TInt aError)
    {
    iOutputStatus = ENotReady;
    if (aError==KErrNone) 
        {
        // normal stream closure
        }    
    else if (aError==KErrUnderflow) 
        {
        // end of audio data stream was reached because of stream underflow,
        }
    else 
        {
        // completed with error(s)
        }    
    }

// ----------------------------------------------------------------------------
// CAudioStreamEngine::ConstructL
// Standard EPOC 2nd phase constructor
// ----------------------------------------------------------------------------
//
void CAudioStreamEngine::ConstructL()
    {
    // Construct streams. We need to construct these here, 
    // so that at least the input stream
    // exists if SetEncodingL() is called before any recording has taken place 
    iOutputStream = CMdaAudioOutputStream::NewL( *this );

    // Get a handle to the RFs session to be used (owned by CEikonEnv, NOT to be closed
    // when this application exits!)        
    iFs = CEikonEnv::Static()->FsSession();

    // Stream buffer allocation (by default for PCM)
    iStreamBuffer = HBufC8::NewMaxL( iFrameSize * iFrameCount );
    iStreamStart = 0;
    iStreamEnd = iFrameCount - 1;
    
    // we used a high priority to avoid the delayed sound
    iIdle = CIdle::NewL( CActive::EPriorityUserInput );
    iIdle->Start( TCallBack( LoadFileL, this ) );
    }

// ----------------------------------------------------------------------------
// CAudioStreamEngine::CAudioStreamEngine()
//
// constructor
// ----------------------------------------------------------------------------
CAudioStreamEngine::CAudioStreamEngine(  const TDesC& aFileName  ) :
    iUseAMR( EFalse ),
    iAudioFile( aFileName ),
    iFrameSize( KFrameSizePCM ), 
    iFrameCount( KFrameCountPCM ), 
    iStreamBuffer( 0 ), 
    iFramePtr(0,0), 
    iBufferOK( EFalse )
    {
    // By default we use PCM and initialise the instance variables accordingly above.

    // Initial audio stream properties for input and output, 8KHz mono. 
    // These settings could also be set/changed using method SetAudioPropertiesL() of
    // the input and output streams.
    iStreamSettings.iChannels = TMdaAudioDataSettings::EChannelsMono;
    iStreamSettings.iSampleRate = TMdaAudioDataSettings::ESampleRate8000Hz;
    }

// ----------------------------------------------------------------------------
// TPtr8& CAudioStreamEngine::GetFrame(TUint aFrameIdx)
//
// Returns a modifiable pointer to a single frame inside the audio buffer 
// ----------------------------------------------------------------------------
TPtr8& CAudioStreamEngine::GetFrame(TUint aFrameIdx)
    {
    __ASSERT_ALWAYS( aFrameIdx < iFrameCount, PanicIfError( KErrArgument ) );

    iFramePtr.Set((TUint8*)(iStreamBuffer->Ptr() + (aFrameIdx * iFrameSize)),
            iFrameSize,
            iFrameSize);
    return iFramePtr;
    }

// ----------------------------------------------------------------------------
// TPtr8& CAudioStreamEngine::GetPlaybackFrames(TUint aLastFrame)
//
// Returns a modifiable pointer to the requested frames inside the audio buffer
// (from the first frame to aLastFrame). 
// ----------------------------------------------------------------------------
TPtr8& CAudioStreamEngine::GetPlaybackFrames( TUint aLastFrame )
    {
    __ASSERT_ALWAYS( aLastFrame < iFrameCount, PanicIfError( KErrArgument ) );

    iFramePtr.Set((TUint8*)( iStreamBuffer->Ptr() ),
           (aLastFrame + 1) * iFrameSize,
           (aLastFrame + 1) * iFrameSize);
    return iFramePtr;
    }


// ----------------------------------------------------------------------------
// CAudioStreamEngine::DoLoadFileL()
//
// loads the audio data from a file into the buffer
// ----------------------------------------------------------------------------
void CAudioStreamEngine::DoLoadFileL()
    {
    RFileReadStream audiofile;

    // Open file
    TInt err = audiofile.Open( iFs, iAudioFile, EFileRead|EFileStream );
    CleanupClosePushL( audiofile );
    // Empty the stream buffer
    iStreamBuffer->Des().FillZ( iFrameCount * iFrameSize );
    if ( err == KErrNone )
        {
        // file opened ok, proceed reading
        if ( iUseAMR )
            {
            // Read the AMR header (the first 6 bytes). 
            // We don't need to save/use the header,
            // since while playback we already know it's an AMR-NB encoded stream.
            TBuf8<KAMRHeaderLength> temp;
            audiofile.ReadL( temp, KAMRHeaderLength );
            }

        TUint idx=0;
        while ( idx < iFrameCount)
            {
            TRAPD( fstatus, audiofile.ReadL( GetFrame( idx ), iFrameSize ) );
            if ( fstatus!=KErrNone )
                break;
            idx++;
            }
        iStreamStart = 0;
        iStreamEnd = idx - 1;
        iBufferOK = ETrue;    
        }    
    else 
        {
        // failed to open file
        iBufferOK = EFalse;
        }

    CleanupStack::PopAndDestroy(); // audiofile
    audiofile.Close();
    }

// ----------------------------------------------------------------------------
// CAudioStreamEngine::DoPlay()
// ----------------------------------------------------------------------------
void CAudioStreamEngine::DoPlay()
    {
    // Open output stream.
    // Upon completion will receive callback in 
    // MMdaAudioOutputStreamCallBack::MaoscOpenComplete()
    iOutputStream->Open( &iStreamSettings );
    }

// ----------------------------------------------------------------------------
// CAudioStreamEngine::DoStop()
// ----------------------------------------------------------------------------
void CAudioStreamEngine::DoStop()
    {
    iOutputStream->Stop();
    }

// ----------------------------------------------------------------------------
// CAudioStreamEngine::LoadFileL()
// ----------------------------------------------------------------------------
TInt CAudioStreamEngine::LoadFileL( TAny* aPtr )
    {
    CAudioStreamEngine* self = static_cast<CAudioStreamEngine*> ( aPtr );
    self->DoLoadFileL();
    return 0;
    }

// ----------------------------------------------------------------------------
// CAudioStreamEngine::PlayCallBack()
// ----------------------------------------------------------------------------
TInt CAudioStreamEngine::PlayCallBack( TAny* aPtr )
    {
    CAudioStreamEngine* self = static_cast<CAudioStreamEngine*> ( aPtr );
    self->DoPlay();
    return 0;
    }

// ----------------------------------------------------------------------------
// CAudioStreamEngine::StopCallBack()
// ----------------------------------------------------------------------------
TInt CAudioStreamEngine::StopCallBack( TAny* aPtr )
    {
    CAudioStreamEngine* self = static_cast<CAudioStreamEngine*> ( aPtr );
    self->DoStop();
    return 0;
    }

// End of file
