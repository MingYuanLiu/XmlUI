// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ThirdParty/TinyXML2/include/tinyxml2.h"

#include "CoreMinimal.h"
#include "XmlUmgTree.h"
#include "UObject/Object.h"
#include "XmlParser.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class XMLUITOOLS_API UXmlParser : public UObject
{
	GENERATED_BODY()

public:
	using NodeTraverseFuncType = TFunction<void(tinyxml2::XMLElement* Node)>;

	UFUNCTION(BlueprintCallable, Category="XmlUmg")
	UXmlUmgTree* ParseTo();

	UFUNCTION(BlueprintCallable, Category="XmlUmg")
	bool SerializeTo(UXmlUmgTree* UmgTree);

	UPROPERTY()
	FString XmlFilePath;

protected:
	tinyxml2::XMLDocument* OpenXmlFile() const;

	void ParseExtraAttributes(tinyxml2::XMLElement* Node, TMap<FString, FExtraAttribute>& OutExtraAttributes);
	void TraverseAllChildAttributes(tinyxml2::XMLElement* Node, TSharedRef<FExtraAttribute> OutExtraAttribute);

	void TraverseAllNodes(tinyxml2::XMLElement* Root, const NodeTraverseFuncType& Func);
	void ForEachChildElements(tinyxml2::XMLElement* Root, const NodeTraverseFuncType& TraverseFunc);
	
	static const char* ExtraAttributeXmlKeyword; 
};
