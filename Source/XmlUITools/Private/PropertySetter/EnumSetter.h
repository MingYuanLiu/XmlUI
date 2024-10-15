#pragma once
#include "PropertySetter.h"

namespace XmlUITools
{
	class FEnumSetter : public IPropertySetter
	{
	public:
		FEnumSetter(FProperty* Property, UEnum* InEnum) : IPropertySetter(Property), mEnum(InEnum) {}

		virtual bool SetValue(void* Container, const FString& Value) override;

		virtual bool SetValue(void* Container, const FString& PropertyName, const FXmlAttribute* XmlAttribute,
			UClass* ContainerClass, void* PropertyValue = nullptr, FString* OutFailureReason = nullptr) override;
	private:
		UEnum* mEnum;
	};
}

