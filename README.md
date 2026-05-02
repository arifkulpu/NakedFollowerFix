# NakedFollowerFix (SKSE Plugin)

An SKSE plugin for Skyrim (1.5.97 and 1.6.xxx) that automatically detects and clothes naked custom NPC followers.

## Features
- **Automatic Detection**: Scans nearby NPCs on cell load and 3D load.
- **Smart Filtering**: Excludes vanilla NPCs, dead NPCs, and non-human creatures (Skeletons, Draugr, etc.).
- **Mod Compatibility**: Automatically detects and ignores NPCs participating in SexLab or OStim animation scenes to prevent immersion-breaking re-clothing.
- **Force Equip**: Uses a "Sledgehammer" approach to bypass stubborn follower management scripts, ensuring NPCs wear their designated armor.
- **Standalone Build**: Compiled with static dependencies for easy installation.
- **Console Command**: Use `fixnaked` (or `fnk`) to manually trigger a scan.

## Requirements
- Skyrim Special Edition (1.5.97) or Anniversary Edition (1.6.xxx)
- SKSE64
- Address Library for SKSE Plugins

## Installation
1. Copy `NakedFollowerFix.dll` to `Data/SKSE/Plugins/`.
2. Play!

## Technical Details
Built using CommonLibSSE-NG. Uses `ActorEquipManager` to force items from the Iron Armor set onto NPCs identified as naked (missing Body slot 32 or wearing skin-only armor).

## License

Copyright (c) 2026 Arif KULPU. All Rights Reserved.

Unauthorized copying, modification, distribution, or use of this software
and associated documentation files, in any medium, is strictly prohibited
without the express prior written permission of the copyright holder.

This software is provided "as is", without warranty of any kind.
The copyright holder shall not be liable for any claim, damages, or other
liability arising from the use of this software.

---

Telif Hakkı (c) 2026 Arif KULPU. Tüm Hakları Saklıdır.

Bu yazılımın ve ilgili belgelerinin herhangi bir ortamda izinsiz olarak
kopyalanması, değiştirilmesi, dağıtılması veya kullanılması, telif hakkı
sahibinin açık ve önceden yazılı izni olmaksızın kesinlikle yasaktır.

Bu yazılım "olduğu gibi" sunulmaktadır; herhangi bir garanti verilmemektedir.
Telif hakkı sahibi, bu yazılımın kullanımından doğan hiçbir talep, zarar
veya yükümlülükten sorumlu tutulamaz.
