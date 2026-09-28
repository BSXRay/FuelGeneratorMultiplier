#pragma once

#include "CoreMinimal.h"

namespace FuelGeneratorMultiplier
{
	inline constexpr TCHAR ModReference[] = TEXT("FuelGeneratorMultiplier");
	inline constexpr TCHAR PowerMultiplierPropertyName[] = TEXT("PowerMultiplier");
	inline constexpr TCHAR FuelGeneratorClassPath[] = TEXT("/Game/FactoryGame/Buildable/Factory/GeneratorFuel/Build_GeneratorFuel.Build_GeneratorFuel_C");
	inline constexpr TCHAR CoalGeneratorClassPath[] = TEXT("/Game/FactoryGame/Buildable/Factory/GeneratorCoal/Build_GeneratorCoal.Build_GeneratorCoal_C");

	inline constexpr float DefaultPowerMultiplier = 1.0f;
	inline constexpr float MinimumPowerMultiplier = 0.0f;
	inline constexpr float MaximumPowerMultiplier = 100.0f;
}
