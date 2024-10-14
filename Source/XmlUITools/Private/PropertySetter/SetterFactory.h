#pragma once
#include "CoreMinimal.h"
#include "UObject/UnrealType.h"

#include "PropertySetter.h"
#include "XmlUmgTree.h"

namespace XmlUITools
{
	class FSetterFactory
	{
	public:
		static IPropertySetter* CreateSetter(FProperty* Property, const FString& Value);

		static IPropertySetter* CreateSetter(FProperty* Property, EXmlAttributeType Type);
	};

}
