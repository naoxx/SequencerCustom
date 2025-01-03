// Fill out your copyright notice in the Description page of Project Settings.


#include "SequencerCustomPlugin_Editor/TrackEditors/MyEventTrackEditor.h"
#include "SequencerCustomPlugin/MovieScene/Tracks/MyMovieSceneEventTrack.h"
#include "SequencerCustomPlugin/MovieScene/Sections/MyMovieSceneEventSection.h"
#include "LevelSequence.h"
#include "MVVM/Views/ViewUtilities.h"
#include "MovieSceneSequenceEditor.h"


/* FMyEventTrackEditor static functions
 *****************************************************************************/

TSharedRef<ISequencerTrackEditor> FMyEventTrackEditor::CreateTrackEditor(TSharedRef<ISequencer> InSequencer)
{
	return MakeShareable(new FMyEventTrackEditor(InSequencer));
}


TSharedRef<ISequencerSection> FMyEventTrackEditor::MakeSectionInterface(UMovieSceneSection& SectionObject, UMovieSceneTrack& Track, FGuid ObjectBinding)
{

	return MakeShared<FSequencerSection>(SectionObject);
}


/* FMyEventTrackEditor structors
 *****************************************************************************/

FMyEventTrackEditor::FMyEventTrackEditor(TSharedRef<ISequencer> InSequencer)
	: FMovieSceneTrackEditor(InSequencer)
{ }


/* ISequencerTrackEditor interface
 *****************************************************************************/

void FMyEventTrackEditor::BuildAddTrackMenu(FMenuBuilder& MenuBuilder)
{
	UMovieSceneSequence* RootMovieSceneSequence = GetSequencer()->GetRootMovieSceneSequence();
	FMovieSceneSequenceEditor* SequenceEditor = FMovieSceneSequenceEditor::Find(RootMovieSceneSequence);


	if (SequenceEditor && SequenceEditor->SupportsEvents(RootMovieSceneSequence))
	{
		MenuBuilder.AddMenuEntry(
			FText::FromString(TEXT("Add My Event Track")),
			FText::FromString(TEXT("Add My Event Track Tooltip")),
			FSlateIcon(),
			FUIAction(
				FExecuteAction::CreateSP(this, &FMyEventTrackEditor::HandleAddEventTrackMenuEntryExecute, TArray<FGuid>(), UMyMovieSceneEventSection::StaticClass())
			)
		);
	}

}

void FMyEventTrackEditor::BuildObjectBindingTrackMenu(FMenuBuilder& MenuBuilder, const TArray<FGuid>& ObjectBindings, const UClass* ObjectClass)
{
	UMovieSceneSequence* RootMovieSceneSequence = GetSequencer()->GetRootMovieSceneSequence();
	FMovieSceneSequenceEditor* SequenceEditor = FMovieSceneSequenceEditor::Find(RootMovieSceneSequence);

	if (SequenceEditor && SequenceEditor->SupportsEvents(RootMovieSceneSequence))
	{
		MenuBuilder.AddMenuEntry(
			FText::FromString(TEXT("Add My Event Track")),
			FText::FromString(TEXT("Add My Event Track Tooltip")),
			FSlateIcon(),
			FUIAction(
				FExecuteAction::CreateSP(this, &FMyEventTrackEditor::HandleAddEventTrackMenuEntryExecute, ObjectBindings, UMyMovieSceneEventSection::StaticClass())
			)
		);

	}
}

TSharedPtr<SWidget> FMyEventTrackEditor::BuildOutlinerEditWidget(const FGuid& ObjectBinding, UMovieSceneTrack* Track, const FBuildEditWidgetParams& Params)
{
	check(Track);

	TSharedPtr<ISequencer> SequencerPtr = GetSequencer();
	if (!SequencerPtr.IsValid())
	{
		return SNullWidget::NullWidget;
	}

	TWeakObjectPtr<UMovieSceneTrack> WeakTrack = Track;
	const int32 RowIndex = Params.TrackInsertRowIndex;
	auto SubMenuCallback = [this, WeakTrack, RowIndex]
		{
			FMenuBuilder MenuBuilder(true, nullptr);

			UMovieSceneTrack* TrackPtr = WeakTrack.Get();

			CreateNewSection(TrackPtr, RowIndex + 1, UMyMovieSceneEventSection::StaticClass(), true);

			return MenuBuilder.MakeWidget();
		};

	return UE::Sequencer::MakeAddButton(FText::FromString(TEXT("Section")), FOnGetContent::CreateLambda(SubMenuCallback), Params.ViewModel);
}

void FMyEventTrackEditor::BuildTrackContextMenu(FMenuBuilder& MenuBuilder, UMovieSceneTrack* Track)
{

}


bool FMyEventTrackEditor::SupportsType(TSubclassOf<UMovieSceneTrack> Type) const
{
	return (Type == UMyMovieSceneEventTrack::StaticClass());
}

bool  FMyEventTrackEditor::SupportsSequence(UMovieSceneSequence* InSequence) const
{
	if (Cast<ULevelSequence>(InSequence))
	{
		return true;
	}
	return false;

}

const FSlateBrush* FMyEventTrackEditor::GetIconBrush() const
{
	return FAppStyle::GetBrush("Sequencer.Tracks.Event");
}

/* FMyEventTrackEditor callbacks
 *****************************************************************************/

void FMyEventTrackEditor::HandleAddEventTrackMenuEntryExecute(TArray<FGuid> InObjectBindingIDs, UClass* SectionType)
{
	UMovieScene* FocusedMovieScene = GetFocusedMovieScene();

	if (FocusedMovieScene == nullptr)
	{
		return;
	}

	if (FocusedMovieScene->IsReadOnly())
	{
		return;
	}

	FocusedMovieScene->Modify();

	TArray<UMyMovieSceneEventTrack*> NewTracks;

	for (FGuid InObjectBindingID : InObjectBindingIDs)
	{
		if (InObjectBindingID.IsValid())
		{
			UMyMovieSceneEventTrack* NewObjectTrack = FocusedMovieScene->AddTrack<UMyMovieSceneEventTrack>(InObjectBindingID);
			NewTracks.Add(NewObjectTrack);

			if (GetSequencer().IsValid())
			{
				GetSequencer()->OnAddTrack(NewObjectTrack, InObjectBindingID);
			}
		}
	}

	if (!NewTracks.Num())
	{
		UMyMovieSceneEventTrack* NewTrack = FocusedMovieScene->AddTrack<UMyMovieSceneEventTrack>();
		NewTracks.Add(NewTrack);
		if (GetSequencer().IsValid())
		{
			GetSequencer()->OnAddTrack(NewTrack, FGuid());
		}
	}

	check(NewTracks.Num() != 0);

	for (UMyMovieSceneEventTrack* NewTrack : NewTracks)
	{
		CreateNewSection(NewTrack, 0, SectionType, false);

		NewTrack->SetDisplayName(FText::FromString(TEXT("My Event Track")));


	}
}

void FMyEventTrackEditor::CreateNewSection(UMovieSceneTrack* Track, int32 RowIndex, UClass* SectionType, bool bSelect)
{
	TSharedPtr<ISequencer> SequencerPtr = GetSequencer();
	if (SequencerPtr.IsValid())
	{
		UMovieScene* FocusedMovieScene = GetFocusedMovieScene();
		FQualifiedFrameTime CurrentTime = SequencerPtr->GetLocalTime();

		FScopedTransaction Transaction(FText::FromString(TEXT("Add Section")));

		UMovieSceneSection* NewSection = NewObject<UMovieSceneSection>(Track, SectionType, NAME_None, RF_Transactional);
		check(NewSection);

		int32 OverlapPriority = 0;
		for (UMovieSceneSection* Section : Track->GetAllSections())
		{
			if (Section->GetRowIndex() >= RowIndex)
			{
				Section->SetRowIndex(Section->GetRowIndex() + 1);
			}
			OverlapPriority = FMath::Max(Section->GetOverlapPriority() + 1, OverlapPriority);
		}

		Track->Modify();

		NewSection->SetRange(TRange<FFrameNumber>::All());

		NewSection->SetOverlapPriority(OverlapPriority);
		NewSection->SetRowIndex(RowIndex);

		Track->AddSection(*NewSection);
		Track->UpdateEasing();

		if (bSelect)
		{
			SequencerPtr->EmptySelection();
			SequencerPtr->SelectSection(NewSection);
			SequencerPtr->ThrobSectionSelection();
		}

		SequencerPtr->NotifyMovieSceneDataChanged(EMovieSceneDataChangeType::MovieSceneStructureItemAdded);
	}
}

