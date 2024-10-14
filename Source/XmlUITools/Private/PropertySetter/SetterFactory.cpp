#include "SetterFactory.h"
#include <regex>

#include "ArraySetter.h"
#include "BoolSetter.h"
#include "EnumSetter.h"
#include "NumberSetter.h"
#include "StringSetter.h"
#include "ObjectSetter.h"
#include "TextSetter.h"
#include "VectorSetter.h"

namespace XmlUITools
{

	IPropertySetter* FSetterFactory::CreateSetter(FProperty* Property, EXmlAttributeType Type)
	{
		if (FEnumProperty* EnumProperty = CastField<FEnumProperty>(Property))
        {
			if (Type == EXmlAttributeType::String)
				return new FEnumSetter(Property, EnumProperty->GetEnum());
        }

		if (FBoolProperty* BoolProperty = CastField<FBoolProperty>(Property))
        {
            if (Type == EXmlAttributeType::Bool)
                return new FBoolSetter(Property);
        }

		if (FNumericProperty* NumericProperty = CastField<FIntProperty>(Property))
        {
            if (Type == EXmlAttributeType::Number)
                return new FNumberSetter(Property);
        }

		if (FArrayProperty* ArrayProperty = CastField<FArrayProperty>(Property))
        {
            if (Type == EXmlAttributeType::Array)
                return new FArraySetter(Property, FArraySetter::String);
        }

		if (FStructProperty* StructProperty = CastField<FStructProperty>(Property))
        {
            if (Type == EXmlAttributeType::Object)
                return new FObjectSetter(Property);
        }

		if (FObjectProperty* ObjectProperty = CastField<FObjectProperty>(Property))
        {
            if (Type == EXmlAttributeType::Object)
                return new FObjectSetter(Property);
        }

		if (FStrProperty *StringProperty = CastField<FStrProperty>(Property))
		{
			if (Type == EXmlAttributeType::String)
                return new FStringSetter(Property);
		}

		if (FSetProperty* SetProperty = CastField<FSetProperty>(Property))
		{
			if (Type == EXmlAttributeType::Array)
                return new FArraySetter(Property, FArraySetter::String);
		}

		if (FTextProperty* TextProperty = CastField<FTextProperty>(Property))
		{
			if (Type == EXmlAttributeType::String)
                return new FTextSetter(Property);
		}
	}

	
	
	IPropertySetter* FSetterFactory::CreateSetter(FProperty* Property, const FString& Value)
	{
		// bool
		if (Value == "true" || Value == "false")
		{
			if (FBoolProperty* BoolProperty = CastField<FBoolProperty>(Property))
			{
				return new FBoolSetter(Property);
			}
		}
		
		// number (int, float)
		if (std::regex_match(TCHAR_TO_UTF8(*Value), std::regex("^\\d+$")) ||
			std::regex_match(TCHAR_TO_UTF8(*Value), std::regex("^\\d+\\.\\d+$")))
		{
			const FIntProperty* IntProperty = CastField<FIntProperty>(Property);
			const FFloatProperty* FloatProperty = CastField<FFloatProperty>(Property);
			if (IntProperty != nullptr || FloatProperty != nullptr)
			{
				return new FNumberSetter(Property); // note: delete it by user
			}
		}

		// array (int, float, string)
		bool bIsIntArray = std::regex_match(TCHAR_TO_UTF8(*Value), std::regex("^\\d+(,\\d+)+$"));
		bool bIsFloatArray = std::regex_match(TCHAR_TO_UTF8(*Value), std::regex("^\\d+\\.\\d+(,\\s*\\d+\\.\\d+)+$"));
		bool bIsStrArray = std::regex_match(TCHAR_TO_UTF8(*Value), std::regex("^[a-zA-Z0-9]+(,\\s*[a-zA-Z0-9]+)+$"));
		if (bIsIntArray || bIsFloatArray || bIsStrArray)
		{
			if (FArrayProperty* ArrayProperty = CastField<FArrayProperty>(Property))
			{
				FArraySetter::EArrayDataType DataType = FArraySetter::Int;
				if (bIsFloatArray) DataType = FArraySetter::Float;
				else if (bIsIntArray) DataType = FArraySetter::Int;
				else if (bIsStrArray) DataType = FArraySetter::String;
				
				return new FArraySetter(Property, DataType); // note delete it by user
			}
		}

		// enum
		const bool bIsEnumName = std::regex_match(TCHAR_TO_UTF8(*Value), std::regex("^[a-zA-Z0-9]+$"));
		if (bIsEnumName)
		{
			FEnumProperty* EnumProperty = CastField<FEnumProperty>(Property);
			FByteProperty* ByteProperty = CastField<FByteProperty>(Property);
			UEnum* Enum = nullptr;
			if (EnumProperty != nullptr)
			{
				Enum = EnumProperty->GetEnum();
			}
			else if (ByteProperty != nullptr)
			{
				Enum = ByteProperty->GetIntPropertyEnum();
			}

			if (Enum && Enum->GetValueByNameString(Value) != INDEX_NONE)
			{
				return new FEnumSetter(Property, Enum);
			}
		}

		// number vector
		if (std::regex_match(TCHAR_TO_UTF8(*Value), std::regex("^\\(\\d+(,\\d+)+\\)$")))
		{
			// struct property
			if (FStructProperty* StructProperty = CastField<FStructProperty>(Property))
			{
				return new FVectorSetter(Property);
			}
		}

		// uobject
		if (std::regex_match(TCHAR_TO_UTF8(*Value), std::regex("^(/.*)+")))
		{
			if (FObjectProperty* ObjectProperty = CastField<FObjectProperty>(Property))
			{
				return new FUObjectSetter(Property);
			}
		}

		// string, name or text
		if (Property->IsA<FStrProperty>() ||
			Property->IsA<FNameProperty>() ||
            Property->IsA<FTextProperty>())
        {
            return new FStringSetter(Property);
        }
		
		return nullptr;
	}
}
