#pragma once
#include "CoreMinimal.h"
#include "UObject/UnrealType.h"

#include "PropertySetter.h"

namespace XmlUITools
{
	class FSetterFactory
	{
	public:
		static IPropertySetter* CreateSetter(FProperty* Property, const FString& Value);
	};

}
