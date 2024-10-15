#pragma once
#include "PropertySetter.h"

namespace XmlUITools
{
	class FArraySetter : public  IPropertySetter
	{
	public:
		enum EArrayDataType : uint8
		{
			Int = 0,
			Float,
			String
		};
		
		FArraySetter(FProperty* Property) : IPropertySetter(Property) {}

		virtual bool SetValue(void* Container, const FString& Value) override;

		virtual bool SetValue(void* Container, const FString& PropertyName, const FXmlAttribute* XmlAttribute,
			UClass* ContainerClass, void* PropertyValue = nullptr, FString* OutFailureReason = nullptr) override;
		
	};
}


