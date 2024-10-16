#include "StructSetter.h"

#include "LogXmlUmg.h"
#include "SetterFactory.h"
#include "StringUtils.h"

namespace XmlUITools
{
	bool FStructSetter::SetValue(void* Container, const FString& PropertyName, const FXmlAttribute* XmlAttribute, UClass* ContainerClass, void* PropertyValue, FString* OutFailureReason)
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
					
					FProperty* SubProperty = *PropertyIter;
					FString PropName = StructProperty->Struct->GetAuthoredNameForField(SubProperty);

					if (FXmlAttribute* const* ChildAttributePtr = ChildAttributes.Find(PropName))
					{
						FXmlAttribute* ChildAttribute = *ChildAttributePtr;
						if (IPropertySetter* Setter = FSetterFactory::CreateSetter(SubProperty, ChildAttribute->Type))
						{
							void* OutValue = SubProperty->ContainerPtrToValuePtr<uint8>(PropertyValue);
							if (!Setter->SetValue(Container, PropName, ChildAttribute, ContainerClass, OutValue, OutFailureReason))
                            {
								if (OutFailureReason)
                                {
                                    *OutFailureReason = FString::Printf(TEXT("Failed to set value for struct property %s "), *PropName);
                                }
                            }

							delete Setter;
						}
					}
					else
					{
						UE_LOG(LogXmlUmg, Warning, TEXT("Has no child attribute for property %s"), *PropName);
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
				else if (StructProperty->Struct->GetFName() == NAME_Color)
				{
					FColor& ColorProperty = *static_cast<FColor*>(PropertyValue);
					ColorProperty.R = Vector.Num() > 0 ? static_cast<uint8>(Vector[0]) : 0;
                    ColorProperty.G = Vector.Num() > 1 ? static_cast<uint8>(Vector[1]) : 0;
                    ColorProperty.B = Vector.Num() > 2 ? static_cast<uint8>(Vector[2]) : 0;
                    ColorProperty.A = Vector.Num() > 3 ? static_cast<uint8>(Vector[3]) : 0;
				}
				else if (StructProperty->Struct->GetFName() == NAME_Box)
				{
					FBox& BoxProperty = *static_cast<FBox*>(PropertyValue);
					BoxProperty.Min.X = Vector.Num() > 0 ? Vector[0] : 0.0f;
                    BoxProperty.Min.Y = Vector.Num() > 1 ? Vector[1] : 0.0f;
                    BoxProperty.Min.Z = Vector.Num() > 2 ? Vector[2] : 0.0f;
                    BoxProperty.Max.X = Vector.Num() > 3 ? Vector[3] : 0.0f;
                    BoxProperty.Max.Y = Vector.Num() > 4 ? Vector[4] : 0.0f;
                    BoxProperty.Max.Z = Vector.Num() > 5 ? Vector[5] : 0.0f;
				}
				else if (StructProperty->Struct->GetFName() == NAME_Plane)
                {
                    FPlane& PlaneProperty = *static_cast<FPlane*>(PropertyValue);
					PlaneProperty.X = Vector.Num() > 0 ? Vector[0] : 0.0f;
                    PlaneProperty.Y = Vector.Num() > 1 ? Vector[1] : 0.0f;
                    PlaneProperty.Z = Vector.Num() > 2 ? Vector[2] : 0.0f;
                    PlaneProperty.W = Vector.Num() > 3 ? Vector[3] : 0.0f;
                }
				else if (StructProperty->Struct->GetFName() == NAME_Matrix)
				{
					FMatrix& MatrixProperty = *static_cast<FMatrix*>(PropertyValue);
					MatrixProperty.M[0][0] = Vector.Num() > 0 ? Vector[0] : 0.0f;
					MatrixProperty.M[0][1] = Vector.Num() > 1 ? Vector[1] : 0.0f;
                    MatrixProperty.M[0][2] = Vector.Num() > 2 ? Vector[2] : 0.0f;
                    MatrixProperty.M[0][3] = Vector.Num() > 3 ? Vector[3] : 0.0f;
                    MatrixProperty.M[1][0] = Vector.Num() > 4 ? Vector[4] : 0.0f;
                    MatrixProperty.M[1][1] = Vector.Num() > 5 ? Vector[5] : 0.0f;
                    MatrixProperty.M[1][2] = Vector.Num() > 6 ? Vector[6] : 0.0f;
                    MatrixProperty.M[1][3] = Vector.Num() > 7 ? Vector[7] : 0.0f;
                    MatrixProperty.M[2][0] = Vector.Num() > 8 ? Vector[8] : 0.0f;
                    MatrixProperty.M[2][1] = Vector.Num() > 9 ? Vector[9] : 0.0f;
					MatrixProperty.M[2][2] = Vector.Num() > 10 ? Vector[10] : 0.0f;
                    MatrixProperty.M[2][3] = Vector.Num() > 11 ? Vector[11] : 0.0f;
					MatrixProperty.M[3][0] = Vector.Num() > 12 ? Vector[12] : 0.0f;
                    MatrixProperty.M[3][1] = Vector.Num() > 13 ? Vector[13] : 0.0f;
                    MatrixProperty.M[3][2] = Vector.Num() > 14 ? Vector[14] : 0.0f;
                    MatrixProperty.M[3][3] = Vector.Num() > 15 ? Vector[15] : 0.0f;
				}
				else if (StructProperty->Struct->GetFName() == NAME_Ray)
				{
					FRay& RayProperty = *static_cast<FRay*>(PropertyValue);
					RayProperty.Origin.X = Vector.Num() > 0 ? Vector[0] : 0.0f;
                    RayProperty.Origin.Y = Vector.Num() > 1 ? Vector[1] : 0.0f;
                    RayProperty.Origin.Z = Vector.Num() > 2 ? Vector[2] : 0.0f;
                    RayProperty.Direction.X = Vector.Num() > 3 ? Vector[3] : 0.0f;
                    RayProperty.Direction.Y = Vector.Num() > 4 ? Vector[4] : 0.0f;
                    RayProperty.Direction.Z = Vector.Num() > 5 ? Vector[5] : 0.0f;
				}
			}
			else if (XmlAttribute->Type == EXmlAttributeType::String)
            {
				FString AttrValue = XmlAttribute->Attributes[PropertyName];
				if (StructProperty->Struct->GetFName() == NAME_Color)
				{
					FColor& ColorOut = *static_cast<FColor*>(PropertyValue);
					ColorOut = FColor::FromHex(AttrValue);
				}
				else if (StructProperty->Struct->GetFName() == NAME_LinearColor)
				{
					FLinearColor& ColorOut = *static_cast<FLinearColor*>(PropertyValue);
                    ColorOut = FColor::FromHex(AttrValue);
				}
				else if (StructProperty->Struct->GetFName() == TEXT("DateTime"))
				{
					FDateTime& DateTimeOut = *static_cast<FDateTime*>(PropertyValue);
					if (AttrValue == TEXT("min"))
					{
						// min representable value for our date struct. Actual date may vary by platform (this is used for sorting)
						DateTimeOut = FDateTime::MinValue();
					}
					else if (AttrValue == TEXT("max"))
					{
						// max representable value for our date struct. Actual date may vary by platform (this is used for sorting)
						DateTimeOut = FDateTime::MaxValue();
					}
					else if (AttrValue == TEXT("now"))
					{
						// this value's not really meaningful from JSON serialization (since we don't know timezone) but handle it anyway since we're handling the other keywords
						DateTimeOut = FDateTime::UtcNow();
					}
					else if (FDateTime::ParseIso8601(*AttrValue, DateTimeOut))
					{
						// ok
					}
					else if (FDateTime::Parse(AttrValue, DateTimeOut))
					{
						// ok
					}
					else
					{
						if (OutFailureReason)
						{
							*OutFailureReason = FString::Printf(TEXT("Unable to import xml string into DateTime property %s"), *Property->GetAuthoredName());
						}
						return false;
					}
				}
				else if (StructProperty->Struct->GetCppStructOps() && StructProperty->Struct->GetCppStructOps()->HasImportTextItem())
				{
					UScriptStruct::ICppStructOps* TheCppStructOps = StructProperty->Struct->GetCppStructOps();

					const TCHAR* ImportTextPtr = *AttrValue;
					if (!TheCppStructOps->ImportTextItem(ImportTextPtr, PropertyValue, PPF_None, nullptr, (FOutputDevice*)GWarn))
					{
						// Fall back to trying the tagged property approach if custom ImportTextItem couldn't get it done
						if (Property->ImportText_Direct(ImportTextPtr, PropertyValue, nullptr, PPF_None) == nullptr)
						{
							if (OutFailureReason)
							{
								*OutFailureReason = FString::Printf(TEXT("Unable to import xml string into %s property %s"), *StructProperty->Struct->GetAuthoredName(), *Property->GetAuthoredName());
							}
							return false;
						}
					}
				}
				else
				{
					const TCHAR* ImportTextPtr = *AttrValue;
					if (Property->ImportText_Direct(ImportTextPtr, PropertyValue, nullptr, PPF_None) == nullptr)
					{
						if (OutFailureReason)
						{
							*OutFailureReason = FString::Printf(TEXT("Unable to import xml string into %s property %s"), *StructProperty->Struct->GetAuthoredName(), *Property->GetAuthoredName());
						}
						return false;
					}
				}
            }

			return true;
		}

		if (OutFailureReason)
		{
			*OutFailureReason = FString::Printf(TEXT("Can not cast to struct property, can not set to struct value"));
		}
		
		return false;
	}

	bool FStructSetter::SetValue(void* Container, const FString& Value)
	{
		return true;
	}


}
