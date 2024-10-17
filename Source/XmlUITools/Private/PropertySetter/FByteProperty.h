#pragma once
#include "PropertySetter.h"

namespace XmlUITools
{
	class FBytePropertySetter : public IPropertySetter
	{
	public:
		FBytePropertySetter(FProperty* InProperty) : IPropertySetter(InProperty) {};

		virtual bool SetValue(void* InObject, const FString& InValue) override;

		virtual bool SetValue(void* Container, const FString& PropertyName, const FXmlAttribute* XmlAttribute, UClass* ContainerClass, void* PropertyValue, FString* OutFailureReason) override;
	};
}

