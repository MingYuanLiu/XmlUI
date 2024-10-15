#pragma once
#include "XmlUmgTree.h"

class FXmlConverter
{
public:
	static bool XmlAttributeToFPropertyValueWithContainer(const FXmlAttribute* XmlAttribute, FProperty* Property, void* OutValue, const UClass* ContainerClass, void* Container, FString* FailureReason);
};
