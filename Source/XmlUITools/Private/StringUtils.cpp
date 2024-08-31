// Fill out your copyright notice in the Description page of Project Settings.


#include "StringUtils.h"

FString FStringUtils::ConvertLowercaseLineFormatNameToCamelFormat(const FString& InputName)
{
	FString CamelFormatName = InputName.ToLower();

	TArray<int32> SplitPos;
	for (int32 i = 0; i < InputName.Len(); ++i)
	{
		if (InputName[i] == '_')
		{
			SplitPos.Add(i);
		}
	}

	CamelFormatName[0] = FChar::ToUpper(CamelFormatName[0]);

	int32 RemoveLen = 0;
	for (const int32 Pos : SplitPos)
	{
		const int32 NewPos = Pos - RemoveLen;
		CamelFormatName[NewPos + 1] = FChar::ToUpper(CamelFormatName[NewPos + 1]);
		CamelFormatName.RemoveAt(NewPos);
		
		++RemoveLen;
	}
	
	return CamelFormatName;
}

FString FStringUtils::ConvertNameToLowercaseUnderLineFormat(const FString& InputName)
{
	FString LowercaseName = InputName;
	
	if (InputName.StartsWith("U"))
	{
		LowercaseName.RemoveAt(0);
	}

	TArray<int32> AllUpperCharPoses;
	for (int i = 1; i < LowercaseName.Len(); ++i)
	{
		if (FChar::IsUpper(LowercaseName[i]))
		{
			AllUpperCharPoses.Add(i);
		}
	}

	LowercaseName = LowercaseName.ToLower();

	int32 AlreadyInsertNum = 0;
	for (const int32 Pos : AllUpperCharPoses)
	{
		LowercaseName.InsertAt(Pos + AlreadyInsertNum, '_');
		++AlreadyInsertNum;
	}
	
	return LowercaseName;
}
