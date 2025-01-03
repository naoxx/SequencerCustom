// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MovieSceneNameableTrack.h"
#include "MovieSceneNameableTrack.h"
#include "Evaluation/MovieSceneEvalTemplate.h"
#include "Compilation/IMovieSceneTrackTemplateProducer.h"
#include "MyMovieSceneEventTrack.generated.h"

/**
 * 
 */
UCLASS()
class SEQUENCERCUSTOMPLUGIN_API UMyMovieSceneEventTrack : public UMovieSceneNameableTrack, public IMovieSceneTrackTemplateProducer
{
	GENERATED_BODY()

public:
	UMyMovieSceneEventTrack()
		: bFireEventsWhenForwards(true)
		, bFireEventsWhenBackwards(true)
	{
#if WITH_EDITORONLY_DATA
		TrackTint = FColor::Red;
#endif
	}


public:

	virtual const TArray<UMovieSceneSection*>& GetAllSections() const override { return SectionArray; }
	virtual FMovieSceneEvalTemplatePtr CreateTemplateForSection(const UMovieSceneSection& InSection) const override;
	virtual void PostCompile(FMovieSceneEvaluationTrack& Track, const FMovieSceneTrackCompilerArgs& Args) const override;

	virtual bool IsEmpty() const override;
	virtual bool SupportsType(TSubclassOf<UMovieSceneSection> SectionClass) const override;
	virtual void AddSection(UMovieSceneSection& Section) override;
	virtual class UMovieSceneSection* CreateNewSection() override;
	virtual bool HasSection(const UMovieSceneSection& Section) const override;
	virtual void RemoveSection(UMovieSceneSection& Section) override;
	virtual void RemoveSectionAt(int32 SectionIndex) override;
	virtual bool SupportsMultipleRows() const override { return false; }

#if WITH_EDITORONLY_DATA
	virtual FText GetDefaultDisplayName() const override { return FText::FromString(TEXT("My Event Track")); };
#endif

	UPROPERTY(EditAnywhere, Category = TrackEvent)
	uint32 bFireEventsWhenForwards : 1;

	UPROPERTY(EditAnywhere, Category = TrackEvent)
	uint32 bFireEventsWhenBackwards : 1;

	UPROPERTY()
	TArray<TObjectPtr<UMovieSceneSection>> SectionArray;

};
