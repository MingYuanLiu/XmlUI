// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "XmlUmgTree.h"

/**
 * 
 */
class XMLUITOOLS_API FStringUtils
{
public:
	static FString ConvertLowercaseLineFormatNameToCamelFormat(const FString& InputName);

	static FString ConvertNameToLowercaseUnderLineFormat(const FString& InputName);

	static EXmlAttributeType GetAttributeTypeFromXmlValue(const FString& Value);
	
	static FString ReplaceEnterLineAndTrimStart(const FString& InString)
	{
		return InString.Replace(TEXT("\n"), TEXT("")).TrimStartAndEnd();
	}

	static FString ConvertXmlAttributeTypeToString(EXmlAttributeType AttributeType);

	static void SplitStringVectorToArray(const FString& InValue, TArray<float>& OutVector);
};
