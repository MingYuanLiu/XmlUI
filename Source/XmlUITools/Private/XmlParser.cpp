﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "XmlParser.h"

#include "LogXmlUmg.h"


tinyxml2::XMLDocument* UXmlParser::OpenXmlFile() const
{
	tinyxml2::XMLDocument* XmlDoc = new tinyxml2::XMLDocument();
	if (XmlDoc->LoadFile(TCHAR_TO_UTF8(*XmlFilePath)) != tinyxml2::XML_SUCCESS)
	{
		UE_LOG(LogXmlUmg, Warning, TEXT("Load xml file failed"))
	}
	
	return XmlDoc;
}

void UXmlParser::ForEachChildElements(tinyxml2::XMLElement* Root, const NodeTraverseFuncType& TraverseFunc)
{
	if (!Root)
	{
		UE_LOG(LogXmlUmg, Warning, TEXT("Root is empty when taversing"));
		return;
	}
	
	for (tinyxml2::XMLElement* Element = Root->FirstChildElement(); Element; Element = Element->NextSiblingElement())
	{
		TraverseFunc(Element); 
	}
}

void UXmlParser::TraverseAllNodes(tinyxml2::XMLElement* Root, const NodeTraverseFuncType& Func)
{
	if (!Root)
	{
		UE_LOG(LogXmlUmg, Display, TEXT("Root node is empty, stop traverse."))
		return;
	}

	TQueue<tinyxml2::XMLElement*> TraverseQueue;
	TraverseQueue.Enqueue(Root);

	while (!TraverseQueue.IsEmpty())
	{
		tinyxml2::XMLElement* Parent;
		TraverseQueue.Dequeue(Parent);
		Func(Parent);
		
		ForEachChildElements(Parent,[&TraverseQueue](tinyxml2::XMLElement* Node)
		{
			TraverseQueue.Enqueue(Node);
		});
	}
}

UXmlUmgTree* UXmlParser::ParseTo()
{
	if (XmlFilePath.IsEmpty())
	{
		UE_LOG(LogXmlUmg, Warning, TEXT("Xml file path is empty, can not parse to tree"))
		return nullptr;
	}
	
	auto XmlDoc = OpenXmlFile();
	UXmlUmgTree* ParsedTree = NewObject<UXmlUmgTree>();

	tinyxml2::XMLElement* StartElement = XmlDoc->RootElement();
	const char* StartElementName = StartElement->Name();
	const FString StartName(UTF8_TO_TCHAR(StartElementName));
	if (!StartName.Equals("XmlUmg", ESearchCase::IgnoreCase))
	{
		UE_LOG(LogXmlUmg, Warning, TEXT("Can not found root element [xmlumg], xml file must be warpped by <xmlumg></xmlumg> which is case-insensitive."))
		return nullptr;
	}
	
	if (StartElement->ChildElementCount() > 1)
	{
		UE_LOG(LogXmlUmg, Warning, TEXT("XmlUmg only support one root element, we will select the first element defined in xmlumg file."))
	}

	tinyxml2::XMLElement* RootElement = StartElement->FirstChildElement();
	if (!RootElement)
	{
		UE_LOG(LogXmlUmg, Warning, TEXT("Not fount any effective elements"))
		return nullptr;
	}

	auto GenerateXmlUmgNode = [](const tinyxml2::XMLElement* Node) -> UXmlUmgNode*
	{
		UXmlUmgNode* XmlUmgNode = NewObject<UXmlUmgNode>();
		XmlUmgNode->WidgetName = UTF8_TO_TCHAR(Node->Name());
		XmlUmgNode->WidgetDisplayText = UTF8_TO_TCHAR(Node->GetText());
		for (const tinyxml2::XMLAttribute* Attr = Node->FirstAttribute(); Attr; Attr = Attr->Next())
		{
			XmlUmgNode->Attributes.Add(UTF8_TO_TCHAR(Attr->Name()), UTF8_TO_TCHAR(Attr->Value()));
		}

		return XmlUmgNode;
	};
	
	// traverse all nodes
	TQueue<TPair<tinyxml2::XMLElement*, UXmlUmgNode*> > TraverseQueue;
	
	ParsedTree->Root = GenerateXmlUmgNode(RootElement);
	TraverseQueue.Enqueue(MakeTuple(RootElement, ParsedTree->Root));

	while(!TraverseQueue.IsEmpty())
	{
		TPair<tinyxml2::XMLElement*, UXmlUmgNode*> Parent;
		TraverseQueue.Dequeue(Parent);

		ForEachChildElements(Parent.Key,[&TraverseQueue, GenerateXmlUmgNode, Parent](tinyxml2::XMLElement* Node)
		{
			UXmlUmgNode* ChildNode = GenerateXmlUmgNode(Node);
			UXmlUmgNode* ParentNode = Parent.Value;
			ParentNode->ChildNodes.Add(ChildNode);
			TraverseQueue.Enqueue(MakeTuple(Node, ChildNode));
		});
	}

	// free xml document
	// todo: maybe use shared ptr
	delete XmlDoc;
	
	return ParsedTree;
}

bool UXmlParser::SerializeTo(UXmlUmgTree* UmgTree)
{
	if (XmlFilePath.IsEmpty())
	{
		UE_LOG(LogXmlUmg, Warning, TEXT("Xml file path is empty, can not serialze to xml file"))
		return false;
	}
	
	const char* Declaration = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>";
	tinyxml2::XMLDocument XmlDoc;
	XmlDoc.Parse(Declaration);

	tinyxml2::XMLElement* Starter = XmlDoc.NewElement("XmlUmg");
	XmlDoc.InsertEndChild(Starter);

	auto ParseNodeToXmlElement = [&XmlDoc](const UXmlUmgNode* Node) -> tinyxml2::XMLElement*
	{
		tinyxml2::XMLElement* XmlElement = XmlDoc.NewElement(TCHAR_TO_UTF8(*Node->WidgetName));

		for (const auto& Attr : Node->Attributes)
		{
			const char* Key = TCHAR_TO_UTF8(*Attr.Key);
			const char* Value = TCHAR_TO_UTF8(*Attr.Value);

			XmlElement->SetAttribute(Key, Value);
		}

		if (!Node->WidgetDisplayText.IsEmpty())
		{
			tinyxml2::XMLText* XmlText = XmlDoc.NewText(TCHAR_TO_UTF8(*Node->WidgetDisplayText));
			XmlElement->InsertEndChild(XmlText);
		}

		return XmlElement;
	};

	UXmlUmgNode* RootNode = UmgTree->Root;
	tinyxml2::XMLElement* Root = ParseNodeToXmlElement(RootNode);
	Starter->InsertEndChild(Root);
	
	TQueue<TPair<UXmlUmgNode*, tinyxml2::XMLElement*> > TraverseQueue;
	TraverseQueue.Enqueue(MakeTuple(RootNode, Root));

	while (!TraverseQueue.IsEmpty())
	{
		TPair<UXmlUmgNode*, tinyxml2::XMLElement*> Parent;
		TraverseQueue.Dequeue(Parent);
		UXmlUmgNode* ParentNode = Parent.Key;
		tinyxml2::XMLElement* ParentElement = Parent.Value;
		
		TArray<UXmlUmgNode*> ChildNodes = ParentNode->ChildNodes;
		for (UXmlUmgNode* Node : ChildNodes)
		{
			tinyxml2::XMLElement* ChildElement = ParseNodeToXmlElement(Node);
			ParentElement->InsertEndChild(ChildElement);

			TraverseQueue.Enqueue(MakeTuple(Node, ChildElement));
		}
	}

	return XmlDoc.SaveFile(TCHAR_TO_UTF8(*XmlFilePath)) == 0;
}
