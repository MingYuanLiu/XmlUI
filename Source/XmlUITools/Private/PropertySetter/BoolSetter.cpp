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
	
	bool FBoolSetter::SetValue(void* Container, const FString& PropertyName, const FXmlAttribute* XmlAttribute,
		UClass* ContainerClass, void* PropertyValue, FString* OutFailureReason)
	{
		if (XmlAttribute == nullptr)
        {
            if (OutFailureReason)
            {
                *OutFailureReason = TEXT("Attribute is null");
            }
            return false;
        }
		
		if (FBoolProperty* BoolProperty = CastField<FBoolProperty>(Property))
		{
			bool BoolVal = false;
			FString AttributeValue = XmlAttribute->Attributes[PropertyName];
			if (AttributeValue.Equals("true", ESearchCase::IgnoreCase))
			{
				BoolVal = true;
			}
			else if (AttributeValue.Equals("false", ESearchCase::IgnoreCase))
			{
				BoolVal = false;
			}
			else
			{
				if (OutFailureReason)
                {
                    *OutFailureReason = FString::Printf( TEXT("Attribute value is not true or false, attribute value is %s"), *AttributeValue);
                }
				return false;
			}

			if (!PropertyValue)
			{
				BoolProperty->SetValue_InContainer(Container, &BoolVal);
			}
			else
			{
				BoolProperty->SetPropertyValue(PropertyValue, BoolVal);
			}
		}
		
		return true;
	}
}
