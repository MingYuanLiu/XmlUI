#pragma once
#include "PropertySetter.h"

namespace XmlUITools
{
	class StructSetter : public IPropertySetter
	{
	public:
		explicit StructSetter(FProperty* Property) : IPropertySetter(Property) {}

		virtual bool SetValue(void* Container, const FString& PropertyName, const FXmlAttribute* XmlAttribute, UClass* ContainerClass, void* PropertyValue, FString* OutFailureReason) override;

		virtual bool SetValue(void* Container, const FString& Value) override;
	};
}

