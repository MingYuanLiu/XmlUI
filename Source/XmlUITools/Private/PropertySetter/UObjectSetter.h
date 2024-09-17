#pragma once
#include "PropertySetter.h"

namespace XmlUITools
{
	class FUObjectSetter : public IPropertySetter
	{
	public:
		FUObjectSetter(FProperty* Property) : IPropertySetter(Property) {}

		virtual bool SetValue(void* Container, const FString& Value) override;
	};
}

