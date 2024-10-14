#pragma once
#include "PropertySetter.h"

namespace XmlUITools
{
	class FObjectSetter : public IPropertySetter
	{
	public:
		FObjectSetter(FProperty* Property) : IPropertySetter(Property) {}

		virtual bool SetValue(void* Container, const FString& Value) override;
	};
}

