#pragma once

#include "PropertySetter.h"

namespace XmlUITools
{
	class FBoolSetter : public IPropertySetter
	{
	public:
		FBoolSetter(FProperty* Property) : IPropertySetter(Property) {}

		virtual bool SetValue(void* Container, const FString& Value) override;

		virtual bool SetValue(void* Container, const FXmlAttribute* XmlAttribute, UClass* ContainerClass, FString* OutFailureReason) override;
	};
}
