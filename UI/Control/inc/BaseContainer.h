/* ====================================================================
 * File: BaseContainer.h
 * Created: 03/15/09
 * Modified: 05/09/09
 * Author: Sun Jinbo
 * Copyright (c): Tieto, All rights reserved
 * ==================================================================== */

#ifndef C_BASECONTAINER_H
#define C_BASECONTAINER_H

// INCLUDE FILES
#include <coecntrl.h>
#include "LayoutManager.h"

// FORWARD DECLARATIONS
class MModelObserver;
class CFbsBitmap;
class CFbsBitmapDevice;
class CFbsBitGc;
class CPeriodic;
class CBackgroundControl;
class CMenubarControl;
class CTe2LayoutManager;

// CLASS DECLARATION
/**
* CBaseContainer class
*
* @since S60 3.1
*/
class CBaseContainer : public CCoeControl
    {
public: // Constructor and Destructor
    virtual ~CBaseContainer();

public: // from base class CCoeControl
    virtual void Draw( const TRect& aRect ) const; 
    virtual void SizeChanged();
    virtual void HandleResourceChange( TInt aType ); 
    virtual TKeyResponse OfferKeyEventL( const TKeyEvent& aKeyEvent, TEventCode aType );
    virtual void HandlePointerEventL(const TPointerEvent& aPointerEvent);

public: // New function
    IMPORT_C void SetMenuBar1L( const TDesC& aLabel, TInt aId );
    IMPORT_C void SetMenuBar2L( const TDesC& aLabel, TInt aId );
    IMPORT_C void StoreL();
    virtual void Draw();

protected: // Constructor
    CBaseContainer( MModelObserver& aObserver );
    void ConstructL( const TRect& aRect );

private: // New functions
    void CreateBmpBufferL();
    void ReleaseBmpBuffer();
    void Stop();
    void DoFrame();
    static TInt TimerCallback( TAny* aPtr );
    void LayoutSoftkeys();

protected: // Data members
    MModelObserver& iObserver;
    CTe2LayoutManager* iLayoutMgr; 
    CPeriodic* iPeriodic;
    CFbsBitmap* iBmpBuffer;
    CFbsBitmapDevice* iBmpBufferDevice;
    CFbsBitGc* iBmpBufferContext;
    CBackgroundControl* iBackground;
    CMenubarControl* iMenubar1;
    CMenubarControl* iMenubar2;
    TInt iSoftkey1Id;
    TInt iSoftkey2Id;
    TResolution iResolution;
    };

#endif // C_BASECONTAINER_H

// End of File
