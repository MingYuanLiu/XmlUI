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

}
