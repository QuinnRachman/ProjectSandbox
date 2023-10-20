// Fill out your copyright notice in the Description page of Project Settings.


#include "MainGameModeBase.h"

FGptDialogueMessageStruct AMainGameModeBase::CreateMessageAs(FString Role, FString Content)
{
	return FGptDialogueMessageStruct {
		Role,
		Content
	};
}
