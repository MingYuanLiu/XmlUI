﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"

#include "XmlUmgTree.generated.h"

enum class EXmlAttributeType : uint8
{
	None,
	String,
	Number,
	Bool,
	Array,
	Object,
	ObjectPtr
};

USTRUCT()
struct FXmlAttribute
{
	GENERATED_BODY()

	FString Value;

	EXmlAttributeType Type;
};

USTRUCT(BlueprintType)
struct FXmlExtraAttribute
{
	GENERATED_BODY()
	
	TMap<FString/* property name */, FXmlAttribute> Properties;
	
	TMap<FString/* struct property name */, TObjectPtr<FXmlExtraAttribute>> ChildProperties;
	
	EXmlAttributeType Type;
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
	TMap<FString/* property name */, FXmlAttribute> Properties; // properties defined in xml node attribute region

	UPROPERTY(EditAnywhere)
	TMap<FString/* property name */, FXmlExtraAttribute> ExtraProperties; // extra properties defined in <XmlProperties></XmlProperties> child element

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
