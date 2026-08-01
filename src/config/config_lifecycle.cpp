#include "config_lifecycle.h"

#include <cstdio>

#include "config/config_decode.h"

namespace config {
namespace {
    bool Fail_(const s_source_location& location, const std::string& message,
               s_cfg_diagnostic* diagnostic) {
        diagnostic->location = location;
        diagnostic->message = message;
        return false;
    }

    bool Append_Utf8_(uint32_t codepoint, std::string* output) {
        if (codepoint <= 0x7F) {
            output->push_back(static_cast<char>(codepoint));
        } else if (codepoint <= 0x7FF) {
            output->push_back(static_cast<char>(0xC0 | codepoint >> 6));
            output->push_back(static_cast<char>(0x80 | (codepoint & 0x3F)));
        } else {
            output->push_back(static_cast<char>(0xE0 | codepoint >> 12));
            output->push_back(static_cast<char>(0x80 | (codepoint >> 6 & 0x3F)));
            output->push_back(static_cast<char>(0x80 | (codepoint & 0x3F)));
        }
        return true;
    }

    void Append_Escaped_String_(const std::string& value, std::string* output) {
        static const char HEX[] = "0123456789ABCDEF";
        static const uint16_t CP850_EXTENDED[] = {
            0x00C7, 0x00FC, 0x00E9, 0x00E2, 0x00E4, 0x00E0, 0x00E5, 0x00E7,
            0x00EA, 0x00EB, 0x00E8, 0x00EF, 0x00EE, 0x00EC, 0x00C4, 0x00C5,
            0x00C9, 0x00E6, 0x00C6, 0x00F4, 0x00F6, 0x00F2, 0x00FB, 0x00F9,
            0x00FF, 0x00D6, 0x00DC, 0x00F8, 0x00A3, 0x00D8, 0x00D7, 0x0192,
            0x00E1, 0x00ED, 0x00F3, 0x00FA, 0x00F1, 0x00D1, 0x00AA, 0x00BA,
            0x00BF, 0x00AE, 0x00AC, 0x00BD, 0x00BC, 0x00A1, 0x00AB, 0x00BB,
            0x2591, 0x2592, 0x2593, 0x2502, 0x2524, 0x00C1, 0x00C2, 0x00C0,
            0x00A9, 0x2563, 0x2551, 0x2557, 0x255D, 0x00A2, 0x00A5, 0x2510,
            0x2514, 0x2534, 0x252C, 0x251C, 0x2500, 0x253C, 0x00E3, 0x00C3,
            0x255A, 0x2554, 0x2569, 0x2566, 0x2560, 0x2550, 0x256C, 0x00A4,
            0x00F0, 0x00D0, 0x00CA, 0x00CB, 0x00C8, 0x0131, 0x00CD, 0x00CE,
            0x00CF, 0x2518, 0x250C, 0x2588, 0x2584, 0x00A6, 0x00CC, 0x2580,
            0x00D3, 0x00DF, 0x00D4, 0x00D2, 0x00F5, 0x00D5, 0x00B5, 0x00FE,
            0x00DE, 0x00DA, 0x00DB, 0x00D9, 0x00FD, 0x00DD, 0x00AF, 0x00B4,
            0x00AD, 0x00B1, 0x2017, 0x00BE, 0x00B6, 0x00A7, 0x00F7, 0x00B8,
            0x00B0, 0x00A8, 0x00B7, 0x00B9, 0x00B3, 0x00B2, 0x25A0, 0x00A0,
        };
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
                } else if (byte < 0x80) {
                    output->push_back(static_cast<char>(byte));
                } else {
                    Append_Utf8_(CP850_EXTENDED[byte - 0x80], output);
                }
                break;
            }
        }
        output->push_back('"');
    }

    const s_symbol_descriptor* Find_Symbol_Value_(const s_symbol_descriptor* symbols,
                                                  size_t count, int64_t value) {
        for (size_t index = 0; index < count; ++index) {
            if (symbols[index].value == value) {
                return &symbols[index];
            }
        }
        return nullptr;
    }

    void Append_Value_(const s_variable_descriptor& variable,
                       const s_typed_value& value, std::string* output) {
        if (value.type == CONFIG_VALUE_BOOL) {
            *output += value.bool_value ? "true" : "false";
        } else if (value.type == CONFIG_VALUE_STRING) {
            Append_Escaped_String_(value.string_value, output);
        } else if (value.type == CONFIG_VALUE_RATIO) {
            *output += std::to_string(value.ratio_numerator) + "/" +
                       std::to_string(value.ratio_denominator);
        } else if (variable.scalar_kind == CONFIG_SCALAR_ENUM) {
            const s_symbol_descriptor* symbol = Find_Symbol_Value_(
                variable.values, variable.value_count, value.integer_value);
            *output += symbol != nullptr ? symbol->name :
                       std::to_string(value.integer_value);
        } else if (variable.scalar_kind == CONFIG_SCALAR_MASK) {
            const s_symbol_descriptor* composite = Find_Symbol_Value_(
                variable.values, variable.value_count, value.integer_value);
            if (composite != nullptr) {
                *output += composite->name;
                return;
            }
            const uint64_t original = static_cast<uint64_t>(value.integer_value);
            uint64_t remaining = original;
            std::string symbolic;
            bool first = true;
            for (size_t index = 0; index < variable.flag_count; ++index) {
                const uint64_t flag = static_cast<uint64_t>(variable.flags[index].value);
                if ((remaining & flag) == 0) {
                    continue;
                }
                if (!first) {
                    symbolic.push_back(',');
                }
                symbolic += variable.flags[index].name;
                remaining &= ~flag;
                first = false;
            }
            if (remaining != 0 || first) {
                *output += std::to_string(value.integer_value);
            } else {
                *output += symbolic;
            }
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

bool Initialize_Effective_Config(s_effective_config* effective) {
    if (effective == nullptr || !Initialize_Defaults(&effective->state)) {
        return false;
    }
    effective->provenance.clear();
    effective->provenance.resize(variable_count);
    effective->prior_values.clear();
    effective->prior_values.resize(variable_count);
    return true;
}

bool Read_Effective_Native_Config(s_effective_config* effective) {
    if (effective == nullptr || !Read_Current_State(&effective->state)) {
        return false;
    }
    effective->provenance.clear();
    effective->provenance.resize(variable_count);
    effective->prior_values.clear();
    effective->prior_values.resize(variable_count);
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
            std::fprintf(stderr,
                         "configuration warning: %s:%u:%u: ignoring deferred enable '%s'\n",
                         statement.location.source.c_str(), statement.location.line,
                         statement.location.column, statement.argument.value.c_str());
            continue;
        }

        s_config_write_set statement_writes;
        if (!Decode_Assignment(&statement, &statement_writes, diagnostic)) {
            return false;
        }
        for (s_config_write& write : statement_writes.writes) {
            const size_t variable_index = write.variable_id;

            s_prior_value prior;
            if (!Read_State_Value(&candidate_state, write.variable_id, &prior.value)) {
                return Fail_(write.provenance, "configuration state accessor is missing", diagnostic);
            }
            prior.provenance = candidate_provenance[variable_index];

            s_config_write_set one_write;
            one_write.writes.push_back(write);
            s_config_state next_state;
            if (!Apply_Write_Set(&one_write, &candidate_state, &next_state, diagnostic)) {
                return false;
            }
            candidate_state = next_state;
            candidate_prior_values[variable_index].push_back(std::move(prior));

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

bool Extract_Canonical_Config(const s_effective_config* effective, bool include_provenance,
                              std::string* output, s_cfg_diagnostic* diagnostic) {
    if (effective == nullptr || output == nullptr || diagnostic == nullptr) {
        return false;
    }
    output->clear();
    *diagnostic = {};
    if (effective->state.values.size() != variable_count ||
        effective->provenance.size() != variable_count ||
        effective->prior_values.size() != variable_count) {
        diagnostic->message = "effective configuration is not initialized";
        return false;
    }

    for (t_variable_id variable_id = 0; variable_id < variable_count; ++variable_id) {
        s_typed_value value;
        if (!Read_State_Value(&effective->state, variable_id, &value)) {
            diagnostic->message = "configuration state accessor is missing for '" +
                                  std::string(variables[variable_id].name) + "'";
            return false;
        }
        if (include_provenance && effective->provenance[variable_id].has_override) {
            const s_value_provenance& provenance = effective->provenance[variable_id];
            *output += "# from ";
            Append_Comment_Text_(provenance.package_id, output);
            *output += ": ";
            Append_Comment_Text_(provenance.location.source, output);
            *output += ":" + std::to_string(provenance.location.line) + "\n";
        }
        *output += variables[variable_id].name;
        *output += " = ";
        Append_Value_(variables[variable_id], value, output);
        *output += ";\n";
    }
    return true;
}

}