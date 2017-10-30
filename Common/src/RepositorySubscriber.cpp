/* ====================================================================
 * File: RepositorySubscriber.cpp
 * Created: 07/06/10
 * Modified: 07/06/10
 * Author: Sun Jinbo
 * Copyright (c): Tieto, All rights reserved
 * ==================================================================== */

// INCLUDE FILES
#include <e32property.h>
#include "RepositorySubscriber.h"

// CONSTANTS
const TInt KDefaultGranularity = 2;

// ============================= LOCAL FUNCTIONS  =============================

// ============================= MEMBER FUNCTIONS =============================

// ----------------------------------------------------------------------------
// CRepositorySubscriber::NewL
// Two-phased contructor
// ----------------------------------------------------------------------------
//
EXPORT_C CRepositorySubscriber* CRepositorySubscriber::NewL( 
                                                const TUid& aCategory,
                                                MRepositoryObserver* aObserver )
    {
    CRepositorySubscriber* self = 
                new ( ELeave ) CRepositorySubscriber( aCategory, aObserver );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    return self;
    }

// ----------------------------------------------------------------------------
// CRepositorySubscriber::~CRepositorySubscriber
// Destructor
// ----------------------------------------------------------------------------
//
EXPORT_C CRepositorySubscriber::~CRepositorySubscriber()
    {
    if ( iKeyArray )
        {
        iKeyArray->ResetAndDestroy();
        delete iKeyArray;
        }
    
    if ( iCR )
        {
        iCR->NotifyCancelAll();
        delete iCR;
        }
    }

// ----------------------------------------------------------------------------
// CRepositorySubscriber::SetObserver
// ----------------------------------------------------------------------------
//    
EXPORT_C void CRepositorySubscriber::SetObserver( MRepositoryObserver* aObserver )
    {
    iObserver = aObserver;
    }

// ----------------------------------------------------------------------------
// CRepositorySubscriber::SetValue
// Set value
// ----------------------------------------------------------------------------
//
EXPORT_C TInt CRepositorySubscriber::SetValue( 
                                    const TUint32& aIndex, 
                                    const TInt& aValue )
    {
    const TInt response = iCR->Set( aIndex, aValue );
    
    return response;
    }

// ----------------------------------------------------------------------------
// CRepositorySubscriber::SetValue
// Set value
// ----------------------------------------------------------------------------
//
EXPORT_C TInt CRepositorySubscriber::SetValue( 
                                    const TUint32& aIndex, 
                                    const TDesC8& aValue )
    {
    const TInt response = iCR->Set( aIndex, aValue );

    return response;
    }

// ----------------------------------------------------------------------------
// CRepositorySubscriber::SetValue
// Set value
// ----------------------------------------------------------------------------
//
EXPORT_C TInt CRepositorySubscriber::SetValue( 
                                    const TUint32& aIndex, 
                                    const TDesC16& aValue )
    {
    const TInt response = iCR->Set( aIndex, aValue );
    
    return response;
    }          

// ----------------------------------------------------------------------------
// CRepositorySubscriber::GetValue
// Get value
// ----------------------------------------------------------------------------
//
EXPORT_C TInt CRepositorySubscriber::GetValue( 
                                    const TUint32& aIndex, 
                                    TInt& aValue )
    {
    const TInt response = iCR->Get( aIndex, aValue );

    return response;
    }    

// ----------------------------------------------------------------------------
// CRepositorySubscriber::GetValue
// Get value
// ----------------------------------------------------------------------------
//
EXPORT_C TInt CRepositorySubscriber::GetValue( 
                                    const TUint32& aIndex, 
                                    TDes8& aValue )
    {
    const TInt response = iCR->Get( aIndex, aValue );

    return response;
    } 

// ----------------------------------------------------------------------------
// CRepositorySubscriber::GetValue
// Get value
// ----------------------------------------------------------------------------
//
EXPORT_C TInt CRepositorySubscriber::GetValue( 
                                    const TUint32& aIndex, 
                                    TDes16& aValue )
    {
    const TInt response = iCR->Get( aIndex, aValue );

    return response;
    }

// ----------------------------------------------------------------------------
// CRepositorySubscriber::SubscribeL
// Subscribe key
// ----------------------------------------------------------------------------
//
EXPORT_C void CRepositorySubscriber::SubscribeL( const TUint32& aKey )
    {
    
    CRepositorySubscriber::CCRepositorySubscriberKey* subscriber = 
            CRepositorySubscriber::CCRepositorySubscriberKey::NewL( 
                                                                aKey,
                                                                *this,
                                                                *iCR );
    CleanupStack::PushL( subscriber );
    iKeyArray->AppendL( subscriber );
    CleanupStack::Pop( subscriber ); // transfer ownership
    }

// ----------------------------------------------------------------------------
// CRepositorySubscriber::Unsubscribe
// ----------------------------------------------------------------------------
//
EXPORT_C void CRepositorySubscriber::Unsubscribe( const TUint32& aKey )
    {
    
    for( TInt i = 0; i < iKeyArray->Count(); ++i )
        {
        CRepositorySubscriber::CCRepositorySubscriberKey* subscriber = 
                                                                (*iKeyArray)[i];
        if ( subscriber->Key() == aKey )
            {
            iKeyArray->Delete( i );
            delete subscriber;
            subscriber = NULL;
            }
        }
    }

// ----------------------------------------------------------------------------
// CRepositorySubscriber::RepositoryValueChangedL
// Internal callback
// ----------------------------------------------------------------------------
//
void CRepositorySubscriber::RepositoryValueChangedL( 
                                                const TUid& /* aCategory */, 
                                                const TUint32& aKey )
    {
    
    if ( iObserver )
        {
        iObserver->RepositoryValueChangedL( iCategory, aKey );
        }
    }       

// ----------------------------------------------------------------------------
// CRepositorySubscriber::CRepositorySubscriber
// C++ Constructor
// ----------------------------------------------------------------------------
//
CRepositorySubscriber::CRepositorySubscriber( 
                                        const TUid& aCategory,
                                        MRepositoryObserver* aObserver ) : 
    iCategory( aCategory ),
    iObserver( aObserver )
    {
    }

// ----------------------------------------------------------------------------
// CRepositorySubscriber::ConstructL
// Constructor
// ----------------------------------------------------------------------------
//
void CRepositorySubscriber::ConstructL()
    {
    
    iKeyArray = new ( ELeave ) 
        CArrayPtrFlat<CRepositorySubscriber::CCRepositorySubscriberKey>( 
                                                        KDefaultGranularity );
    iCR = CRepository::NewL( iCategory );
    }

// ----------------------------------------------------------------------------
// CRepositorySubscriber::CCRepositorySubscriberKey::NewL
// ----------------------------------------------------------------------------
//
CRepositorySubscriber::CCRepositorySubscriberKey* 
        CRepositorySubscriber::CCRepositorySubscriberKey::NewL( 
                                                const TUint32& aKey,
                                                MRepositoryObserver& aObserver,
                                                CRepository& aCR )
    {
    CRepositorySubscriber::CCRepositorySubscriberKey* self = 
       new ( ELeave ) CRepositorySubscriber::CCRepositorySubscriberKey( 
                                                                   aKey,
                                                                   aObserver,
                                                                   aCR );
                                                                   
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop(); // self

    return self;
    }

// ----------------------------------------------------------------------------
// CRepositorySubscriber::CCRepositorySubscriberKey::~CCRepositorySubscriberKey
// ----------------------------------------------------------------------------
//
CRepositorySubscriber::CCRepositorySubscriberKey::~CCRepositorySubscriberKey()
    {
    if ( IsActive() )
        {
        Cancel();
        }
    }

// ----------------------------------------------------------------------------
// CRepositorySubscriber::CCRepositorySubscriberKey::Key
// ----------------------------------------------------------------------------
//    
TUint32 CRepositorySubscriber::CCRepositorySubscriberKey::Key()
    {
    return iKey;
    }

// ----------------------------------------------------------------------------
// CRepositorySubscriber::CCRepositorySubscriberKey::~CCRepositorySubscriberKey
// ----------------------------------------------------------------------------
//
void CRepositorySubscriber::CCRepositorySubscriberKey::RunL()
    {

    iObserver.RepositoryValueChangedL( TUid::Uid( 0 ), iKey );
    
    Subscribe();
    }

// ----------------------------------------------------------------------------
// CRepositorySubscriber::CCRepositorySubscriberKey::DoCancel
// ----------------------------------------------------------------------------
//
void CRepositorySubscriber::CCRepositorySubscriberKey::DoCancel()
    {

    iCR.NotifyCancel( iKey );
    }

// ----------------------------------------------------------------------------
// CRepositorySubscriber::CCRepositorySubscriberKey::Subscribe
// ----------------------------------------------------------------------------
//
void CRepositorySubscriber::CCRepositorySubscriberKey::Subscribe()
    {

    iCR.NotifyRequest( iKey, iStatus );
    SetActive();
    }

// ----------------------------------------------------------------------------
// CRepositorySubscriber::CCRepositorySubscriberKey::ConstructL
// ----------------------------------------------------------------------------
//
void CRepositorySubscriber::CCRepositorySubscriberKey::ConstructL()
    {
    Subscribe();
    }

// ----------------------------------------------------------------------------
// CRepositorySubscriber::CCRepositorySubscriberKey::CCRepositorySubscriberKey
// C++ Constructor
// ----------------------------------------------------------------------------
//
CRepositorySubscriber::CCRepositorySubscriberKey::CCRepositorySubscriberKey( 
                                                const TUint32& aKey,
                                                MRepositoryObserver& aObserver,
                                                CRepository& aCR ) : 
    CActive( EPriorityNormal ),
    iKey( aKey ),
    iObserver( aObserver ),
    iCR( aCR )
    {
    CActiveScheduler::Add( this );            
    }                                                                           

// ========================= OTHER EXPORTED FUNCTIONS =========================

// End of File
