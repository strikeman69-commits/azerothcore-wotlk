# mod-instant-swing

AzerothCore module for WoTLK 3.3.5a that converts on-next-swing abilities to instant cast, matching the quality-of-life changes made to these abilities in modern World of Warcraft.

---

## Overview

In WoTLK, several melee abilities work by silently queuing on your next auto-attack rather than firing immediately. This means pressing the button does nothing visible until your swing timer fires, which feels sluggish and reactive compared to modern gameplay.

This module patches those spells at server startup so they fire immediately on press — consuming a GCD and dealing damage right away. Your auto-attack swing timer continues unaffected, so the spell fires **in addition to**, not instead of, your normal auto-attacks.

### Affected abilities

| Ability | Class |
|---|---|
| Heroic Strike (all ranks) | Warrior |
| Cleave (all ranks) | Warrior |
| Maul (all ranks) | Druid |
| Rune Strike | Death Knight |

### Before vs. After

| | Before (WoTLK default) | After (this module) |
|---|---|---|
| Activation | Queues on next auto-attack | Fires immediately |
| GCD | Consumed on queue | Consumed on press |
| Swing timer | Replaced by spell on next swing | Continues independently |
| Damage timing | On next auto | Immediately |

---

## Requirements

- [AzerothCore](https://www.azerothcore.org/) WoTLK 3.3.5a
- CMake + MSVC (Windows) or GCC/Clang (Linux)

No database changes or client files required.

---

## Installation

### Step 1 — Place the module

Clone or copy this folder into your AzerothCore modules directory:

```
azerothcore/
└── modules/
    └── mod-instant-swing/
```

### Step 2 — Build

Run `Rebuild-Server.bat` (Windows) or your equivalent build script. The module compiles as part of the standard modules build — no extra steps needed.

Look for this line during compilation to confirm it was picked up:

```
mod_instant_swing.cpp
```

### Step 3 — (Optional) Configure

The module works out of the box with no configuration required. If you want to disable it without removing the module, copy `conf/mod_instant_swing.conf.dist` to your server's `configs/modules/` folder and set:

```ini
InstantSwing.Enable = 0
```

If no config file is present, the module defaults to enabled.

---

## How It Works

The module registers a `GlobalScript::OnLoadSpellCustomAttr` hook that fires once per spell during server startup (inside `SpellMgr::LoadSpellInfoCustomAttributes`). For each affected spell ID, it clears the `SPELL_ATTR0_ON_NEXT_SWING_NO_DAMAGE` and `SPELL_ATTR0_ON_NEXT_SWING` flags from the spell's `Attributes` field in memory. This causes `Spell::GetCurrentContainer()` to route these spells through `CURRENT_GENERIC_SPELL` (instant cast) rather than `CURRENT_MELEE_SPELL` (on-next-swing queue).

No DBC files, no SQL, no client changes needed.

---

## License

MIT — see [LICENSE](LICENSE).
