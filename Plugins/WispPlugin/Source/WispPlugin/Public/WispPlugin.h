// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"

/**
 * @class FWispPluginModule
 * @brief The main module for the WispPlugin.
 *
 * This class handles the startup and shutdown of the plugin.
 */
class FWispPluginModule : public IModuleInterface
{
public:
	/**
	 * @brief Called when the module is started up.
	 */
	virtual void StartupModule() override;

	/**
	 * @brief Called when the module is shut down.
	 */
	virtual void ShutdownModule() override;
};
