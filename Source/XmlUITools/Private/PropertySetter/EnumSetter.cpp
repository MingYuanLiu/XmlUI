#include "EnumSetter.h"

#include "LogXmlUmg.h"

namespace XmlUITools
{
	bool FEnumSetter::SetValue(void* Container, const FString& Value)
	{
		if (!Container)
		{
			UE_LOG(LogXmlUmg, Error, TEXT("Can not set enum value due to container is null"))
			return false;
		}

		auto EnumValue = Enum->GetValueByNameString(Value);
		if (EnumValue == INDEX_NONE)
		{
			UE_LOG(LogXmlUmg, Error, TEXT("Enum value %s can not found"), *Value)
			return false;
		}

		if (Property)
		{
			Property->SetValue_InContainer(Container, &EnumValue);
		}
		
		return true;
	}

}
