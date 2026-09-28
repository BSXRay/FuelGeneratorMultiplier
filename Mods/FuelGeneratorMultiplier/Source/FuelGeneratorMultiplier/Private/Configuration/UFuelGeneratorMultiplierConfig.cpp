#include "Configuration/UFuelGeneratorMultiplierConfig.h"
#include "Configuration/UFGMPowerMultiplierProperty.h"
#include "Configuration/UFGMPowerMultiplierSection.h"
#include "Configuration/ConfigManager.h"
#include "Engine/GameInstance.h"
#include "Engine/World.h"
#include "FuelGeneratorMultiplierConstants.h"

#define LOCTEXT_NAMESPACE "FuelGeneratorMultiplier"

UFuelGeneratorMultiplierConfig::UFuelGeneratorMultiplierConfig()
{
	ConfigId = FConfigId();
	ConfigId.ModReference = FuelGeneratorMultiplier::ModReference;
	ConfigId.ConfigCategory = TEXT("");

	DisplayName = LOCTEXT("ConfigDisplayName", "Fuel Generator Multiplier");
	Description = LOCTEXT("ConfigDescription", "Scales the electrical output of Fuel-Powered Generators without changing fuel consumption.");

	RootSection = CreateDefaultSubobject<UFGMPowerMultiplierSection>(TEXT("RootSection"));
	RootSection->DisplayName = LOCTEXT("RootSectionDisplayName", "Generator Output");
	RootSection->Tooltip = LOCTEXT("RootSectionTooltip", "Settings applied to every Fuel-Powered Generator in the save.");

	UFGMPowerMultiplierProperty* PowerMultiplier = CreateDefaultSubobject<UFGMPowerMultiplierProperty>(TEXT("PowerMultiplier"));
	RootSection->SectionProperties.Add(FuelGeneratorMultiplier::PowerMultiplierPropertyName, PowerMultiplier);
}

FConfigId UFuelGeneratorMultiplierConfig::GetConfigId()
{
	FConfigId Result;
	Result.ModReference = FuelGeneratorMultiplier::ModReference;
	Result.ConfigCategory = TEXT("");
	return Result;
}

UConfigManager* UFuelGeneratorMultiplierConfig::FindConfigManager(const UObject* WorldContextObject)
{
	if (!IsValid(WorldContextObject)) {
		return nullptr;
	}

	const UWorld* World = WorldContextObject->GetWorld();
	if (!IsValid(World)) {
		return nullptr;
	}

	UGameInstance* GameInstance = World->GetGameInstance();
	if (!IsValid(GameInstance)) {
		return nullptr;
	}

	return GameInstance->GetSubsystem<UConfigManager>();
}

float UFuelGeneratorMultiplierConfig::ReadPowerMultiplier(const UObject* WorldContextObject)
{
	const UConfigManager* ConfigManager = FindConfigManager(WorldContextObject);
	if (!IsValid(ConfigManager)) {
		return FuelGeneratorMultiplier::DefaultPowerMultiplier;
	}

	UConfigPropertySection* RootSection = ConfigManager->GetConfigurationRootSection(GetConfigId());
	if (!IsValid(RootSection)) {
		return FuelGeneratorMultiplier::DefaultPowerMultiplier;
	}

	const TObjectPtr<UConfigProperty>* Property = RootSection->SectionProperties.Find(FuelGeneratorMultiplier::PowerMultiplierPropertyName);
	if (Property == nullptr || !IsValid(*Property)) {
		return FuelGeneratorMultiplier::DefaultPowerMultiplier;
	}

	const UFGMPowerMultiplierProperty* PowerMultiplier = Cast<UFGMPowerMultiplierProperty>(*Property);
	if (!IsValid(PowerMultiplier)) {
		return FuelGeneratorMultiplier::DefaultPowerMultiplier;
	}

	return PowerMultiplier->Value;
}

#undef LOCTEXT_NAMESPACE
