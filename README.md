# NakedFollowerFix (SKSE Plugin)

An SKSE plugin for Skyrim Special Edition (1.6.1170) that automatically detects and clothes naked custom NPC followers.

## Features
- **Automatic Detection**: Scans nearby NPCs on cell load and 3D load.
- **Smart Filtering**: Excludes vanilla NPCs, dead NPCs, and non-human creatures (Skeletons, Draugr, etc.).
- **Force Equip**: Uses a "Sledgehammer" approach to bypass stubborn follower management scripts, ensuring NPCs wear their designated armor.
- **Standalone Build**: Compiled with static dependencies for easy installation.
- **Console Command**: Use `fixnaked` (or `fnk`) to manually trigger a scan.

## Requirements
- Skyrim Special Edition 1.6.1170
- SKSE64
- Address Library for SKSE Plugins

## Installation
1. Copy `NakedFollowerFix.dll` to `Data/SKSE/Plugins/`.
2. Play!

## Technical Details
Built using CommonLibSSE-NG. Uses `ActorEquipManager` to force items from the Iron Armor set onto NPCs identified as naked (missing Body slot 32 or wearing skin-only armor).
