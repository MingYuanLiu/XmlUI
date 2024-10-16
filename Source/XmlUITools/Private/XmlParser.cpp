// Fill out your copyright notice in the Description page of Project Settings.


#include "XmlParser.h"

#include "LogXmlUmg.h"
#include "StringUtils.h"

const char* UXmlParser::ExtraAttributeXmlKeyword = "XmlProperties";

tinyxml2::XMLDocument* UXmlParser::OpenXmlFile() const
{
	tinyxml2::XMLDocument* XmlDoc = new tinyxml2::XMLDocument();
	if (XmlDoc->LoadFile(TCHAR_TO_UTF8(*XmlFilePath)) != tinyxml2::XML_SUCCESS)
	{
		UE_LOG(LogXmlUmg, Error, TEXT("Load xml file failed"))
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

// todo: parse 'attributes' element in xml file

UXmlUmgTree* UXmlParser::ParseFromXml(FString& Out_FailureReason)
{
	if (XmlFilePath.IsEmpty())
	{
		UE_LOG(LogXmlUmg, Warning, TEXT("Xml file path is empty, can not parse to tree"))
		return nullptr;
	}
	
	auto XmlDoc = OpenXmlFile();
	if (XmlDoc && XmlDoc->Error())
	{
		UE_LOG(LogXmlUmg, Error, TEXT("Can not open xml file %s, error %s"), *XmlFilePath, *FString(UTF8_TO_TCHAR(XmlDoc->ErrorStr())))
		return nullptr;
	}
	
	UXmlUmgTree* ParsedTree = NewObject<UXmlUmgTree>();

	tinyxml2::XMLElement* StartElement = XmlDoc->RootElement();
	if (StartElement == nullptr)
	{
		UE_LOG(LogXmlUmg, Error, TEXT("Xml file format is error, can not find root element"))
		return nullptr;
	}
	
	const FString StartName(UTF8_TO_TCHAR(StartElement->Name()));
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

	auto GenerateXmlUmgNode = [this](const tinyxml2::XMLElement* Node) -> UXmlUmgNode*
	{
		UXmlUmgNode* XmlUmgNode = NewObject<UXmlUmgNode>();
		XmlUmgNode->WidgetName = UTF8_TO_TCHAR(Node->Name());
		XmlUmgNode->WidgetDisplayText = FStringUtils::ReplaceEnterLineAndTrimStart(UTF8_TO_TCHAR(Node->GetText()));
		for (const tinyxml2::XMLAttribute* Attr = Node->FirstAttribute(); Attr; Attr = Attr->Next())
		{
			FXmlAttribute Attribute;
			FString AttributeValue = FStringUtils::ReplaceEnterLineAndTrimStart(UTF8_TO_TCHAR(Attr->Value()));
			Attribute.Attributes.Add(UTF8_TO_TCHAR(Attr->Name()), AttributeValue);
			Attribute.Type = FStringUtils::GetAttributeTypeFromXmlValue(AttributeValue);
			
			XmlUmgNode->Properties.Add(UTF8_TO_TCHAR(Attr->Name()), Attribute);
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

		ForEachChildElements(Parent.Key,[this, &TraverseQueue, GenerateXmlUmgNode, Parent](tinyxml2::XMLElement* Node)
		{
			if (strcmp(Node->Name(), ExtraAttributeXmlKeyword) == 0)
			{
				UXmlUmgNode* ParentNode = Parent.Value;
				TMap<FString, FXmlAttribute> ExtraProperties;
				ParseExtraProperties(Node, ExtraProperties);
				ParentNode->Properties.Append(ExtraProperties);
				return;
			}
			
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

bool UXmlParser::SerializeToXml(UXmlUmgTree* UmgTree)
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

		for (const auto& Attr : Node->Properties)
		{
			const char* Key = TCHAR_TO_UTF8(*Attr.Key);
			const char* Value = TCHAR_TO_UTF8(*Attr.Value.Attributes[Key]);

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

void UXmlParser::ParseExtraProperties(tinyxml2::XMLElement* Node, TMap<FString, FXmlAttribute>& OutExtraAttributes)
{
	if (strcmp(Node->Name(), ExtraAttributeXmlKeyword) != 0)
	{
		UE_LOG(LogXmlUmg, Warning, TEXT("Xml extra attribute node must be %hs"), ExtraAttributeXmlKeyword)
		return;
	}

	TMap<FString, FXmlAttribute*> ExtraAttributes;
	ParseObjectAttributes(Node, ExtraAttributes);

	for (const auto& Item : ExtraAttributes)
    {
        OutExtraAttributes.Add(Item.Key, *Item.Value);
		delete Item.Value;
    }
}

void UXmlParser::ParseObjectAttributes(tinyxml2::XMLElement* Node, TMap<FString, FXmlAttribute*>& OutExtraAttributes)
{
	if (Node == nullptr)
	{
		return;
	}

	for (tinyxml2::XMLElement* Element = Node->FirstChildElement(); Element; Element = Element->NextSiblingElement())
	{
		if (Element == nullptr)
		{
			continue;
		}
		
		FXmlAttribute* CurrentElementXmlExtraProperty = new FXmlAttribute();
		
		const char* PropertyName = Element->Name();
		const char* PropertyValue = Element->GetText();
		const bool bHasChild = Element->ChildElementCount() > 0;
		const bool bIsArray = IsArrayElement(Element);
		  
		if (!bHasChild && PropertyValue != nullptr)
		{
			FString Value = FString(UTF8_TO_TCHAR(PropertyValue)).Replace(TEXT("\n"), TEXT("")).TrimStartAndEnd();
			EXmlAttributeType Type = FStringUtils::GetAttributeTypeFromXmlValue(Value);
			
			CurrentElementXmlExtraProperty->Attributes.Add(PropertyName, Value);
			CurrentElementXmlExtraProperty->Type = Type;
		}

		if (bHasChild && bIsArray)
		{
			for (auto* ListElement = Element->FirstChildElement(); ListElement; ListElement = ListElement->NextSiblingElement())
            {
				if (strcmp(ListElement->Name(), "List.Element") != 0)
                {
                    continue;
                }

				FXmlAttribute CurrentListElementExtraProperty;
				if (ListElement->ChildElementCount() > 0)
				{
					
					ParseObjectAttributes(ListElement, CurrentListElementExtraProperty.ChildAttributes);
				}
				else
				{
					const char* ElementValue = ListElement->GetText();
					FString Value = FString(UTF8_TO_TCHAR(ElementValue)).Replace(TEXT("\n"), TEXT("")).TrimStartAndEnd();
					CurrentListElementExtraProperty.Attributes.Add(PropertyName, Value);
					CurrentListElementExtraProperty.Type = FStringUtils::GetAttributeTypeFromXmlValue(Value);
				}
				CurrentElementXmlExtraProperty->ArrayAttributes.Add(CurrentListElementExtraProperty);
            }

			CurrentElementXmlExtraProperty->Type = EXmlAttributeType::Array;
		}

		if (bHasChild && !bIsArray)
		{
			ParseObjectAttributes(Element, CurrentElementXmlExtraProperty->ChildAttributes);
			CurrentElementXmlExtraProperty->Type = EXmlAttributeType::Object;
		}

		OutExtraAttributes.Add(UTF8_TO_TCHAR(Element->Name()), CurrentElementXmlExtraProperty);
	}
}

bool UXmlParser::IsArrayElement(tinyxml2::XMLElement* Node)
{
	bool IsArray = Node->ChildElementCount() > 0;
	// if all the name of child elements is List.Element, the element type is array
	for (tinyxml2::XMLElement* Element = Node->FirstChildElement(); Element; Element = Element->NextSiblingElement())
	{
		if (strcmp(Element->Name(), "List.Element") != 0)
		{
			IsArray = false;
			break;
		}
	}

	return IsArray;
}