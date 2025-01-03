// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MovieSceneSection.h"
#include "Channels/MovieSceneChannel.h"
#include "Channels/MovieSceneChannelData.h"
#include "Channels/MovieSceneChannelTraits.h"
#include "MovieSceneClipboard.h"

#include "MyMovieSceneEventSection.generated.h"

 /**
  *
  */
struct FMyEventPayload;
namespace MovieSceneClipboard
{
	template<> inline FName GetKeyTypeName<FMyEventPayload>()
	{
		return "MyEventPayload";
	}
}


USTRUCT(BlueprintType)
struct FMyMovieSceneEventParameters
{
	GENERATED_BODY()

	FMyMovieSceneEventParameters()
	{
	}


	UPROPERTY(EditAnywhere)
	int TestInt = 0;

	UPROPERTY(EditAnywhere)
	FString TestString = TEXT("Test");

};

USTRUCT(BlueprintType)
struct FMyEventPayload
{
	GENERATED_BODY()

	FMyEventPayload() {}
	FMyEventPayload(FName InEventName) : EventName(InEventName) {}

	/** The name of the event to trigger */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Event)
	FName EventName;

	/** The event parameters */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Event, meta = (ShowOnlyInnerProperties))
	FMyMovieSceneEventParameters Parameters;
};

USTRUCT()
struct FMyMovieSceneEventData : public FMovieSceneChannel
{
	GENERATED_BODY()


	/**
	 * Access a mutable interface for this channel's data
	 *
	 * @return An object that is able to manipulate this channel's data
	 */
	FORCEINLINE TMovieSceneChannelData<FMyEventPayload> GetData()
	{
		return TMovieSceneChannelData<FMyEventPayload>(&Times, &KeyValues, &KeyHandles, this);
	}

	/**
	 * Access a constant interface for this channel's data
	 *
	 * @return An object that is able to interrogate this channel's data
	 */
	FORCEINLINE TMovieSceneChannelData<const FMyEventPayload> GetData() const
	{
		return TMovieSceneChannelData<const FMyEventPayload>(&Times, &KeyValues);
	}

	TArrayView<const FFrameNumber> GetKeyTimes() const
	{
		return Times;
	}

	TArrayView<const FMyEventPayload> GetKeyValues() const
	{
		return KeyValues;
	}

public:

	// ~ FMovieSceneChannel Interface
	virtual void GetKeys(const TRange<FFrameNumber>& WithinRange, TArray<FFrameNumber>* OutKeyTimes, TArray<FKeyHandle>* OutKeyHandles) override;
	virtual void GetKeyTimes(TArrayView<const FKeyHandle> InHandles, TArrayView<FFrameNumber> OutKeyTimes) override;
	virtual void SetKeyTimes(TArrayView<const FKeyHandle> InHandles, TArrayView<const FFrameNumber> InKeyTimes) override;
	virtual void DuplicateKeys(TArrayView<const FKeyHandle> InHandles, TArrayView<FKeyHandle> OutNewHandles) override;
	virtual void DeleteKeys(TArrayView<const FKeyHandle> InHandles) override;
	virtual void DeleteKeysFrom(FFrameNumber InTime, bool bDeleteKeysBefore) override;
	virtual void ChangeFrameResolution(FFrameRate SourceRate, FFrameRate DestinationRate) override;
	virtual TRange<FFrameNumber> ComputeEffectiveRange() const override;
	virtual int32 GetNumKeys() const override;
	virtual void Reset() override;
	virtual void Offset(FFrameNumber DeltaPosition) override;
	virtual FKeyHandle GetHandle(int32 Index) override;
	virtual int32 GetIndex(FKeyHandle Handle) override;
	virtual void Optimize(const FKeyDataOptimizationParams& InParameters) override {}
	virtual void ClearDefault() override {}

private:

	UPROPERTY(meta = (KeyTimes))
	TArray<FFrameNumber> Times;

	/** Array of values that correspond to each key time */
	UPROPERTY(meta = (KeyValues))
	TArray<FMyEventPayload> KeyValues;

	/** This needs to be a UPROPERTY so it gets saved into editor transactions but transient so it doesn't get saved into assets. */
	UPROPERTY(Transient)
	FMovieSceneKeyHandleMap KeyHandles;

};

UCLASS()
class SEQUENCERCUSTOMPLUGIN_API UMyMovieSceneEventSection : public UMovieSceneSection
{
	GENERATED_BODY()

public:
	UMyMovieSceneEventSection();

	const FMyMovieSceneEventData& GetEventData() const { return EventData; }

private:
	UPROPERTY()
	FMyMovieSceneEventData EventData;
};

template<>
struct TMovieSceneChannelTraits<FMyMovieSceneEventData> : TMovieSceneChannelTraitsBase<FMyMovieSceneEventData>
{
	enum { SupportsDefaults = false };
};

inline bool EvaluateChannel(const FMyMovieSceneEventData* InChannel, FFrameTime InTime, FMyEventPayload& OutValue)
{
	// Can't evaluate event section data in the typical sense
	return false;
}

inline bool ValueExistsAtTime(const FMyMovieSceneEventData* InChannel, FFrameNumber Time, const FMyEventPayload& Value)
{
	// true if any value exists
	return InChannel->GetData().FindKey(Time) != INDEX_NONE;
}
