#pragma once
#include "PropertySetter.h"

namespace XmlUITools
{
	class FVectorSetter : public IPropertySetter
	{
	public:
		FVectorSetter(FProperty* Property) : IPropertySetter(Property) {}

		virtual bool SetValue(void* Container, const FString& Value) override;

	private:
		void SplitStringToFloatArray(const FString& Value, TArray<float>& OutFloatVector);
	};
}

