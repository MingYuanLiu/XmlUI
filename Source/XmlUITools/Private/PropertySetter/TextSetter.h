#pragma once
#include "PropertySetter.h"

namespace XmlUITools
{
	class FTextSetter : public IPropertySetter
	{
	public:
	public:
		FTextSetter(FProperty* Property) : IPropertySetter(Property) {}

		virtual bool SetValue(void* Container, const FString& Value) override;
	};
}

