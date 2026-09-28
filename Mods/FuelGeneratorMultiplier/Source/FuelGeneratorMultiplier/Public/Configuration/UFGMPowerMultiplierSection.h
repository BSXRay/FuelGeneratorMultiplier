#pragma once

#include "CoreMinimal.h"
#include "Configuration/Properties/ConfigPropertySection.h"
#include "UFGMPowerMultiplierSection.generated.h"

UCLASS()
class FUELGENERATORMULTIPLIER_API UFGMPowerMultiplierSection : public UConfigPropertySection
{
	GENERATED_BODY()

public:
	virtual UUserWidget* CreateEditorWidget_Implementation(UUserWidget* ParentWidget) const override;
};
