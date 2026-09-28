#pragma once

#include "CoreMinimal.h"
#include "Configuration/Properties/WidgetExtension/CP_Float.h"
#include "UFGMPowerMultiplierProperty.generated.h"

UCLASS(EditInlineNew)
class FUELGENERATORMULTIPLIER_API UFGMPowerMultiplierProperty : public UCP_Float
{
	GENERATED_BODY()

public:
	UFGMPowerMultiplierProperty();
};
