// Fill out your copyright notice in the Description page of Project Settings.


#include "RequestHelper.h"
#include "JsonObjectConverter.h"
#include "HttpModule.h"
#include "HttpManager.h"
#include "Components/AudioComponent.h"
#include "Interfaces/IHttpResponse.h"

void UGptDialogueHandler::SendGptHttpRequest(FString UserPrompt, FGptDialogueBodyStruct RequestBody)
{
		Npc->ConversationHistory.Add(AMainGameModeBase::CreateMessageAs(AMainGameModeBase::User, UserPrompt));

	RequestBody.messages = Npc->ConversationHistory;
	
	FString JSONString;
	
	if (!FJsonObjectConverter::UStructToJsonObjectString(RequestBody, JSONString))
	{
		UE_LOG(LogTemp, Error, TEXT("Struct to JSON conversion failed."))
		return;
	}

	UE_LOG(LogTemp, Error, TEXT("Json Request: %s"), *JSONString)

	const TSharedRef<IHttpRequest> HttpRequest = FHttpModule::Get().CreateRequest();
	HttpRequest->SetHeader("Authorization", FString::Printf(TEXT("Bearer %s"), *GptKey));
	HttpRequest->SetHeader("Content-Type", "application/json");
	HttpRequest->SetContentAsString(JSONString);
	HttpRequest->SetVerb("POST");
	HttpRequest->SetURL("https://api.openai.com/v1/chat/completions");
	HttpRequest->ProcessRequest();
	
	HttpRequest->OnProcessRequestComplete().BindLambda([&](FHttpRequestPtr Request, FHttpResponsePtr Response, bool IsComplete)
	{
		if (IsComplete)
		{
			const FString JsonContent = Response->GetContentAsString();

			const TSharedPtr<FJsonObject> JsonObject = JsonStringToJsonObject(JsonContent);

			if (GptJsonConversionFailed)
			{
				UE_LOG(LogTemp, Warning, TEXT("Failed json: %s"), *JsonContent);
				
				return;
			}

			const TSharedPtr<FJsonObject>* possibleErrorObject;

			if (JsonObject->TryGetObjectField("error", possibleErrorObject))
			{
				UE_LOG(LogTemp, Error, TEXT("Request returned the following error: %s"), *possibleErrorObject->Get()->GetStringField("message"));

				return;
			}
			
			GptAnswerString = JsonObject->GetArrayField("choices")[0]->AsObject()->GetObjectField("message")->GetStringField("content");

			Npc->ConversationHistory.Add(AMainGameModeBase::CreateMessageAs(AMainGameModeBase::Assistant, GptAnswerString));

			UE_LOG(LogTemp, Warning, TEXT("Json object: %s"), *JsonContent);

			UE_LOG(LogTemp, Warning, TEXT("GPT answer: %s"), *GptAnswerString);

			UE_LOG(LogTemp, Warning, TEXT("Request complete."));
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Request failed."));
		}
	});
}

void UGptDialogueHandler::SendELabsHttpRequest(ELabsOutputFormats OutputFormat, FString Text, float Stability, float SimilarityBoost, float Style)
{
	const int32 Hertz = OutputFormat * 10000;

	const FString URL = FString::Printf(
		TEXT(R"(https://api.elevenlabs.io/v1/text-to-speech/XB0fDUnXU5powFXDhCwa?optimize_streaming_latency=0&output_format=pcm_%d)"),
		Hertz
	);
	
	const TSharedRef<IHttpRequest> ELabsHttpRequest = FHttpModule::Get().CreateRequest();
	ELabsHttpRequest->SetHeader("xi-api-key", *ELabsKey);
	ELabsHttpRequest->SetHeader("Content-Type", "application/json");
	ELabsHttpRequest->SetHeader("accept", "audio/mpeg'");
	ELabsHttpRequest->SetContentAsString(FString::Printf(TEXT(R"(
{
  "text": "%s",
  "model_id": "eleven_multilingual_v2",
  "voice_settings": {
    "stability": %.2f,
    "similarity_boost": %.2f,
    "style": %.2f,
    "use_speaker_boost": true
  }
}
	)"), *Text, Stability, SimilarityBoost, Style));
	ELabsHttpRequest->SetVerb("POST");
	ELabsHttpRequest->SetURL(URL);
	ELabsHttpRequest->ProcessRequest();
	ELabsHttpRequest->OnProcessRequestComplete().BindLambda([&](FHttpRequestPtr Request, FHttpResponsePtr Response, bool IsComplete)
	{
		if (IsComplete)
		{
			auto ELabJsonContent = Response->GetContent();

			const auto AudioComponent = Npc->FindComponentByClass<UAudioComponent>();

			TArray<uint8> WaveHeaderBytes;

			Wav_Header Header;

			UE_LOG(LogTemp, Warning, TEXT("Header Size: %llu"), sizeof(Header));
						
			Header.ChunkSize = ELabJsonContent.Num() + sizeof(Header) - 8;
			Header.Subchunk2Size = ELabJsonContent.Num() + sizeof(Header);

			WaveHeaderBytes.Init(0, sizeof(Header));

			if (WaveHeaderBytes.GetData() == nullptr)
			{
				UE_LOG(LogTemp, Error, TEXT("Null Pointer."));
				return;
			}

			UE_LOG(LogTemp, Warning, TEXT("Array Size: %d"), WaveHeaderBytes.Num());
							
			FMemory::Memcpy(WaveHeaderBytes.GetData(), &Header, sizeof(Header));

			WaveHeaderBytes.Append(ELabJsonContent);

			FWaveModInfo WaveModInfo;
				
			if (WaveModInfo.ReadWaveInfo(WaveHeaderBytes.GetData(), sizeof(Header) + WaveHeaderBytes.Num()))
			{
				USoundWave* NpcSoundWave = NewObject<USoundWave>();

				NpcSoundWave->SetSampleRate(Header.SamplesPerSec);
				NpcSoundWave->NumChannels = Header.NumOfChan;

				const int32 DataBytesPerSecond = Header.bytesPerSec;

				UE_LOG(LogTemp, Warning, TEXT("ELabs data bytes per second: %d"), DataBytesPerSecond);

				if (DataBytesPerSecond)
				{
					NpcSoundWave->Duration = WaveModInfo.SampleDataSize / DataBytesPerSecond;

					NpcSoundWave->RawPCMDataSize = WaveModInfo.SampleDataSize;

					NpcSoundWave->RawPCMData = static_cast<uint8*>(FMemory::Malloc(WaveModInfo.SampleDataSize));

					FMemory::Memcpy(NpcSoundWave->RawPCMData, WaveModInfo.SampleDataStart, WaveModInfo.SampleDataSize);
				}

				AudioComponent->SetSound(NpcSoundWave);

				AudioComponent->Play();
			}	
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("ELabs Request failed."));
		}
	});
}

TSharedPtr<FJsonObject> UGptDialogueHandler::JsonStringToJsonObject(FString JsonBody)
{
	TSharedPtr<FJsonObject> jsonObject = MakeShareable(new FJsonObject);

	if (!FJsonSerializer::Deserialize(TJsonReaderFactory<>::Create(JsonBody), jsonObject))
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to deserialize json body."));

		GptJsonConversionFailed = true;
		
		return jsonObject;
	}

	return jsonObject;
}
