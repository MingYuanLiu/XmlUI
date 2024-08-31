// Copyright Epic Games, Inc. All Rights Reserved.

#include "XmlUITools.h"

#include "LogXmlUmg.h"
#include "tinyxml2.h"
#include "XmlParser.h"
#include "XmlUmgGenerator.h"

#define LOCTEXT_NAMESPACE "FXmlUIToolsModule"

void FXmlUIToolsModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	/*auto TestTinyXml = []()
	{
		auto ModuleDirectory = FPaths::ConvertRelativePathToFull(FPaths::Combine(FPaths::ProjectPluginsDir(), "XmlUITools"));
		auto XMLResourceDir = FPaths::Combine(ModuleDirectory, "Resources", "XMLTestSources");
		
		tinyxml2::XMLDocument Doc;

		auto XmlFile = FPaths::Combine(XMLResourceDir, "parse_test.xml");
		Doc.LoadFile(TCHAR_TO_UTF8(*XmlFile));
		if (Doc.Error())
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to load dream.xml"))
		}
		else
		{
			const char* FirstValue = Doc.FirstChild()->ToDeclaration()->Value();
			UE_LOG(LogTemp, Display, TEXT("First Child %s"), UTF8_TO_TCHAR(FirstValue));

			tinyxml2::XMLElement* FirstTitle = Doc.FirstChildElement("TITLE");
			if (FirstTitle)
			{
				auto Text = FirstTitle->GetText();
				UE_LOG(LogTemp, Display, TEXT("Title text %s"), UTF8_TO_TCHAR(Text));
			}
		}
	};

	TestTinyXml();*/
	auto ModuleDirectory = FPaths::ConvertRelativePathToFull(FPaths::Combine(FPaths::ProjectPluginsDir(), "XmlUITools"));
	auto XMLResourceDir = FPaths::Combine(ModuleDirectory, "Resources", "XMLTestSources");
	auto XmlFile = FPaths::Combine(XMLResourceDir, "parse_test.xml");
	UXmlParser* Parser = NewObject<UXmlParser>();
	Parser->XmlFilePath = XmlFile;
	
	UXmlUmgTree* Tree = Parser->ParseTo();
	UE_LOG(LogXmlUmg, Display, TEXT("name %s"), *Tree->Root->WidgetName);

	Parser->SerializeTo(Tree);

	UXmlUmgGenerator* Generator = NewObject<UXmlUmgGenerator>();
	Generator->BuildAllWidgetClassList(nullptr);
}

void FXmlUIToolsModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FXmlUIToolsModule, XmlUITools)
