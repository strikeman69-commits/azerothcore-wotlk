#include "ScriptMgr.h"
#include "SpellInfo.h"
#include "Config.h"
#include <unordered_set>

// Spells using SPELL_ATTR0_ON_NEXT_SWING_NO_DAMAGE (0x4) or SPELL_ATTR0_ON_NEXT_SWING (0x400).
// These queue silently on the next auto-attack rather than firing immediately.
// Clearing both flags routes them through CURRENT_GENERIC_SPELL so they fire instantly
// on use (consuming a GCD). The swing timer continues uninterrupted.
static const std::unordered_set<uint32> ON_NEXT_SWING_SPELLS =
{
    // Warrior — Heroic Strike
    78, 284, 285, 1608, 11564, 11565, 25286, 29707, 47449, 47450,
    // Warrior — Cleave
    845, 7369, 11608, 11609, 25231, 47519, 47520,
    // Druid — Maul
    6807, 6808, 6809, 8972, 9745, 9880, 9881, 26996, 48479, 48480,
    // Death Knight — Rune Strike
    56815,
    // Hunter — Raptor Strike
    2973, 14260, 14261, 14262, 14263, 14264, 14265, 14266, 48995, 48996,
};

class InstantSwingScript : public GlobalScript
{
public:
    InstantSwingScript() : GlobalScript("InstantSwingScript") {}

    void OnLoadSpellCustomAttr(SpellInfo* spell) override
    {
        if (!sConfigMgr->GetOption<bool>("InstantSwing.Enable", true))
            return;

        if (ON_NEXT_SWING_SPELLS.count(spell->Id))
            spell->Attributes &= ~(SPELL_ATTR0_ON_NEXT_SWING | SPELL_ATTR0_ON_NEXT_SWING_NO_DAMAGE);
    }
};

void AddSC_InstantSwing()
{
    new InstantSwingScript();
}
