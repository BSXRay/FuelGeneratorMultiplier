#pragma once

#include "CoreMinimal.h"
#include "Blueprint/Widget.h"
#include "UFGMPowerMultiplierWidget.generated.h"

class UFGMPowerMultiplierProperty;

UCLASS()
class FUELGENERATORMULTIPLIER_API UFGMPowerMultiplierWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetPowerMultiplierProperty(UFGMPowerMultiplierProperty* InPowerMultiplierProperty);

protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;

private:
	FText HandleDisplayName() const;
	FText HandleTooltip() const;
	float HandleValue() const;
	void HandleValueChanged(float NewValue);
	FReply HandleResetClicked();

	TWeakObjectPtr<UFGMPowerMultiplierProperty> PowerMultiplierProperty;
};
