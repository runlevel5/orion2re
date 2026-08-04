#include "generate_docs.h"

#include <algorithm>
#include <filesystem>
#include <set>

#include "file_util.h"

namespace config::tool {
namespace {
    std::string Html_(const std::string& text) {
        std::string escaped;
        for (char character : text) {
            switch (character) {
            case '&': escaped += "&amp;"; break;
            case '<': escaped += "&lt;"; break;
            case '>': escaped += "&gt;"; break;
            case '"': escaped += "&quot;"; break;
            case '\'': escaped += "&#39;"; break;
            default: escaped += character; break;
            }
        }
        return escaped;
    }

    std::string Json_(const std::string& text) {
        std::string escaped = "\"";
        static const char hex[] = "0123456789abcdef";
        for (unsigned char character : text) {
            switch (character) {
            case '"': escaped += "\\\""; break;
            case '\\': escaped += "\\\\"; break;
            case '\n': escaped += "\\n"; break;
            case '\r': escaped += "\\r"; break;
            case '\t': escaped += "\\t"; break;
            case '<': escaped += "\\u003c"; break;
            case '>': escaped += "\\u003e"; break;
            case '&': escaped += "\\u0026"; break;
            default:
                if (character < 0x20) {
                    escaped += "\\u00";
                    escaped += hex[character >> 4];
                    escaped += hex[character & 0x0F];
                } else {
                    escaped += static_cast<char>(character);
                }
                break;
            }
        }
        return escaped + "\"";
    }

    std::string Anchor_(const char* kind, const std::string& id) {
        return std::string(kind) + "-" + id;
    }

    std::string Parameter_Path_(const s_parameter_declaration& parameter) {
        return parameter.name_space.empty() ? parameter.id :
            parameter.name_space + " " + parameter.id;
    }

    std::string Parameter_Prefix_(const s_parameter_declaration& parameter) {
        return parameter.name_space.empty() ? parameter.id :
            parameter.name_space + "." + parameter.id;
    }

    std::string Cfg_Node_Path_(const s_schema* schema, uint32_t node_index) {
        std::vector<std::string> parts;
        while (node_index != UINT32_MAX) {
            const s_node& node = schema->nodes[node_index];
            if (!node.id.empty()) {
                parts.push_back(node.id);
            } else if (node.parent != UINT32_MAX) {
                const s_node& array = schema->nodes[node.parent];
                std::string selector = std::to_string(node.external_index);
                for (const s_symbol& alias : array.index_aliases) {
                    if (alias.value == node.external_index) {
                        selector = alias.name;
                        break;
                    }
                }
                parts.push_back(selector);
            }
            node_index = node.parent;
        }
        std::reverse(parts.begin(), parts.end());

        std::string result;
        for (const std::string& part : parts) {
            result += result.empty() ? part : " " + part;
        }
        return result;
    }

    uint32_t Parameter_Node_(const s_schema* schema, size_t parameter_index) {
        for (size_t index = 0; index < schema->nodes.size(); ++index) {
            if (schema->nodes[index].parameter == parameter_index) {
                return static_cast<uint32_t>(index);
            }
        }
        return UINT32_MAX;
    }

    std::string Cfg_Path_(const s_schema* schema, size_t parameter_index) {
        const uint32_t node_index = Parameter_Node_(schema, parameter_index);
        if (node_index != UINT32_MAX) {
            return Cfg_Node_Path_(schema, node_index);
        }

        std::string result = schema->parameters[parameter_index].path;
        std::replace(result.begin(), result.end(), '.', ' ');
        return result;
    }

    std::string Cfg_String_(const std::string& value) {
        std::string result = "\"";
        for (char character : value) {
            switch (character) {
            case '"': result += "\\\""; break;
            case '\\': result += "\\\\"; break;
            case '\b': result += "\\b"; break;
            case '\f': result += "\\f"; break;
            case '\n': result += "\\n"; break;
            case '\r': result += "\\r"; break;
            case '\t': result += "\\t"; break;
            default: result += character; break;
            }
        }
        return result + "\"";
    }

    std::string Group_Title_(const s_schema* schema, const std::string& id) {
        for (const s_group& group : schema->groups) {
            if (group.id == id) {
                return group.metadata.title;
            }
        }
        return id;
    }

    std::string Symbolic_Integer_(const s_parameter& parameter) {
        for (const s_symbol& symbol : parameter.values) {
            if (symbol.value == parameter.default_integer) {
                return symbol.name;
            }
        }
        if (parameter.scalar_kind != SCALAR_MASK) {
            return std::to_string(parameter.default_integer);
        }

        uint64_t remaining = static_cast<uint64_t>(parameter.default_integer);
        std::string result;
        for (const s_symbol& flag : parameter.flags) {
            const uint64_t value = static_cast<uint64_t>(flag.value);
            if (value != 0 && (remaining & value) == value) {
                result += result.empty() ? flag.name : "," + flag.name;
                remaining &= ~value;
            }
        }
        return remaining == 0 && !result.empty() ? result :
            std::to_string(parameter.default_integer);
    }

    std::string Symbolic_Bitmap_(const s_parameter& parameter) {
        for (const s_bitmap_symbol& symbol : parameter.bitmap_values) {
            if (symbol.value == parameter.default_bitmap) {
                return symbol.name;
            }
        }

        std::vector<uint8_t> remaining = parameter.default_bitmap;
        std::string result;
        for (const s_bitmap_symbol& flag : parameter.bitmap_flags) {
            bool contained = flag.value.size() == remaining.size();
            bool nonzero = false;
            for (size_t index = 0; contained && index < remaining.size(); ++index) {
                contained = (remaining[index] & flag.value[index]) == flag.value[index];
                nonzero = nonzero || flag.value[index] != 0;
            }
            if (contained && nonzero) {
                result += result.empty() ? flag.name : "," + flag.name;
                for (size_t index = 0; index < remaining.size(); ++index) {
                    remaining[index] &= static_cast<uint8_t>(~flag.value[index]);
                }
            }
        }
        const bool empty = std::all_of(remaining.begin(), remaining.end(),
                                       [](uint8_t byte) { return byte == 0; });
        return empty && !result.empty() ? result : "none";
    }

    std::string Default_Value_(const s_parameter& parameter) {
        switch (parameter.scalar_kind) {
        case SCALAR_ENUM:
        case SCALAR_MASK:
            return Symbolic_Integer_(parameter);
        case SCALAR_BITMAP:
            return Symbolic_Bitmap_(parameter);
        case SCALAR_RATIO:
            return std::to_string(parameter.default_ratio_numerator) + "/" +
                   std::to_string(parameter.default_ratio_denominator);
        case SCALAR_PRIMITIVE:
            if (parameter.type == PRIMITIVE_BOOL) {
                return parameter.default_bool ? "true" : "false";
            }
            if (parameter.type == PRIMITIVE_STRING) {
                return Cfg_String_(parameter.default_string);
            }
            return std::to_string(parameter.default_integer);
        }
        return "";
    }

    bool Leaf_Record_(const s_schema* schema, uint32_t node_index) {
        if (schema->nodes[node_index].kind != NODE_RECORD) {
            return false;
        }
        bool has_fields = false;
        for (const s_node& node : schema->nodes) {
            if (node.parent != node_index) {
                continue;
            }
            has_fields = true;
            if (node.kind != NODE_SCALAR) {
                return false;
            }
        }
        return has_fields;
    }

    bool Compact_Node_(const s_schema* schema, uint32_t node_index) {
        if (Leaf_Record_(schema, node_index)) {
            return true;
        }
        if (schema->nodes[node_index].kind != NODE_ARRAY) {
            return false;
        }
        bool has_elements = false;
        for (size_t index = 0; index < schema->nodes.size(); ++index) {
            const s_node& node = schema->nodes[index];
            if (node.parent != node_index) {
                continue;
            }
            has_elements = true;
            if (node.kind != NODE_SCALAR &&
                !Leaf_Record_(schema, static_cast<uint32_t>(index))) {
                return false;
            }
        }
        return has_elements;
    }

    std::string Default_Configuration_(const s_schema* schema,
                                       const s_parameter_declaration& declaration) {
        const std::string prefix = Parameter_Prefix_(declaration);
        std::string defaults;
        size_t parameter_index = 0;
        while (parameter_index < schema->parameters.size()) {
            const s_parameter& parameter = schema->parameters[parameter_index];
            if (parameter.path != prefix && !parameter.path.starts_with(prefix + ".")) {
                ++parameter_index;
                continue;
            }

            const uint32_t node_index = Parameter_Node_(schema, parameter_index);
            uint32_t assignment_node = node_index;
            while (assignment_node != UINT32_MAX) {
                const uint32_t parent = schema->nodes[assignment_node].parent;
                if (parent == UINT32_MAX || !Compact_Node_(schema, parent) ||
                    schema->nodes[parent].first_parameter != parameter_index) {
                    break;
                }
                assignment_node = parent;
            }

            size_t last_parameter = parameter_index;
            if (assignment_node != UINT32_MAX) {
                last_parameter = schema->nodes[assignment_node].last_parameter;
            }
            while (last_parameter > parameter_index &&
                   (last_parameter >= schema->parameters.size() ||
                    (schema->parameters[last_parameter].path != prefix &&
                     !schema->parameters[last_parameter].path.starts_with(prefix + ".")))) {
                --last_parameter;
            }

            const std::string path = assignment_node == UINT32_MAX ?
                Cfg_Path_(schema, parameter_index) :
                Cfg_Node_Path_(schema, assignment_node);
            if (assignment_node != UINT32_MAX &&
                schema->nodes[assignment_node].kind == NODE_ARRAY) {
                defaults += path + " =\n";
                for (const s_node& child : schema->nodes) {
                    if (child.parent != assignment_node) {
                        continue;
                    }
                    defaults += "    ";
                    for (size_t index = child.first_parameter;
                         index <= child.last_parameter; ++index) {
                        defaults += (index == child.first_parameter ? "" : " ") +
                                    Default_Value_(schema->parameters[index]);
                    }
                    defaults += child.last_parameter == last_parameter ? ";\n" : "\n";
                }
            } else {
                defaults += path + " =";
                for (size_t index = parameter_index; index <= last_parameter; ++index) {
                    defaults += " " + Default_Value_(schema->parameters[index]);
                }
                defaults += ";\n";
            }
            parameter_index = last_parameter + 1;
        }
        return defaults;
    }

    bool Public_Type_(const s_schema* schema, const std::string& id) {
        for (const s_type_declaration& declaration : schema->type_declarations) {
            if (declaration.id == id) {
                return !declaration.internal;
            }
        }
        return false;
    }

    bool Public_Record_(const s_schema* schema, const std::string& id) {
        for (const s_record_declaration& declaration : schema->record_declarations) {
            if (declaration.id == id) {
                return !declaration.internal;
            }
        }
        return false;
    }

    std::string Type_Title_(const s_schema* schema, const std::string& id) {
        for (const s_type_declaration& declaration : schema->type_declarations) {
            if (declaration.id == id) {
                return declaration.metadata.title;
            }
        }
        for (const s_record_declaration& declaration : schema->record_declarations) {
            if (declaration.id == id) {
                return declaration.metadata.title;
            }
        }
        return id;
    }

    std::string Type_Link_(const s_schema* schema, const s_documented_type& type) {
        if (!type.reference.empty()) {
            const std::string name = Html_(Type_Title_(schema, type.reference));
            if (Public_Type_(schema, type.reference)) {
                return "<a href=\"#" + Anchor_("type", type.reference) + "\">" +
                       name + "</a>";
            }
            if (Public_Record_(schema, type.reference)) {
                return "<a href=\"#" + Anchor_("record", type.reference) + "\">" +
                       name + "</a>";
            }
            return name;
        }
        if (type.node_kind == NODE_ARRAY) {
            const std::string element = type.array_element != nullptr ?
                Type_Link_(schema, *type.array_element) : "value";
            return std::to_string(type.array_count) + " entries, each using " + element;
        }
        if (type.node_kind == NODE_RECORD) {
            return "a group of named fields";
        }
        switch (type.scalar_kind) {
        case SCALAR_ENUM: return "one of the choices below";
        case SCALAR_MASK: return "one or more options from the list below";
        case SCALAR_BITMAP: return "one or more options from the list below";
        case SCALAR_RATIO: return "a ratio";
        case SCALAR_PRIMITIVE:
            if (type.primitive == PRIMITIVE_BOOL) {
                return "<code>true</code> or <code>false</code>";
            }
            if (type.primitive == PRIMITIVE_STRING) {
                return "text";
            }
            return "number";
        }
        return "value";
    }

    void Metadata_(const s_metadata& metadata, bool include_remarks, std::string* output) {
        if (!metadata.description.empty()) {
            *output += "<p>" + Html_(metadata.description) + "</p>";
        }
        if (include_remarks && !metadata.remarks.empty()) {
            *output += "<div class=\"note\"><strong>Notes:</strong> " +
                       Html_(metadata.remarks) + "</div>";
        }
    }

    bool Primitive_Range_(e_primitive_type primitive, int64_t* minimum, int64_t* maximum) {
        switch (primitive) {
        case PRIMITIVE_INT8: *minimum = -128; *maximum = 127; return true;
        case PRIMITIVE_UINT8: *minimum = 0; *maximum = 255; return true;
        case PRIMITIVE_INT16: *minimum = -32768; *maximum = 32767; return true;
        case PRIMITIVE_UINT16: *minimum = 0; *maximum = 65535; return true;
        case PRIMITIVE_INT32: *minimum = -2147483648LL; *maximum = 2147483647; return true;
        case PRIMITIVE_UINT32: *minimum = 0; *maximum = 4294967295LL; return true;
        default: return false;
        }
    }

    void Constraints_(const s_documented_type& type, std::string* output) {
        const s_constraints& constraints = type.constraints;
        if (constraints.has_min_length || constraints.has_max_length) {
            *output += "<dt>Accepted length</dt><dd>";
            if (constraints.has_min_length && constraints.has_max_length) {
                *output += std::to_string(constraints.min_length) + " to " +
                           std::to_string(constraints.max_length) + " characters";
            } else if (constraints.has_min_length) {
                *output += "At least " + std::to_string(constraints.min_length) + " characters";
            } else {
                *output += "Up to " + std::to_string(constraints.max_length) + " characters";
            }
            *output += "</dd>";
            return;
        }

        int64_t minimum = 0;
        int64_t maximum = 0;
        if (type.node_kind != NODE_SCALAR || type.scalar_kind != SCALAR_PRIMITIVE ||
            !Primitive_Range_(type.primitive, &minimum, &maximum)) {
            return;
        }
        if (constraints.has_min) {
            minimum = constraints.min;
        }
        if (constraints.has_max) {
            maximum = constraints.max;
        }
        *output += "<dt>Accepted values</dt><dd>" + std::to_string(minimum) + " to " +
                   std::to_string(maximum) + "</dd>";
    }

    void Symbols_(const char* heading, const std::vector<s_symbol>& symbols,
                  std::string* output) {
        if (symbols.empty()) {
            return;
        }
        *output += "<h4>" + std::string(heading) + "</h4><div class=\"option-grid\">";
        for (const s_symbol& symbol : symbols) {
            *output += "<div class=\"option\"><code>" + Html_(symbol.name) + "</code>";
            if (!symbol.description.empty()) {
                *output += "<span>" + Html_(symbol.description) + "</span>";
            }
            *output += "</div>";
        }
        *output += "</div>";
    }

    void Bitmap_Symbols_(const char* heading,
                         const std::vector<s_bitmap_symbol>& symbols,
                         std::string* output) {
        if (symbols.empty()) {
            return;
        }
        *output += "<h4>" + std::string(heading) + "</h4><div class=\"option-grid\">";
        for (const s_bitmap_symbol& symbol : symbols) {
            *output += "<div class=\"option\"><code>" + Html_(symbol.name) + "</code>";
            if (!symbol.description.empty()) {
                *output += "<span>" + Html_(symbol.description) + "</span>";
            }
            *output += "</div>";
        }
        *output += "</div>";
    }

    void Examples_(const s_metadata& metadata, std::string* output) {
        if (metadata.examples.empty()) {
            return;
        }
        *output += "<h4>Examples to copy</h4>";
        for (const std::string& example : metadata.examples) {
            *output += "<pre><code>" + Html_(example) + "</code></pre>";
        }
    }

    void Type_Details_(const s_schema* schema, const s_documented_type& type,
                       std::string* output) {
        *output += "<dl class=\"facts\"><dt>What to enter</dt><dd>" +
                   Type_Link_(schema, type) + "</dd>";
        Constraints_(type, output);
        if (type.scalar_kind == SCALAR_ENUM) {
            *output += "<dt>How to write it</dt><dd>";
            *output += type.allow_numeric ?
                "Use a name shown below. The matching number is also accepted." :
                "Use a name shown below.";
            if (type.reject_unknown_numeric) {
                *output += " Other numbers are not accepted.";
            }
            *output += "</dd>";
        } else if (type.scalar_kind == SCALAR_MASK) {
            *output += "<dt>Using several options</dt><dd>Separate their names with commas.";
            if (type.reject_unknown_numeric) {
                *output += " Unlisted numeric values are not accepted.";
            }
            *output += "</dd>";
        } else if (type.scalar_kind == SCALAR_BITMAP) {
            *output += "<dt>Using several options</dt><dd>Separate their names with commas. Use names, not a numeric value.</dd>";
        }
        if (type.node_kind == NODE_ARRAY) {
            *output += "<dt>Entries required</dt><dd>" +
                       std::to_string(type.array_count) + "</dd>";
            if (!type.array_index_type.empty()) {
                s_documented_type index_type;
                index_type.reference = type.array_index_type;
                *output += "<dt>Entry names</dt><dd>Use a value from " +
                           Type_Link_(schema, index_type) + "</dd>";
            }
        }
        *output += "</dl>";
        Symbols_("Allowed choices", type.values, output);
        Symbols_("Available flags", type.flags, output);
        Bitmap_Symbols_("Allowed choices", type.bitmap_values, output);
        Bitmap_Symbols_("Available flags", type.bitmap_flags, output);
    }

    void Fields_(const s_schema* schema, const std::vector<s_documented_field>& fields,
                 std::string* output) {
        *output += "<div class=\"field-list\">";
        for (const s_documented_field& field : fields) {
            if (field.internal) {
                continue;
            }
            *output += "<section class=\"field\"><h4><code>" + Html_(field.id) + "</code>";
            if (!field.metadata.title.empty()) {
                *output += " - " + Html_(field.metadata.title);
            }
            *output += "</h4><p>" + Html_(field.metadata.description) + "</p><dl class=\"facts\">"
                       "<dt>What to enter</dt><dd>";
            if (field.type != nullptr) {
                *output += Type_Link_(schema, *field.type);
            }
            *output += "</dd>";
            if (field.has_default) {
                *output += "<dt>Default</dt><dd><code>" +
                           Html_(field.default_value) + "</code></dd>";
            }
            *output += "</dl></section>";
        }
        *output += "</div>";
    }

    void References_(const std::vector<s_documentation_reference>& references,
                     std::string* output) {
        if (references.empty()) {
            return;
        }
        *output += "<h4>Used by</h4><ul class=\"links\">";
        for (const s_documentation_reference& reference : references) {
            *output += "<li><a href=\"#" + Html_(reference.anchor) + "\">" +
                       Html_(reference.title) + "</a></li>";
        }
        *output += "</ul>";
    }

    void Search_Terms_(const s_schema* schema, const s_documented_type& type,
                       std::set<std::string>* terms) {
        if (!type.reference.empty()) {
            terms->insert(type.reference);
        }
        if (!type.array_index_type.empty()) {
            terms->insert(type.array_index_type);
        }
        for (const s_symbol& symbol : type.values) {
            terms->insert(symbol.name);
            terms->insert(symbol.description);
        }
        for (const s_symbol& symbol : type.flags) {
            terms->insert(symbol.name);
            terms->insert(symbol.description);
        }
        for (const s_bitmap_symbol& symbol : type.bitmap_values) {
            terms->insert(symbol.name);
            terms->insert(symbol.description);
        }
        for (const s_bitmap_symbol& symbol : type.bitmap_flags) {
            terms->insert(symbol.name);
            terms->insert(symbol.description);
        }
        if (type.array_element != nullptr) {
            Search_Terms_(schema, *type.array_element, terms);
        }
        if (!type.array_index_type.empty()) {
            for (const s_type_declaration& declaration : schema->type_declarations) {
                if (declaration.id == type.array_index_type) {
                    for (const s_symbol& selector : declaration.type.values) {
                        terms->insert(selector.name);
                        terms->insert(selector.description);
                    }
                }
            }
        }
        for (const s_documented_field& field : type.fields) {
            terms->insert(field.id);
            terms->insert(field.metadata.title);
            terms->insert(field.metadata.description);
            if (field.type != nullptr) {
                Search_Terms_(schema, *field.type, terms);
            }
        }
    }

    std::string Search_Entry_(const std::string& id, const std::string& path,
                              const std::string& title, const std::string& description,
                              const std::string& anchor, const std::set<std::string>& terms) {
        std::string search = id + " " + path + " " + title + " " + description;
        for (const std::string& term : terms) {
            search += " " + term;
        }
        return "{\"id\":" + Json_(id) + ",\"path\":" + Json_(path) +
               ",\"title\":" + Json_(title) + ",\"description\":" +
               Json_(description) + ",\"anchor\":" + Json_(anchor) +
               ",\"search\":" + Json_(search) + "}";
    }

    std::string Style_() {
        return R"CSS(
*{box-sizing:border-box}html{scroll-behavior:smooth}body{margin:0;background:#fff;color:rgba(0,0,0,.8);font-family:"Noto Serif","DejaVu Serif",serif;font-size:100%;line-height:1.6;word-wrap:anywhere;-moz-osx-font-smoothing:grayscale;-webkit-font-smoothing:antialiased}a{color:#2156a5;text-decoration:underline}a:hover,a:focus{color:#1d4b8f}h1,h2,h3,h4{color:#ba3925;font-family:"Open Sans","DejaVu Sans",sans-serif;font-weight:300;line-height:1.2;text-rendering:optimizeLegibility}h1{font-size:2.125em}h2{font-size:1.6875em}h3{font-size:1.375em}h4{font-size:1.125em}p{font-size:1.0625rem;line-height:1.6;margin:0 0 1.25rem}code{color:rgba(0,0,0,.9);font-family:"Droid Sans Mono","DejaVu Sans Mono",monospace;font-weight:400}pre{overflow:auto;white-space:pre-wrap;background:#f7f7f8;border-radius:4px;padding:1em;font-family:"Droid Sans Mono","DejaVu Sans Mono",monospace;font-size:.90625em;line-height:1.45}pre code{font-size:1em}.layout{width:100%;max-width:62.5em;margin:0 auto;padding:0 .9375em 3rem}.sidebar h1{color:rgba(0,0,0,.85);border-bottom:1px solid #dddddf;margin:2.25rem 0 1.25rem;padding-bottom:8px}.sidebar h1 a{color:inherit;text-decoration:none}.toc{border:1px solid #e0e0dc;border-radius:4px;background:#f8f8f7;margin-bottom:1.25em;padding:1.25em}.toc label{display:block;color:#7a2518;font-family:"Open Sans","DejaVu Sans",sans-serif;font-size:1.125em;margin-bottom:.35em}.sidebar nav{display:flex;flex-wrap:wrap;gap:.25em 1.25em;margin-top:.75em}.search-box{width:100%;border:1px solid #d5d5d5;border-radius:4px;background:#fff;color:rgba(0,0,0,.9);font:1em "Open Sans","DejaVu Sans",sans-serif;padding:.55em .7em}.search-results{display:none;background:#fff;border:1px solid #dedede;border-radius:4px;max-height:45vh;overflow:auto}.search-results.active{display:block}.search-results a{display:block;padding:.55em .7em;border-bottom:1px solid #e7e7e9;text-decoration:none}.search-results small{display:block;color:rgba(0,0,0,.6)}.hero{padding-bottom:.625em}.hero>p:first-of-type{color:rgba(0,0,0,.85);font-size:1.21875em}.section-title{border-top:1px solid #e7e7e9;margin-top:1.25em;padding-top:1em}.card{border-bottom:1px solid #e7e7e9;margin-bottom:1.25em;padding:0 0 1.25em}.card h3{margin:1em 0 .5em}.badge{display:inline-block;border-radius:4px;background:#f3f3f2;color:rgba(0,0,0,.7);font:600 .8em "Open Sans","DejaVu Sans",sans-serif;margin:0 .35em .5em 0;padding:.2em .55em}.facts{display:grid;grid-template-columns:max-content minmax(0,1fr);gap:.25em 1em;margin:1em 0 1.25em}.facts dt{font-weight:700}.facts dd{margin:0}.option-grid{display:grid;grid-template-columns:repeat(auto-fit,minmax(17rem,1fr));gap:.5em;margin-bottom:1.25em}.option{display:flex;align-items:flex-start;gap:.6em;border:1px solid #dedede;padding:.625em}.option code{flex:none}.note{border-left:1px solid #dddddf;color:rgba(0,0,0,.6);margin:1.25em 0;padding:.25em 1.125em}.field-list{display:grid;grid-template-columns:repeat(auto-fit,minmax(20rem,1fr));gap:.75em;margin-bottom:1.25em}.field{border:1px solid #dedede;padding:.75em}.field h4{margin:0 0 .5em}.field p{font-size:1em;margin-bottom:.75em}.field .facts{margin:0}.links{columns:2}.defaults{margin:1em 0 1.25em}.defaults summary{color:#2156a5;cursor:pointer;font-size:1.0625rem}.defaults pre{margin-top:.75em}.empty{color:rgba(0,0,0,.6);font-style:italic}@media(max-width:760px){.sidebar nav{display:block}.sidebar nav a{display:block;padding:.2em 0}.facts{grid-template-columns:1fr}.field-list{grid-template-columns:1fr}.links{columns:1}}@media print{.toc{display:none}.layout{max-width:none}.section-title{page-break-after:avoid}.card,pre,.field{page-break-inside:avoid}}
)CSS";
    }

    std::string Script_() {
        return R"JS(
(function(){const input=document.getElementById('search'),box=document.getElementById('search-results'),items=window.CONFIG_SEARCH_INDEX||[];function close(){box.classList.remove('active');box.innerHTML=''}function render(){const q=input.value.trim().toLowerCase();if(!q){close();return}const ranked=items.map(x=>{const id=x.id.toLowerCase(),path=x.path.toLowerCase(),text=x.search.toLowerCase();let score=id===q||path===q?0:id.startsWith(q)||path.startsWith(q)?1:text.includes(q)?2:99;return{x,score}}).filter(x=>x.score<99).sort((a,b)=>a.score-b.score||a.x.title.localeCompare(b.x.title)).slice(0,12);box.innerHTML='';ranked.forEach(r=>{const a=document.createElement('a');a.href='#'+r.x.anchor;const strong=document.createElement('strong');strong.textContent=r.x.title;a.appendChild(strong);const small=document.createElement('small');small.textContent=r.x.path;a.appendChild(small);a.addEventListener('click',close);box.appendChild(a)});box.classList.toggle('active',ranked.length>0)}input.addEventListener('input',render);document.addEventListener('keydown',e=>{if((e.key==='/'&&!e.metaKey&&!e.ctrlKey)||(e.key.toLowerCase()==='k'&&(e.ctrlKey||e.metaKey))){if(!/input|textarea/i.test(document.activeElement.tagName)){e.preventDefault();input.focus()}}else if(e.key==='Escape'){close();input.blur()}})})();
)JS";
    }
}

bool Generate_Documentation(const s_schema* schema, const char* output_path,
                            std::string* diagnostic) {
    const std::filesystem::path parent = std::filesystem::path(output_path).parent_path();
    std::error_code error;
    if (!parent.empty()) {
        std::filesystem::create_directories(parent, error);
    }
    if (error) {
        *diagnostic = parent.string() + ": unable to create documentation directory";
        return false;
    }

    std::string output = "<!doctype html><html lang=\"en\"><head><meta charset=\"utf-8\">"
        "<meta name=\"viewport\" content=\"width=device-width,initial-scale=1\">"
        "<title>Orion2 configuration reference</title>"
        "<link rel=\"stylesheet\" href=\"https://fonts.googleapis.com/css?family=Open+Sans:300,300italic,400,400italic,600,600italic%7CNoto+Serif:400,400italic,700,700italic%7CDroid+Sans+Mono:400,700\">"
        "<style>" + Style_() +
        "</style></head><body><div class=\"layout\"><aside class=\"sidebar\">"
        "<h1><a href=\"#overview\">Orion2 configuration reference</a></h1><div class=\"toc\">"
        "<label for=\"search\">Search settings</label>"
        "<input id=\"search\" class=\"search-box\" type=\"search\" autocomplete=\"off\" "
        "placeholder=\"Press / to search\"><div id=\"search-results\" class=\"search-results\"></div>"
        "<nav><a href=\"#overview\">How to use this page</a><a href=\"#groups\">Categories</a>"
        "<a href=\"#parameters\">Settings</a><a href=\"#types\">Allowed values</a>"
        "<a href=\"#records\">Setting layouts</a></nav></div></aside><main class=\"content\">"
        "<header id=\"overview\" class=\"hero\">"
        "<h2>Customize your game</h2><p>This guide explains the settings you can place in your Orion2 configuration files. "
        "Copy an example into a <code>.cfg</code> file, then change only the values you want.</p>"
        "<p>The <strong>Type in the file</strong> line gives the exact words that start a setting. "
        "Spaces separate its parts. Words shown in code style are meant to be typed exactly as shown.</p>"
        "<p>Compact assignments can set a complete layout at once. Their values follow the field order "
        "shown under <a href=\"#records\">Setting layouts</a>.</p>"
        "<div class=\"note\"><strong>Before you begin:</strong> Keep a backup of a working configuration. "
        "Prefer the listed names over numbers because they are easier to read and less error-prone.</div></header>";

    output += "<h2 id=\"groups\" class=\"section-title\">Setting categories</h2>";
    for (const s_group& group : schema->groups) {
        output += "<article id=\"" + Anchor_("group", group.id) + "\" class=\"card\"><h3>" +
                  Html_(group.metadata.title) + "</h3>";
        Metadata_(group.metadata, true, &output);
        output += "<span class=\"badge\">" + std::string(group.save ? "Saved in saved games" : "Only affects this computer") +
                  "</span><span class=\"badge\">" +
                  std::string(group.synchronize ? "Must match in multiplayer" : "May differ in multiplayer") +
                  "</span></article>";
    }

    output += "<h2 id=\"parameters\" class=\"section-title\">Settings</h2>";
    for (const s_parameter_declaration& parameter : schema->parameter_declarations) {
        const std::string path = Parameter_Path_(parameter);
        output += "<article id=\"" + Anchor_("parameter", parameter.id) + "\" class=\"card\"><h3>" +
                  Html_(parameter.metadata.title) + "</h3>";
        Metadata_(parameter.metadata, true, &output);
        output += "<dl class=\"facts\"><dt>Type in the file</dt><dd><code>" +
                  Html_(path) + "</code></dd><dt>Category</dt><dd><a href=\"#" +
                  Anchor_("group", parameter.group) + "\">" +
                  Html_(Group_Title_(schema, parameter.group)) + "</a></dd>";
        if (!parameter.metadata.units.empty()) {
            output += "<dt>Units</dt><dd>" + Html_(parameter.metadata.units) + "</dd>";
        }
        output += "</dl>";
        for (const s_group& group : schema->groups) {
            if (group.id == parameter.group) {
                output += "<span class=\"badge\">" +
                          std::string(group.save ? "Saved in saved games" : "Only affects this computer") +
                          "</span><span class=\"badge\">" +
                          std::string(group.synchronize ? "Must match in multiplayer" :
                                                          "May differ in multiplayer") +
                          "</span>";
                break;
            }
        }
        Type_Details_(schema, parameter.type, &output);
        if (parameter.type.node_kind == NODE_RECORD) {
            Fields_(schema, parameter.type.fields, &output);
        }
        const std::string defaults = Default_Configuration_(schema, parameter);
        if (!defaults.empty()) {
            output += "<details class=\"defaults\"";
            if (parameter.type.node_kind == NODE_SCALAR) {
                output += " open";
            }
            output += "><summary>Built-in default configuration</summary><pre><code>" +
                      Html_(defaults) + "</code></pre></details>";
        }
        Examples_(parameter.metadata, &output);
        output += "</article>";
    }

    output += "<h2 id=\"types\" class=\"section-title\">Allowed values</h2>"
              "<p>These lists show the names accepted by settings that offer a fixed set of choices.</p>";
    size_t public_types = 0;
    for (const s_type_declaration& declaration : schema->type_declarations) {
        if (declaration.internal) {
            continue;
        }
        ++public_types;
        output += "<article id=\"" + Anchor_("type", declaration.id) + "\" class=\"card\"><h3>" +
                  Html_(declaration.metadata.title) + "</h3>";
        Metadata_(declaration.metadata, false, &output);
        Type_Details_(schema, declaration.type, &output);
        Examples_(declaration.metadata, &output);
        References_(declaration.used_by, &output);
        output += "</article>";
    }
    if (public_types == 0) {
        output += "<p class=\"empty\">There are no additional allowed-value lists.</p>";
    }

    output += "<h2 id=\"records\" class=\"section-title\">Setting layouts</h2>"
              "<p>These entries explain the named fields used inside more detailed settings.</p>";
    size_t public_records = 0;
    for (const s_record_declaration& declaration : schema->record_declarations) {
        if (declaration.internal) {
            continue;
        }
        ++public_records;
        output += "<article id=\"" + Anchor_("record", declaration.id) + "\" class=\"card\"><h3>" +
                  Html_(declaration.metadata.title) + "</h3>";
        Metadata_(declaration.metadata, false, &output);
        Fields_(schema, declaration.fields, &output);
        Examples_(declaration.metadata, &output);
        References_(declaration.used_by, &output);
        output += "</article>";
    }
    if (public_records == 0) {
        output += "<p class=\"empty\">There are no additional setting layouts.</p>";
    }

    std::vector<std::string> search_entries;
    for (const s_group& group : schema->groups) {
        search_entries.push_back(Search_Entry_(group.id, "Category: " + group.metadata.title, group.metadata.title,
            group.metadata.description, Anchor_("group", group.id), {}));
    }
    for (const s_parameter_declaration& parameter : schema->parameter_declarations) {
        const std::string path = Parameter_Path_(parameter);
        std::set<std::string> terms = {parameter.group};
        for (const s_group& group : schema->groups) {
            if (group.id == parameter.group) {
                terms.insert(group.metadata.title);
            }
        }
        Search_Terms_(schema, parameter.type, &terms);
        search_entries.push_back(Search_Entry_(parameter.id, path,
            parameter.metadata.title, parameter.metadata.description,
            Anchor_("parameter", parameter.id), terms));
    }
    for (const s_type_declaration& declaration : schema->type_declarations) {
        if (!declaration.internal) {
            std::set<std::string> terms;
            Search_Terms_(schema, declaration.type, &terms);
            search_entries.push_back(Search_Entry_(declaration.id, "Allowed values: " + declaration.metadata.title,
                declaration.metadata.title, declaration.metadata.description,
                Anchor_("type", declaration.id), terms));
        }
    }
    for (const s_record_declaration& declaration : schema->record_declarations) {
        if (!declaration.internal) {
            std::set<std::string> terms;
            s_documented_type type;
            type.fields = declaration.fields;
            Search_Terms_(schema, type, &terms);
            search_entries.push_back(Search_Entry_(declaration.id, "Setting layout: " + declaration.metadata.title,
                declaration.metadata.title, declaration.metadata.description,
                Anchor_("record", declaration.id), terms));
        }
    }
    output += "</main></div><script>window.CONFIG_SEARCH_INDEX=[";
    for (size_t index = 0; index < search_entries.size(); ++index) {
        output += index == 0 ? "" : ",";
        output += search_entries[index];
    }
    output += "];" + Script_() + "</script></body></html>\n";
    return Write_File_If_Different(output_path, output, diagnostic);
}

}