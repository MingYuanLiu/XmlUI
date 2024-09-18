#pragma once
#include "PropertySetter.h"

namespace XmlUITools
{
	class FStringSetter : public IPropertySetter
	{
	public:
		FStringSetter(FProperty* Property) : IPropertySetter(Property) {}

		virtual bool SetValue(void* Container, const FString& Value) override;
	};
}

