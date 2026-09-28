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

This is a **native C++ mod**, so it cannot be installed as source: it has to be compiled once, because the game cannot load
uncompiled C++ code. There are two ways to get a working build into your game.

### Option A - Alpakit (recommended: compiles, packages and installs in one step)

This is the workflow the SML documentation prescribes.

1. Set up the modding environment once: Satisfactory, Visual Studio 2022, the custom Unreal Engine `5.6.1-CSS`, Wwise
   `2023.1.14.8770` and the [Starter Project](https://docs.ficsit.app/Development/BeginnersGuide/dependencies.html).
   See the [Required Software](https://docs.ficsit.app/Development/BeginnersGuide/dependencies.html) page for details.
2. Clone this repository into the Starter Project, so the plugin ends up at `<StarterProject>/Mods/FuelGeneratorMultiplier/`.
   SML must be present at `<StarterProject>/Mods/SML`.
3. Open `FactoryGame.uproject` in Unreal and open the **Alpakit Dev** panel (`File > Alpakit Dev`).
4. Under **Dev Packaging Settings > Windows**, tick `Enabled`, tick `Copy to Game Path`, and point it at your game install
   directory, for example `C:\Program Files\EpicGames\SatisfactoryEarlyAccess\`.
   Also tick `Launch Game Type` if you want Alpakit to start the game for you once packaging is done.
5. Press the **`Alpakit!`** button next to `Fuel Generator Multiplier (FuelGeneratorMultiplier)`.

Alpakit compiles the module, packages the plugin and copies it into the game installation's `Mods` directory. Start the game and
the multiplier is available under `Mods > Fuel Generator Multiplier > Config`.

To uninstall, delete the `FuelGeneratorMultiplier` folder from that same `Mods` directory.

### Option B - Manual copy

If you already have a compiled build, copy the whole plugin folder including the compiled binary into the game's mod directory:

```
<Satisfactory>/FactoryGame/Mods/FuelGeneratorMultiplier/
```

It has to contain at least `FuelGeneratorMultiplier.uplugin` and the compiled
`Binaries/Win64/FuelGeneratorMultiplier.dll`. SML discovers the plugin automatically, no `Mods.json` entry is required.

Prefer Alpakit if you can: it also sets the per-target `GameFeature` field and the `Binaries` layout that SML expects.

### No effect in the editor

Hooks are **not** installed in editor builds (`WITH_EDITOR`), because applying function hooks at editor time is unreliable. There
will be no visible effect while playing inside the editor - test in the packaged game, which is what Alpakit produces.

### Multiplayer

The mod is server authoritative. Install it on the server (or on the listen server host) and set the multiplier there. Clients do
not need the mod, the `.uplugin` declares `"RequiredOnRemote": false` and the multiplied value arrives through normal property
replication.

## Packaging a release

To build a distributable archive:

1. Open `File > Alpakit Release`.
2. Under **Release Targets**, tick all three targets: `Windows`, `Windows Server` and `Linux Server`. A C++ mod has to be compiled
   separately for every target, and leaving one out makes the mod fail to load on that platform. For local singleplayer testing
   you can skip this and use the `Alpakit Dev` workflow from Option A instead.
3. Press the **`Alpakit!`** button next to your mod and wait for the packaging to finish. The first run compiles the additional
   targets and therefore takes noticeably longer.

Alpakit writes the result to:

```
<StarterProject>/Saved/ArchivedPlugins/FuelGeneratorMultiplier/FuelGeneratorMultiplier.zip
```

That multi-target zip is the file to upload to the [Satisfactory Mod Repository](https://ficsit.app). After the first release run,
a folder button appears in the mod's row in the Alpakit Release window that opens exactly that directory.

If you change any fields in the `.uplugin`, re-Alpak the mod before uploading, so the packaged descriptor contains your changes.

Do not add a `GameFeature` field to the `.uplugin` yourself - Alpakit sets it per target when it packages a release.

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

## License

Distributed under the terms of the **GNU General Public License v3.0**. The full text is in [LICENSE](LICENSE).

GPL-3.0 was chosen because the mod links against and is loaded at runtime by
[Satisfactory Mod Loader](https://github.com/satisfactorymodding/SatisfactoryModLoader), which is itself GPL-3.0.

## Status

The mod has not been compiled or run in the development environment it was written in, because no Satisfactory installation,
Unreal Engine toolchain or Starter Project was available there. All engine and SML APIs it uses were taken from the SML `3.12.0`
and Satisfactory 1.2 headers (`FGPowerInfoComponent.h`, `Patching/NativeHookManager.h`, `Configuration/ConfigManager.h`,
`Module/GameInstanceModule.h`), and the build configuration mirrors the official Alpakit C++ mod template, but a real compile and
an in-game test are still required before release.
