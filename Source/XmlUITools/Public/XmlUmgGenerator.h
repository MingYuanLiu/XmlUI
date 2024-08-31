// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "XmlUmgTree.h"
#include "Components/Widget.h"
#include "UObject/Object.h"
#include "XmlUmgGenerator.generated.h"

/**
 * 
 */
class FWidgetClassTemplate : public TSharedFromThis<FWidgetClassTemplate>
{
public:
	explicit FWidgetClassTemplate(TSubclassOf<UWidget> InWidgetClass);

	explicit FWidgetClassTemplate(const FAssetData& InWidgetAssetData, TSubclassOf<UWidget> InWidgetClass);

	/** Creates an instance of the widget for the tree. */
	UWidget* Create(UWidgetTree* Tree);

	/** Gets the WidgetClass which might be null. */
	TWeakObjectPtr<UClass> GetWidgetClass() const { return WidgetClass; }

	/** Returns the asset data for this widget which might be invalid. */
	const FAssetData& GetWidgetAssetData() { return WidgetAssetData; }

	/** The name of the widget template. */
	FText Name;

protected:
	/** Creates a widget template class without any class reference */
	FWidgetClassTemplate();

	/** Called when objects need to be swapped out for new versions, like after a blueprint recompile. */
	void OnObjectsReplaced(const TMap<UObject*, UObject*>& ReplacementMap);

	/** Constructs the widget template with an overridden object name. */
	UWidget* CreateNamed(class UWidgetTree* Tree, FName NameOverride);
	
private:
	/** The widget class that will be created by this template */
	TWeakObjectPtr<UClass> WidgetClass;

	/** The asset data for the widget blueprint */
	FAssetData WidgetAssetData;
};

/**
 * 
 */
UCLASS(BlueprintType)
class XMLUITOOLS_API UXmlUmgGenerator : public UObject
{
	GENERATED_BODY()

public:
	UWidgetTree* GenerateWidgetTree(UUserWidget* Outer, UXmlUmgTree* XmlUmgTree);
	
	void BuildAllWidgetClassList(const UClass* ActivateCurrentClass);

protected:
	bool IsUsableWidgetClass(const UClass* WidgetClass);

	FString ConvertNameToLowercaseUnderLineFormat(const FString& ClassName);
	
	TMap<FString, TSharedPtr<FWidgetClassTemplate>> AllWidgetClasses; 
};
