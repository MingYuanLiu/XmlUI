// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class XMLUITOOLS_API FStringUtils
{
public:
	static FString ConvertLowercaseLineFormatNameToCamelFormat(const FString& InputName);

	static FString ConvertNameToLowercaseUnderLineFormat(const FString& InputName);
};
