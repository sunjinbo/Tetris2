/* ====================================================================
 * File: PropertyListener.h
 * Created: 07/06/10
 * Modified: 07/06/10
 * Author: Sun Jinbo
 * Copyright (c): Tieto, All rights reserved
 * ==================================================================== */

#ifndef C_PROPERTYLISTENER_H
#define C_PROPERTYLISTENER_H

// INCLUDE FILES
#include <e32base.h>
#include <e32property.h>

// FORWARD DECLARATIONS
class MPropertyListenerCallBack;

/**
 * CPropertyListener
 */
class CPropertyListener : public CActive
	{
public:

	// Two phased constructors
	IMPORT_C static CPropertyListener* NewL(const TUid& aCategory,
            const TUint32& aKey, MPropertyListenerCallBack& aCallBack);
	IMPORT_C static CPropertyListener* NewLC(const TUid& aCategory,
            const TUint32& aKey, MPropertyListenerCallBack& aCallBack);

    // Destructor
	IMPORT_C virtual ~CPropertyListener();

public:
    // New methods

    // Start listening
	IMPORT_C void Listen();

	/**
    * Set value
    *
    * @param aValue New value
    * @return KErrNone, or system wide error code
    */
	IMPORT_C TInt SetValue( const TInt& aValue );

	/**
    * Set value
    *
    * @param aValue New value
    * @return KErrNone, or system wide error code
    */
	IMPORT_C TInt SetValue( const TDesC8& aValue );                     

	/**
    * Set value
    *
    * @param aValue New value
    * @return KErrNone, or system wide error code
    */
	IMPORT_C TInt SetValue( const TDesC16& aValue );      

	/**
    * Get value
    *
    * @param aValue Value
    * @return KErrNone, or system wide error code
    */
	IMPORT_C TInt GetValue( TInt& aValue );

	/**
    * Get value
    *
    * @param aValue Value
    * @return KErrNone, or system wide error code
    */
	IMPORT_C TInt GetValue( TDes8& aValue );

	/**
    * Get value
    *
    * @param aValue Value
    * @return KErrNone, or system wide error code
    */
	IMPORT_C TInt GetValue( TDes16& aValue );

private:
    // From base classes

    // @see CActive
    void RunL();

    // @see CActive
    void DoCancel();

    // @see CActive
    TInt RunError(TInt aError);

private:
    CPropertyListener(MPropertyListenerCallBack& aCallBack);
    
    void ConstructL(const TUid& aCategory, const TUint32& aKey);

private:

    // Own: P&S property
    RProperty iProperty;

    // Ref: Call back
    MPropertyListenerCallBack& iCallBack;

	// The category of the Property to be observed
	TUid iCategory;

	// The subkey of the Property to be observed
	TUint iKey;

    };

#endif //C_PROPERTYLISTENER_H
