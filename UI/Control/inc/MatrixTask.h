/* ====================================================================
 * File: MatrixTask.h
 * Created: 04/19/09
 * Modified: 04/19/09
 * Author: Sun Jinbo
 * Copyright (c): Tieto, All rights reserved
 * ==================================================================== */

#ifndef C_MATRIXTASK_H
#define C_MATRIXTASK_H

// INCLUDE FILES
#include <e32std.h>
#include <e32base.h>
#include <Block.h>

// ENUMERATES
// for specify the type of matrix task
enum TMatrixTaskType
    {
    EMatrixTaskNone = -1,
    EMatrixTaskDarken,
    EMatrixTaskLighten,
    EMatrixTaskDrawing,
    EMatrixTaskClear,
    EMatrixTaskBlasting,
    EMatrixTaskCollapsed
    };

// FORWARD DECLARATIONS
class MMatrixTaskObserver;
class CMatrixControl;

// CLASS DECLARATION
/**
* CMatrixTask class
*
* @since S60 3.1
*/
class CMatrixTask : public CTimer
    {
public: // Constructor and Destructor
    static CMatrixTask* NewL( CMatrixControl& aControl, MMatrixTaskObserver& aObserver );
    virtual ~CMatrixTask();

public: // New functions
    void Append( const TBlock& aBlock );
    void Start();
    void SetTaskType( TMatrixTaskType aType );

protected: // Constructor
    CMatrixTask( CMatrixControl& aControl, MMatrixTaskObserver& aObserver );
    void ConstructL();

protected: // from base class CTimer
    virtual void RunL();

private: // New fucntions
    void DrawMatrix();
    void DrawBlock( const TBlock& aBlock, TInt aOffsetY = 0 );
    void DrawMask( const TBlock& aMask, TInt aOffsetY = 0 );
    void DoCollapsed();

protected: // Data members
    MMatrixTaskObserver& iObserver;
    CMatrixControl& iControl;
    TMatrixTaskType iType;
    RArray<TBlock> iArray;
    TInt iOpacity;
    TBool iCollapsed;
    };

#endif // C_MATRIXTASK_H

// End of File
