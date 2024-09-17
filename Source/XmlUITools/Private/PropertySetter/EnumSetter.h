#pragma once
#include "PropertySetter.h"

namespace XmlUITools
{
	class FEnumSetter : public IPropertySetter
	{
	public:
		FEnumSetter(FProperty* Property, UEnum* InEnum) : IPropertySetter(Property), Enum(InEnum) {}

		virtual bool SetValue(void* Container, const FString& Value) override;
	private:
		UEnum* Enum;
	};
}

