#include "Settings/FuelGeneratorMultiplierSettings.h"
#include "FuelGeneratorMultiplierConstants.h"

namespace
{
	float GPowerMultiplier = FuelGeneratorMultiplier::DefaultPowerMultiplier;
}

float FFuelGeneratorMultiplierSettings::GetPowerMultiplier()
{
	return GPowerMultiplier;
}

void FFuelGeneratorMultiplierSettings::SetPowerMultiplier(const float InPowerMultiplier)
{
	const float Clamped = FMath::Clamp(InPowerMultiplier, FuelGeneratorMultiplier::MinimumPowerMultiplier, FuelGeneratorMultiplier::MaximumPowerMultiplier);

	if (FMath::IsNearlyEqual(Clamped, GPowerMultiplier, SMALL_NUMBER)) {
		return;
	}

	GPowerMultiplier = Clamped;
}

void FFuelGeneratorMultiplierSettings::ResetToDefault()
{
	GPowerMultiplier = FuelGeneratorMultiplier::DefaultPowerMultiplier;
}
