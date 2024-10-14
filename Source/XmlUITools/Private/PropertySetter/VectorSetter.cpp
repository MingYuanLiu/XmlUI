#include "VectorSetter.h"

#include "LogXmlUmg.h"

namespace XmlUITools
{
	bool FVectorSetter::SetValue(void* Container, const FString& Value)
	{
		if (!Container)
		{
			UE_LOG(LogXmlUmg, Error, TEXT("Can not set vector value due to container is null"))
			return false;
		}

		if (FStructProperty* StructProperty = CastField<FStructProperty>(Property))
		{
			UScriptStruct* PropertyStruct = StructProperty->Struct;
			FString StructName = PropertyStruct->GetStructCPPName();

			TArray<float> VectorData;
			SplitStringToFloatArray(Value, VectorData);
			
			// if (StructProperty->Struct)
			// 选出UE中所有可以接受向量数据的结构体
			
		}
		
		return true;
	}

	void FVectorSetter::SplitStringToFloatArray(const FString& Value, TArray<float>& OutFloatVector)
	{
		TArray<FString> SplitArray;
		Value.ParseIntoArray(SplitArray, TEXT(","), true);

		if (SplitArray[0].StartsWith("("))
		{
			SplitArray[0].RemoveFromStart("(");
		}

		auto Len = SplitArray.Num();
		if (SplitArray[Len-1].EndsWith(")"))
		{
			SplitArray[Len-1].RemoveFromEnd(")");
		}

		for (const auto& Val : SplitArray)
		{
			FString TrimVal = Val.TrimStartAndEnd();
			float FloatVal = FCString::Atof(*TrimVal);
			OutFloatVector.Add(FloatVal);
		}
	}
}
