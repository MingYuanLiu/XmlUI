#include "UObjectSetter.h"

#include "LogXmlUmg.h"

namespace XmlUITools
{
	bool FUObjectSetter::SetValue(void* Container, const FString& Value)
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

}
