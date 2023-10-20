// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "HttpModule.h"
#include "HttpManager.h"
#include "InteractableNpc.h"
#include "RequestHelper.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGetGptAnswerDelegate, FString, Answer);

typedef struct FWav_Header
{
	/* RIFF Chunk Descriptor */
	uint8_t RIFF[4] = {'R', 'I', 'F', 'F'}; // RIFF Header Magic header
	uint32_t ChunkSize;                     // RIFF Chunk Size
	uint8_t WAVE[4] = {'W', 'A', 'V', 'E'}; // WAVE Header
	/* "fmt" sub-chunk */
	uint8_t fmt[4] = {'f', 'm', 't', ' '}; // FMT header
	uint32_t Subchunk1Size = 16;           // Size of the fmt chunk
	uint16_t AudioFormat = 1; // Audio format 1=PCM,6=mulaw,7=alaw,257=IBM
	// Mu-Law, 258=IBM A-Law, 259=ADPCM
	uint16_t NumOfChan = 1;   // Number of channels 1=Mono 2=Sterio
	uint32_t SamplesPerSec = 16000;   // Sampling Frequency in Hz
	uint32_t bytesPerSec = 16000 * 2; // bytes per second
	uint16_t blockAlign = 4;          // 2=16-bit mono, 4=16-bit stereo
	uint16_t bitsPerSample = 16;      // Number of bits per sample
	/* "data" sub-chunk */
	uint8_t Subchunk2ID[4] = {'d', 'a', 't', 'a'}; // "data"  string
	uint32_t Subchunk2Size;                        // Sampled data length
} Wav_Header;

UENUM()
enum ELabsOutputFormats {
	SixteenKHertz = 16,
	TwentyFourKHertz = 24
};

UCLASS(Blueprintable)
class UGptDialogueHandler : public UObject
{
	GENERATED_BODY()
	
	TSharedPtr<FJsonObject> JsonStringToJsonObject(FString JsonBody);

	UPROPERTY()
	FString GptKey;

	UPROPERTY()
	FString ELabsKey;

	UPROPERTY()
	FString GptAnswerString;

	UPROPERTY()
	bool GptJsonConversionFailed = false;
	
public:
	UPROPERTY(BlueprintReadWrite)
	AInteractableNpc* Npc;

	UPROPERTY(BlueprintReadWrite)
	USoundWave* NpcDialogueAsset;

	UPROPERTY(BlueprintAssignable)
	FGetGptAnswerDelegate GetGptAnswerEvent;

	UFUNCTION(BlueprintCallable)
	void SendGptHttpRequest(FString UserPrompt, FGptDialogueBodyStruct RequestBody);

	UFUNCTION()
	void SendELabsHttpRequest(ELabsOutputFormats OutputFormat, FString Text, float Stability = 0.5, float SimilarityBoost = 0.75, float Style = 0.0);
};
