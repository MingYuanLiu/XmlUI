#include "ArraySetter.h"

#include "LogXmlUmg.h"
#include "SetterFactory.h"
#include "StringUtils.h"

namespace XmlUITools
{
	bool FArraySetter:: SetValue(void* Container, const FString& Value)
	{

		
		return true;
	}

	bool FArraySetter::SetValue(void* Container, const FString& PropertyName, const FXmlAttribute* XmlAttribute, UClass* ContainerClass, void* PropertyValue, FString* OutFailureReason)
	{
		if (!XmlAttribute)
        {
            if (OutFailureReason)
            {
                *OutFailureReason = TEXT("xml attribute is null");
            }
            return false;
        }

		if (!Container)
		{
			if (OutFailureReason)
            {
                *OutFailureReason = TEXT("container is null");
            }
			return false;
		}

		if (FArrayProperty* ArrayProperty = CastField<FArrayProperty>(Property))
		{
			if (XmlAttribute->Type != EXmlAttributeType::Array)
			{
				if (OutFailureReason)
                {
                    *OutFailureReason = TEXT("xml attribute type is not array");
                }
				return false;
			}

			if (XmlAttribute->ArrayAttributes.IsEmpty())
			{
				if (OutFailureReason)
                {
                    *OutFailureReason = TEXT("Cxml attribute array attributes is empty");
                }
				return false;
			}

			if (!PropertyValue)
			{
				PropertyValue = ArrayProperty->ContainerPtrToValuePtr<uint8>(Container);
			}

			auto ArrayLen = XmlAttribute->ArrayAttributes.Num();
			// make the output array size match
			FScriptArrayHelper Helper(ArrayProperty, PropertyValue);
			Helper.EmptyAndAddValues(ArrayLen);
			for (int32 i = 0; i < ArrayLen; ++i)
			{
				FXmlAttribute ElementAttr = XmlAttribute->ArrayAttributes[i];
				if (IPropertySetter* InnerSetter = FSetterFactory::CreateSetter(ArrayProperty->Inner, ElementAttr.Type))
				{
					if (!InnerSetter->SetValue(Container, PropertyName, &ElementAttr, ContainerClass, Helper.GetRawPtr(i), OutFailureReason))
                    {
						if (OutFailureReason)
						{
							*OutFailureReason = FString::Printf(TEXT("set array element value failed with index %d"), i);
						}
                        return false;
                    }
				}
				else
				{
					if (OutFailureReason)
                    {
                        *OutFailureReason = FString::Printf(TEXT("Create property setter for array element failed, Inner element type is %s"),
                        	*FStringUtils::ConvertXmlAttributeTypeToString(ElementAttr.Type));
                    }
					return false;
				}
			}
		}

		return true;
	}

}
