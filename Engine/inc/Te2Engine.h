/* ====================================================================
 * File: Te2Engine.h
 * Created: 01/17/09
 * Modified: 05/04/09
 * Author: Sun Jinbo
 * Copyright (c): Tieto, All rights reserved
 * ==================================================================== */

#ifndef C_TE2ENGINE_H
#define C_TE2ENGINE_H

// INCLUDE FILES
#include <e32def.h>
#include "TetriminoObserver.h"
#include "ScorekeeperObserver.h"


// FORWARD DECLARATIONS
class TBlock;
class CMatrix;
class CTetriminoFactory;
class CFixedTetrimino;
class CNextRemovableTetrimino;
class CRemovableTetrimino;
class CAnimationTetrimino;
class CScorekeeper;
class MEngineObserver;

// Enumeration for specifying
enum TEngineType // for create what kinds of Engine type
    {
    EEngineNormal = 0,
#ifdef RD_TE2_ADVANCE_FUNC
    EEngineExtra
#endif // RD_TE2_ADVANCE_FUNC
    };

enum TEngineCmd // for handle all kinds of commands
    {
    EEngineStart = 1,
    EEngineStop,
    EEngineRestart,
    EEnginePause,
    EEngineResume,
    EEngineLeft,
    EEngineRight,
    EEngineRotate,
    EEngineDescend,
    EEngineFastDescendOn,
    EEngineFastDescendOff,
    EEngineDoubleDescend,
    EEngineUpdateAll,
    EEngineCleanRank
    };


// CLASS DECLARATION
/**
* CTe2Engine class
*
* @since S60 3.1
*/
class CTe2Engine : public CBase,
    public MTetriminoObserver,
    public MScorekeeperObserver
    {
public:
    IMPORT_C static CTe2Engine* NewL( 
        MEngineObserver& aObserver, TEngineType aType = EEngineNormal );

    IMPORT_C ~CTe2Engine();

    IMPORT_C TInt HandleCommandL( TEngineCmd aCmd );

    IMPORT_C TInt GetRankItem( TInt aRank, TTime& aTime, TInt& aScore  );

protected: // from base class MTetriminoObserver
    void StateChangedL( TTetriminoState aNewState ); 

protected: // from base class MScorekeeperObserver
    void StateChangedL( TScorekeeperState aNewState );

private:
    CTe2Engine( MEngineObserver& aObserver );
    void ConstructL( TEngineType aType );
    void StartL();
    void Stop();

private:
    MEngineObserver& iObserver;
    CScorekeeper* iScorekeeper;
    CMatrix* iPrimaryMatrix;
    CMatrix* iSecondaryMatrix;
    CTetriminoFactory* iTetriminoFactory;
    CRemovableTetrimino* iRemovableTetrimino;
    CNextRemovableTetrimino* iNextRemovableTetrimino;
    CFixedTetrimino* iFixedTetrimino;
    CAnimationTetrimino* iAnimationTetrimino;
    TBool iPauseState;
    TBool iFirstTetrimino;
    };

#endif // C_TE2ENGINE_H

// End of File
