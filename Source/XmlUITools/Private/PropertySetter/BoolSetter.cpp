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
	
	bool FBoolSetter::SetValue(void* Container, const FXmlAttribute* XmlAttribute, UClass* ContainerClass, FString* OutFailureReason)
	{
		if (FBoolProperty* BoolProperty = CastField<FBoolProperty>(Property))
		{
			bool BoolVal = false;
			if (XmlAttribute->Value.Equals("true", ESearchCase::IgnoreCase))
			{
				BoolVal = true;
			}
			else if (XmlAttribute->Value.Equals("false", ESearchCase::IgnoreCase))
			{
				BoolVal = false;	
			}
			
			BoolProperty->SetValue_InContainer(Container, &BoolVal);
		}
		
		return true;
	}
}
