# Native Animation Framework

A multi-character animation framework for Fallout 4.

## Supported game versions

The plugin is a single DLL that picks its addresses at runtime from the F4SE Address Library (`REL::VariantID`/`REL::Offset`):

| Fallout 4 | Runtime |
|---|---|
| 1.10.163 | OG |
| 1.10.984 | NG |
| 1.11.240 | AE |

AE has been tested in game (player and NPC scenes, saving and reloading an NPC-only scene, face creator, HUD, fly cam, LookAt Cam); OG has been tested for scenes, the fly cam and LookAt Cam; NG has not been tested yet. Camera and input restoration after stopping a player scene has also been tested. Saving is blocked by the game while the player is in a scene. Per-runtime differences are handled in the plugin, not in CommonLibF4.

LooksMenu (F4EE) morph support works with every original F4EE build: 1.6.18, 1.6.20, 1.7.0.1, 1.7.0.2, 1.7.0.3 and 1.7.0.4. The build is identified from `f4ee.dll`'s PE header.

## Features

- Custom face animation system with an in-game creator.
- Custom full-body animation system with an in-game creator (aka the Animation Studio).
- Arbitrary inverse kinematic chains with support for skipped-bones, integrated into the full-body animation creator.
- A configurable multi-character scene system for playing synchronized animations with precisely positioned actors, compatible with XMLs made for AAF.
- AI package override system that can apply any arbitrary AI package to an actor without being overriden by quests/etc.
- Extensive papyrus API functions for all aforementioned features.

## Technical Capabilities

- Cubic spline-based keyframe sampling for natural movement, with either simple float values or quaternions.
- Precise timing thread for delayed tasks, which can be serialized to the F4SE cosave to maintain state between game sessions.

## Build Requirements

- [Visual Studio 2022 with C++ Desktop Development Component](https://visualstudio.microsoft.com/vs/)
- [xmake](https://xmake.io) (3.x). Dependencies (cereal, nlohmann_json, zstr, libzippp, FP16, pugixml) are fetched by xmake.
- [Python 3](https://www.python.org/downloads/) added to PATH environment variable (used to generate the `ik` library headers).
- Git submodules: [CommonLibF4](https://github.com/frakkin64/commonlibf4) (`f4se-plugin/lib/commonlibf4`, multi-runtime fork) and [ik](https://github.com/Deweh/ik) (`f4se-plugin/extern/ik`).

## Building

```
git clone --recurse-submodules <repository url>
cd <repository folder>/f4se-plugin
xmake f -y -p windows -a x64 -m releasedbg
xmake build NAF
```

The DLL is written to `f4se-plugin/build/windows/x64/releasedbg/NAF.dll`. Copy it to `Data/F4SE/Plugins`; the Scaleform menus (`NAFMenu.swf`, `NAFHUDMenu.swf`, `NAFStudioMenu.swf`) built from `as3` go in `Data/Interface`.
