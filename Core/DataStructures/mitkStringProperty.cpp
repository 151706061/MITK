#include "mitkStringProperty.h"

mitk::StringProperty::StringProperty( const char* string ) 
: m_String( string ) {

}

bool mitk::StringProperty::operator==(const BaseProperty& property ) const {
	
	// auchtung Type-Pr�fung durchf�hren !!!!!
	if ( ((mitk::StringProperty&)property).m_String == m_String )
		return true;

	return false;
}