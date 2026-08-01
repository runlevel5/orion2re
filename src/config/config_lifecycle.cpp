#include "config_lifecycle.h"

#include "config/config_decode.h"

namespace config {
namespace {
    bool Fail_(const s_source_location& location, const std::string& message,
               s_cfg_diagnostic* diagnostic) {
        diagnostic->location = location;
        diagnostic->message = message;
        return false;
    }

    void Reset_Metadata_(s_effective_config* effective) {
        effective->provenance.clear();
        effective->provenance.resize(variable_count);
        effective->prior_values.clear();
        effective->prior_values.resize(variable_count);
    }
}

bool Initialize_Effective_Config(s_effective_config* effective) {
    if (effective == nullptr || !Initialize_Defaults(&effective->state)) {
        return false;
    }
    Reset_Metadata_(effective);
    return true;
}

bool Read_Effective_Native_Config(s_effective_config* effective) {
    if (effective == nullptr || !Read_Current_State(&effective->state)) {
        return false;
    }
    Reset_Metadata_(effective);
    return true;
}

bool Apply_Config_Document(const s_cfg_document* document, const char* package_id,
                           s_effective_config* effective, s_cfg_diagnostic* diagnostic) {
    if (document == nullptr || package_id == nullptr || effective == nullptr || diagnostic == nullptr) {
        return false;
    }
    *diagnostic = {};
    if (effective->state.values.size() != variable_count ||
        effective->provenance.size() != variable_count ||
        effective->prior_values.size() != variable_count) {
        diagnostic->message = "effective configuration is not initialized";
        return false;
    }

    s_config_state candidate_state = effective->state;
    std::vector<s_value_provenance> candidate_provenance = effective->provenance;
    std::vector<std::vector<s_prior_value>> candidate_prior_values = effective->prior_values;
    for (size_t statement_index = 0; statement_index < document->statements.size(); ++statement_index) {
        const s_cfg_statement& statement = document->statements[statement_index];
        if (statement.kind == CFG_STATEMENT_STOP) {
            break;
        }
        if (statement.kind == CFG_STATEMENT_INCLUDE) {
            return Fail_(statement.location, "include execution requires the configuration loader",
                         diagnostic);
        }
        if (statement.kind == CFG_STATEMENT_ENABLE) {
            continue;
        }

        s_config_write_set statement_writes;
        if (!Decode_Assignment(&statement, &statement_writes, diagnostic)) {
            return false;
        }
        std::vector<s_prior_value> statement_prior_values;
        statement_prior_values.reserve(statement_writes.writes.size());
        for (s_config_write& write : statement_writes.writes) {
            const size_t variable_index = write.variable_id;

            statement_prior_values.emplace_back();
            s_prior_value& prior = statement_prior_values.back();
            if (!Read_State_Value(&candidate_state, write.variable_id, &prior.value)) {
                return Fail_(write.provenance, "configuration state accessor is missing", diagnostic);
            }
            prior.provenance = candidate_provenance[variable_index];
        }

        if (!Apply_Write_Set(&statement_writes, &candidate_state, diagnostic)) {
            return false;
        }

        for (size_t write_index = 0; write_index < statement_writes.writes.size(); ++write_index) {
            const s_config_write& write = statement_writes.writes[write_index];
            const size_t variable_index = write.variable_id;
            candidate_prior_values[variable_index].push_back(
                std::move(statement_prior_values[write_index]));
            s_value_provenance provenance;
            provenance.has_override = true;
            provenance.package_id = package_id;
            provenance.location = write.provenance;
            provenance.statement = statement.statement_number != 0 ?
                                   statement.statement_number :
                                   static_cast<uint32_t>(statement_index + 1);
            candidate_provenance[variable_index] = std::move(provenance);
        }
    }

    if (!Commit_State(&candidate_state)) {
        diagnostic->message = "unable to commit configuration state";
        return false;
    }
    effective->state = candidate_state;
    effective->provenance = std::move(candidate_provenance);
    effective->prior_values = std::move(candidate_prior_values);
    return true;
}

}