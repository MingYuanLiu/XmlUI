#pragma once
#include "PropertySetter.h"

namespace XmlUITools
{
	class FNumberSetter : public IPropertySetter
	{
	public:
		FNumberSetter(FProperty* Property) : IPropertySetter(Property) {}

		virtual bool SetValue(void* Container, const FString& Value) override;

		virtual bool SetValue(void* Container, const FXmlAttribute* XmlAttribute, UClass* ContainerClass, FString* OutFailureReason) override;
	};
}

