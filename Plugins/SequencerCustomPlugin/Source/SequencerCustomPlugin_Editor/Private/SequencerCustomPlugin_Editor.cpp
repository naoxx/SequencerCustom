#include "SequencerCustomPlugin_Editor.h"
#include "ISequencerModule.h"

#include "SequencerCustomPlugin_Editor/TrackEditors/MyEventTrackEditor.h"
#include "SequencerCustomPlugin/MovieScene/Tracks/MyMovieSceneEventTrack.h"
#include "SequencerCustomPlugin/MovieScene/Sections/MyMovieSceneEventSection.h"
#include "ISequencerChannelInterface.h"
#include "SequencerChannelInterface.h"

DEFINE_LOG_CATEGORY(SequencerCustomPlugin_Editor);

#define LOCTEXT_NAMESPACE "FSequencerCustomPlugin_Editor"

void FSequencerCustomPlugin_Editor::StartupModule()
{
	UE_LOG(SequencerCustomPlugin_Editor, Warning, TEXT("SequencerCustomPlugin_Editor module has been loaded"));

	if (GIsEditor)
	{
		ISequencerModule& SequencerModule = FModuleManager::Get().LoadModuleChecked<ISequencerModule>("Sequencer");

		MyEventTrackEditorHandle = SequencerModule.RegisterTrackEditor(FOnCreateTrackEditor::CreateStatic(&FMyEventTrackEditor::CreateTrackEditor));

		SequencerModule.RegisterChannelInterface<FMyMovieSceneEventData>();
	}
}

void FSequencerCustomPlugin_Editor::ShutdownModule()
{
	UE_LOG(SequencerCustomPlugin_Editor, Warning, TEXT("SequencerCustomPlugin_Editor module has been unloaded"));

	ISequencerModule& SequencerModule = FModuleManager::Get().GetModuleChecked<ISequencerModule>("Sequencer");
	SequencerModule.UnRegisterTrackEditor(MyEventTrackEditorHandle);
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FSequencerCustomPlugin_Editor, SequencerCustomPlugin_Editor)