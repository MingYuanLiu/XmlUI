#include "BoolSetter.h"

#include "LogXmlUmg.h"

namespace XmlUITools
{
	bool FBoolSetter::SetValue(void* Container, const FString& Value)
	{
		if (!Container)
		{
			UE_LOG(LogXmlUmg, Error, TEXT("Can not set value to null container"))
			return false;
		}

		if (FBoolProperty* BoolProperty = CastField<FBoolProperty>(Property))
		{
			bool BoolVal = Value.ToBool();
			BoolProperty->SetValue_InContainer(Container, &BoolVal);
		}
		
		return true;
	}
}
