#include "Configuration/UFGMPowerMultiplierSection.h"
#include "Configuration/UFGMPowerMultiplierProperty.h"
#include "UI/UFGMPowerMultiplierWidget.h"
#include "Blueprint/Widget.h"
#include "Engine/World.h"
#include "FuelGeneratorMultiplierConstants.h"

UUserWidget* UFGMPowerMultiplierSection::CreateEditorWidget_Implementation(UUserWidget* ParentWidget) const
{
	UFGMPowerMultiplierWidget* Widget = nullptr;

	if (IsValid(ParentWidget)) {
		Widget = CreateWidget<UFGMPowerMultiplierWidget>(ParentWidget, UFGMPowerMultiplierWidget::StaticClass());
	} else if (UWorld* World = GetWorld()) {
		Widget = CreateWidget<UFGMPowerMultiplierWidget>(World, UFGMPowerMultiplierWidget::StaticClass());
	}

	if (!IsValid(Widget)) {
		return nullptr;
	}

	const TObjectPtr<UConfigProperty>* Property = SectionProperties.Find(FuelGeneratorMultiplier::PowerMultiplierPropertyName);
	if (Property != nullptr && IsValid(*Property)) {
		Widget->SetPowerMultiplierProperty(Cast<UFGMPowerMultiplierProperty>(*Property));
	}

	return Widget;
}
