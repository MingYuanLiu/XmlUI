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
		
		FArraySetter(FProperty* Property, EArrayDataType InDataType) : IPropertySetter(Property), ArrayDataType(InDataType) {}

		virtual bool SetValue(void* Container, const FString& Value) override;

	private:

		// todo: maybe use template to simplify coding
		void SetIntArray(void* Container, const FString& Value) const;

		void SetFloatArray(void* Container, const FString& Value) const;

		void SetStrArray(void* Container, const FString& Value) const;
		
		EArrayDataType ArrayDataType;
	};
}


