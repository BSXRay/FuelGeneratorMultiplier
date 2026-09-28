#pragma once

#include "CoreMinimal.h"
#include "Configuration/ModConfiguration.h"
#include "UFuelGeneratorMultiplierConfig.generated.h"

class UConfigManager;

UCLASS()
class FUELGENERATORMULTIPLIER_API UFuelGeneratorMultiplierConfig : public UModConfiguration
{
	GENERATED_BODY()

public:
	UFuelGeneratorMultiplierConfig();

	static FConfigId GetConfigId();
	static float ReadPowerMultiplier(const UObject* WorldContextObject);
	static UConfigManager* FindConfigManager(const UObject* WorldContextObject);
};
