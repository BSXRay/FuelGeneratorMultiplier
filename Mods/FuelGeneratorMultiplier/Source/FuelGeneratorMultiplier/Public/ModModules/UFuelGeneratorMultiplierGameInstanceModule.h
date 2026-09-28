#pragma once

#include "CoreMinimal.h"
#include "Module/GameInstanceModule.h"
#include "UFuelGeneratorMultiplierGameInstanceModule.generated.h"

class UConfigProperty;

UCLASS()
class FUELGENERATORMULTIPLIER_API UFuelGeneratorMultiplierGameInstanceModule : public UGameInstanceModule
{
	GENERATED_BODY()

public:
	UFuelGeneratorMultiplierGameInstanceModule();

	virtual void DispatchLifecycleEvent(ELifecyclePhase Phase) override;

	UFUNCTION()
	void HandleConfigurationValueChanged();

private:
	void RefreshCachedSettings();

	TWeakObjectPtr<UConfigProperty> PowerMultiplierProperty;
};
