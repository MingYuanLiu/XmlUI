#include "NumberSetter.h"

#include "LogXmlUmg.h"
#include "StringUtils.h"

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

	bool FNumberSetter::SetValue(void* Container, const FString& PropertyName, const FXmlAttribute* XmlAttribute,
		UClass* ContainerClass, void* PropertyValue, FString* OutFailureReason)
	{
		if (!Container)
		{
			if (OutFailureReason)
			{
				*OutFailureReason = TEXT("Container is null, can not use number setter.");
			}
			return false;
		}
		
		if (XmlAttribute == nullptr)
        {
            if (OutFailureReason)
            {
                *OutFailureReason = TEXT("XmlAttribute is null, can not use number setter.");
            }
            return false;
        }
		
		if (FNumericProperty* NumericProperty = CastField<FNumericProperty>(Property))
		{
			if (!PropertyValue)
			{
				PropertyValue = Property->ContainerPtrToValuePtr<uint8>(Container);
			}
			
			const FString AttributeValue = XmlAttribute->Attributes[PropertyName];
			
			if (NumericProperty->IsEnum() && XmlAttribute->Type == EXmlAttributeType::String)
			{
				// see if we were passed a string for the enum
				const UEnum* Enum = NumericProperty->GetIntPropertyEnum();
				check(Enum); // should be assured by IsEnum()
				
				const int64 IntValue = Enum->GetValueByName(FName(*AttributeValue), EGetByNameFlags::CheckAuthoredName);
				if (IntValue == INDEX_NONE)
				{
					if (OutFailureReason)
					{
						*OutFailureReason = FString::Printf(TEXT("Unable to import enum %s from string value %s for not existing property %s"), *Enum->CppType, *AttributeValue, *Property->GetAuthoredName());
					}
					return false;
				}
				NumericProperty->SetIntPropertyValue(PropertyValue, IntValue);
			}
			else if (NumericProperty->IsFloatingPoint())
			{
				const float FloatVal = FCString::Atof(*AttributeValue); 
				NumericProperty->SetFloatingPointPropertyValue(PropertyValue, FloatVal);
			}
			else if (NumericProperty->IsInteger())
			{
				const int64 IntVal = FCString::Atoi64(*AttributeValue);
				NumericProperty->SetIntPropertyValue(PropertyValue, IntVal);
			}
			else
			{
				if (OutFailureReason)
				{
					*OutFailureReason = FString::Printf(TEXT("Unable to import numberic value into %s numeric property %s"), *Property->GetClass()->GetName(), *Property->GetAuthoredName());
				}
				
				return false;
			}

			return true;
		}
		else if (FIntProperty* IntProperty = CastField<FIntProperty>(Property))
		{
			FString AttributeValue = XmlAttribute->Attributes[PropertyName];
			AttributeValue = FStringUtils::ReplaceEnterLineAndTrimStart(AttributeValue);
			int32 IntVal = FCString::Atoi(*AttributeValue);
			IntProperty->SetPropertyValue(PropertyValue, IntVal);
		}
		else if (FFloatProperty* FloatProperty = CastField<FFloatProperty>(Property))
		{
			FString AttributeValue = XmlAttribute->Attributes[PropertyName];
			AttributeValue = FStringUtils::ReplaceEnterLineAndTrimStart(AttributeValue);
			float FloatVal = FCString::Atof(*AttributeValue);
			FloatProperty->SetPropertyValue(PropertyValue, FloatVal);
		}
		else if (FDoubleProperty* DoubleProperty = CastField<FDoubleProperty>(Property))
		{
			FString AttributeValue = XmlAttribute->Attributes[PropertyName];
			AttributeValue = FStringUtils::ReplaceEnterLineAndTrimStart(AttributeValue);
			double DoubleVal = FCString::Atod(*AttributeValue);
			DoubleProperty->SetPropertyValue(PropertyValue, DoubleVal);
		}
		else 
		{
			FString AttributeValue = XmlAttribute->Attributes[PropertyName];
			AttributeValue = FStringUtils::ReplaceEnterLineAndTrimStart(AttributeValue);
			UE_LOG(LogXmlUmg, Warning, TEXT("Not match int and float property, default set int value to container"))
			int32 Val = FCString::Atoi(*AttributeValue);
			Property->SetValue_InContainer(Container, &Val);
		}

		if (OutFailureReason)
		{
			*OutFailureReason = TEXT("Property not match to numeric property, can not use number setter.");
		}

		return false;
	}
}
