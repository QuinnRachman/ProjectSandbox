// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MainGameModeBase.generated.h"

USTRUCT(BlueprintType)
struct FGptDialogueMessageStruct
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString role;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString content;
};

USTRUCT(BlueprintType)
struct FGptDialogueBodyStruct
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString model;

	UPROPERTY(BlueprintReadOnly)
	TArray<FGptDialogueMessageStruct> messages;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float temperature;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 max_tokens;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float top_p;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float frequency_penalty;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float presence_penalty;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool stream;
};

UCLASS()
class AMainGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	const static inline FString System = "system";
	
	const static inline FString User = "user";
	
	const static inline FString Assistant = "assistant";
	
	static FGptDialogueMessageStruct CreateMessageAs(FString Role, FString Content);
};
