// Fill out your copyright notice in the Description page of Project Settings.


#include "SequencerCustomPlugin/MovieScene/Sections/MyMovieSceneEventSection.h"
#include "Channels/MovieSceneChannelEditorData.h"
#include "Channels/MovieSceneChannelProxy.h"

/*
*****************************************************************************/

void FMyMovieSceneEventData::GetKeys(const TRange<FFrameNumber>& WithinRange, TArray<FFrameNumber>* OutKeyTimes, TArray<FKeyHandle>* OutKeyHandles)
{
	GetData().GetKeys(WithinRange, OutKeyTimes, OutKeyHandles);
}

void FMyMovieSceneEventData::GetKeyTimes(TArrayView<const FKeyHandle> InHandles, TArrayView<FFrameNumber> OutKeyTimes)
{
	GetData().GetKeyTimes(InHandles, OutKeyTimes);
}

void FMyMovieSceneEventData::SetKeyTimes(TArrayView<const FKeyHandle> InHandles, TArrayView<const FFrameNumber> InKeyTimes)
{
	GetData().SetKeyTimes(InHandles, InKeyTimes);
}

void FMyMovieSceneEventData::DuplicateKeys(TArrayView<const FKeyHandle> InHandles, TArrayView<FKeyHandle> OutNewHandles)
{
	GetData().DuplicateKeys(InHandles, OutNewHandles);
}

void FMyMovieSceneEventData::DeleteKeys(TArrayView<const FKeyHandle> InHandles)
{
	GetData().DeleteKeys(InHandles);
}

void FMyMovieSceneEventData::DeleteKeysFrom(FFrameNumber InTime, bool bDeleteKeysBefore)
{
	GetData().DeleteKeysFrom(InTime, bDeleteKeysBefore);
}

void FMyMovieSceneEventData::ChangeFrameResolution(FFrameRate SourceRate, FFrameRate DestinationRate)
{
	GetData().ChangeFrameResolution(SourceRate, DestinationRate);
}

TRange<FFrameNumber> FMyMovieSceneEventData::ComputeEffectiveRange() const
{
	return GetData().GetTotalRange();
}

int32 FMyMovieSceneEventData::GetNumKeys() const
{
	return Times.Num();
}

void FMyMovieSceneEventData::Reset()
{
	Times.Reset();
	KeyValues.Reset();
	KeyHandles.Reset();
}

void FMyMovieSceneEventData::Offset(FFrameNumber DeltaPosition)
{
	GetData().Offset(DeltaPosition);
}


FKeyHandle FMyMovieSceneEventData::GetHandle(int32 Index)
{
	return GetData().GetHandle(Index);
}

int32 FMyMovieSceneEventData::GetIndex(FKeyHandle Handle)
{
	return GetData().GetIndex(Handle);
}


/*
*****************************************************************************/
UMyMovieSceneEventSection::UMyMovieSceneEventSection()
{
	bSupportsInfiniteRange = true;
	SetRange(TRange<FFrameNumber>::All());

#if WITH_EDITOR

	ChannelProxy = MakeShared<FMovieSceneChannelProxy>(EventData, FMovieSceneChannelMetaData());

#else

	ChannelProxy = MakeShared<FMovieSceneChannelProxy>(EventData);

#endif
}
