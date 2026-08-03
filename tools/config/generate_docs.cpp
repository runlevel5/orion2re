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
            parameter.name_space + "." + parameter.id;
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

    std::string Type_Link_(const s_schema* schema, const s_documented_type& type) {
        if (!type.reference.empty()) {
            const std::string name = Html_(type.reference);
            if (Public_Type_(schema, type.reference)) {
                return "<a href=\"#" + Anchor_("type", type.reference) + "\"><code>" +
                       name + "</code></a>";
            }
            if (Public_Record_(schema, type.reference)) {
                return "<a href=\"#" + Anchor_("record", type.reference) + "\"><code>" +
                       name + "</code></a>";
            }
            return "<code>" + name + "</code>";
        }
        if (type.node_kind == NODE_ARRAY) {
            const std::string element = type.array_element != nullptr ?
                Type_Link_(schema, *type.array_element) : "value";
            return "array of " + std::to_string(type.array_count) + " " + element +
                   " values";
        }
        if (type.node_kind == NODE_RECORD) {
            return "record";
        }
        switch (type.scalar_kind) {
        case SCALAR_ENUM: return "named choice";
        case SCALAR_MASK: return "one or more flags";
        case SCALAR_BITMAP: return "one or more flags";
        case SCALAR_RATIO: return "ratio";
        case SCALAR_PRIMITIVE:
            if (type.primitive == PRIMITIVE_BOOL) {
                return "on or off";
            }
            if (type.primitive == PRIMITIVE_STRING) {
                return "text";
            }
            return "number";
        }
        return "value";
    }

    void Metadata_(const s_metadata& metadata, std::string* output) {
        if (!metadata.description.empty()) {
            *output += "<p>" + Html_(metadata.description) + "</p>";
        }
        if (!metadata.remarks.empty()) {
            *output += "<div class=\"note\"><strong>Notes:</strong> " +
                       Html_(metadata.remarks) + "</div>";
        }
    }

    void Constraints_(const s_constraints& constraints, std::string* output) {
        if (!constraints.has_min && !constraints.has_max &&
            !constraints.has_min_length && !constraints.has_max_length) {
            return;
        }
        *output += "<dt>Allowed range</dt><dd>";
        if (constraints.has_min_length || constraints.has_max_length) {
            *output += constraints.has_min_length ? std::to_string(constraints.min_length) : "0";
            *output += " to ";
            *output += constraints.has_max_length ?
                std::to_string(constraints.max_length) : "any number of";
            *output += " characters";
        } else {
            *output += constraints.has_min ? std::to_string(constraints.min) : "minimum";
            *output += " to ";
            *output += constraints.has_max ? std::to_string(constraints.max) : "maximum";
        }
        *output += "</dd>";
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
        *output += "<h4>Examples</h4>";
        for (const std::string& example : metadata.examples) {
            *output += "<pre><code>" + Html_(example) + "</code></pre>";
        }
    }

    void Type_Details_(const s_schema* schema, const s_documented_type& type,
                       std::string* output) {
        *output += "<dl class=\"facts\"><dt>Value</dt><dd>" +
                   Type_Link_(schema, type) + "</dd>";
        if (type.node_kind == NODE_SCALAR) {
            if (type.scalar_kind == SCALAR_BITMAP) {
                *output += "<dt>Storage</dt><dd>" +
                           std::to_string(type.bitmap_bit_count) + " bits</dd>";
            } else {
                *output += "<dt>Storage</dt><dd><code>" +
                           Html_(Primitive_Name(type.primitive)) + "</code></dd>";
            }
        }
        Constraints_(type.constraints, output);
        if (type.scalar_kind == SCALAR_ENUM) {
            *output += "<dt>Numbers</dt><dd>";
            *output += type.allow_numeric ? "Names or numbers are accepted." :
                                             "Use the names shown below.";
            if (type.reject_unknown_numeric) {
                *output += " Unknown numbers are rejected.";
            }
            *output += "</dd>";
        } else if (type.scalar_kind == SCALAR_MASK) {
            *output += "<dt>Combining flags</dt><dd>Separate multiple flag names with commas.";
            if (type.reject_unknown_numeric) {
                *output += " Unknown bits are rejected.";
            }
            *output += "</dd>";
        } else if (type.scalar_kind == SCALAR_BITMAP) {
            *output += "<dt>Combining flags</dt><dd>Separate multiple flag names with commas. Raw numeric bitmaps are not accepted.</dd>";
        }
        if (type.node_kind == NODE_ARRAY) {
            *output += "<dt>Number of entries</dt><dd>" +
                       std::to_string(type.array_count) + "</dd>";
            if (!type.array_index_type.empty()) {
                s_documented_type index_type;
                index_type.reference = type.array_index_type;
                *output += "<dt>Entry selectors</dt><dd>" +
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
        *output += "<div class=\"table-wrap\"><table><thead><tr><th>Field</th><th>What it controls</th><th>Value</th><th>Default</th></tr></thead><tbody>";
        for (const s_documented_field& field : fields) {
            if (field.internal) {
                continue;
            }
            *output += "<tr><td><code>" + Html_(field.id) + "</code>";
            if (!field.metadata.title.empty()) {
                *output += "<br>" + Html_(field.metadata.title);
            }
            *output += "</td><td>" + Html_(field.metadata.description) + "</td><td>";
            if (field.type != nullptr) {
                *output += Type_Link_(schema, *field.type);
            }
            *output += "</td><td>";
            if (field.has_default) {
                *output += "<code>" + Html_(field.default_value) + "</code>";
            } else {
                *output += "Inherited";
            }
            *output += "</td></tr>";
        }
        *output += "</tbody></table></div>";
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
:root{color-scheme:light dark;--bg:#f5f1e8;--panel:#fffdf7;--ink:#25231f;--muted:#6c675d;--line:#d8d0c0;--accent:#854d0e;--accent2:#b45309;--code:#f1e8d8;--nav:#29251f;--navink:#fff7e6}*{box-sizing:border-box}html{scroll-behavior:smooth}body{margin:0;background:var(--bg);color:var(--ink);font:16px/1.58 system-ui,-apple-system,"Segoe UI",sans-serif}a{color:var(--accent);text-underline-offset:2px}code{background:var(--code);border-radius:.25rem;padding:.1rem .3rem;font:90% ui-monospace,SFMono-Regular,Consolas,monospace}pre{overflow:auto;background:#201d19;color:#fff8e8;border-radius:.5rem;padding:1rem}pre code{background:transparent;padding:0}.layout{display:grid;grid-template-columns:18rem minmax(0,1fr);min-height:100vh}.sidebar{position:sticky;top:0;height:100vh;overflow:auto;background:var(--nav);color:var(--navink);padding:1.25rem}.sidebar h1{font-size:1.15rem;margin:0 0 1rem}.sidebar a{color:var(--navink);text-decoration:none}.sidebar nav>a{display:block;padding:.35rem 0}.search-box{width:100%;border:1px solid #746b5e;border-radius:.4rem;padding:.65rem;background:#fff;color:#222}.search-results{display:none;margin:.5rem 0 1rem;padding:.35rem;background:#fff;color:#222;border-radius:.4rem;max-height:45vh;overflow:auto}.search-results.active{display:block}.search-results a{display:block;color:#472b0b;padding:.45rem;border-bottom:1px solid #e8dfd0}.search-results small{display:block;color:#6c675d}.content{max-width:74rem;padding:2rem clamp(1.2rem,4vw,4rem) 6rem}.hero{padding:clamp(1rem,4vw,3rem) 0}.hero h2{font-size:clamp(2rem,5vw,3.6rem);line-height:1.05;margin:.2rem 0}.eyebrow{text-transform:uppercase;letter-spacing:.12em;color:var(--accent2);font-weight:700}.card{background:var(--panel);border:1px solid var(--line);border-radius:.7rem;padding:1.25rem;margin:1rem 0 1.5rem;box-shadow:0 2px 10px #0000000a}.card h3{margin-top:0}.section-title{margin-top:3.5rem;border-bottom:2px solid var(--line);padding-bottom:.35rem}.badge{display:inline-block;border-radius:1rem;background:var(--code);padding:.15rem .55rem;margin:.1rem;font-size:.85rem}.facts{display:grid;grid-template-columns:max-content 1fr;gap:.25rem 1rem}.facts dt{font-weight:700}.facts dd{margin:0}.option-grid{display:grid;grid-template-columns:repeat(auto-fit,minmax(17rem,1fr));gap:.5rem}.option{display:flex;align-items:flex-start;gap:.6rem;border:1px solid var(--line);border-radius:.4rem;padding:.65rem}.option code{flex:none}.note{border-left:4px solid var(--accent2);background:var(--code);padding:.7rem 1rem;margin:1rem 0}.table-wrap{overflow:auto}table{width:100%;border-collapse:collapse}th,td{text-align:left;vertical-align:top;padding:.6rem;border-bottom:1px solid var(--line)}details{margin-top:1rem;color:var(--muted)}.links{columns:2}.empty{color:var(--muted);font-style:italic}@media(max-width:760px){.layout{display:block}.sidebar{position:relative;height:auto}.sidebar nav{columns:2}.content{padding-top:1rem}.facts{grid-template-columns:1fr}.links{columns:1}}@media(prefers-color-scheme:dark){:root{--bg:#171512;--panel:#211e19;--ink:#f6eddd;--muted:#bdb3a4;--line:#494238;--accent:#fbbf24;--accent2:#f59e0b;--code:#332b20;--nav:#0d0c0a;--navink:#fff4d8}}
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
        "<title>Orion2 configuration reference</title><style>" + Style_() +
        "</style></head><body><div class=\"layout\"><aside class=\"sidebar\">"
        "<h1><a href=\"#overview\">Orion2 settings</a></h1>"
        "<label for=\"search\">Search settings</label>"
        "<input id=\"search\" class=\"search-box\" type=\"search\" autocomplete=\"off\" "
        "placeholder=\"Press / to search\"><div id=\"search-results\" class=\"search-results\"></div>"
        "<nav><a href=\"#overview\">Overview</a><a href=\"#groups\">Groups</a>"
        "<a href=\"#parameters\">Parameters</a><a href=\"#types\">Types</a>"
        "<a href=\"#records\">Records</a></nav></aside><main class=\"content\">"
        "<header id=\"overview\" class=\"hero\"><div class=\"eyebrow\">Offline reference</div>"
        "<h2>Customize your game</h2><p>This guide explains the settings you can place in your Orion2 configuration files. "
        "Copy an example, change the named values you want, and keep this page nearby while you tune your game.</p>"
        "<div class=\"note\"><strong>Before you begin:</strong> Keep a backup of a working configuration. "
        "Names are safer and easier to read than raw numbers.</div></header>";

    output += "<h2 id=\"groups\" class=\"section-title\">Setting groups</h2>";
    for (const s_group& group : schema->groups) {
        output += "<article id=\"" + Anchor_("group", group.id) + "\" class=\"card\"><h3>" +
                  Html_(group.metadata.title) + "</h3>";
        Metadata_(group.metadata, &output);
        output += "<span class=\"badge\">" + std::string(group.save ? "Saved with games" : "Local setting") +
                  "</span><span class=\"badge\">" +
                  std::string(group.synchronize ? "Shared in multiplayer" : "Not shared in multiplayer") +
                  "</span></article>";
    }

    output += "<h2 id=\"parameters\" class=\"section-title\">Parameters</h2>";
    for (const s_parameter_declaration& parameter : schema->parameter_declarations) {
        const std::string path = Parameter_Path_(parameter);
        output += "<article id=\"" + Anchor_("parameter", parameter.id) + "\" class=\"card\"><h3>" +
                  Html_(parameter.metadata.title) + "</h3><p><code>" + Html_(path) + "</code></p>";
        Metadata_(parameter.metadata, &output);
        output += "<dl class=\"facts\"><dt>Configuration path</dt><dd><code>" +
                  Html_(path) + "</code></dd><dt>Group</dt><dd><a href=\"#" +
                  Anchor_("group", parameter.group) + "\">" + Html_(parameter.group) + "</a></dd>";
        if (parameter.has_default) {
            output += "<dt>Default</dt><dd><code>" + Html_(parameter.default_value) + "</code></dd>";
        }
        if (!parameter.metadata.units.empty()) {
            output += "<dt>Units</dt><dd>" + Html_(parameter.metadata.units) + "</dd>";
        }
        output += "</dl>";
        for (const s_group& group : schema->groups) {
            if (group.id == parameter.group) {
                output += "<span class=\"badge\">" +
                          std::string(group.save ? "Saved with games" : "Local setting") +
                          "</span><span class=\"badge\">" +
                          std::string(group.synchronize ? "Shared in multiplayer" :
                                                          "Not shared in multiplayer") +
                          "</span>";
                break;
            }
        }
        Type_Details_(schema, parameter.type, &output);
        if (parameter.type.node_kind == NODE_RECORD) {
            Fields_(schema, parameter.type.fields, &output);
        }
        Examples_(parameter.metadata, &output);
        if (!parameter.binding.header.empty() || !parameter.native_targets.empty()) {
            output += "<details><summary>Implementation details</summary><p>Source binding: <code>" +
                      Html_(parameter.binding.symbol) + "</code> in <code>" +
                      Html_(parameter.binding.header) + "</code></p>";
            if (!parameter.native_targets.empty()) {
                output += "<p>Native targets: ";
                for (size_t index = 0; index < parameter.native_targets.size(); ++index) {
                    output += index == 0 ? "" : ", ";
                    output += "<code>" + Html_(parameter.native_targets[index]) + "</code>";
                }
                output += "</p>";
            }
            output += "</details>";
        }
        output += "</article>";
    }

    output += "<h2 id=\"types\" class=\"section-title\">Named value types</h2>";
    size_t public_types = 0;
    for (const s_type_declaration& declaration : schema->type_declarations) {
        if (declaration.internal) {
            continue;
        }
        ++public_types;
        output += "<article id=\"" + Anchor_("type", declaration.id) + "\" class=\"card\"><h3>" +
                  Html_(declaration.metadata.title) + "</h3><p><code>" + Html_(declaration.id) + "</code></p>";
        Metadata_(declaration.metadata, &output);
        Type_Details_(schema, declaration.type, &output);
        Examples_(declaration.metadata, &output);
        References_(declaration.used_by, &output);
        output += "</article>";
    }
    if (public_types == 0) {
        output += "<p class=\"empty\">This schema has no public named value types.</p>";
    }

    output += "<h2 id=\"records\" class=\"section-title\">Records</h2>";
    size_t public_records = 0;
    for (const s_record_declaration& declaration : schema->record_declarations) {
        if (declaration.internal) {
            continue;
        }
        ++public_records;
        output += "<article id=\"" + Anchor_("record", declaration.id) + "\" class=\"card\"><h3>" +
                  Html_(declaration.metadata.title) + "</h3><p><code>" + Html_(declaration.id) + "</code></p>";
        Metadata_(declaration.metadata, &output);
        Fields_(schema, declaration.fields, &output);
        Examples_(declaration.metadata, &output);
        References_(declaration.used_by, &output);
        output += "</article>";
    }
    if (public_records == 0) {
        output += "<p class=\"empty\">This schema has no public record types.</p>";
    }

    std::vector<std::string> search_entries;
    for (const s_group& group : schema->groups) {
        search_entries.push_back(Search_Entry_(group.id, group.id, group.metadata.title,
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
            search_entries.push_back(Search_Entry_(declaration.id, "types." + declaration.id,
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
            search_entries.push_back(Search_Entry_(declaration.id, "records." + declaration.id,
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