#include "FuelGeneratorMultiplierModule.h"
#include "Hooks/GeneratorPowerHooks.h"

void FFuelGeneratorMultiplierModule::StartupModule()
{
	FGeneratorPowerHooks::Install();
}

void FFuelGeneratorMultiplierModule::ShutdownModule()
{
	FGeneratorPowerHooks::Uninstall();
}

IMPLEMENT_MODULE(FFuelGeneratorMultiplierModule, FuelGeneratorMultiplier)
