#include "ArraySetter.h"

#include "LogXmlUmg.h"

namespace XmlUITools
{
	bool FArraySetter:: SetValue(void* Container, const FString& Value)
	{
		if (!Container)
		{
			UE_LOG(LogXmlUmg, Error, TEXT("Can not set array value due to container is null"))
			return false;
		}

		// split value to array
		switch (ArrayDataType)
		{
		case EArrayDataType::Int:
			SetIntArray(Container, Value);
			break;
		case EArrayDataType::Float:
			SetFloatArray(Container, Value);
			break;
		case EArrayDataType::String:
			SetStrArray(Container, Value);
			break;
		default:
			break;
		}

		
		return true;
	}

	void FArraySetter::SetIntArray(void* Container, const FString& Value) const
	{
		TArray<FString> SplitArray;
		Value.ParseIntoArray(SplitArray, TEXT(","), true);

		TArray<int32> IntArray;
		for (FString Val : SplitArray)
		{
			Val = Val.TrimStartAndEnd();
			int32 IntVal = FCString::Atoi(*Val);
			IntArray.Add(IntVal);
		}

		if (FArrayProperty* ArrayProperty = CastField<FArrayProperty>(Property))
		{
			FScriptArrayHelper ArrayHelper(ArrayProperty, ArrayProperty->ContainerPtrToValuePtr<void>(Container));
			ArrayHelper.EmptyAndAddValues(IntArray.Num());

			for (int32 Idx = 0; Idx < ArrayHelper.Num(); ++Idx)
			{
				FProperty* InnerProperty = ArrayProperty->Inner;
				if (FIntProperty* IntProperty = CastField<FIntProperty>(InnerProperty))
				{
					void* ElementPtr = ArrayHelper.GetRawPtr(Idx);
					IntProperty->SetPropertyValue(ElementPtr, IntArray[Idx]);
				}
			}
		}
	}

	void FArraySetter::SetFloatArray(void* Container, const FString& Value) const
	{
		TArray<FString> SplitArray;
		Value.ParseIntoArray(SplitArray, TEXT(","), true);

		TArray<float> FloatArray;
		for (FString Val : SplitArray)
		{
			Val = Val.TrimStartAndEnd();
			float FloatVal = FCString::Atof(*Val);
			FloatArray.Add(FloatVal);
		}

		if (FArrayProperty* ArrayProperty = CastField<FArrayProperty>(Property))
		{
			FScriptArrayHelper ArrayHelper(ArrayProperty, ArrayProperty->ContainerPtrToValuePtr<void>(Container));
			ArrayHelper.EmptyAndAddValues(FloatArray.Num());

			for (int32 Idx = 0; Idx < ArrayHelper.Num(); ++Idx)
			{
				FProperty* InnerProperty = ArrayProperty->Inner;
				if (FFloatProperty* FloatProperty = CastField<FFloatProperty>(InnerProperty))
				{
					void* ElementPtr = ArrayHelper.GetRawPtr(Idx);
					FloatProperty->SetPropertyValue(ElementPtr, FloatArray[Idx]);
				}
			}
		}
	}

	void FArraySetter::SetStrArray(void* Container, const FString& Value) const
	{
		TArray<FString> SplitArray;
		Value.ParseIntoArray(SplitArray, TEXT(","), true);

		TArray<FString> StrArray;
		for (FString Val : SplitArray)
		{
			Val = Val.TrimStartAndEnd();
			StrArray.Add(Val);
		}

		if (FArrayProperty* ArrayProperty = CastField<FArrayProperty>(Property))
		{
			FScriptArrayHelper ArrayHelper(ArrayProperty, ArrayProperty->ContainerPtrToValuePtr<void>(Container));
			ArrayHelper.EmptyAndAddValues(StrArray.Num());

			for (int32 Idx = 0; Idx < ArrayHelper.Num(); ++Idx)
			{
				FProperty* InnerProperty = ArrayProperty->Inner;
				if (FStrProperty* StrProperty = CastField<FStrProperty>(InnerProperty))
				{
					void* ElementPtr = ArrayHelper.GetRawPtr(Idx);
					StrProperty->SetPropertyValue(ElementPtr, StrArray[Idx]);
				}
			}
		}
	}


}
