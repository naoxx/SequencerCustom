#pragma once

#include "Modules/ModuleManager.h"

DECLARE_LOG_CATEGORY_EXTERN(SequencerCustomPlugin_Editor, All, All);

class FSequencerCustomPlugin_Editor : public IModuleInterface
{
	public:

	/* Called when the module is loaded */
	virtual void StartupModule() override;

	/* Called when the module is unloaded */
	virtual void ShutdownModule() override;

private:
	FDelegateHandle MyEventTrackEditorHandle;
};