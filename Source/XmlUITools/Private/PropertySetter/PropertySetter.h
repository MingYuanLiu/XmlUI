#pragma once

#include "CoreMinimal.h"

namespace XmlUITools
{
	class IPropertySetter
	{
	public:
		virtual bool SetValue(void* Container, const FString& Value) = 0;

		virtual ~IPropertySetter() {}
	
	protected:
		explicit IPropertySetter(FProperty* TheProperty) : Property(TheProperty) {}

		FProperty* Property;
	};
}

