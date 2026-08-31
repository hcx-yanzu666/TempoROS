// Copyright Tempo Simulation, LLC. All Rights Reserved

#include "TempoROSBootstrap.h"

#include "Interfaces/IPluginManager.h"

#define LOCTEXT_NAMESPACE "FTempoROSBootstrapModule"

FString GetTempoROSDllDirectory()
{
	const FString TempoROSPluginPath = IPluginManager::Get().FindPlugin(TEXT("TempoROS"))->GetBaseDir();
	return FPaths::Combine(TempoROSPluginPath, TEXT("Source"), TEXT("ThirdParty"), TEXT("rclcpp"), TEXT("Binaries"), TEXT("Windows"));
}

#if PLATFORM_WINDOWS
FString GetEngineOpenSSLDllDirectory()
{
	return FPaths::Combine(
		FPaths::EngineDir(),
		TEXT("Extras"),
		TEXT("ThirdPartyNotUE"),
		TEXT("libimobiledevice"),
		TEXT("x64"));
}
#endif

void FTempoROSBootstrapModule::StartupModule()
{
#if PLATFORM_WINDOWS
	const FString OpenSSLDllDirectory = GetEngineOpenSSLDllDirectory();
	if (FPaths::DirectoryExists(OpenSSLDllDirectory))
	{
		const FString CurrentPath = FPlatformMisc::GetEnvironmentVariable(TEXT("PATH"));
		const FString UpdatedPath = FString::Printf(TEXT("%s;%s"), *OpenSSLDllDirectory, *CurrentPath);
		FPlatformMisc::SetEnvironmentVar(TEXT("PATH"), *UpdatedPath);
	}

	FPlatformProcess::PushDllDirectory(*GetTempoROSDllDirectory());
#endif
}

void FTempoROSBootstrapModule::ShutdownModule()
{
#if PLATFORM_WINDOWS
	FPlatformProcess::PopDllDirectory(*GetTempoROSDllDirectory());
#endif
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FTempoROSBootstrapModule, TempoROSBootstrap)
