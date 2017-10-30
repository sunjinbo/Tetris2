/* ====================================================================
 * File: Te2PSKeys.h
 * Created: 07/10/10
 * Modifed: 07/10/10
 * Author: Sun Jinbo
 * Copyright (c): Tieto, All rights reserved
 * ==================================================================== */

#ifndef TE2PSKEYS_H
#define TE2PSKEYS_H

/**
 * The Uid for the Publish&Subscribe itself
 * This PS holds the settings for Tetris2
 */
const TUid KPSUidTetris2 = { 0xe01ff1c2 };

const TUint32 KTetris2AudioCommand = 0x00000001;

enum TAudioCommands
    {
    EAudioNotSet,
    EAudioOn,
    EAudioOff,
    EAudioIncVolume,
    EAudioDecVolume,
    EAudioRewardSound,
    EAudioGameOverSound,
    EAudioExit
    };

const TUint32 KTetris2AudioFeedback = 0x00000002;

enum TAudioFeedback
    {
    EUninitialized,
    EPreparedCompleted,
    EPreparedFailed,
    EExitCompleted
    };

#endif // TE2PSKEYS_H

// End Of File
