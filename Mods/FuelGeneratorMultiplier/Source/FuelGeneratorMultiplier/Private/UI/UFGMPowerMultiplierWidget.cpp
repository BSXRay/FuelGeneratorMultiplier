#include "UI/UFGMPowerMultiplierWidget.h"
#include "Configuration/UFGMPowerMultiplierProperty.h"
#include "FuelGeneratorMultiplierConstants.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Input/SSpinBox.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Text/STextBlock.h"

#define LOCTEXT_NAMESPACE "FuelGeneratorMultiplier"

void UFGMPowerMultiplierWidget::SetPowerMultiplierProperty(UFGMPowerMultiplierProperty* InPowerMultiplierProperty)
{
	PowerMultiplierProperty = InPowerMultiplierProperty;
}

TSharedRef<SWidget> UFGMPowerMultiplierWidget::RebuildWidget()
{
	return SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(0.0f, 2.0f)
		[
			SNew(STextBlock)
			.Text(this, &UFGMPowerMultiplierWidget::HandleDisplayName)
		]
		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(0.0f, 2.0f)
		[
			SNew(SSpinBox<float>)
			.MinValue(FuelGeneratorMultiplier::MinimumPowerMultiplier)
			.MaxValue(FuelGeneratorMultiplier::MaximumPowerMultiplier)
			.MinSliderValue(0.0f)
			.MaxSliderValue(10.0f)
			.Value(this, &UFGMPowerMultiplierWidget::HandleValue)
			.OnValueChanged(this, &UFGMPowerMultiplierWidget::HandleValueChanged)
		]
		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(0.0f, 2.0f)
		[
			SNew(SButton)
			.Text(LOCTEXT("ResetToDefault", "Reset to default"))
			.OnClicked(this, &UFGMPowerMultiplierWidget::HandleResetClicked)
		]
		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(0.0f, 2.0f)
		[
			SNew(STextBlock)
			.Text(this, &UFGMPowerMultiplierWidget::HandleTooltip)
			.AutoWrapText(true)
		];
}

FText UFGMPowerMultiplierWidget::HandleDisplayName() const
{
	if (const UFGMPowerMultiplierProperty* Property = PowerMultiplierProperty.Get()) {
		return Property->DisplayName;
	}
	return LOCTEXT("FallbackDisplayName", "Power Multiplier");
}

FText UFGMPowerMultiplierWidget::HandleTooltip() const
{
	if (const UFGMPowerMultiplierProperty* Property = PowerMultiplierProperty.Get()) {
		return Property->Tooltip;
	}
	return FText::GetEmpty();
}

float UFGMPowerMultiplierWidget::HandleValue() const
{
	if (const UFGMPowerMultiplierProperty* Property = PowerMultiplierProperty.Get()) {
		return Property->Value;
	}
	return FuelGeneratorMultiplier::DefaultPowerMultiplier;
}

void UFGMPowerMultiplierWidget::HandleValueChanged(const float NewValue)
{
	UFGMPowerMultiplierProperty* Property = PowerMultiplierProperty.Get();
	if (!IsValid(Property)) {
		return;
	}

	Property->Value = NewValue;
	Property->MarkDirty();
}

FReply UFGMPowerMultiplierWidget::HandleResetClicked()
{
	if (UFGMPowerMultiplierProperty* Property = PowerMultiplierProperty.Get()) {
		Property->ResetToDefault();
	}

	return FReply::Handled();
}

#undef LOCTEXT_NAMESPACE
