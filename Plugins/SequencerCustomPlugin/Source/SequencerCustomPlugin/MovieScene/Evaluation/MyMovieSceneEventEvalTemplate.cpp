// Fill out your copyright notice in the Description page of Project Settings.


#include "SequencerCustomPlugin/MovieScene/Evaluation/MyMovieSceneEventEvalTemplate.h"


/*
*****************************************************************************/
struct FMyMovieSceneExcuteEventData
{
	FMyMovieSceneExcuteEventData(const FMyEventPayload& InPayload, float InGlobalPosition) : Payload(InPayload), GlobalPosition(InGlobalPosition) {}

	FMyEventPayload Payload;
	float GlobalPosition;
};

/** A movie scene execution token that stores a specific transform, and an operand */
struct FNCEventTrackExecutionToken
	: IMovieSceneExecutionToken
{
	FNCEventTrackExecutionToken(TArray<FMyMovieSceneExcuteEventData> InEvents) : Events(MoveTemp(InEvents)) {}

	/** Execute this token, operating on all objects referenced by 'Operand' */
	virtual void Execute(const FMovieSceneContext& Context, const FMovieSceneEvaluationOperand& Operand, FPersistentEvaluationData& PersistentData, IMovieScenePlayer& Player) override
	{

		// Resolve event contexts to trigger the event on
		TArray<UObject*> EventContexts = Player.GetEventContexts();

		for (UObject* EventContextObject : EventContexts)
		{
			if (!EventContextObject)
			{
				continue;
			}

			for (FMyMovieSceneExcuteEventData& Event : Events)
			{
				FString log = TEXT("sec:") + FString::SanitizeFloat(Event.GlobalPosition) + TEXT(" testparam:") + FString::FromInt(Event.Payload.Parameters.TestInt) + Event.Payload.Parameters.TestString;
				UE_LOG(LogTemp, Display, TEXT("%s"), *log);
			}
		}


	}


	TArray<FMyMovieSceneExcuteEventData> Events;
};


FMyMovieSceneEventEvalTemplate::FMyMovieSceneEventEvalTemplate(const UMyMovieSceneEventSection& Section, const UMyMovieSceneEventTrack& Track)
	: EventData(Section.GetEventData())
	, bFireEventsWhenForwards(Track.bFireEventsWhenForwards)
	, bFireEventsWhenBackwards(Track.bFireEventsWhenBackwards)
{
}

void FMyMovieSceneEventEvalTemplate::EvaluateSwept(const FMovieSceneEvaluationOperand& Operand, const FMovieSceneContext& Context, const TRange<FFrameNumber>& SweptRange, const FPersistentEvaluationData& PersistentData, FMovieSceneExecutionTokens& ExecutionTokens) const
{
	// Don't allow events to fire when playback is in a stopped state. This can occur when stopping 
	// playback and returning the current position to the start of playback. It's not desireable to have 
	// all the events from the last playback position to the start of playback be fired.
	if (Context.GetStatus() == EMovieScenePlayerStatus::Stopped || Context.IsSilent())
	{
		return;
	}

	const bool bBackwards = Context.GetDirection() == EPlayDirection::Backwards;

	if ((!bBackwards && !bFireEventsWhenForwards) ||
		(bBackwards && !bFireEventsWhenBackwards))
	{
		return;
	}

	TArray<FMyMovieSceneExcuteEventData> Events;

	TArrayView<const FFrameNumber>  KeyTimes = EventData.GetKeyTimes();
	TArrayView<const FMyEventPayload> KeyValues = EventData.GetKeyValues();

	const int32 First = bBackwards ? KeyTimes.Num() - 1 : 0;
	const int32 Last = bBackwards ? 0 : KeyTimes.Num() - 1;
	const int32 Inc = bBackwards ? -1 : 1;

	const float PositionInSeconds = Context.GetTime() * Context.GetRootToSequenceTransform().InverseNoLooping() / Context.GetFrameRate();

	if (bBackwards)
	{
		// Trigger events backwards
		for (int32 KeyIndex = KeyTimes.Num() - 1; KeyIndex >= 0; --KeyIndex)
		{
			FFrameNumber Time = KeyTimes[KeyIndex];
			if (SweptRange.Contains(Time))
			{
				Events.Add(FMyMovieSceneExcuteEventData(KeyValues[KeyIndex], PositionInSeconds));
			}
		}
	}
	// Trigger events forwards
	else for (int32 KeyIndex = 0; KeyIndex < KeyTimes.Num(); ++KeyIndex)
	{
		FFrameNumber Time = KeyTimes[KeyIndex];
		if (SweptRange.Contains(Time))
		{
			Events.Add(FMyMovieSceneExcuteEventData(KeyValues[KeyIndex], PositionInSeconds));
		}
	}


	if (Events.Num())
	{
		ExecutionTokens.Add(FNCEventTrackExecutionToken(MoveTemp(Events)));
	}
}