// Fill out your copyright notice in the Description page of Project Settings.

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
	Vector,
	Object,
	ObjectPtr
};

USTRUCT(BlueprintType)
struct FXmlAttribute
{
	GENERATED_BODY()
	TMap<FString/* property name */, FString> Attributes;
	
	TMap<FString/* struct property name */, FXmlAttribute*> ChildAttributes;

	TArray<FXmlAttribute> ArrayAttributes;

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
