#include "Hooks/GeneratorPowerHooks.h"
#include "Patching/NativeHookManager.h"
#include "FGPowerInfoComponent.h"
#include "GameFramework/Actor.h"
#include "UObject/Class.h"
#include "FuelGeneratorMultiplierConstants.h"
#include "Settings/FuelGeneratorMultiplierSettings.h"

DEFINE_LOG_CATEGORY_STATIC(LogFuelGeneratorMultiplierHooks, Log, All);

namespace
{
	TWeakObjectPtr<UClass> FuelGeneratorClass;
	TWeakObjectPtr<UClass> CoalGeneratorClass;
	FDelegateHandle SetDynamicProductionCapacityHandle;
	bool bInsideScaledWrite = false;

	using FSetDynamicProductionCapacityInvoker = HookInvoker<
		decltype(&UFGPowerInfoComponent::SetDynamicProductionCapacity),
		&UFGPowerInfoComponent::SetDynamicProductionCapacity>;

	const TCHAR* SetDynamicProductionCapacitySymbol = TEXT("UFGPowerInfoComponent::SetDynamicProductionCapacity");

	UClass* ResolveGeneratorClass(const TCHAR* ClassPath)
	{
		UClass* Resolved = FindObject<UClass>(nullptr, ClassPath);
		if (Resolved == nullptr) {
			Resolved = LoadObject<UClass>(nullptr, ClassPath);
		}
		return Resolved;
	}

	bool IsFuelPoweredGenerator(const AActor* Actor)
	{
		UClass* FuelClass = FuelGeneratorClass.Get();
		if (FuelClass == nullptr) {
			FuelGeneratorClass = ResolveGeneratorClass(FuelGeneratorMultiplier::FuelGeneratorClassPath);
			FuelClass = FuelGeneratorClass.Get();
		}
		if (FuelClass == nullptr) {
			return false;
		}

		if (CoalGeneratorClass.Get() == nullptr) {
			CoalGeneratorClass = ResolveGeneratorClass(FuelGeneratorMultiplier::CoalGeneratorClassPath);
		}

		const UClass* ActorClass = Actor->GetClass();
		if (ActorClass == CoalGeneratorClass.Get()) {
			return false;
		}
		return ActorClass == FuelClass || ActorClass->IsChildOf(FuelClass);
	}
}

void FGeneratorPowerHooks::Install()
{
	if (WITH_EDITOR) {
		return;
	}

	if (SetDynamicProductionCapacityHandle.IsValid()) {
		return;
	}

	FSetDynamicProductionCapacityInvoker::InstallHook(SetDynamicProductionCapacitySymbol);

	SetDynamicProductionCapacityHandle = FSetDynamicProductionCapacityInvoker::AddHandlerAfter(
		[](UFGPowerInfoComponent* Self, float NewProduction) {
			if (bInsideScaledWrite) {
				return;
			}

			const float Multiplier = FFuelGeneratorMultiplierSettings::GetPowerMultiplier();
			if (Multiplier <= 0.0f || FMath::IsNearlyEqual(Multiplier, 1.0f)) {
				return;
			}

			if (!IsValid(Self) || FMath::IsNearlyZero(NewProduction)) {
				return;
			}

			AActor* Owner = Self->GetOwner();
			if (!IsValid(Owner) || !Owner->HasAuthority()) {
				return;
			}

			if (!IsFuelPoweredGenerator(Owner)) {
				return;
			}

			bInsideScaledWrite = true;
			Self->SetDynamicProductionCapacity(NewProduction * Multiplier);
			bInsideScaledWrite = false;
		});

	UE_LOG(LogFuelGeneratorMultiplierHooks, Log, TEXT("Installed generator output hook"));
}

void FGeneratorPowerHooks::Uninstall()
{
	if (WITH_EDITOR) {
		return;
	}

	if (!SetDynamicProductionCapacityHandle.IsValid()) {
		return;
	}

	FSetDynamicProductionCapacityInvoker::RemoveHandler(SetDynamicProductionCapacitySymbol, SetDynamicProductionCapacityHandle);
	SetDynamicProductionCapacityHandle.Reset();

	UE_LOG(LogFuelGeneratorMultiplierHooks, Log, TEXT("Removed generator output hook"));
}
