#pragma once

#include "CoreMinimal.h"

class FUELGENERATORMULTIPLIER_API FFuelGeneratorMultiplierSettings
{
public:
	static float GetPowerMultiplier();
	static void SetPowerMultiplier(float InPowerMultiplier);
	static void ResetToDefault();
};
