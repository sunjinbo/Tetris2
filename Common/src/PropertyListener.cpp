/* ====================================================================
 * File: PropertyListener.cpp
 * Created: 07/06/10
 * Modified: 07/06/10
 * Author: Sun Jinbo
 * Copyright (c): Tieto, All rights reserved
 * ==================================================================== */


// INCLUDE FILES
#include "PropertyListener.h"
#include "PropertyListenerCallback.h"

// CONSTANTS
#ifdef _DEBUG
_LIT( KPanicCat, "PropertyListener" );

enum TPanicReason
    {
    EAlreadyListening
    };
    
LOCAL_C void Panic( TInt aReason )
    {
    User::Panic( KPanicCat, aReason );
    }
#endif

//-----------------------------------------------------------------------------
// CPropertyListener::NewL
// Constructor
//-----------------------------------------------------------------------------
//
EXPORT_C CPropertyListener* CPropertyListener::NewL(const TUid& aCategory,
		const TUint32& aKey, MPropertyListenerCallBack& aCallBack)
	{
	CPropertyListener* self = CPropertyListener::NewLC( aCategory, aKey,
			aCallBack );
	CleanupStack::Pop( self );

	return self;
    }

//-----------------------------------------------------------------------------
// CPropertyListener::NewLC
// Constructor
//-----------------------------------------------------------------------------
//
EXPORT_C CPropertyListener* CPropertyListener::NewLC(const TUid& aCategory,
		const TUint32& aKey, MPropertyListenerCallBack& aCallBack)
	{
	CPropertyListener* self = new (ELeave) CPropertyListener( aCallBack );
	CleanupStack::PushL( self );
	self->ConstructL( aCategory, aKey );

	return self;
	}

//-----------------------------------------------------------------------------
// CPropertyListener::~CPropertyListener
// Destructor
//-----------------------------------------------------------------------------
//
EXPORT_C CPropertyListener::~CPropertyListener()
	{
	Cancel();
	iProperty.Close();
    }

//-----------------------------------------------------------------------------
// CPropertyListener::Listen
//-----------------------------------------------------------------------------
//
EXPORT_C void CPropertyListener::Listen()
    {
    __ASSERT_DEBUG( !IsActive(), Panic( EAlreadyListening ) );
    
    iProperty.Subscribe( iStatus );
    SetActive();
    }


// ----------------------------------------------------------------------------
// CPropertyListener::SetValue
// Set value
// ----------------------------------------------------------------------------
//
EXPORT_C TInt CPropertyListener::SetValue( const TInt& aValue )
    {
    const TInt response = RProperty::Set( iCategory, iKey, aValue );
    
    return response;
    }

// ----------------------------------------------------------------------------
// CPropertyListener::SetValue
// Set value
// ----------------------------------------------------------------------------
//
EXPORT_C TInt CPropertyListener::SetValue( const TDesC8& aValue )
    {
    const TInt response = RProperty::Set( iCategory, iKey, aValue );

    return response;
    }

// ----------------------------------------------------------------------------
// CPropertyListener::SetValue
// Set value
// ----------------------------------------------------------------------------
//
EXPORT_C TInt CPropertyListener::SetValue( const TDesC16& aValue )
    {
    const TInt response = RProperty::Set( iCategory, iKey, aValue );
    
    return response;
    }          

// ----------------------------------------------------------------------------
// CPropertyListener::GetValue
// Get value
// ----------------------------------------------------------------------------
//
EXPORT_C TInt CPropertyListener::GetValue( TInt& aValue )
    {
    const TInt response = RProperty::Get( iCategory, iKey, aValue );

    return response;
    }    

// ----------------------------------------------------------------------------
// CPropertyListener::GetValue
// Get value
// ----------------------------------------------------------------------------
//
EXPORT_C TInt CPropertyListener::GetValue( TDes8& aValue )
    {
    const TInt response = RProperty::Get( iCategory, iKey, aValue );

    return response;
    }

// ----------------------------------------------------------------------------
// CPropertyListener::GetValue
// Get value
// ----------------------------------------------------------------------------
//
EXPORT_C TInt CPropertyListener::GetValue( TDes16& aValue )
    {
    const TInt response = RProperty::Get( iCategory, iKey, aValue );

    return response;
    }

// METHODS

//-----------------------------------------------------------------------------
// CPropertyListener::RunL
//-----------------------------------------------------------------------------
//
void CPropertyListener::RunL()
    {
	if( iStatus == KErrNone )
		{
		Listen();
		iCallBack.HandlePropertyChangedL();
		}
	}

//-----------------------------------------------------------------------------
// CPropertyListener::DoCancel
//-----------------------------------------------------------------------------
//
void CPropertyListener::DoCancel()
    {
    iProperty.Cancel();
    }

//-----------------------------------------------------------------------------
// CPropertyListener::RunError
//-----------------------------------------------------------------------------
//
TInt CPropertyListener::RunError( TInt /*aError*/ )
    {
    Listen();
    return KErrNone;
    }

//-----------------------------------------------------------------------------
// CPropertyListener::CPropertyListener
// Constructor
//-----------------------------------------------------------------------------
//
CPropertyListener::CPropertyListener( MPropertyListenerCallBack& aCallBack ):
    CActive( EPriorityUserInput ),
    iCallBack( aCallBack )
    {
    CActiveScheduler::Add( this );
    }

//-----------------------------------------------------------------------------
// CPropertyListener::ConstructL
// Constructor
//-----------------------------------------------------------------------------
//
void CPropertyListener::ConstructL(const TUid& aCategory, const TUint32& aKey)
	{
	iCategory = aCategory;
	iKey = aKey;
	User::LeaveIfError( iProperty.Attach( iCategory, iKey ) );
    }

// End of file

