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

	bool FNumberSetter::SetValue(void* Container, const FXmlAttribute* XmlAttribute, UClass* ContainerClass, FString* OutFailureReason)
	{
		if (FNumericProperty* NumericProperty = CastField<FNumericProperty>(Property))
		{
			void* Value = Property->ContainerPtrToValuePtr<uint8>(Container);
			
			if (NumericProperty->IsEnum() && XmlAttribute->Type == EXmlAttributeType::String)
			{
				// see if we were passed a string for the enum
				const UEnum* Enum = NumericProperty->GetIntPropertyEnum();
				check(Enum); // should be assured by IsEnum()
				
				const FString AttributeValue = XmlAttribute->Value;
				const int64 IntValue = Enum->GetValueByName(FName(*AttributeValue), EGetByNameFlags::CheckAuthoredName);
				if (IntValue == INDEX_NONE)
				{
					if (OutFailureReason)
					{
						*OutFailureReason = FString::Printf(TEXT("Unable to import enum %s from string value %s for not existing property %s"), *Enum->CppType, *AttributeValue, *Property->GetAuthoredName());
					}
					return false;
				}
				NumericProperty->SetIntPropertyValue(Value, IntValue);
			}
			else if (NumericProperty->IsFloatingPoint())
			{
				const float FloatVal = FCString::Atof(*XmlAttribute->Value); 
				NumericProperty->SetFloatingPointPropertyValue(Value, FloatVal);
			}
			else if (NumericProperty->IsInteger())
			{
				const int64 IntVal = FCString::Atoi64(*XmlAttribute->Value);
				NumericProperty->SetIntPropertyValue(Value, IntVal);
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

		if (OutFailureReason)
		{
			*OutFailureReason = TEXT("Property not match to numeric property, can not use number setter.");
		}

		return false;
	}
}
