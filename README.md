# Fuel Generator Multiplier

A minimal C++ mod for [Satisfactory Mod Loader](https://github.com/satisfactorymodding/SatisfactoryModLoader) that multiplies the
electrical output of **Fuel-Powered Generators** by a configurable factor, without changing how fast they burn fuel.

* Mod reference: `FuelGeneratorMultiplier`
* Targets Satisfactory 1.2 (CL `#491125` and newer) and SML `3.12.0`
* No content, no assets, no game feature data asset - native code only

## How it works

Satisfactory generators report their electrical output to the power grid through a single value:
`UFGPowerInfoComponent::SetDynamicProductionCapacity(float)`. That value is what the power circuit uses to decide how much power
a machine can deliver, and it is the only channel that output flows through.

This mod hooks that setter and, for Fuel-Powered Generators only, writes back the value multiplied by the configured factor.

What it deliberately does **not** touch:

* `AFGBuildableGenerator::mPowerProduction` - the generator's own production capacity, which is the value fuel consumption is
  derived from. Leaving it alone is what keeps fuel burn rate and burn duration unchanged.
* Fuel items, fuel energy values, recipes, or any other generator state.

Coal Generators are explicitly excluded, and only authoritative (server side) generators are modified, so the multiplied value is
computed once and replicated to clients through the normal `mDynamicProductionCapacity` replication.

Overclocking and underclocking are scaled too, because vanilla already multiplies the published capacity by the generator's
current potential. Production Boost Shards remain a separate additive vanilla bonus.

## Configuration

| Setting | Default | Meaning |
| --- | --- | --- |
| `Power Multiplier` | `1.0` | `1.0` = vanilla, `2.0` = twice the output, `0.0` = generators produce nothing |

The multiplier is editable **in game** from the SML mod menu (`Mods` → `Fuel Generator Multiplier` → `Config`), both from the main
menu and from the pause menu in a running save. Changes are applied immediately, no world reload needed.

Because SML's native configuration classes have no default editor widget (`UConfigProperty::CreateEditorWidget_Implementation`
returns `NULL` in C++, it is only implemented in SML's own Blueprint assets), this mod ships a small C++/Slate widget
(`UFGMPowerMultiplierWidget`) with a spin box and a `Reset to default` button, wired to the live SML config value. Saving is
handled by SML itself, on the regular config flush timer and on exit.

Config file location:

```
<Satisfactory>/FactoryGame/Configs/FuelGeneratorMultiplier.cfg
```

Example:

```json
{
    "RootSection": {
        "PowerMultiplier": 2.0
    }
}
```

On dedicated servers there is no configuration screen, so edit the file and restart the server.

## Installation

Release archives are published on the [Satisfactory Mod Repository](https://ficsit.app). To install manually, copy the mod folder
into the game's mod directory:

```
<Satisfactory>/FactoryGame/Mods/FuelGeneratorMultiplier/
```

SML discovers the plugin automatically, no `Mods.json` entry is required.

### Multiplayer

The mod is server authoritative. Install it on the server (or on the listen server host) and set the multiplier there. Clients do
not need the mod, the `.uplugin` declares `"RequiredOnRemote": false` and the multiplied value arrives through normal property
replication.

## Building from source

The mod expects to live inside a Satisfactory Starter Project.

1. Clone this repository into the Starter Project's `Mods` directory, so that the plugin is found at
   `<StarterProject>/Mods/FuelGeneratorMultiplier/`.
2. Make sure SML `3.12.0` or newer is present at `<StarterProject>/Mods/SML`.
3. Regenerate the Visual Studio project files for `FactoryGame.uproject`, then build the `Development Editor` `Win64` target of
   `FactoryGame` with the editor closed.
4. For a dedicated server, build the `Shipping Server` `Win64` target of `FactoryGame`.

Per the SML native hooking documentation, hooks are **not** installed in editor builds (`WITH_EDITOR`), because applying function
hooks at editor time is unreliable. Use Alpakit `Release` (or a packaged build) to observe the effect in game.

## Verification checklist

1. Set the multiplier to `1.0` - a Fuel-Powered Generator produces `250 MW` and behaves exactly like vanilla.
2. Set the multiplier to `2.0` - the same generator produces up to `500 MW` from the same fuel consumption.
3. Watch a fuel item: burn duration per item is identical at any multiplier, because nothing about the fuel path is modified.
4. Place a Coal Generator - it is unaffected at any multiplier.

## Project layout

```
Mods/FuelGeneratorMultiplier/
├── FuelGeneratorMultiplier.uplugin
└── Source/FuelGeneratorMultiplier/
    ├── FuelGeneratorMultiplier.Build.cs
    ├── Public/
    │   ├── FuelGeneratorMultiplierConstants.h
    │   ├── FuelGeneratorMultiplierModule.h
    │   ├── Configuration/
    │   │   ├── UFGMPowerMultiplierProperty.h
    │   │   ├── UFGMPowerMultiplierSection.h
    │   │   └── UFuelGeneratorMultiplierConfig.h
    │   ├── ModModules/UFuelGeneratorMultiplierGameInstanceModule.h
    │   ├── Settings/FuelGeneratorMultiplierSettings.h
    │   └── UI/UFGMPowerMultiplierWidget.h
    └── Private/
        ├── FuelGeneratorMultiplierModule.cpp
        ├── Configuration/
        │   ├── UFGMPowerMultiplierProperty.cpp
        │   ├── UFGMPowerMultiplierSection.cpp
        │   └── UFuelGeneratorMultiplierConfig.cpp
        ├── Hooks/GeneratorPowerHooks.cpp
        ├── ModModules/UFuelGeneratorMultiplierGameInstanceModule.cpp
        ├── Settings/FuelGeneratorMultiplierSettings.cpp
        └── UI/UFGMPowerMultiplierWidget.cpp
```

## Status

The mod has not been compiled or run in the development environment it was written in, because no Satisfactory installation,
Unreal Engine toolchain or Starter Project was available there. All engine and SML APIs it uses were taken from the SML `3.12.0`
and Satisfactory 1.2 headers (`FGPowerInfoComponent.h`, `Patching/NativeHookManager.h`, `Configuration/ConfigManager.h`,
`Module/GameInstanceModule.h`), and the build configuration mirrors the official Alpakit C++ mod template, but a real compile and
an in-game test are still required before release.
