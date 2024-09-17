#include "NumberSetter.h"

#include "LogXmlUmg.h"

namespace XmlUITools
{
	bool FNumberSetter::SetValue(void* Container, const FString& Value)
	{
		if (!Property)
		{
			UE_LOG(LogXmlUmg, Error, TEXT("Can not set number due to property is null"))
			return false;
		}

		if (FIntProperty* IntProperty = CastField<FIntProperty>(Property))
		{
			int32 IntVal = FCString::Atoi(*Value);
			IntProperty->SetValue_InContainer(Container, IntVal);
		}
		else if (FFloatProperty* FloatProperty = CastField<FFloatProperty>(Property))
		{
			float FloatVal = FCString::Atof(*Value);
			FloatProperty->SetValue_InContainer(Container, FloatVal);
		}
		else
		{
			UE_LOG(LogXmlUmg, Warning, TEXT("Not match int and float property, default set int value to container"))
			int32 Val = FCString::Atoi(*Value);
			Property->SetValue_InContainer(Container, &Val);
		}
		
		return true;
	}
}
