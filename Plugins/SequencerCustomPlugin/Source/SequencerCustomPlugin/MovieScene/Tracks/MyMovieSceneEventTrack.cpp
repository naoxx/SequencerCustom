// Fill out your copyright notice in the Description page of Project Settings.


#include "SequencerCustomPlugin/MovieScene/Tracks/MyMovieSceneEventTrack.h"
#include "SequencerCustomPlugin/MovieScene/Sections/MyMovieSceneEventSection.h"
#include "SequencerCustomPlugin/MovieScene/Evaluation/MyMovieSceneEventEvalTemplate.h"
#include "Evaluation/MovieSceneEvaluation.h"

/*
*****************************************************************************/
FMovieSceneEvalTemplatePtr UMyMovieSceneEventTrack::CreateTemplateForSection(const UMovieSceneSection& InSection) const
{
	return FMyMovieSceneEventEvalTemplate(*Cast<const UMyMovieSceneEventSection>(&InSection), *this);
}

void UMyMovieSceneEventTrack::PostCompile(FMovieSceneEvaluationTrack& Track, const FMovieSceneTrackCompilerArgs& Args) const
{
	Track.SetEvaluationMethod(EEvaluationMethod::Swept);
}

void UMyMovieSceneEventTrack::AddSection(UMovieSceneSection& Section)
{
	if (UMyMovieSceneEventSection* TestSection = Cast<UMyMovieSceneEventSection>(&Section))
	{
		SectionArray.Add(TestSection);
	}
}

bool UMyMovieSceneEventTrack::SupportsType(TSubclassOf<UMovieSceneSection> SectionClass) const
{
	return SectionClass == UMyMovieSceneEventSection::StaticClass();
}

UMovieSceneSection* UMyMovieSceneEventTrack::CreateNewSection()
{
	return NewObject<UMyMovieSceneEventSection>(this, NAME_None, RF_Transactional);
}


bool UMyMovieSceneEventTrack::HasSection(const UMovieSceneSection& Section) const
{
	return SectionArray.Contains(&Section);
}


bool UMyMovieSceneEventTrack::IsEmpty() const
{
	return SectionArray.Num() == 0;
}

void UMyMovieSceneEventTrack::RemoveSection(UMovieSceneSection& Section)
{
	SectionArray.Remove(&Section);
}

void UMyMovieSceneEventTrack::RemoveSectionAt(int32 SectionIndex)
{
	SectionArray.RemoveAt(SectionIndex);
}