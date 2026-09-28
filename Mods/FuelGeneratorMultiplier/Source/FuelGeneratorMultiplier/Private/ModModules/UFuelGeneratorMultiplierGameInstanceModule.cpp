#include "ModModules/UFuelGeneratorMultiplierGameInstanceModule.h"
#include "Configuration/ConfigManager.h"
#include "Configuration/UFuelGeneratorMultiplierConfig.h"
#include "FuelGeneratorMultiplierConstants.h"
#include "Settings/FuelGeneratorMultiplierSettings.h"

DEFINE_LOG_CATEGORY_STATIC(LogFuelGeneratorMultiplier, Log, All);

UFuelGeneratorMultiplierGameInstanceModule::UFuelGeneratorMultiplierGameInstanceModule()
{
	bRootModule = true;
	ModConfigurations.Add(UFuelGeneratorMultiplierConfig::StaticClass());
}

void UFuelGeneratorMultiplierGameInstanceModule::DispatchLifecycleEvent(ELifecyclePhase Phase)
{
	Super::DispatchLifecycleEvent(Phase);

	if (Phase == ELifecyclePhase::INITIALIZATION || Phase == ELifecyclePhase::POST_INITIALIZATION) {
		RefreshCachedSettings();
	}
}

void UFuelGeneratorMultiplierGameInstanceModule::HandleConfigurationValueChanged()
{
	RefreshCachedSettings();
}

void UFuelGeneratorMultiplierGameInstanceModule::RefreshCachedSettings()
{
	UConfigManager* ConfigManager = UFuelGeneratorMultiplierConfig::FindConfigManager(this);
	if (!IsValid(ConfigManager)) {
		return;
	}

	UConfigPropertySection* RootSection = ConfigManager->GetConfigurationRootSection(UFuelGeneratorMultiplierConfig::GetConfigId());
	if (!IsValid(RootSection)) {
		return;
	}

	FFuelGeneratorMultiplierSettings::SetPowerMultiplier(UFuelGeneratorMultiplierConfig::ReadPowerMultiplier(this));
	UE_LOG(LogFuelGeneratorMultiplier, Log, TEXT("Power multiplier set to %f"), FFuelGeneratorMultiplierSettings::GetPowerMultiplier());

	if (!PowerMultiplierProperty.IsValid()) {
		const TObjectPtr<UConfigProperty>* Property = RootSection->SectionProperties.Find(FuelGeneratorMultiplier::PowerMultiplierPropertyName);
		if (Property != nullptr && IsValid(*Property)) {
			PowerMultiplierProperty = *Property;
			PowerMultiplierProperty->OnPropertyValueChanged.AddUniqueDynamic(this, &UFuelGeneratorMultiplierGameInstanceModule::HandleConfigurationValueChanged);
		}
	}
}
