// Copyright Epic Games, Inc. All Rights Reserved.

#include "ProjectC_5_4.h"
//#include "Modules/ModuleManager.h"

void FMyProjectModule::StartupModule()
{
	UE_LOG(LogTemp, Warning, TEXT("Module Started"));

	FString ShaderDir = FPaths::Combine(FPaths::ProjectDir(), "Shaders");
	AddShaderSourceDirectoryMapping("/Project", ShaderDir);
}

void FMyProjectModule::ShutdownModule()
{
	UE_LOG(LogTemp, Warning, TEXT("Module Shutdown"));
}

IMPLEMENT_PRIMARY_GAME_MODULE(FMyProjectModule, ProjectC_5_4, "ProjectC_5_4");

//IMPLEMENT_PRIMARY_GAME_MODULE( FDefaultGameModuleImpl, ProjectC_5_4, "ProjectC_5_4" );
