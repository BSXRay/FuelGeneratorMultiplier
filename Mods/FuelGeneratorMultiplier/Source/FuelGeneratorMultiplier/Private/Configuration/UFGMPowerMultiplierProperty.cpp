#include "Configuration/UFGMPowerMultiplierProperty.h"
#include "FuelGeneratorMultiplierConstants.h"

#define LOCTEXT_NAMESPACE "FuelGeneratorMultiplier"

UFGMPowerMultiplierProperty::UFGMPowerMultiplierProperty()
{
	WidgetType = ECP_FloatWidgetType::CPF_Spinbox;
	MinValue = FuelGeneratorMultiplier::MinimumPowerMultiplier;
	MaxValue = FuelGeneratorMultiplier::MaximumPowerMultiplier;
	DefaultValue = FuelGeneratorMultiplier::DefaultPowerMultiplier;
	Value = FuelGeneratorMultiplier::DefaultPowerMultiplier;
	DisplayName = LOCTEXT("PowerMultiplierDisplayName", "Power Multiplier");
	Tooltip = LOCTEXT("PowerMultiplierTooltip", "Multiplies the electrical output of every Fuel-Powered Generator. 1.0 is vanilla behaviour, 2.0 doubles the output. Fuel consumption is not affected.");
	bRequiresWorldReload = false;
}

#undef LOCTEXT_NAMESPACE
