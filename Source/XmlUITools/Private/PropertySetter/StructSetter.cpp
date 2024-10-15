#include "StructSetter.h"

#include "LogXmlUmg.h"
#include "SetterFactory.h"
#include "StringUtils.h"

namespace XmlUITools
{
	bool StructSetter::SetValue(void* Container, const FString& PropertyName, const FXmlAttribute* XmlAttribute, UClass* ContainerClass, void* PropertyValue, FString* OutFailureReason)
	{
		if (!Container)
		{
			if (OutFailureReason)
            {
                *OutFailureReason = TEXT("Container is nullptr, faile to set struct value");
            }
		}

		if (!XmlAttribute)
		{
			if (OutFailureReason)
            {
                *OutFailureReason = TEXT("XmlAttribute is nullptr, faile to set struct value");
            }
		}

		if (!PropertyValue)
		{
			PropertyValue = Property->ContainerPtrToValuePtr<uint8>(Container);
		}

		if (FStructProperty* StructProperty = CastField<FStructProperty>(Property))
		{
			if (XmlAttribute->Type == EXmlAttributeType::Object)
			{
				const TMap<FString, FXmlAttribute*>& ChildAttributes = XmlAttribute->ChildAttributes;
				if (ChildAttributes.IsEmpty())
				{
					UE_LOG(LogXmlUmg, Warning, TEXT("Has no child attributes, do not set struct property value"));
					return true;
				}
				
				for (TFieldIterator<FProperty> PropertyIter(StructProperty->Struct); PropertyIter; ++PropertyIter)
				{
					
					FProperty* Property = *PropertyIter;
					FString PropName = StructProperty->Struct->GetAuthoredNameForField(Property);

					if (FXmlAttribute* const* ChildAttributePtr = ChildAttributes.Find(PropName))
					{
						FXmlAttribute* ChildAttribute = *ChildAttributePtr;
						if (IPropertySetter* Setter = FSetterFactory::CreateSetter(Property, ChildAttribute->Type))
						{
							void* OutValue = Property->ContainerPtrToValuePtr<uint8>(PropertyValue);
							if (!Setter->SetValue(Container, PropName, ChildAttribute, ContainerClass, OutValue, OutFailureReason))
                            {
								if (OutFailureReason)
                                {
                                    *OutFailureReason = FString::Printf(TEXT("Failed to set value for struct property %s "), *PropName);
                                }

								return false;
                            }
						}
					}
				}
			}
			// special type for struct property
			else if (XmlAttribute->Type == EXmlAttributeType::Vector)
			{
				FString VectorValue = XmlAttribute->Attributes[PropertyName];
				TArray<float> Vector;
				FStringUtils::SplitStringVectorToArray(VectorValue, Vector);
				
				if (StructProperty->Struct->GetFName() == NAME_Vector)
				{
					FVector& VectorProperty = *static_cast<FVector*>(PropertyValue);
                    VectorProperty.X = Vector.Num() > 0 ? Vector[0] : 0.0f;
                    VectorProperty.Y = Vector.Num() > 1 ? Vector[1] : 0.0f;
                    VectorProperty.Z = Vector.Num() > 2 ? Vector[2] : 0.0f;
				}
				else if (StructProperty->Struct->GetFName() == NAME_Vector2D)
				{
					FVector2D& VectorProperty = *static_cast<FVector2D*>(PropertyValue);
                    VectorProperty.X = Vector.Num() > 0 ? Vector[0] : 0.0f;
                    VectorProperty.Y = Vector.Num() > 1 ? Vector[1] : 0.0f;
				}
				else if (StructProperty->Struct->GetFName() == NAME_Vector4)
				{
					FVector4& VectorProperty = *static_cast<FVector4*>(PropertyValue);
                    VectorProperty.X = Vector.Num() > 0 ? Vector[0] : 0.0f;
                    VectorProperty.Y = Vector.Num() > 1 ? Vector[1] : 0.0f;
                    VectorProperty.Z = Vector.Num() > 2 ? Vector[2] : 0.0f;
                    VectorProperty.W = Vector.Num() > 3 ? Vector[3] : 0.0f;
				}
				else if (StructProperty->Struct->GetFName() == NAME_Quat)
				{
					FQuat& QuatProperty = *static_cast<FQuat*>(PropertyValue);
					QuatProperty.X = Vector.Num() > 0 ? Vector[0] : 0.0f;
					QuatProperty.Y = Vector.Num() > 1 ? Vector[1] : 0.0f;
                    QuatProperty.Z = Vector.Num() > 2 ? Vector[2] : 0.0f;
                    QuatProperty.W = Vector.Num() > 3 ? Vector[3] : 0.0f;
				}
				else if (StructProperty->Struct->GetFName() == NAME_Rotator)
                {
                    FRotator& RotatorProperty = *static_cast<FRotator*>(PropertyValue);
                    RotatorProperty.Pitch = Vector.Num() > 0 ? Vector[0] : 0.0f;
                    RotatorProperty.Yaw = Vector.Num() > 1 ? Vector[1] : 0.0f;
                    RotatorProperty.Roll = Vector.Num() > 2 ? Vector[2] : 0.0f;
                }
			}
		}
		
		return true;
	}

	bool StructSetter::SetValue(void* Container, const FString& Value)
	{
		return true;
	}


}
