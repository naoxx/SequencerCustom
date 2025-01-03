// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Evaluation/MovieSceneEvalTemplate.h"
#include "SequencerCustomPlugin/MovieScene/Tracks/MyMovieSceneEventTrack.h"
#include "SequencerCustomPlugin/MovieScene/Sections/MyMovieSceneEventSection.h"

#include "MyMovieSceneEventEvalTemplate.generated.h"

/**
 * 
 */
USTRUCT()
struct FMyMovieSceneEventEvalTemplate : public FMovieSceneEvalTemplate
{
	GENERATED_BODY()

	FMyMovieSceneEventEvalTemplate() : bFireEventsWhenForwards(false), bFireEventsWhenBackwards(false) {}
	FMyMovieSceneEventEvalTemplate(const UMyMovieSceneEventSection& Section, const UMyMovieSceneEventTrack& Track);

	UPROPERTY()
	FMyMovieSceneEventData EventData;

	UPROPERTY()
	uint32 bFireEventsWhenForwards : 1;

	UPROPERTY()
	uint32 bFireEventsWhenBackwards : 1;

private:

	virtual UScriptStruct& GetScriptStructImpl() const { return *StaticStruct(); }
	virtual void EvaluateSwept(const FMovieSceneEvaluationOperand& Operand, const FMovieSceneContext& Context, const TRange<FFrameNumber>& SweptRange, const FPersistentEvaluationData& PersistentData, FMovieSceneExecutionTokens& ExecutionTokens) const override;

};