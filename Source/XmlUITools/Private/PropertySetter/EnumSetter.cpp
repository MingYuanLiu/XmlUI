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

		auto EnumValue = mEnum->GetValueByNameString(Value);
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

	bool FEnumSetter::SetValue(void* Container, const FString& PropertyName, const FXmlAttribute* XmlAttribute,
		UClass* ContainerClass, void* PropertyValue, FString* OutFailureReason)
	{
		if (FEnumProperty* EnumProperty = CastField<FEnumProperty>(Property))
		{
			if (XmlAttribute->Type == EXmlAttributeType::String)
			{
				// see if we were passed a string for the enum
				const UEnum* Enum = EnumProperty->GetEnum();
				check(Enum);
				FString EnumValue = XmlAttribute->Attributes[PropertyName];
				int64 IntValue = Enum->GetValueByName(FName(*EnumValue), EGetByNameFlags::CheckAuthoredName);
				if (IntValue == INDEX_NONE)
				{
					FString Failed = FString::Printf(TEXT("Unable to import enum %s from string value %s for not existing property %s"), *Enum->CppType, *EnumValue, *Property->GetAuthoredName());
					UE_LOG(LogXmlUmg, Error, TEXT("EnumSetter - %s"), *Failed)
					if (OutFailureReason)
					{
						*OutFailureReason = Failed;
					}
				}

				if (!PropertyValue)
				{
					EnumProperty->SetValue_InContainer(Container, &IntValue);
				}
				else
				{
					EnumProperty->GetUnderlyingProperty()->SetIntPropertyValue(PropertyValue, IntValue);
				}
			}
			else
			{
				if (!PropertyValue)
				{
					PropertyValue = Property->ContainerPtrToValuePtr<uint8>(Container);
				}
				// AsNumber will log an error for completely inappropriate types (then give us a default)
				FString EnumValue = XmlAttribute->Attributes[PropertyName];
				if (!EnumValue.IsEmpty())
					EnumProperty->GetUnderlyingProperty()->SetIntPropertyValue(PropertyValue, static_cast<uint64>(FCString::Atoi(*EnumValue)));
			}

			return true;
		}

		if (OutFailureReason)
		{
			*OutFailureReason = TEXT("Property is not enum property, can not use enum setter");
		}
		
		return false;
	}
}
