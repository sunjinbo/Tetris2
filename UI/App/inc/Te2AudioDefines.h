/* ====================================================================
 * File: Te2AudioDefines.h
 * Created: 06/17/10
 * Author: Sun Jinbo
 * Copyright (c): Tieto, All rights reserved
 * ==================================================================== */

#ifndef TE2AUDIODEFINES_H
#define TE2AUDIODEFINES_H

// INCLUDE FILES
#include <e32cmn.h>

// CLASS DECLARATION
class CAudioUtils;

// ENUMERATIONS
enum TAudioSourceId
    {
    EAudioWater = 0,
    EAudioTwitter,
    EAudioWind,
    EAudioChirp,
    EAudioTweedle,
    EAudioBreeze,
    EAudioBillow,
    EAudioCaw,
    EAudioGameOver,
    EAudioReward,
    EAudioLastElement
    };

enum TAudioPriority
    {
    TAudioPriorityLow,
    TAudioPriorityNormal,
    TAudioPriorityHigh
    };

// STRUCT DECLARATION
typedef CAudioUtils* TAudioUtilsPointer;

typedef struct
{
    TAudioSourceId iSourceId;   // audio source id
    TFileName   iAudioFileName;    // path of the file holding the audio
} TAudioSource;

typedef struct
{
    TAudioSourceId iSourceId;   // audio source id
    TAudioPriority iPriority; // audio priority
    TTime iTime; // audio creation time
    TTimeIntervalMicroSeconds iExpiriedInterval; // expiried interval
    TAudioUtilsPointer iAudioUtilsPointer; // pointer of audio utils
} TAudioRequest;

const TAudioSourceId KOpeningSourceId = EAudioWater;

#endif // TE2AUDIODEFINES_H

// End of File
