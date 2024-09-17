﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"

#include "XmlUmgTree.generated.h"


USTRUCT(BlueprintType)
struct FExtraAttribute
{
	GENERATED_BODY()
	
	TMap<FString, FString> Properties;
	
	TMap<FString, TSharedRef<FExtraAttribute>> ChildProperties; 
};

UCLASS(BlueprintType)
class UXmlUmgNode : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	FString WidgetName;

	UPROPERTY(EditAnywhere)
	FString WidgetDisplayText;

	UPROPERTY(EditAnywhere)
	TMap<FString, FString> Attributes;

	UPROPERTY(EditAnywhere)
	TMap<FString, FExtraAttribute> ExtraAttributes; // attributes defined in <attributes></attributes> xml element

	UPROPERTY(EditAnywhere)
	TArray<UXmlUmgNode*> ChildNodes;
};

/**
 * 
 */
UCLASS(BlueprintType)
class XMLUITOOLS_API  UXmlUmgTree : public UObject
{
	GENERATED_BODY()

public:
	UXmlUmgNode* Root;

	UUserWidget* GenerateUserWidget();
	
};
