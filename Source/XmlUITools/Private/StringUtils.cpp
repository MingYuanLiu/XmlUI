// Fill out your copyright notice in the Description page of Project Settings.


#include "StringUtils.h"

#include <regex>

FString FStringUtils::ConvertLowercaseLineFormatNameToCamelFormat(const FString& InputName)
{
	FString CamelFormatName = InputName.ToLower();

	TArray<int32> SplitPos;
	for (int32 i = 0; i < InputName.Len(); ++i)
	{
		if (InputName[i] == '_')
		{
			SplitPos.Add(i);
		}
	}

	CamelFormatName[0] = FChar::ToUpper(CamelFormatName[0]);

	int32 RemoveLen = 0;
	for (const int32 Pos : SplitPos)
	{
		const int32 NewPos = Pos - RemoveLen;
		CamelFormatName[NewPos + 1] = FChar::ToUpper(CamelFormatName[NewPos + 1]);
		CamelFormatName.RemoveAt(NewPos);
		
		++RemoveLen;
	}
	
	return CamelFormatName;
}

FString FStringUtils::ConvertNameToLowercaseUnderLineFormat(const FString& InputName)
{
	FString LowercaseName = InputName;
	
	if (InputName.StartsWith("U"))
	{
		LowercaseName.RemoveAt(0);
	}

	TArray<int32> AllUpperCharPoses;
	for (int i = 1; i < LowercaseName.Len(); ++i)
	{
		if (FChar::IsUpper(LowercaseName[i]))
		{
			AllUpperCharPoses.Add(i);
		}
	}

	LowercaseName = LowercaseName.ToLower();

	int32 AlreadyInsertNum = 0;
	for (const int32 Pos : AllUpperCharPoses)
	{
		LowercaseName.InsertAt(Pos + AlreadyInsertNum, '_');
		++AlreadyInsertNum;
	}
	
	return LowercaseName;
}

EXmlAttributeType FStringUtils::GetAttributeTypeFromXmlValue(const FString& Value)
{
	EXmlAttributeType Type = EXmlAttributeType::None;
	// bool
	if (Value.Equals("true", ESearchCase::IgnoreCase) ||
		Value.Equals("false", ESearchCase::IgnoreCase))
	{
		Type = EXmlAttributeType::Bool;
	}
	
	// number (int, float)
	if (std::regex_match(TCHAR_TO_UTF8(*Value), std::regex("^\\d+$")) ||
		std::regex_match(TCHAR_TO_UTF8(*Value), std::regex("^\\d+\\.\\d+$")))
	{
		Type = EXmlAttributeType::Number;
	}

	// array (int, float, string)
	bool bIsIntArray = std::regex_match(TCHAR_TO_UTF8(*Value), std::regex("^\\d+(,\\d+)+$"));
	bool bIsFloatArray = std::regex_match(TCHAR_TO_UTF8(*Value), std::regex("^\\d+\\.\\d+(,\\s*\\d+\\.\\d+)+$"));
	bool bIsStrArray = std::regex_match(TCHAR_TO_UTF8(*Value), std::regex("^[a-zA-Z0-9]+(,\\s*[a-zA-Z0-9]+)+$"));
	if (bIsIntArray || bIsFloatArray || bIsStrArray)
	{
		return EXmlAttributeType::Array;
	}

	// uobject ptr
	if (std::regex_match(TCHAR_TO_UTF8(*Value), std::regex("^(/.*)+")))
	{
		return EXmlAttributeType::ObjectPtr;
	}

	if (!Value.IsEmpty())
	{
		Type = EXmlAttributeType::String;
	}
	
	return Type;
}

FString FStringUtils::ConvertXmlAttributeTypeToString(EXmlAttributeType AttributeType)
{
	switch (AttributeType)
	{
		case EXmlAttributeType::Bool:
            return "bool";
        case EXmlAttributeType::Number:
            return "number";
        case EXmlAttributeType::Array:
            return "array";
        case EXmlAttributeType::ObjectPtr:
            return "uobject ptr";
        case EXmlAttributeType::String:
            return "string";
		case EXmlAttributeType::Object:
			return "object";
        default:
            return "none";
	}
}