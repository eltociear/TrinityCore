/*
 * This file is part of the TrinityCore Project. See AUTHORS file for Copyright information
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

/*
 * Scripts for spells with SPELLFAMILY_PRIEST and SPELLFAMILY_GENERIC spells used by priest players.
 * Ordered alphabetically using scriptname.
 * Scriptnames of files in this file should be prefixed with "spell_pri_".
 */

#include "ScriptMgr.h"
#include "AreaTriggerAI.h"
#include "Containers.h"
#include "G3DPosition.hpp"
#include "GridNotifiers.h"
#include "Log.h"
#include "MoveSplineInitArgs.h"
#include "ObjectAccessor.h"
#include "PathGenerator.h"
#include "Player.h"
#include "SpellAuraEffects.h"
#include "SpellHistory.h"
#include "SpellMgr.h"
#include "SpellScript.h"
#include "TaskScheduler.h"

enum PriestSpells
{
    SPELL_PRIEST_ANGELIC_FEATHER_AREATRIGGER        = 158624,
    SPELL_PRIEST_ANGELIC_FEATHER_AURA               = 121557,
    SPELL_PRIEST_ARMOR_OF_FAITH                     = 28810,
    SPELL_PRIEST_ATONEMENT                          = 81749,
    SPELL_PRIEST_ATONEMENT_HEAL                     = 81751,
    SPELL_PRIEST_ATONEMENT_TRIGGERED                = 194384,
    SPELL_PRIEST_ATONEMENT_TRIGGERED_TRINITY        = 214206,
    SPELL_PRIEST_BLESSED_HEALING                    = 70772,
    SPELL_PRIEST_BODY_AND_SOUL                      = 64129,
    SPELL_PRIEST_BODY_AND_SOUL_SPEED                = 65081,
    SPELL_PRIEST_DARK_REPRIMAND                     = 400169,
    SPELL_PRIEST_DARK_REPRIMAND_CHANNEL_DAMAGE      = 373129,
    SPELL_PRIEST_DARK_REPRIMAND_CHANNEL_HEALING     = 400171,
    SPELL_PRIEST_DARK_REPRIMAND_DAMAGE              = 373130,
    SPELL_PRIEST_DARK_REPRIMAND_HEALING             = 400187,
    SPELL_PRIEST_DIVINE_BLESSING                    = 40440,
    SPELL_PRIEST_DIVINE_STAR_HOLY                   = 110744,
    SPELL_PRIEST_DIVINE_STAR_SHADOW                 = 122121,
    SPELL_PRIEST_DIVINE_STAR_HOLY_DAMAGE            = 122128,
    SPELL_PRIEST_DIVINE_STAR_HOLY_HEAL              = 110745,
    SPELL_PRIEST_DIVINE_STAR_SHADOW_DAMAGE          = 390845,
    SPELL_PRIEST_DIVINE_STAR_SHADOW_HEAL            = 390981,
    SPELL_PRIEST_DIVINE_WRATH                       = 40441,
    SPELL_PRIEST_EMPOWERED_RENEW_HEAL               = 391359,
    SPELL_PRIEST_FLASH_HEAL                         = 2061,
    SPELL_PRIEST_GUARDIAN_SPIRIT_HEAL               = 48153,
    SPELL_PRIEST_HALO_HOLY                          = 120517,
    SPELL_PRIEST_HALO_SHADOW                        = 120644,
    SPELL_PRIEST_HALO_HOLY_DAMAGE                   = 120696,
    SPELL_PRIEST_HALO_HOLY_HEAL                     = 120692,
    SPELL_PRIEST_HALO_SHADOW_DAMAGE                 = 390964,
    SPELL_PRIEST_HALO_SHADOW_HEAL                   = 390971,
    SPELL_PRIEST_HEAL                               = 2060,
    SPELL_PRIEST_HOLY_MENDING_HEAL                  = 391156,
    SPELL_PRIEST_HOLY_WORD_CHASTISE                 = 88625,
    SPELL_PRIEST_HOLY_WORD_SANCTIFY                 = 34861,
    SPELL_PRIEST_HOLY_WORD_SERENITY                 = 2050,
    SPELL_PRIEST_ITEM_EFFICIENCY                    = 37595,
    SPELL_PRIEST_LEAP_OF_FAITH_EFFECT               = 92832,
    SPELL_PRIEST_LEVITATE_EFFECT                    = 111759,
    SPELL_PRIEST_MASOCHISM_TALENT                   = 193063,
    SPELL_PRIEST_MASOCHISM_PERIODIC_HEAL            = 193065,
    SPELL_PRIEST_MASTERY_GRACE                      = 271534,
    SPELL_PRIEST_MIND_BOMB_STUN                     = 226943,
    SPELL_PRIEST_ORACULAR_HEAL                      = 26170,
    SPELL_PRIEST_PENANCE                            = 47540,
    SPELL_PRIEST_PENANCE_CHANNEL_DAMAGE             = 47758,
    SPELL_PRIEST_PENANCE_CHANNEL_HEALING            = 47757,
    SPELL_PRIEST_PENANCE_DAMAGE                     = 47666,
    SPELL_PRIEST_PENANCE_HEALING                    = 47750,
    SPELL_PRIEST_POWER_OF_THE_DARK_SIDE             = 198069,
    SPELL_PRIEST_POWER_OF_THE_DARK_SIDE_TINT        = 225795,
    SPELL_PRIEST_POWER_WORD_SHIELD                  = 17,
    SPELL_PRIEST_POWER_WORD_SOLACE_ENERGIZE         = 129253,
    SPELL_PRIEST_PRAYER_OF_HEALING                  = 596,
    SPELL_PRIEST_PURGE_THE_WICKED                   = 204197,
    SPELL_PRIEST_PURGE_THE_WICKED_DUMMY             = 204215,
    SPELL_PRIEST_PURGE_THE_WICKED_PERIODIC          = 204213,
    SPELL_PRIEST_RAPTURE                            = 47536,
    SPELL_PRIEST_RENEW                              = 139,
    SPELL_PRIEST_RENEWED_HOPE                       = 197469,
    SPELL_PRIEST_RENEWED_HOPE_EFFECT                = 197470,
    SPELL_PRIEST_REVEL_IN_PURITY                    = 373003,
    SPELL_PRIEST_SAY_YOUR_PRAYERS                   = 391186,
    SPELL_PRIEST_SHADOW_MEND_DAMAGE                 = 186439,
    SPELL_PRIEST_SHADOW_MEND_PERIODIC_DUMMY         = 187464,
    SPELL_PRIEST_SHADOW_WORD_PAIN                   = 589,
    SPELL_PRIEST_SHIELD_DISCIPLINE_ENERGIZE         = 47755,
    SPELL_PRIEST_SHIELD_DISCIPLINE_PASSIVE          = 197045,
    SPELL_PRIEST_SINS_OF_THE_MANY                   = 280398,
    SPELL_PRIEST_SMITE                              = 585,
    SPELL_PRIEST_SPIRIT_OF_REDEMPTION               = 27827,
    SPELL_PRIEST_STRENGTH_OF_SOUL                   = 197535,
    SPELL_PRIEST_STRENGTH_OF_SOUL_EFFECT            = 197548,
    SPELL_PRIEST_THE_PENITENT_AURA                  = 200347,
    SPELL_PRIEST_TRINITY                            = 214205,
    SPELL_PRIEST_VAMPIRIC_EMBRACE_HEAL              = 15290,
    SPELL_PRIEST_VAMPIRIC_TOUCH_DISPEL              = 64085,
    SPELL_PRIEST_VOID_SHIELD                        = 199144,
    SPELL_PRIEST_VOID_SHIELD_EFFECT                 = 199145,
    SPELL_PRIEST_PRAYER_OF_MENDING_AURA             = 41635,
    SPELL_PRIEST_PRAYER_OF_MENDING_HEAL             = 33110,
    SPELL_PRIEST_PRAYER_OF_MENDING_JUMP             = 155793,
    SPELL_PRIEST_WEAKENED_SOUL                      = 6788
};

enum MiscSpells
{
    SPELL_GEN_REPLENISHMENT                         = 57669
};

class RaidCheck
{
    public:
        explicit RaidCheck(Unit const* caster) : _caster(caster) { }

        bool operator()(WorldObject* obj) const
        {
            if (Unit* target = obj->ToUnit())
                return !_caster->IsInRaidWith(target);

            return true;
        }

    private:
        Unit const* _caster;
};

// 121536 - Angelic Feather talent
class spell_pri_angelic_feather_trigger : public SpellScript
{
    PrepareSpellScript(spell_pri_angelic_feather_trigger);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_PRIEST_ANGELIC_FEATHER_AREATRIGGER });
    }

    void HandleEffectDummy(SpellEffIndex /*effIndex*/)
    {
        Position destPos = GetHitDest()->GetPosition();
        float radius = GetEffectInfo().CalcRadius();

        // Caster is prioritary
        if (GetCaster()->IsWithinDist2d(&destPos, radius))
        {
            GetCaster()->CastSpell(GetCaster(), SPELL_PRIEST_ANGELIC_FEATHER_AURA, true);
        }
        else
        {
            CastSpellExtraArgs args;
            args.TriggerFlags = TRIGGERED_FULL_MASK;
            args.CastDifficulty = GetCastDifficulty();
            GetCaster()->CastSpell(destPos, SPELL_PRIEST_ANGELIC_FEATHER_AREATRIGGER, args);
        }
    }

    void Register() override
    {
        OnEffectHit += SpellEffectFn(spell_pri_angelic_feather_trigger::HandleEffectDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
    }
};

// Angelic Feather areatrigger - created by SPELL_PRIEST_ANGELIC_FEATHER_AREATRIGGER
struct areatrigger_pri_angelic_feather : AreaTriggerAI
{
    areatrigger_pri_angelic_feather(AreaTrigger* areatrigger) : AreaTriggerAI(areatrigger) { }

    // Called when the AreaTrigger has just been initialized, just before added to map
    void OnInitialize() override
    {
        if (Unit* caster = at->GetCaster())
        {
            std::vector<AreaTrigger*> areaTriggers = caster->GetAreaTriggers(SPELL_PRIEST_ANGELIC_FEATHER_AREATRIGGER);

            if (areaTriggers.size() >= 3)
                areaTriggers.front()->SetDuration(0);
        }
    }

    void OnUnitEnter(Unit* unit) override
    {
        if (Unit* caster = at->GetCaster())
        {
            if (caster->IsFriendlyTo(unit))
            {
                // If target already has aura, increase duration to max 130% of initial duration
                caster->CastSpell(unit, SPELL_PRIEST_ANGELIC_FEATHER_AURA, true);
                at->SetDuration(0);
            }
        }
    }
};

// 26169 - Oracle Healing Bonus
class spell_pri_aq_3p_bonus : public AuraScript
{
    PrepareAuraScript(spell_pri_aq_3p_bonus);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_PRIEST_ORACULAR_HEAL });
    }

    void HandleProc(AuraEffect* aurEff, ProcEventInfo& eventInfo)
    {
        PreventDefaultAction();
        Unit* caster = eventInfo.GetActor();
        if (caster == eventInfo.GetProcTarget())
            return;

        HealInfo* healInfo = eventInfo.GetHealInfo();
        if (!healInfo || !healInfo->GetHeal())
            return;

        CastSpellExtraArgs args(aurEff);
        args.AddSpellBP0(CalculatePct(static_cast<int32>(healInfo->GetHeal()), 10));
        caster->CastSpell(caster, SPELL_PRIEST_ORACULAR_HEAL, args);
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_pri_aq_3p_bonus::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

// 81749 - Atonement
class spell_pri_atonement : public AuraScript
{
    PrepareAuraScript(spell_pri_atonement);

    bool Validate(SpellInfo const* spellInfo) override
    {
        return ValidateSpellInfo({ SPELL_PRIEST_ATONEMENT_HEAL, SPELL_PRIEST_SINS_OF_THE_MANY })
            && ValidateSpellEffect({ { spellInfo->Id, EFFECT_1 }, { SPELL_PRIEST_SINS_OF_THE_MANY, EFFECT_2 } });
    }

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetDamageInfo() != nullptr;
    }

    void HandleProc(AuraEffect* aurEff, ProcEventInfo& eventInfo)
    {
        DamageInfo* damageInfo = eventInfo.GetDamageInfo();
        CastSpellExtraArgs args(aurEff);
        args.AddSpellMod(SPELLVALUE_BASE_POINT0, CalculatePct(damageInfo->GetDamage(), aurEff->GetAmount()));
        _appliedAtonements.erase(std::remove_if(_appliedAtonements.begin(), _appliedAtonements.end(), [this, &args](ObjectGuid const& targetGuid)
        {
            if (Unit* target = ObjectAccessor::GetUnit(*GetTarget(), targetGuid))
            {
                if (target->GetExactDist(GetTarget()) < GetEffectInfo(EFFECT_1).CalcValue())
                    GetTarget()->CastSpell(target, SPELL_PRIEST_ATONEMENT_HEAL, args);

                return false;
            }
            return true;
        }), _appliedAtonements.end());
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(spell_pri_atonement::CheckProc);
        OnEffectProc += AuraEffectProcFn(spell_pri_atonement::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }

    std::vector<ObjectGuid> _appliedAtonements;

public:
    void AddAtonementTarget(ObjectGuid const& target)
    {
        _appliedAtonements.push_back(target);

        UpdateSinsOfTheManyValue();
    }

    void RemoveAtonementTarget(ObjectGuid const& target)
    {
        _appliedAtonements.erase(std::remove(_appliedAtonements.begin(), _appliedAtonements.end(), target), _appliedAtonements.end());

        UpdateSinsOfTheManyValue();
    }

    void UpdateSinsOfTheManyValue()
    {
        // Note: the damage dimish starts at the 6th application as of 10.0.5.
        constexpr std::array<float, 20> damageByStack = { 40.0f, 40.0f, 40.0f, 40.0f, 40.0f, 35.0f, 30.0f, 25.0f, 20.0f, 15.0f, 11.0f, 8.0f, 5.0f, 4.0f, 3.0f, 2.5f, 2.0f, 1.5f, 1.25f, 1.0f };

        for (SpellEffIndex effectIndex : { EFFECT_0, EFFECT_1, EFFECT_2 })
            if (AuraEffect* sinOfTheMany = GetUnitOwner()->GetAuraEffect(SPELL_PRIEST_SINS_OF_THE_MANY, effectIndex))
                sinOfTheMany->ChangeAmount(damageByStack[std::min(_appliedAtonements.size(), damageByStack.size() - 1)]);
    }
};

// 194384, 214206 - Atonement
class spell_pri_atonement_triggered : public AuraScript
{
    PrepareAuraScript(spell_pri_atonement_triggered);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_PRIEST_ATONEMENT });
    }

    void HandleOnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        RegisterHelper<&spell_pri_atonement::AddAtonementTarget>();
    }

    void HandleOnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        RegisterHelper<&spell_pri_atonement::RemoveAtonementTarget>();
    }

    template<void(spell_pri_atonement::*func)(ObjectGuid const&)>
    void RegisterHelper()
    {
        if (Unit* caster = GetCaster())
            if (Aura* atonement = caster->GetAura(SPELL_PRIEST_ATONEMENT))
                if (spell_pri_atonement* script = atonement->GetScript<spell_pri_atonement>())
                    (script->*func)(GetTarget()->GetGUID());
    }

    void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(spell_pri_atonement_triggered::HandleOnApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        OnEffectRemove += AuraEffectRemoveFn(spell_pri_atonement_triggered::HandleOnRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
    }
};

// 204883 - Circle of Healing
class spell_pri_circle_of_healing : public SpellScript
{
    PrepareSpellScript(spell_pri_circle_of_healing);

    bool Validate(SpellInfo const* spellInfo) override
    {
        return ValidateSpellEffect({ { spellInfo->Id, EFFECT_1 } });
    }

    void FilterTargets(std::list<WorldObject*>& targets)
    {
        // Note: we must remove one since target is always chosen.
        uint32 const maxTargets = uint32(GetSpellInfo()->GetEffect(EFFECT_1).CalcValue(GetCaster()) - 1);

        Trinity::SelectRandomInjuredTargets(targets, maxTargets, true);

        if (Unit* explicitTarget = GetExplTargetUnit())
            targets.push_front(explicitTarget);
    }

    void Register() override
    {
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_pri_circle_of_healing::FilterTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ALLY);
    }
};

// 64844 - Divine Hymn
class spell_pri_divine_hymn : public SpellScript
{
    PrepareSpellScript(spell_pri_divine_hymn);

    void FilterTargets(std::list<WorldObject*>& targets)
    {
        targets.remove_if(RaidCheck(GetCaster()));

        uint32 const maxTargets = 3;

        if (targets.size() > maxTargets)
        {
            targets.sort(Trinity::HealthPctOrderPred());
            targets.resize(maxTargets);
        }
    }

    void Register() override
    {
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_pri_divine_hymn::FilterTargets, EFFECT_ALL, TARGET_UNIT_SRC_AREA_ALLY);
    }
};

// 122121 - Divine Star (Shadow)
class spell_pri_divine_star_shadow : public SpellScript
{
    PrepareSpellScript(spell_pri_divine_star_shadow);

    void HandleHitTarget(SpellEffIndex effIndex)
    {
        Unit* caster = GetCaster();

        if (caster->GetPowerType() != GetEffectInfo().MiscValue)
            PreventHitDefaultEffect(effIndex);
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_pri_divine_star_shadow::HandleHitTarget, EFFECT_2, SPELL_EFFECT_ENERGIZE);
    }
};

// 110744 - Divine Star (Holy)
// 122121 - Divine Star (Shadow)
struct areatrigger_pri_divine_star : AreaTriggerAI
{
    areatrigger_pri_divine_star(AreaTrigger* areatrigger) : AreaTriggerAI(areatrigger), _maxTravelDistance(0.0f) { }

    void OnInitialize() override
    {
        SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(at->GetSpellId(), DIFFICULTY_NONE);
        if (!spellInfo)
            return;

        if (spellInfo->GetEffects().size() <= EFFECT_1)
            return;

        Unit* caster = at->GetCaster();
        if (!caster)
            return;

        _casterCurrentPosition = caster->GetPosition();

        // Note: max. distance at which the Divine Star can travel to is EFFECT_1's BasePoints yards.
        _maxTravelDistance = float(spellInfo->GetEffect(EFFECT_1).CalcValue(caster));

        Position destPos = _casterCurrentPosition;
        at->MovePositionToFirstCollision(destPos, _maxTravelDistance, 0.0f);

        PathGenerator firstPath(at);
        firstPath.CalculatePath(destPos.GetPositionX(), destPos.GetPositionY(), destPos.GetPositionZ(), false);

        G3D::Vector3 const& endPoint = firstPath.GetPath().back();

        // Note: it takes 1000ms to reach EFFECT_1's BasePoints yards, so it takes (1000 / EFFECT_1's BasePoints)ms to run 1 yard.
        at->InitSplines(firstPath.GetPath(), at->GetDistance(endPoint.x, endPoint.y, endPoint.z) * float(1000 / _maxTravelDistance));
    }

    void OnUpdate(uint32 diff) override
    {
        _scheduler.Update(diff);
    }

    void OnUnitEnter(Unit* unit) override
    {
        HandleUnitEnterExit(unit);
    }

    void OnUnitExit(Unit* unit) override
    {
        // Note: this ensures any unit receives a second hit if they happen to be inside the AT when Divine Star starts its return path.
        HandleUnitEnterExit(unit);
    }

    void HandleUnitEnterExit(Unit* unit)
    {
        Unit* caster = at->GetCaster();
        if (!caster)
            return;

        if (std::find(_affectedUnits.begin(), _affectedUnits.end(), unit->GetGUID()) != _affectedUnits.end())
            return;

        constexpr TriggerCastFlags TriggerFlags = TriggerCastFlags(TRIGGERED_IGNORE_GCD | TRIGGERED_IGNORE_CAST_IN_PROGRESS);

        if (caster->IsValidAttackTarget(unit))
            caster->CastSpell(unit, at->GetSpellId() == SPELL_PRIEST_DIVINE_STAR_SHADOW ? SPELL_PRIEST_DIVINE_STAR_SHADOW_DAMAGE : SPELL_PRIEST_DIVINE_STAR_HOLY_DAMAGE,
                TriggerFlags);
        else if (caster->IsValidAssistTarget(unit))
            caster->CastSpell(unit, at->GetSpellId() == SPELL_PRIEST_DIVINE_STAR_SHADOW ? SPELL_PRIEST_DIVINE_STAR_SHADOW_HEAL : SPELL_PRIEST_DIVINE_STAR_HOLY_HEAL,
                TriggerFlags);

        _affectedUnits.push_back(unit->GetGUID());
    }

    void OnDestinationReached() override
    {
        Unit* caster = at->GetCaster();
        if (!caster)
            return;

        if (at->GetDistance(_casterCurrentPosition) > 0.05f)
        {
            _affectedUnits.clear();

            ReturnToCaster();
        }
        else
            at->Remove();
    }

    void ReturnToCaster()
    {
        _scheduler.Schedule(0ms, [this](TaskContext task)
        {
            Unit* caster = at->GetCaster();
            if (!caster)
                return;

            _casterCurrentPosition = caster->GetPosition();

            Movement::PointsArray returnSplinePoints;

            returnSplinePoints.push_back(PositionToVector3(at));
            returnSplinePoints.push_back(PositionToVector3(at));
            returnSplinePoints.push_back(PositionToVector3(caster));
            returnSplinePoints.push_back(PositionToVector3(caster));

            at->InitSplines(returnSplinePoints, uint32(at->GetDistance(caster) / _maxTravelDistance * 1000));

            task.Repeat(250ms);
        });
    }

private:
    TaskScheduler _scheduler;
    Position _casterCurrentPosition;
    std::vector<ObjectGuid> _affectedUnits;
    float _maxTravelDistance;
};

// 391339 - Empowered Renew
class spell_pri_empowered_renew : public AuraScript
{
    PrepareAuraScript(spell_pri_empowered_renew);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_PRIEST_RENEW, SPELL_PRIEST_EMPOWERED_RENEW_HEAL })
            && ValidateSpellEffect({ { SPELL_PRIEST_RENEW, EFFECT_0 } })
            && sSpellMgr->AssertSpellInfo(SPELL_PRIEST_RENEW, DIFFICULTY_NONE)->GetEffect(EFFECT_0).IsAura(SPELL_AURA_PERIODIC_HEAL);
    }

    void HandleProc(AuraEffect* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = eventInfo.GetActor();
        Unit* target = eventInfo.GetProcTarget();

        SpellInfo const* renewSpellInfo = sSpellMgr->AssertSpellInfo(SPELL_PRIEST_RENEW, GetCastDifficulty());
        SpellEffectInfo const& renewEffect = renewSpellInfo->GetEffect(EFFECT_0);
        int32 estimatedTotalHeal = AuraEffect::CalculateEstimatedfTotalPeriodicAmount(caster, target, renewSpellInfo, renewEffect, renewEffect.CalcValue(caster), 1);
        int32 healAmount = CalculatePct(estimatedTotalHeal, aurEff->GetAmount());

        caster->CastSpell(target, SPELL_PRIEST_EMPOWERED_RENEW_HEAL, CastSpellExtraArgs(aurEff).AddSpellBP0(healAmount));
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_pri_empowered_renew::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

// 47788 - Guardian Spirit
class spell_pri_guardian_spirit : public AuraScript
{
    PrepareAuraScript(spell_pri_guardian_spirit);

    uint32 healPct = 0;

    bool Validate(SpellInfo const* spellInfo) override
    {
        return ValidateSpellInfo({ SPELL_PRIEST_GUARDIAN_SPIRIT_HEAL }) && ValidateSpellEffect({ { spellInfo->Id, EFFECT_1 } });
    }

    bool Load() override
    {
        healPct = GetEffectInfo(EFFECT_1).CalcValue();
        return true;
    }

    void CalculateAmount(AuraEffect const* /*aurEff*/, int32 & amount, bool & /*canBeRecalculated*/)
    {
        // Set absorbtion amount to unlimited
        amount = -1;
    }

    void Absorb(AuraEffect* /*aurEff*/, DamageInfo & dmgInfo, uint32 & absorbAmount)
    {
        Unit* target = GetTarget();
        if (dmgInfo.GetDamage() < target->GetHealth())
            return;

        int32 healAmount = int32(target->CountPctFromMaxHealth(healPct));
        // remove the aura now, we don't want 40% healing bonus
        Remove(AURA_REMOVE_BY_ENEMY_SPELL);
        CastSpellExtraArgs args(TRIGGERED_FULL_MASK);
        args.AddSpellBP0(healAmount);
        target->CastSpell(target, SPELL_PRIEST_GUARDIAN_SPIRIT_HEAL, args);
        absorbAmount = dmgInfo.GetDamage();
    }

    void Register() override
    {
        DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_pri_guardian_spirit::CalculateAmount, EFFECT_1, SPELL_AURA_SCHOOL_ABSORB);
        OnEffectAbsorb += AuraEffectAbsorbFn(spell_pri_guardian_spirit::Absorb, EFFECT_1);
    }
};

// 120644 - Halo (Shadow)
class spell_pri_halo_shadow : public SpellScript
{
    PrepareSpellScript(spell_pri_halo_shadow);

    void HandleHitTarget(SpellEffIndex effIndex)
    {
        Unit* caster = GetCaster();

        if (caster->GetPowerType() != GetEffectInfo().MiscValue)
            PreventHitDefaultEffect(effIndex);
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_pri_halo_shadow::HandleHitTarget, EFFECT_1, SPELL_EFFECT_ENERGIZE);
    }
};

// 120517 - Halo (Holy)
// 120644 - Halo (Shadow)
struct areatrigger_pri_halo : AreaTriggerAI
{
    areatrigger_pri_halo(AreaTrigger* areatrigger) : AreaTriggerAI(areatrigger) {}

    void OnUnitEnter(Unit* unit) override
    {
        if (Unit* caster = at->GetCaster())
        {
            if (caster->IsValidAttackTarget(unit))
                caster->CastSpell(unit, at->GetSpellId() == SPELL_PRIEST_HALO_SHADOW ? SPELL_PRIEST_HALO_SHADOW_DAMAGE : SPELL_PRIEST_HALO_HOLY_DAMAGE,
                    CastSpellExtraArgs(TriggerCastFlags(TRIGGERED_IGNORE_GCD | TRIGGERED_IGNORE_CAST_IN_PROGRESS)));
            else if (caster->IsValidAssistTarget(unit))
                caster->CastSpell(unit, at->GetSpellId() == SPELL_PRIEST_HALO_SHADOW ? SPELL_PRIEST_HALO_SHADOW_HEAL : SPELL_PRIEST_HALO_HOLY_HEAL,
                    CastSpellExtraArgs(TriggerCastFlags(TRIGGERED_IGNORE_GCD | TRIGGERED_IGNORE_CAST_IN_PROGRESS)));
        }
    }
};

// 391154 - Holy Mending
class spell_pri_holy_mending : public AuraScript
{
    PrepareAuraScript(spell_pri_holy_mending);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_PRIEST_RENEW, SPELL_PRIEST_HOLY_MENDING_HEAL });
    }

    bool CheckProc(AuraEffect const* /*aurEff*/, ProcEventInfo& procInfo)
    {
        return procInfo.GetProcTarget()->HasAura(SPELL_PRIEST_RENEW, procInfo.GetActor()->GetGUID());
    }

    void HandleProc(AuraEffect* aurEff, ProcEventInfo& eventInfo)
    {
        eventInfo.GetActor()->CastSpell(eventInfo.GetProcTarget(), SPELL_PRIEST_HOLY_MENDING_HEAL, CastSpellExtraArgs(aurEff));
    }

    void Register() override
    {
        DoCheckEffectProc += AuraCheckEffectProcFn(spell_pri_holy_mending::CheckProc, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL);
        OnEffectProc += AuraEffectProcFn(spell_pri_holy_mending::HandleProc, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL);
    }
};

// 63733 - Holy Words
class spell_pri_holy_words : public AuraScript
{
    PrepareAuraScript(spell_pri_holy_words);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo(
        {
            SPELL_PRIEST_HEAL,
            SPELL_PRIEST_FLASH_HEAL,
            SPELL_PRIEST_PRAYER_OF_HEALING,
            SPELL_PRIEST_RENEW,
            SPELL_PRIEST_SMITE,
            SPELL_PRIEST_HOLY_WORD_CHASTISE,
            SPELL_PRIEST_HOLY_WORD_SANCTIFY,
            SPELL_PRIEST_HOLY_WORD_SERENITY
        }) && ValidateSpellEffect(
        {
            { SPELL_PRIEST_HOLY_WORD_SERENITY, EFFECT_1 },
            { SPELL_PRIEST_HOLY_WORD_SANCTIFY, EFFECT_3 },
            { SPELL_PRIEST_HOLY_WORD_CHASTISE, EFFECT_1 }
        });
    }

    void HandleProc(AuraEffect* /*aurEff*/, ProcEventInfo& eventInfo)
    {
        SpellInfo const* spellInfo = eventInfo.GetSpellInfo();
        if (!spellInfo)
            return;

        uint32 targetSpellId;
        SpellEffIndex cdReductionEffIndex;
        switch (spellInfo->Id)
        {
            case SPELL_PRIEST_HEAL:
            case SPELL_PRIEST_FLASH_HEAL: // reduce Holy Word: Serenity cd by 6 seconds
                targetSpellId = SPELL_PRIEST_HOLY_WORD_SERENITY;
                cdReductionEffIndex = EFFECT_1;
                // cdReduction = sSpellMgr->GetSpellInfo(SPELL_PRIEST_HOLY_WORD_SERENITY, GetCastDifficulty())->GetEffect(EFFECT_1)->CalcValue(player);
                break;
            case SPELL_PRIEST_PRAYER_OF_HEALING: // reduce Holy Word: Sanctify cd by 6 seconds
                targetSpellId = SPELL_PRIEST_HOLY_WORD_SANCTIFY;
                cdReductionEffIndex = EFFECT_2;
                break;
            case SPELL_PRIEST_RENEW: // reuce Holy Word: Sanctify cd by 2 seconds
                targetSpellId = SPELL_PRIEST_HOLY_WORD_SANCTIFY;
                cdReductionEffIndex = EFFECT_3;
                break;
            case SPELL_PRIEST_SMITE: // reduce Holy Word: Chastise cd by 4 seconds
                targetSpellId = SPELL_PRIEST_HOLY_WORD_CHASTISE;
                cdReductionEffIndex = EFFECT_1;
                break;
            default:
                TC_LOG_WARN("spells.priest", "HolyWords aura has been proced by an unknown spell: {}", GetSpellInfo()->Id);
                return;
        }

        SpellInfo const* targetSpellInfo = sSpellMgr->AssertSpellInfo(targetSpellId, GetCastDifficulty());
        int32 cdReduction = targetSpellInfo->GetEffect(cdReductionEffIndex).CalcValue(GetTarget());
        GetTarget()->GetSpellHistory()->ModifyCooldown(targetSpellInfo, Seconds(-cdReduction), true);
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_pri_holy_words::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

// 40438 - Priest Tier 6 Trinket
class spell_pri_item_t6_trinket : public AuraScript
{
    PrepareAuraScript(spell_pri_item_t6_trinket);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_PRIEST_DIVINE_BLESSING, SPELL_PRIEST_DIVINE_WRATH });
    }

    void HandleProc(AuraEffect* /*aurEff*/, ProcEventInfo& eventInfo)
    {
        PreventDefaultAction();
        Unit* caster = eventInfo.GetActor();
        if (eventInfo.GetSpellTypeMask() & PROC_SPELL_TYPE_HEAL)
            caster->CastSpell(nullptr, SPELL_PRIEST_DIVINE_BLESSING, true);

        if (eventInfo.GetSpellTypeMask() & PROC_SPELL_TYPE_DAMAGE)
            caster->CastSpell(nullptr, SPELL_PRIEST_DIVINE_WRATH, true);
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_pri_item_t6_trinket::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

// 92833 - Leap of Faith
class spell_pri_leap_of_faith_effect_trigger : public SpellScript
{
    PrepareSpellScript(spell_pri_leap_of_faith_effect_trigger);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_PRIEST_LEAP_OF_FAITH_EFFECT });
    }

    void HandleEffectDummy(SpellEffIndex /*effIndex*/)
    {
        Position destPos = GetHitDest()->GetPosition();

        SpellCastTargets targets;
        targets.SetDst(destPos);
        targets.SetUnitTarget(GetCaster());
        GetHitUnit()->CastSpell(std::move(targets), GetEffectValue(), GetCastDifficulty());
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_pri_leap_of_faith_effect_trigger::HandleEffectDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
    }
};

// 1706 - Levitate
class spell_pri_levitate : public SpellScript
{
    PrepareSpellScript(spell_pri_levitate);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_PRIEST_LEVITATE_EFFECT });
    }

    void HandleDummy(SpellEffIndex /*effIndex*/)
    {
        GetCaster()->CastSpell(GetHitUnit(), SPELL_PRIEST_LEVITATE_EFFECT, true);
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_pri_levitate::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
    }
};

// 205369 - Mind Bomb
class spell_pri_mind_bomb : public AuraScript
{
    PrepareAuraScript(spell_pri_mind_bomb);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_PRIEST_MIND_BOMB_STUN });
    }

    void RemoveEffect(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        if (GetTargetApplication()->GetRemoveMode() == AURA_REMOVE_BY_DEATH || GetTargetApplication()->GetRemoveMode() == AURA_REMOVE_BY_EXPIRE)
            if (Unit* caster = GetCaster())
                caster->CastSpell(GetTarget()->GetPosition(), SPELL_PRIEST_MIND_BOMB_STUN, true);
    }

    void Register() override
    {
        AfterEffectRemove += AuraEffectRemoveFn(spell_pri_mind_bomb::RemoveEffect, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
    }
};

// 390686 - Painful Punishment
class spell_pri_painful_punishment : public AuraScript
{
    PrepareAuraScript(spell_pri_painful_punishment);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo
        ({
            SPELL_PRIEST_SHADOW_WORD_PAIN,
            SPELL_PRIEST_PURGE_THE_WICKED_PERIODIC
        });
    }

    void HandleEffectProc(AuraEffect* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = eventInfo.GetActor();
        Unit* target = eventInfo.GetActionTarget();
        if (!caster || !target)
            return;

        int32 additionalDuration = aurEff->GetAmount();

        if (Aura* shadowWordPain = target->GetOwnedAura(SPELL_PRIEST_SHADOW_WORD_PAIN, caster->GetGUID()))
            shadowWordPain->SetDuration(shadowWordPain->GetDuration() + additionalDuration);

        if (Aura* purgeTheWicked = target->GetOwnedAura(SPELL_PRIEST_PURGE_THE_WICKED_PERIODIC, caster->GetGUID()))
            purgeTheWicked->SetDuration(purgeTheWicked->GetDuration() + additionalDuration);
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_pri_painful_punishment::HandleEffectProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

// 47540 - Penance
// 400169 - Dark Reprimand
class spell_pri_penance : public SpellScript
{
    PrepareSpellScript(spell_pri_penance);

public:
    spell_pri_penance(uint32 damageSpellId, uint32 healingSpellId) : _damageSpellId(damageSpellId), _healingSpellId(healingSpellId)
    {
    }

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ _damageSpellId, _healingSpellId });
    }

    SpellCastResult CheckCast()
    {
        Unit* caster = GetCaster();

        if (Unit* target = GetExplTargetUnit())
        {
            if (!caster->IsFriendlyTo(target))
            {
                if (!caster->IsValidAttackTarget(target))
                    return SPELL_FAILED_BAD_TARGETS;

                if (!caster->isInFront(target))
                    return SPELL_FAILED_UNIT_NOT_INFRONT;
            }
        }

        return SPELL_CAST_OK;
    }

    void HandleDummy(SpellEffIndex /*effIndex*/)
    {
        Unit* caster = GetCaster();

        if (Unit* target = GetHitUnit())
        {
            if (caster->IsFriendlyTo(target))
                caster->CastSpell(target, _healingSpellId, CastSpellExtraArgs().SetTriggeringSpell(GetSpell()));
            else
                caster->CastSpell(target, _damageSpellId, CastSpellExtraArgs().SetTriggeringSpell(GetSpell()));
        }
    }

    void Register() override
    {
        OnCheckCast += SpellCheckCastFn(spell_pri_penance::CheckCast);
        OnEffectHitTarget += SpellEffectFn(spell_pri_penance::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
    }

private:
    uint32 _damageSpellId;
    uint32 _healingSpellId;
};

// 47758 - Penance (Channel Damage), 47757 - Penance (Channel Healing)
// 373129 - Dark Reprimand (Channel Damage), 400171 - Dark Reprimand (Channel Healing)
class spell_pri_penance_or_dark_reprimand_channeled : public AuraScript
{
    PrepareAuraScript(spell_pri_penance_or_dark_reprimand_channeled);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_PRIEST_POWER_OF_THE_DARK_SIDE });
    }

    void HandleOnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        if (Unit* caster = GetCaster())
            caster->RemoveAura(SPELL_PRIEST_POWER_OF_THE_DARK_SIDE);
    }

    void Register() override
    {
        OnEffectRemove += AuraEffectRemoveFn(spell_pri_penance_or_dark_reprimand_channeled::HandleOnRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
    }
};

// 198069 - Power of the Dark Side
class spell_pri_power_of_the_dark_side : public AuraScript
{
    PrepareAuraScript(spell_pri_power_of_the_dark_side);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_PRIEST_POWER_OF_THE_DARK_SIDE_TINT });
    }

    void HandleOnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        if (Unit* caster = GetCaster())
            caster->CastSpell(caster, SPELL_PRIEST_POWER_OF_THE_DARK_SIDE_TINT, true);
    }

    void HandleOnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        if (Unit* caster = GetCaster())
            caster->RemoveAura(SPELL_PRIEST_POWER_OF_THE_DARK_SIDE_TINT);
    }

    void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(spell_pri_power_of_the_dark_side::HandleOnApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        OnEffectRemove += AuraEffectRemoveFn(spell_pri_power_of_the_dark_side::HandleOnRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
    }
};

// 47666 - Penance (Damage)
// 373130 - Dark Reprimand (Damage)
class spell_pri_power_of_the_dark_side_damage_bonus : public SpellScript
{
    PrepareSpellScript(spell_pri_power_of_the_dark_side_damage_bonus);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_PRIEST_POWER_OF_THE_DARK_SIDE });
    }

    void HandleLaunchTarget(SpellEffIndex effIndex)
    {
        if (AuraEffect* powerOfTheDarkSide = GetCaster()->GetAuraEffect(SPELL_PRIEST_POWER_OF_THE_DARK_SIDE, EFFECT_0))
        {
            PreventHitDefaultEffect(effIndex);

            float damageBonus = GetCaster()->SpellDamageBonusDone(GetHitUnit(), GetSpellInfo(), GetEffectValue(), SPELL_DIRECT_DAMAGE, GetEffectInfo());
            float value = damageBonus + damageBonus * GetEffectVariance();
            value *= 1.0f + (powerOfTheDarkSide->GetAmount() / 100.0f);
            value = GetHitUnit()->SpellDamageBonusTaken(GetCaster(), GetSpellInfo(), value, SPELL_DIRECT_DAMAGE);
            SetHitDamage(value);
        }
    }

    void Register() override
    {
        OnEffectLaunchTarget += SpellEffectFn(spell_pri_power_of_the_dark_side_damage_bonus::HandleLaunchTarget, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
    }
};

// 47750 - Penance (Healing)
// 400187 - Dark Reprimand (Healing)
class spell_pri_power_of_the_dark_side_healing_bonus : public SpellScript
{
    PrepareSpellScript(spell_pri_power_of_the_dark_side_healing_bonus);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_PRIEST_POWER_OF_THE_DARK_SIDE });
    }

    void HandleLaunchTarget(SpellEffIndex effIndex)
    {
        if (AuraEffect* powerOfTheDarkSide = GetCaster()->GetAuraEffect(SPELL_PRIEST_POWER_OF_THE_DARK_SIDE, EFFECT_0))
        {
            PreventHitDefaultEffect(effIndex);

            float healingBonus = GetCaster()->SpellHealingBonusDone(GetHitUnit(), GetSpellInfo(), GetEffectValue(), HEAL, GetEffectInfo());
            float value = healingBonus + healingBonus * GetEffectVariance();
            value *= 1.0f + (powerOfTheDarkSide->GetAmount() / 100.0f);
            value = GetHitUnit()->SpellHealingBonusTaken(GetCaster(), GetSpellInfo(), value, HEAL);
            SetHitHeal(value);
        }
    }

    void Register() override
    {
        OnEffectLaunchTarget += SpellEffectFn(spell_pri_power_of_the_dark_side_healing_bonus::HandleLaunchTarget, EFFECT_0, SPELL_EFFECT_HEAL);
    }
};

// 194509 - Power Word: Radiance
class spell_pri_power_word_radiance : public SpellScript
{
    PrepareSpellScript(spell_pri_power_word_radiance);

    bool Validate(SpellInfo const* spellInfo) override
    {
        return ValidateSpellInfo({ SPELL_PRIEST_ATONEMENT, SPELL_PRIEST_ATONEMENT_TRIGGERED, SPELL_PRIEST_TRINITY })
            && ValidateSpellEffect({ { spellInfo->Id, EFFECT_3 } });
    }

    void OnTargetSelect(std::list<WorldObject*>& targets)
    {
        uint32 maxTargets = GetEffectInfo(EFFECT_2).CalcValue(GetCaster()) + 1; // adding 1 for explicit target unit
        if (targets.size() > maxTargets)
        {
            Unit* explTarget = GetExplTargetUnit();

            // Sort targets so units with no atonement are first, then units who are injured, then oher units
            // Make sure explicit target unit is first
            targets.sort([this, explTarget](WorldObject* lhs, WorldObject* rhs)
            {
                if (lhs == explTarget) // explTarget > anything: always true
                    return true;
                if (rhs == explTarget) // anything > explTarget: always false
                    return false;
                return MakeSortTuple(lhs) > MakeSortTuple(rhs);
            });

            targets.resize(maxTargets);
        }
    }

    void HandleEffectHitTarget(SpellEffIndex /*effIndex*/)
    {
        Unit* caster = GetCaster();
        if (caster->HasAura(SPELL_PRIEST_TRINITY))
            return;

        uint32 durationPct = GetEffectInfo(EFFECT_3).CalcValue(caster);
        if (caster->HasAura(SPELL_PRIEST_ATONEMENT))
            caster->CastSpell(GetHitUnit(), SPELL_PRIEST_ATONEMENT_TRIGGERED, CastSpellExtraArgs(SPELLVALUE_DURATION_PCT, durationPct).SetTriggerFlags(TRIGGERED_FULL_MASK));
    }

    void Register() override
    {
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_pri_power_word_radiance::OnTargetSelect, EFFECT_1, TARGET_UNIT_DEST_AREA_ALLY);
        OnEffectHitTarget += SpellEffectFn(spell_pri_power_word_radiance::HandleEffectHitTarget, EFFECT_1, SPELL_EFFECT_HEAL);
    }

private:
    std::tuple<bool, bool> MakeSortTuple(WorldObject* obj)
    {
        return std::make_tuple(IsUnitWithNoAtonement(obj), IsUnitInjured(obj));
    }

    // Returns true if obj is a unit but has no atonement
    bool IsUnitWithNoAtonement(WorldObject* obj)
    {
        Unit* unit = obj->ToUnit();
        return unit && !unit->HasAura(SPELL_PRIEST_ATONEMENT_TRIGGERED, GetCaster()->GetGUID());
    }

    // Returns true if obj is a unit and is injured
    static bool IsUnitInjured(WorldObject* obj)
    {
        Unit* unit = obj->ToUnit();
        return unit && !unit->IsFullHealth();
    }
};

// 17 - Power Word: Shield
class spell_pri_power_word_shield : public SpellScript
{
    PrepareSpellScript(spell_pri_power_word_shield);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_PRIEST_WEAKENED_SOUL });
    }

    SpellCastResult CheckCast()
    {
        Unit* caster = GetCaster();

        if (Unit* target = GetExplTargetUnit())
            if (!caster->HasAura(SPELL_PRIEST_RAPTURE))
                if (target->HasAura(SPELL_PRIEST_WEAKENED_SOUL, caster->GetGUID()))
                    return SPELL_FAILED_BAD_TARGETS;

        return SPELL_CAST_OK;
    }

    void HandleEffectHit()
    {
        Unit* caster = GetCaster();

        if (Unit* target = GetHitUnit())
            if (!caster->HasAura(SPELL_PRIEST_RAPTURE))
                caster->CastSpell(target, SPELL_PRIEST_WEAKENED_SOUL, true);
    }

    void Register() override
    {
        OnCheckCast += SpellCheckCastFn(spell_pri_power_word_shield::CheckCast);
        AfterHit += SpellHitFn(spell_pri_power_word_shield::HandleEffectHit);
    }
};

class spell_pri_power_word_shield_aura : public AuraScript
{
    PrepareAuraScript(spell_pri_power_word_shield_aura);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo
        ({
            SPELL_PRIEST_BODY_AND_SOUL,
            SPELL_PRIEST_BODY_AND_SOUL_SPEED,
            SPELL_PRIEST_STRENGTH_OF_SOUL,
            SPELL_PRIEST_STRENGTH_OF_SOUL_EFFECT,
            SPELL_PRIEST_RENEWED_HOPE,
            SPELL_PRIEST_RENEWED_HOPE_EFFECT,
            SPELL_PRIEST_VOID_SHIELD,
            SPELL_PRIEST_VOID_SHIELD_EFFECT,
            SPELL_PRIEST_ATONEMENT,
            SPELL_PRIEST_TRINITY,
            SPELL_PRIEST_ATONEMENT_TRIGGERED,
            SPELL_PRIEST_ATONEMENT_TRIGGERED_TRINITY,
            SPELL_PRIEST_SHIELD_DISCIPLINE_PASSIVE,
            SPELL_PRIEST_SHIELD_DISCIPLINE_ENERGIZE,
            SPELL_PRIEST_RAPTURE,
            SPELL_PRIEST_MASTERY_GRACE
        });
    }

    void CalculateAmount(AuraEffect const* /*auraEffect*/, int32& amount, bool& canBeRecalculated)
    {
        canBeRecalculated = false;

        if (Unit* caster = GetCaster())
        {
            float amountF = caster->SpellBaseDamageBonusDone(GetSpellInfo()->GetSchoolMask()) * 1.65f;

            if (Player* player = caster->ToPlayer())
            {
                AddPct(amountF, player->GetRatingBonusValue(CR_VERSATILITY_DAMAGE_DONE));

                if (AuraEffect const* mastery = caster->GetAuraEffect(SPELL_PRIEST_MASTERY_GRACE, EFFECT_0))
                    if (GetUnitOwner()->HasAura(SPELL_PRIEST_ATONEMENT_TRIGGERED) || GetUnitOwner()->HasAura(SPELL_PRIEST_ATONEMENT_TRIGGERED_TRINITY))
                        AddPct(amountF, mastery->GetAmount());
            }

            if (AuraEffect const* rapture = caster->GetAuraEffect(SPELL_PRIEST_RAPTURE, EFFECT_1))
                AddPct(amountF, rapture->GetAmount());

            amount = amountF;
        }
    }

    void HandleOnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        Unit* caster = GetCaster();
        Unit* target = GetTarget();

        if (!caster)
            return;

        if (caster->HasAura(SPELL_PRIEST_BODY_AND_SOUL))
            caster->CastSpell(target, SPELL_PRIEST_BODY_AND_SOUL_SPEED, true);

        if (caster->HasAura(SPELL_PRIEST_STRENGTH_OF_SOUL))
            caster->CastSpell(target, SPELL_PRIEST_STRENGTH_OF_SOUL_EFFECT, true);

        if (caster->HasAura(SPELL_PRIEST_RENEWED_HOPE))
            caster->CastSpell(target, SPELL_PRIEST_RENEWED_HOPE_EFFECT, true);

        if (caster->HasAura(SPELL_PRIEST_VOID_SHIELD) && caster == target)
            caster->CastSpell(target, SPELL_PRIEST_VOID_SHIELD_EFFECT, true);

        if (caster->HasAura(SPELL_PRIEST_ATONEMENT))
            caster->CastSpell(target, caster->HasAura(SPELL_PRIEST_TRINITY) ? SPELL_PRIEST_ATONEMENT_TRIGGERED_TRINITY : SPELL_PRIEST_ATONEMENT_TRIGGERED, true);
    }

    void HandleOnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        GetTarget()->RemoveAura(SPELL_PRIEST_STRENGTH_OF_SOUL_EFFECT);

        if (Unit* caster = GetCaster())
            if (GetTargetApplication()->GetRemoveMode() == AURA_REMOVE_BY_ENEMY_SPELL && caster->HasAura(SPELL_PRIEST_SHIELD_DISCIPLINE_PASSIVE))
                caster->CastSpell(caster, SPELL_PRIEST_SHIELD_DISCIPLINE_ENERGIZE, true);
    }

    void Register() override
    {
        DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_pri_power_word_shield_aura::CalculateAmount, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
        AfterEffectApply += AuraEffectApplyFn(spell_pri_power_word_shield_aura::HandleOnApply, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB, AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
        AfterEffectRemove += AuraEffectRemoveFn(spell_pri_power_word_shield_aura::HandleOnRemove, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB, AURA_EFFECT_HANDLE_REAL);
    }
};

// 129250 - Power Word: Solace
class spell_pri_power_word_solace : public SpellScript
{
    PrepareSpellScript(spell_pri_power_word_solace);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_PRIEST_POWER_WORD_SOLACE_ENERGIZE });
    }

    void RestoreMana(SpellEffIndex /*effIndex*/)
    {
        GetCaster()->CastSpell(GetCaster(), SPELL_PRIEST_POWER_WORD_SOLACE_ENERGIZE,
            CastSpellExtraArgs(TRIGGERED_IGNORE_CAST_IN_PROGRESS).SetTriggeringSpell(GetSpell())
                .AddSpellMod(SPELLVALUE_BASE_POINT0, GetEffectValue() / 100));
    }

    void Register() override
    {
        OnEffectLaunch += SpellEffectFn(spell_pri_power_word_solace::RestoreMana, EFFECT_1, SPELL_EFFECT_DUMMY);
    }
};

// Base class used by various prayer of mending spells
class spell_pri_prayer_of_mending_SpellScriptBase : public SpellScript
{
public:
    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_PRIEST_PRAYER_OF_MENDING_HEAL, SPELL_PRIEST_PRAYER_OF_MENDING_AURA })
            && ValidateSpellEffect({ { SPELL_PRIEST_PRAYER_OF_MENDING_HEAL, EFFECT_0 } });
    }

    bool Load() override
    {
        _spellInfoHeal = sSpellMgr->AssertSpellInfo(SPELL_PRIEST_PRAYER_OF_MENDING_HEAL, DIFFICULTY_NONE);
        _healEffectDummy = &_spellInfoHeal->GetEffect(EFFECT_0);
        return true;
    }

    void CastPrayerOfMendingAura(Unit* caster, Unit* target, uint8 stack)
    {
        uint32 basePoints = caster->SpellHealingBonusDone(target, _spellInfoHeal, _healEffectDummy->CalcValue(caster), HEAL, *_healEffectDummy);
        CastSpellExtraArgs args;
        args.TriggerFlags = TRIGGERED_FULL_MASK;
        args.AddSpellMod(SPELLVALUE_AURA_STACK, stack);
        args.AddSpellMod(SPELLVALUE_BASE_POINT0, basePoints);
        caster->CastSpell(target, SPELL_PRIEST_PRAYER_OF_MENDING_AURA, args);
    }

protected:
    SpellInfo const* _spellInfoHeal;
    SpellEffectInfo const* _healEffectDummy;
};

// 33076 - Prayer of Mending
class spell_pri_prayer_of_mending : public spell_pri_prayer_of_mending_SpellScriptBase
{
    PrepareSpellScript(spell_pri_prayer_of_mending);

    void HandleEffectDummy(SpellEffIndex /*effIndex*/)
    {
        CastPrayerOfMendingAura(GetCaster(), GetHitUnit(), GetEffectValue());
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_pri_prayer_of_mending::HandleEffectDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
    }
};

// 41635 - Prayer of Mending (Aura) - SPELL_PRIEST_PRAYER_OF_MENDING_AURA
class spell_pri_prayer_of_mending_aura : public AuraScript
{
    PrepareAuraScript(spell_pri_prayer_of_mending_aura);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo
        ({
            SPELL_PRIEST_PRAYER_OF_MENDING_HEAL,
            SPELL_PRIEST_PRAYER_OF_MENDING_JUMP,
            SPELL_PRIEST_SAY_YOUR_PRAYERS
        })
            && ValidateSpellEffect({ { SPELL_PRIEST_SAY_YOUR_PRAYERS, EFFECT_0 } });
    }

    void HandleHeal(AuraEffect* aurEff, ProcEventInfo& /*eventInfo*/)
    {
        // Caster: player (priest) that cast the Prayer of Mending
        // Target: player that currently has Prayer of Mending aura on him
        Unit* target = GetTarget();
        if (Unit* caster = GetCaster())
        {
            // Cast the spell to heal the owner
            caster->CastSpell(target, SPELL_PRIEST_PRAYER_OF_MENDING_HEAL, aurEff);

            // Only cast jump if stack is higher than 0
            int32 stackAmount = GetStackAmount();
            if (stackAmount > 1)
            {
                CastSpellExtraArgs args(aurEff);
                args.OriginalCaster = caster->GetGUID();

                int32 newStackAmount = stackAmount - 1;
                if (AuraEffect* sayYourPrayers = caster->GetAuraEffect(SPELL_PRIEST_SAY_YOUR_PRAYERS, EFFECT_0))
                    if (roll_chance_i(sayYourPrayers->GetAmount()))
                        ++newStackAmount;

                args.AddSpellMod(SPELLVALUE_BASE_POINT0, newStackAmount);

                target->CastSpell(target, SPELL_PRIEST_PRAYER_OF_MENDING_JUMP, args);
            }

            Remove();
        }
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_pri_prayer_of_mending_aura::HandleHeal, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

// 155793 - Prayer of Mending (Jump) - SPELL_PRIEST_PRAYER_OF_MENDING_JUMP
class spell_pri_prayer_of_mending_jump : public spell_pri_prayer_of_mending_SpellScriptBase
{
    PrepareSpellScript(spell_pri_prayer_of_mending_jump);

    void OnTargetSelect(std::list<WorldObject*>& targets)
    {
        // Find the best target - prefer players over pets
        bool foundPlayer = false;
        for (WorldObject* worldObject : targets)
        {
            if (worldObject->IsPlayer())
            {
                foundPlayer = true;
                break;
            }
        }

        if (foundPlayer)
            targets.remove_if(Trinity::ObjectTypeIdCheck(TYPEID_PLAYER, false));

        // choose one random target from targets
        if (targets.size() > 1)
        {
            WorldObject* selected = Trinity::Containers::SelectRandomContainerElement(targets);
            targets.clear();
            targets.push_back(selected);
        }
    }

    void HandleJump(SpellEffIndex /*effIndex*/)
    {
        Unit* origCaster = GetOriginalCaster(); // the one that started the prayer of mending chain
        Unit* target = GetHitUnit(); // the target we decided the aura should jump to

        if (origCaster)
            CastPrayerOfMendingAura(origCaster, target, GetEffectValue());
    }

    void Register() override
    {
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_pri_prayer_of_mending_jump::OnTargetSelect, EFFECT_0, TARGET_UNIT_SRC_AREA_ALLY);
        OnEffectHitTarget += SpellEffectFn(spell_pri_prayer_of_mending_jump::HandleJump, EFFECT_0, SPELL_EFFECT_DUMMY);
    }
};

// 204197 - Purge the Wicked
// Called by Penance - 47540, Dark Reprimand - 400169
class spell_pri_purge_the_wicked : public SpellScript
{
    PrepareSpellScript(spell_pri_purge_the_wicked);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo
        ({
            SPELL_PRIEST_PURGE_THE_WICKED_PERIODIC,
            SPELL_PRIEST_PURGE_THE_WICKED_DUMMY
        });
    }

    void HandleDummy(SpellEffIndex /*effIndex*/)
    {
        Unit* caster = GetCaster();
        Unit* target = GetHitUnit();

        if (target->HasAura(SPELL_PRIEST_PURGE_THE_WICKED_PERIODIC, caster->GetGUID()))
            caster->CastSpell(target, SPELL_PRIEST_PURGE_THE_WICKED_DUMMY, CastSpellExtraArgs(TriggerCastFlags(TRIGGERED_IGNORE_GCD | TRIGGERED_IGNORE_CAST_IN_PROGRESS)));
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_pri_purge_the_wicked::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
    }
};

// 204215 - Purge the Wicked
class spell_pri_purge_the_wicked_dummy : public SpellScript
{
    PrepareSpellScript(spell_pri_purge_the_wicked_dummy);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_PRIEST_PURGE_THE_WICKED_PERIODIC, SPELL_PRIEST_REVEL_IN_PURITY })
            && ValidateSpellEffect({ { SPELL_PRIEST_REVEL_IN_PURITY, EFFECT_1 } });
    }

    void FilterTargets(std::list<WorldObject*>& targets)
    {
        Unit* caster = GetCaster();
        Unit* explTarget = GetExplTargetUnit();

        targets.remove_if([&](WorldObject* object) -> bool
        {
            // Note: we must remove any non-unit target, the explicit target and any other target that may be under any crowd control aura.
            Unit* target = object->ToUnit();
            return !target || target == explTarget || target->HasBreakableByDamageCrowdControlAura();
        });

        if (targets.empty())
            return;

        // Note: there's no SPELL_EFFECT_DUMMY with BasePoints 1 in any of the spells related to use as reference so we hardcode the value.
        uint32 spreadCount = 1;

        // Note: we must sort our list of targets whose priority is 1) aura, 2) distance, and 3) duration.
        targets.sort([&](WorldObject const* lhs, WorldObject const* rhs) -> bool
        {
            Unit const* targetA = lhs->ToUnit();
            Unit const* targetB = rhs->ToUnit();

            Aura* auraA = targetA->GetAura(SPELL_PRIEST_PURGE_THE_WICKED_PERIODIC, caster->GetGUID());
            Aura* auraB = targetB->GetAura(SPELL_PRIEST_PURGE_THE_WICKED_PERIODIC, caster->GetGUID());

            if (!auraA)
            {
                if (auraB)
                    return true;
                return explTarget->GetExactDist(targetA) < explTarget->GetExactDist(targetB);
            }
            if (!auraB)
                return false;

            return auraA->GetDuration() < auraB->GetDuration();
        });

        // Note: Revel in Purity talent.
        if (caster->HasAura(SPELL_PRIEST_REVEL_IN_PURITY))
            spreadCount += sSpellMgr->AssertSpellInfo(SPELL_PRIEST_REVEL_IN_PURITY, DIFFICULTY_NONE)->GetEffect(EFFECT_1).CalcValue(GetCaster());

        if (targets.size() > spreadCount)
            targets.resize(spreadCount);
    }

    void HandleDummy(SpellEffIndex /*effIndex*/)
    {
        Unit* caster = GetCaster();
        Unit* target = GetHitUnit();

        caster->CastSpell(target, SPELL_PRIEST_PURGE_THE_WICKED_PERIODIC, CastSpellExtraArgs(TriggerCastFlags(TRIGGERED_IGNORE_GCD | TRIGGERED_IGNORE_CAST_IN_PROGRESS)));
    }

    void Register() override
    {
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_pri_purge_the_wicked_dummy::FilterTargets, EFFECT_1, TARGET_UNIT_DEST_AREA_ENEMY);
        OnEffectHitTarget += SpellEffectFn(spell_pri_purge_the_wicked_dummy::HandleDummy, EFFECT_1, SPELL_EFFECT_DUMMY);
    }
};

// 47536 - Rapture
class spell_pri_rapture : public SpellScript
{
    PrepareSpellScript(spell_pri_rapture);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_PRIEST_POWER_WORD_SHIELD });
    }

    void HandleEffectDummy(SpellEffIndex /*effIndex*/)
    {
        _raptureTarget = GetHitUnit()->GetGUID();
    }

    void HandleAfterCast()
    {
        Unit* caster = GetCaster();

        if (Unit* target = ObjectAccessor::GetUnit(*caster, _raptureTarget))
            caster->CastSpell(target, SPELL_PRIEST_POWER_WORD_SHIELD,
                CastSpellExtraArgs(TriggerCastFlags(TRIGGERED_IGNORE_GCD | TRIGGERED_IGNORE_POWER_AND_REAGENT_COST | TRIGGERED_IGNORE_CAST_IN_PROGRESS))
                .SetTriggeringSpell(GetSpell()));
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_pri_rapture::HandleEffectDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
        AfterCast += SpellCastFn(spell_pri_rapture::HandleAfterCast);
    }

private:
    ObjectGuid _raptureTarget;
};

// 280391 - Sins of the Many
class spell_pri_sins_of_the_many : public AuraScript
{
    PrepareAuraScript(spell_pri_sins_of_the_many);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_PRIEST_SINS_OF_THE_MANY });
    }

    void HandleOnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        GetTarget()->CastSpell(GetTarget(), SPELL_PRIEST_SINS_OF_THE_MANY, true);
    }

    void HandleOnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        GetTarget()->RemoveAura(SPELL_PRIEST_SINS_OF_THE_MANY);
    }

    void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(spell_pri_sins_of_the_many::HandleOnApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        OnEffectRemove += AuraEffectRemoveFn(spell_pri_sins_of_the_many::HandleOnRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
    }
};

// 20711 - Spirit of Redemption
class spell_pri_spirit_of_redemption : public AuraScript
{
    PrepareAuraScript(spell_pri_spirit_of_redemption);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_PRIEST_SPIRIT_OF_REDEMPTION });
    }

    void HandleAbsorb(AuraEffect* aurEff, DamageInfo& dmgInfo, uint32& absorbAmount)
    {
        Unit* target = GetTarget();
        target->CastSpell(target, SPELL_PRIEST_SPIRIT_OF_REDEMPTION, aurEff);
        target->SetFullHealth();

        absorbAmount = dmgInfo.GetDamage();
    }

    void Register() override
    {
        OnEffectAbsorb += AuraEffectAbsorbOverkillFn(spell_pri_spirit_of_redemption::HandleAbsorb, EFFECT_0);
    }
};

// 186263 - Shadow Mend
class spell_pri_shadow_mend : public SpellScript
{
    PrepareSpellScript(spell_pri_shadow_mend);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo
        ({
            SPELL_PRIEST_ATONEMENT,
            SPELL_PRIEST_ATONEMENT_TRIGGERED,
            SPELL_PRIEST_TRINITY,
            SPELL_PRIEST_MASOCHISM_TALENT,
            SPELL_PRIEST_MASOCHISM_PERIODIC_HEAL,
            SPELL_PRIEST_SHADOW_MEND_PERIODIC_DUMMY
        });
    }

    void HandleEffectHit()
    {
        if (Unit* target = GetHitUnit())
        {
            Unit* caster = GetCaster();

            int32 periodicAmount = GetHitHeal() / 20;
            int32 damageForAuraRemoveAmount = periodicAmount * 10;
            if (caster->HasAura(SPELL_PRIEST_ATONEMENT) && !caster->HasAura(SPELL_PRIEST_TRINITY))
                caster->CastSpell(target, SPELL_PRIEST_ATONEMENT_TRIGGERED, GetSpell());

            // Handle Masochism talent
            if (caster->HasAura(SPELL_PRIEST_MASOCHISM_TALENT) && caster->GetGUID() == target->GetGUID())
                caster->CastSpell(caster, SPELL_PRIEST_MASOCHISM_PERIODIC_HEAL, CastSpellExtraArgs(GetSpell()).AddSpellMod(SPELLVALUE_BASE_POINT0, periodicAmount));
            else if (target->IsInCombat() && periodicAmount)
            {
                CastSpellExtraArgs args(TRIGGERED_FULL_MASK);
                args.SetTriggeringSpell(GetSpell());
                args.AddSpellMod(SPELLVALUE_BASE_POINT0, periodicAmount);
                args.AddSpellMod(SPELLVALUE_BASE_POINT1, damageForAuraRemoveAmount);
                caster->CastSpell(target, SPELL_PRIEST_SHADOW_MEND_PERIODIC_DUMMY, args);
            }
        }
    }

    void Register() override
    {
        AfterHit += SpellHitFn(spell_pri_shadow_mend::HandleEffectHit);
    }
};

// 187464 - Shadow Mend (Damage)
class spell_pri_shadow_mend_periodic_damage : public AuraScript
{
    PrepareAuraScript(spell_pri_shadow_mend_periodic_damage);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_PRIEST_SHADOW_MEND_DAMAGE });
    }

    void HandleDummyTick(AuraEffect const* aurEff)
    {
        CastSpellExtraArgs args(TRIGGERED_FULL_MASK);
        args.SetOriginalCaster(GetCasterGUID());
        args.SetTriggeringAura(aurEff);
        args.AddSpellMod(SPELLVALUE_BASE_POINT0, aurEff->GetAmount());
        GetTarget()->CastSpell(GetTarget(), SPELL_PRIEST_SHADOW_MEND_DAMAGE, args);
    }

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetDamageInfo() != nullptr;
    }

    void HandleProc(AuraEffect* aurEff, ProcEventInfo& eventInfo)
    {
        int32 newAmount = aurEff->GetAmount() - eventInfo.GetDamageInfo()->GetDamage();

        aurEff->ChangeAmount(newAmount);
        if (newAmount < 0)
            Remove();
    }

    void Register() override
    {
        OnEffectPeriodic += AuraEffectPeriodicFn(spell_pri_shadow_mend_periodic_damage::HandleDummyTick, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
        DoCheckProc += AuraCheckProcFn(spell_pri_shadow_mend_periodic_damage::CheckProc);
        OnEffectProc += AuraEffectProcFn(spell_pri_shadow_mend_periodic_damage::HandleProc, EFFECT_1, SPELL_AURA_DUMMY);
    }
};

// 28809 - Greater Heal
class spell_pri_t3_4p_bonus : public AuraScript
{
    PrepareAuraScript(spell_pri_t3_4p_bonus);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_PRIEST_ARMOR_OF_FAITH });
    }

    void HandleProc(AuraEffect* aurEff, ProcEventInfo& eventInfo)
    {
        PreventDefaultAction();
        eventInfo.GetActor()->CastSpell(eventInfo.GetProcTarget(), SPELL_PRIEST_ARMOR_OF_FAITH, aurEff);
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_pri_t3_4p_bonus::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

// 37594 - Greater Heal Refund
class spell_pri_t5_heal_2p_bonus : public AuraScript
{
    PrepareAuraScript(spell_pri_t5_heal_2p_bonus);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_PRIEST_ITEM_EFFICIENCY });
    }

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        if (HealInfo* healInfo = eventInfo.GetHealInfo())
            if (Unit* healTarget = healInfo->GetTarget())
                if (healInfo->GetEffectiveHeal())
                    if (healTarget->GetHealth() >= healTarget->GetMaxHealth())
                        return true;

        return false;
    }

    void HandleProc(AuraEffect* aurEff, ProcEventInfo& /*eventInfo*/)
    {
        PreventDefaultAction();
        GetTarget()->CastSpell(GetTarget(), SPELL_PRIEST_ITEM_EFFICIENCY, aurEff);
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(spell_pri_t5_heal_2p_bonus::CheckProc);
        OnEffectProc += AuraEffectProcFn(spell_pri_t5_heal_2p_bonus::HandleProc, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL);
    }
};

// 70770 - Item - Priest T10 Healer 2P Bonus
class spell_pri_t10_heal_2p_bonus : public AuraScript
{
    PrepareAuraScript(spell_pri_t10_heal_2p_bonus);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_PRIEST_BLESSED_HEALING });
    }

    void HandleProc(AuraEffect* aurEff, ProcEventInfo& eventInfo)
    {
        PreventDefaultAction();

        HealInfo* healInfo = eventInfo.GetHealInfo();
        if (!healInfo || !healInfo->GetHeal())
            return;

        SpellInfo const* spellInfo = sSpellMgr->AssertSpellInfo(SPELL_PRIEST_BLESSED_HEALING, GetCastDifficulty());
        int32 amount = CalculatePct(static_cast<int32>(healInfo->GetHeal()), aurEff->GetAmount());

        ASSERT(spellInfo->GetMaxTicks() > 0);
        amount /= spellInfo->GetMaxTicks();

        Unit* caster = eventInfo.GetActor();
        Unit* target = eventInfo.GetProcTarget();

        CastSpellExtraArgs args(aurEff);
        args.AddSpellBP0(amount);
        caster->CastSpell(target, SPELL_PRIEST_BLESSED_HEALING, args);
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_pri_t10_heal_2p_bonus::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

// 109142 - Twist of Fate (Shadow)
// 265259 - Twist of Fate (Discipline)
class spell_pri_twist_of_fate : public AuraScript
{
    PrepareAuraScript(spell_pri_twist_of_fate);

    bool CheckProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        return eventInfo.GetProcTarget()->GetHealthPct() < aurEff->GetAmount();
    }

    void Register() override
    {
        DoCheckEffectProc += AuraCheckEffectProcFn(spell_pri_twist_of_fate::CheckProc, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL);
    }
};

// 15286 - Vampiric Embrace
class spell_pri_vampiric_embrace : public AuraScript
{
    PrepareAuraScript(spell_pri_vampiric_embrace);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_PRIEST_VAMPIRIC_EMBRACE_HEAL });
    }

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        // Not proc from Mind Sear
        return !(eventInfo.GetDamageInfo()->GetSpellInfo()->SpellFamilyFlags[1] & 0x80000);
    }

    void HandleEffectProc(AuraEffect* aurEff, ProcEventInfo& eventInfo)
    {
        PreventDefaultAction();
        DamageInfo* damageInfo = eventInfo.GetDamageInfo();
        if (!damageInfo || !damageInfo->GetDamage())
            return;

        int32 selfHeal = int32(CalculatePct(damageInfo->GetDamage(), aurEff->GetAmount()));
        int32 teamHeal = selfHeal / 2;

        CastSpellExtraArgs args(aurEff);
        args.AddSpellMod(SPELLVALUE_BASE_POINT0, teamHeal);
        args.AddSpellMod(SPELLVALUE_BASE_POINT1, selfHeal);
        GetTarget()->CastSpell(nullptr, SPELL_PRIEST_VAMPIRIC_EMBRACE_HEAL, args);
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(spell_pri_vampiric_embrace::CheckProc);
        OnEffectProc += AuraEffectProcFn(spell_pri_vampiric_embrace::HandleEffectProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

// 15290 - Vampiric Embrace (heal)
class spell_pri_vampiric_embrace_target : public SpellScript
{
    PrepareSpellScript(spell_pri_vampiric_embrace_target);

    void FilterTargets(std::list<WorldObject*>& unitList)
    {
        unitList.remove(GetCaster());
    }

    void Register() override
    {
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_pri_vampiric_embrace_target::FilterTargets, EFFECT_0, TARGET_UNIT_CASTER_AREA_PARTY);
    }
};

// 34914 - Vampiric Touch
class spell_pri_vampiric_touch : public AuraScript
{
    PrepareAuraScript(spell_pri_vampiric_touch);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_PRIEST_VAMPIRIC_TOUCH_DISPEL, SPELL_GEN_REPLENISHMENT });
    }

    void HandleDispel(DispelInfo* /*dispelInfo*/)
    {
        if (Unit* caster = GetCaster())
        {
            if (Unit* target = GetUnitOwner())
            {
                if (AuraEffect const* aurEff = GetEffect(EFFECT_1))
                {
                    // backfire damage
                    int32 bp = aurEff->GetAmount();
                    bp = target->SpellDamageBonusTaken(caster, aurEff->GetSpellInfo(), bp, DOT);
                    bp *= 8;

                    CastSpellExtraArgs args(aurEff);
                    args.AddSpellBP0(bp);
                    caster->CastSpell(target, SPELL_PRIEST_VAMPIRIC_TOUCH_DISPEL, args);
                }
            }
        }
    }

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetProcTarget() == GetCaster();
    }

    void HandleEffectProc(AuraEffect* aurEff, ProcEventInfo& eventInfo)
    {
        PreventDefaultAction();
        eventInfo.GetProcTarget()->CastSpell(nullptr, SPELL_GEN_REPLENISHMENT, aurEff);
    }

    void Register() override
    {
        AfterDispel += AuraDispelFn(spell_pri_vampiric_touch::HandleDispel);
        DoCheckProc += AuraCheckProcFn(spell_pri_vampiric_touch::CheckProc);
        OnEffectProc += AuraEffectProcFn(spell_pri_vampiric_touch::HandleEffectProc, EFFECT_2, SPELL_AURA_DUMMY);
    }
};

void AddSC_priest_spell_scripts()
{
    RegisterSpellScript(spell_pri_angelic_feather_trigger);
    RegisterAreaTriggerAI(areatrigger_pri_angelic_feather);
    RegisterSpellScript(spell_pri_aq_3p_bonus);
    RegisterSpellScript(spell_pri_atonement);
    RegisterSpellScript(spell_pri_atonement_triggered);
    RegisterSpellScript(spell_pri_circle_of_healing);
    RegisterSpellScript(spell_pri_divine_hymn);
    RegisterSpellScript(spell_pri_divine_star_shadow);
    RegisterAreaTriggerAI(areatrigger_pri_divine_star);
    RegisterSpellScript(spell_pri_empowered_renew);
    RegisterSpellScript(spell_pri_guardian_spirit);
    RegisterSpellScript(spell_pri_halo_shadow);
    RegisterAreaTriggerAI(areatrigger_pri_halo);
    RegisterSpellScript(spell_pri_holy_words);
    RegisterSpellScript(spell_pri_item_t6_trinket);
    RegisterSpellScript(spell_pri_leap_of_faith_effect_trigger);
    RegisterSpellScript(spell_pri_levitate);
    RegisterSpellScript(spell_pri_mind_bomb);
    RegisterSpellScript(spell_pri_painful_punishment);
    RegisterSpellScriptWithArgs(spell_pri_penance, "spell_pri_penance", SPELL_PRIEST_PENANCE_CHANNEL_DAMAGE, SPELL_PRIEST_PENANCE_CHANNEL_HEALING);
    RegisterSpellScriptWithArgs(spell_pri_penance, "spell_pri_dark_reprimand", SPELL_PRIEST_DARK_REPRIMAND_CHANNEL_DAMAGE, SPELL_PRIEST_DARK_REPRIMAND_CHANNEL_HEALING);
    RegisterSpellScript(spell_pri_penance_or_dark_reprimand_channeled);
    RegisterSpellScript(spell_pri_power_of_the_dark_side);
    RegisterSpellScript(spell_pri_power_of_the_dark_side_damage_bonus);
    RegisterSpellScript(spell_pri_power_of_the_dark_side_healing_bonus);
    RegisterSpellScript(spell_pri_power_word_radiance);
    RegisterSpellAndAuraScriptPair(spell_pri_power_word_shield, spell_pri_power_word_shield_aura);
    RegisterSpellScript(spell_pri_power_word_solace);
    RegisterSpellScript(spell_pri_prayer_of_mending);
    RegisterSpellScript(spell_pri_prayer_of_mending_aura);
    RegisterSpellScript(spell_pri_prayer_of_mending_jump);
    RegisterSpellScript(spell_pri_purge_the_wicked);
    RegisterSpellScript(spell_pri_purge_the_wicked_dummy);
    RegisterSpellScript(spell_pri_rapture);
    RegisterSpellScript(spell_pri_sins_of_the_many);
    RegisterSpellScript(spell_pri_spirit_of_redemption);
    RegisterSpellScript(spell_pri_shadow_mend);
    RegisterSpellScript(spell_pri_shadow_mend_periodic_damage);
    RegisterSpellScript(spell_pri_t3_4p_bonus);
    RegisterSpellScript(spell_pri_t5_heal_2p_bonus);
    RegisterSpellScript(spell_pri_t10_heal_2p_bonus);
    RegisterSpellScript(spell_pri_twist_of_fate);
    RegisterSpellScript(spell_pri_vampiric_embrace);
    RegisterSpellScript(spell_pri_vampiric_embrace_target);
    RegisterSpellScript(spell_pri_vampiric_touch);
}
