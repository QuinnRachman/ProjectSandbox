// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MainGameModeBase.h"
#include "GameFramework/Pawn.h"
#include "InteractableNpc.generated.h"

// TODO: When an actor inherits from interactable npc, when property set by child is empty let parent set value
UCLASS()
class AInteractableNpc : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AInteractableNpc();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	FString ReturnFullPrompt();

	UPROPERTY(BlueprintReadOnly)
	TArray<FGptDialogueMessageStruct> ConversationHistory;

	UPROPERTY(BlueprintReadWrite)
	FString Name;

	UPROPERTY(BlueprintReadWrite)
	FString Gender;

	UPROPERTY(BlueprintReadWrite)
	FString Job;
	
	UPROPERTY(BlueprintReadWrite)
	FString Region;
	
	UPROPERTY(BlueprintReadWrite)
	FString PersonalityPrompt;

	UPROPERTY(BlueprintReadWrite)
	FString Context;
	
	UPROPERTY(BlueprintReadWrite)
	bool bIsPlayerStranger;

	UPROPERTY(BlueprintReadWrite)
	bool bCanRomance;
};
