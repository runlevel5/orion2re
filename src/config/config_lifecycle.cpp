#include "config_lifecycle.h"

#include "config/config_decode.h"
#include "config/generated/config_schema.h"

namespace config {
namespace {
    bool Fail_(const s_source_location& location, const std::string& message,
               s_cfg_diagnostic* diagnostic) {
        diagnostic->location = location;
        diagnostic->message = message;
        return false;
    }

    void Append_Escaped_String_(const std::string& value, std::string* output) {
        static const char HEX[] = "0123456789ABCDEF";
        output->push_back('"');
        for (uint8_t byte : value) {
            switch (byte) {
            case '"': *output += "\\\""; break;
            case '\\': *output += "\\\\"; break;
            case '\b': *output += "\\b"; break;
            case '\f': *output += "\\f"; break;
            case '\n': *output += "\\n"; break;
            case '\r': *output += "\\r"; break;
            case '\t': *output += "\\t"; break;
            default:
                if (byte < 0x20) {
                    *output += "\\u00";
                    output->push_back(HEX[byte >> 4]);
                    output->push_back(HEX[byte & 0x0F]);
                } else {
                    output->push_back(static_cast<char>(byte));
                }
                break;
            }
        }
        output->push_back('"');
    }

    void Append_Value_(const s_typed_value& value, std::string* output) {
        if (value.type == generated::CONFIG_VALUE_BOOL) {
            *output += value.bool_value ? "true" : "false";
        } else if (value.type == generated::CONFIG_VALUE_STRING) {
            Append_Escaped_String_(value.string_value, output);
        } else {
            *output += std::to_string(value.integer_value);
        }
    }

    void Append_Comment_Text_(const std::string& value, std::string* output) {
        for (uint8_t byte : value) {
            output->push_back(byte < 0x20 || byte == 0x7F ? '?' : static_cast<char>(byte));
        }
    }
}

void Initialize_Effective_Config(s_effective_config* effective) {
    generated::Initialize_Defaults(&effective->state);
    effective->provenance.clear();
    effective->provenance.resize(generated::parameter_count);
    effective->prior_values.clear();
    effective->prior_values.resize(generated::parameter_count);
}

void Read_Effective_Native_Config(s_effective_config* effective) {
    generated::Read_Native_State(&effective->state);
    effective->provenance.clear();
    effective->provenance.resize(generated::parameter_count);
    effective->prior_values.clear();
    effective->prior_values.resize(generated::parameter_count);
}

bool Apply_Config_Document(const s_cfg_document* document, const char* package_id,
                           s_effective_config* effective, s_cfg_diagnostic* diagnostic) {
    if (document == nullptr || package_id == nullptr || effective == nullptr || diagnostic == nullptr) {
        return false;
    }
    *diagnostic = {};
    if (effective->provenance.size() != generated::parameter_count ||
        effective->prior_values.size() != generated::parameter_count) {
        diagnostic->message = "effective configuration is not initialized";
        return false;
    }

    generated::s_config_state candidate_state = effective->state;
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
            return Fail_(statement.location, "enable execution requires the mod loader", diagnostic);
        }

        s_config_write_set statement_writes;
        if (!Decode_Assignment(&statement, &statement_writes, diagnostic)) {
            return false;
        }
        for (s_config_write& write : statement_writes.writes) {
            size_t parameter_index = 0;
            if (!generated::Find_Parameter_Index(write.parameter_id, &parameter_index)) {
                return Fail_(write.provenance, "generated parameter index is missing", diagnostic);
            }

            s_prior_value prior;
            if (!generated::Read_State_Value(&candidate_state, write.parameter_id, &prior.value)) {
                return Fail_(write.provenance, "generated state accessor is missing", diagnostic);
            }
            prior.provenance = candidate_provenance[parameter_index];

            s_config_write_set one_write;
            one_write.writes.push_back(write);
            generated::s_config_state next_state;
            if (!generated::Apply_Write_Set(&one_write, &candidate_state,
                                            &next_state, diagnostic)) {
                return false;
            }
            candidate_state = next_state;
            candidate_prior_values[parameter_index].push_back(std::move(prior));

            s_value_provenance provenance;
            provenance.has_override = true;
            provenance.package_id = package_id;
            provenance.location = write.provenance;
            provenance.statement = statement.statement_number != 0 ?
                                   statement.statement_number :
                                   static_cast<uint32_t>(statement_index + 1);
            candidate_provenance[parameter_index] = std::move(provenance);
        }
    }

    generated::Commit_State(&candidate_state);
    effective->state = candidate_state;
    effective->provenance = std::move(candidate_provenance);
    effective->prior_values = std::move(candidate_prior_values);
    return true;
}

bool Extract_Canonical_Config(const s_effective_config* effective, bool include_provenance,
                              std::string* output, s_cfg_diagnostic* diagnostic) {
    if (effective == nullptr || output == nullptr || diagnostic == nullptr) {
        return false;
    }
    output->clear();
    *diagnostic = {};
    if (effective->provenance.size() != generated::parameter_count ||
        effective->prior_values.size() != generated::parameter_count) {
        diagnostic->message = "effective configuration is not initialized";
        return false;
    }

    for (size_t index = 0; index < generated::parameter_count; ++index) {
        s_typed_value value;
        if (!generated::Read_State_Value(&effective->state,
                                         generated::parameters[index].id, &value)) {
            diagnostic->message = "generated state accessor is missing for '" +
                                  std::string(generated::parameters[index].path) + "'";
            return false;
        }
        if (include_provenance && effective->provenance[index].has_override) {
            const s_value_provenance& provenance = effective->provenance[index];
            *output += "# from ";
            Append_Comment_Text_(provenance.package_id, output);
            *output += ": ";
            Append_Comment_Text_(provenance.location.source, output);
            *output += ":" + std::to_string(provenance.location.line) + "\n";
        }
        *output += generated::parameters[index].path;
        *output += " = ";
        Append_Value_(value, output);
        *output += ";\n";
    }
    return true;
}

}