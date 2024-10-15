#include "StringSetter.h"

#include "LogXmlUmg.h"

namespace XmlUITools
{
	bool FStringSetter::SetValue(void* Container, const FString& Value)
	{
		if (!Container)
        {
			UE_LOG(LogXmlUmg, Error, TEXT("Can not set string value due to container is null"));
            return false;
        }

		if (FStrProperty* StrProperty = CastField<FStrProperty>(Property))
        {
            StrProperty->SetPropertyValue_InContainer(Container, Value);
            return true;
        }

		if (FNameProperty* NameProperty = CastField<FNameProperty>(Property))
        {
            NameProperty->SetPropertyValue_InContainer(Container, FName(*Value));
            return true;
        }

		if (FTextProperty* TextProperty = CastField<FTextProperty>(Property))
        {
            TextProperty->SetPropertyValue_InContainer(Container, FText::FromString(Value));
            return true;
        }

		return true;
	}

	bool FStringSetter::SetValue(void* Container, const FString& PropertyName, const FXmlAttribute* XmlAttribute,
		UClass* ContainerClass, void* PropertyValue, FString* OutFailureReason)
	{
		if (!Container)
		{
			UE_LOG(LogXmlUmg, Error, TEXT("Can not set string value due to container is null"));
			return false;
		}

		if (XmlAttribute == nullptr)
		{
			if (OutFailureReason)
			{
				*OutFailureReason = TEXT("XmlAttribute is null, can not use string setter.");
			}
			return false;
		}

		FString StrVal = XmlAttribute->Attributes[PropertyName];

		if (FStrProperty* StrProperty = CastField<FStrProperty>(Property))
		{
			if (!PropertyValue)
			{
				StrProperty->SetPropertyValue_InContainer(Container, StrVal);
			}
			else
			{
				StrProperty->SetPropertyValue(PropertyValue, StrVal);
			}
			
			return true;
		}

		if (FNameProperty* NameProperty = CastField<FNameProperty>(Property))
		{
			if (!PropertyValue)
			{
				NameProperty->SetPropertyValue_InContainer(Container, FName(*StrVal));
			}
			else
			{
				NameProperty->SetPropertyValue(PropertyValue, FName(*StrVal));
			}
			
			return true;
		}

		if (FTextProperty* TextProperty = CastField<FTextProperty>(Property))
		{
			if (!PropertyValue)
			{
				TextProperty->SetPropertyValue_InContainer(Container, FText::FromString(StrVal));
			}
			else
			{
				TextProperty->SetPropertyValue(PropertyValue, FText::FromString(StrVal));
			}
	
			return true;
		}
		
		return false;
	}

}
