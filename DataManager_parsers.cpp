// ... existing from_json functions ...

void from_json(const json& j, Attunement& a) {
    j.at("id").get_to(a.id);
    j.at("name").get_to(a.name);
    j.at("description").get_to(a.description);
    j.at("starting_spells").get_to(a.starting_spell_ids);
    j.at("mana_types").get_to(a.mana_types);
}

void from_json(const json& j, Spell& s) {
    j.at("id").get_to(s.id);
    j.at("name").get_to(s.name);
    j.at("costAmount").get_to(s.costAmount);
    s.costType = j.at("costType").get<GemSubType>();
    
    s.effects.clear();
    for (const auto& effect_data : j.at("effects")) {
        Effect effect;
        effect.type = stringToEffectType(effect_data.at("type").get<std::string>());
        effect.amount = effect_data.at("amount").get<int>();
        effect.target = stringToTargetType(effect_data.at("target").get<std::string>());
        s.effects.push_back(effect);
    }
}

void from_json(const json& j, JudgementTrial& trial) {
    trial.trialId = j.at("trialId").get<std::string>();
    trial.objective = j.at("objective").get<std::string>();
    trial.type = static_cast<JudgementTrialType>(j.at("type").get<int>());
    if (j.contains("gems")) {
        trial.gems = j.at("gems").get<std::vector<GemSubType>>();
    }
    trial.turnLimit = j.at("turnLimit").get<int>();
    if (j.contains("timeLimit")) {
        trial.timeLimit = j.at("timeLimit").get<int>();
    }
    trial.scoreGoal = j.at("scoreGoal").get<int>();
}
