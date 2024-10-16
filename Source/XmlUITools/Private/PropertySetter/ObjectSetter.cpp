#include "ObjectSetter.h"

#include "LogXmlUmg.h"
#include "SetterFactory.h"
#include "StructSetter.h"

namespace XmlUITools
{
	bool FObjectSetter::SetValue(void* Container, const FString& Value)
	{
		
		if (!Container)
		{
			UE_LOG(LogXmlUmg, Error, TEXT("Can not set uobject due to container is null"))
			return false;
		}

		if (FObjectProperty* ObjectProperty = CastField<FObjectProperty>(Property))
		{
			// Value is uclass path allocate uobject
			UObject* NewObject = LoadObject<UObject>(ANY_PACKAGE, *Value);
			ObjectProperty->SetValue_InContainer(Container, NewObject);
		}
		
		return true;
	}

	bool FObjectSetter::SetValue(void* Container, const FString& PropertyName, const FXmlAttribute* XmlAttribute, UClass* ContainerClass, void* PropertyValue, FString* OutFailureReason)
	{
		if (!Container)
		{
			if (OutFailureReason)
			{
				*OutFailureReason = TEXT("Container is nullptr, faile to set object value");
			}
			return false;
		}

		if (!XmlAttribute)
		{
			if (OutFailureReason)
			{
				*OutFailureReason = TEXT("XmlAttribute is nullptr, faile to set object value");
			}

			return false;
		}

		if (FObjectProperty* ObjectProperty = CastField<FObjectProperty>(Property))
		{
			if (XmlAttribute->Type == EXmlAttributeType::Object)
			{
				UObject* Outer = GetTransientPackage();
				if (ContainerClass->IsChildOf(UObject::StaticClass()))
				{
					Outer = static_cast<UObject*>(Container);
				}
				UClass* PropertyClass = ObjectProperty->PropertyClass;
				UObject* CreatedObj = StaticAllocateObject(PropertyClass, Outer, NAME_None, EObjectFlags::RF_NoFlags, EInternalObjectFlags::None, false);
				(*PropertyClass->ClassConstructor)(FObjectInitializer(CreatedObj, PropertyClass->ClassDefaultObject, EObjectInitializerOptions::None));
				ObjectProperty->SetObjectPropertyValue(PropertyValue, CreatedObj);

				FStructSetter* Setter = new FStructSetter(Property);
				if (!Setter->SetValue(CreatedObj, PropertyName, XmlAttribute, PropertyClass, CreatedObj, OutFailureReason))
				{
					if (OutFailureReason)
					{
						*OutFailureReason = FString::Printf(TEXT("Failed to set object value %s"), *PropertyName);
					}
				}

				delete Setter;
			}
			else if (XmlAttribute->Type == EXmlAttributeType::ObjectPtr)
			{
				UObject* Outer = GetTransientPackage();
				if (ContainerClass->IsChildOf(UObject::StaticClass()))
				{
					Outer = static_cast<UObject*>(Container);
				}
				
				FString ClassString = XmlAttribute->Attributes[PropertyName];
				UClass* PropertyClass = ObjectProperty->PropertyClass;
				if (!ClassString.IsEmpty())
				{
					UClass* FoundClass = FPackageName::IsShortPackageName(ClassString) ? FindFirstObject<UClass>(*ClassString) : UClass::TryFindTypeSlow<UClass>(ClassString);
					if (FoundClass)
					{
						UObject* CreatedObj = StaticAllocateObject(PropertyClass, Outer, NAME_None, EObjectFlags::RF_NoFlags, EInternalObjectFlags::None, false);
						(*PropertyClass->ClassConstructor)(FObjectInitializer(CreatedObj, PropertyClass->ClassDefaultObject, EObjectInitializerOptions::None));
						ObjectProperty->SetObjectPropertyValue(PropertyValue, CreatedObj);
					}
					else
					{
						if (OutFailureReason)
						{
							*OutFailureReason = FString::Printf(TEXT("Can not fount class path %s"), *ClassString);
						}
						return false;
					}
				}

			}

			return true;
		}
		
		if (OutFailureReason)
		{
			*OutFailureReason = FString::Printf(TEXT("Can not cast property to object property, failed to set object value"));
		}
		
		return false;
	}

}
