#include "FByteProperty.h"

#include "LogXmlUmg.h"

namespace XmlUITools
{
	bool FBytePropertySetter::SetValue(void* InObject, const FString& InValue)
	{
		return false;
	}

	bool FBytePropertySetter::SetValue(void* Container, const FString& PropertyName, const FXmlAttribute* XmlAttribute, UClass* ContainerClass, void* PropertyValue, FString* OutFailureReason)
	{
		if (!Container)
		{
			return false;
		}

		if (!XmlAttribute)
		{
			return false;
		}
		
		if (FByteProperty* ByteProperty = CastField<FByteProperty>(Property))
		{
			FString AttributeValue = XmlAttribute->Attributes[PropertyName];
			int64 EnumIntVal = ByteProperty->Enum->GetValueByName(FName(*AttributeValue), EGetByNameFlags::CheckAuthoredName);
			if (EnumIntVal == INDEX_NONE)
			{
				FString Failed = FString::Printf(TEXT("Unable to import byte enum %s from string value %s for not existing property %s"), *ByteProperty->GetCPPType(), *AttributeValue, *Property->GetAuthoredName());
				UE_LOG(LogXmlUmg, Error, TEXT("BytePropertySetter - %s"), *Failed)
				if (OutFailureReason)
				{
					*OutFailureReason = Failed;
				}
				return false;
			}
			else
			{
				if (!PropertyValue)
				{
					ByteProperty->SetValue_InContainer(Container, EnumIntVal);
				}
				else
				{
					ByteProperty->SetIntPropertyValue(PropertyValue, EnumIntVal);
				}

				return true;
			}
		}

		if (OutFailureReason)
		{
			*OutFailureReason = TEXT("BytePropertySetter - Property is not FByteProperty");
		}
		
		return false;
	}


}
