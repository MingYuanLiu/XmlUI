#pragma once

#include "CoreMinimal.h"
#include "XmlUmgTree.h"

namespace XmlUITools
{
	class IPropertySetter
	{
	public:
		virtual bool SetValue(void* Container, const FString& Value) = 0;

		virtual bool SetValue(void* Container, const FString& PropertyName, const FXmlAttribute* XmlAttribute,
							UClass* ContainerClass, void* PropertyValue, FString* OutFailureReason) = 0;
		
		virtual ~IPropertySetter() {}
	
	protected:
		explicit IPropertySetter(FProperty* TheProperty) : Property(TheProperty) {}

		FProperty* Property;
	};
}

