#include "cfg_parser.h"

#include <cctype>
#include <cstring>
#include <utility>

namespace config {
namespace {
    struct s_parser {
        const char* source_name = nullptr;
        const char* source = nullptr;
        size_t source_size = 0;
        size_t offset = 0;
        uint32_t line = 1;
        uint32_t column = 1;
        s_cfg_diagnostic* diagnostic = nullptr;
    };

    s_source_location Location_(const s_parser* parser) {
        s_source_location location;
        location.source = parser->source_name;
        location.offset = parser->offset;
        location.line = parser->line;
        location.column = parser->column;
        return location;
    }

    bool Fail_(s_parser* parser, const s_source_location& location, const std::string& message) {
        parser->diagnostic->location = location;
        parser->diagnostic->message = message;
        return false;
    }

    bool At_End_(const s_parser* parser) {
        return parser->offset >= parser->source_size;
    }

    char Current_(const s_parser* parser) {
        return At_End_(parser) ? '\0' : parser->source[parser->offset];
    }

    void Advance_(s_parser* parser) {
        if (At_End_(parser)) {
            return;
        }
        const uint8_t byte = static_cast<uint8_t>(parser->source[parser->offset]);
        if (byte == '\r') {
            ++parser->offset;
            if (!At_End_(parser) && parser->source[parser->offset] == '\n') {
                ++parser->offset;
            }
            ++parser->line;
            parser->column = 1;
        } else if (byte == '\n') {
            ++parser->offset;
            ++parser->line;
            parser->column = 1;
        } else {
            ++parser->offset;
            if ((byte & 0xC0) != 0x80) {
                ++parser->column;
            }
        }
    }

    bool Decode_Utf8_(const char* source, size_t size, size_t* offset, uint32_t* codepoint) {
        const uint8_t first = static_cast<uint8_t>(source[*offset]);
        if (first < 0x80) {
            *codepoint = first;
            ++*offset;
            return true;
        }

        size_t count = 0;
        uint32_t value = 0;
        uint32_t minimum = 0;
        if ((first & 0xE0) == 0xC0) {
            count = 2;
            value = first & 0x1F;
            minimum = 0x80;
        } else if ((first & 0xF0) == 0xE0) {
            count = 3;
            value = first & 0x0F;
            minimum = 0x800;
        } else if ((first & 0xF8) == 0xF0) {
            count = 4;
            value = first & 0x07;
            minimum = 0x10000;
        } else {
            return false;
        }
        if (*offset + count > size) {
            return false;
        }
        for (size_t index = 1; index < count; ++index) {
            const uint8_t next = static_cast<uint8_t>(source[*offset + index]);
            if ((next & 0xC0) != 0x80) {
                return false;
            }
            value = value << 6 | (next & 0x3F);
        }
        if (value < minimum || value > 0x10FFFF || (value >= 0xD800 && value <= 0xDFFF)) {
            return false;
        }
        *offset += count;
        *codepoint = value;
        return true;
    }

    bool Validate_Utf8_(s_parser* parser) {
        size_t offset = 0;
        uint32_t line = 1;
        uint32_t column = 1;
        while (offset < parser->source_size) {
            const uint8_t byte = static_cast<uint8_t>(parser->source[offset]);
            if (byte == '\r') {
                ++offset;
                if (offset < parser->source_size && parser->source[offset] == '\n') {
                    ++offset;
                }
                ++line;
                column = 1;
                continue;
            }
            if (byte == '\n') {
                ++offset;
                ++line;
                column = 1;
                continue;
            }
            const size_t invalid_offset = offset;
            uint32_t codepoint = 0;
            if (!Decode_Utf8_(parser->source, parser->source_size, &offset, &codepoint)) {
                s_source_location location;
                location.source = parser->source_name;
                location.offset = invalid_offset;
                location.line = line;
                location.column = column;
                return Fail_(parser, location, "source is not valid UTF-8");
            }
            ++column;
        }
        return true;
    }

    void Skip_Whitespace_And_Comments_(s_parser* parser) {
        while (!At_End_(parser)) {
            if (std::isspace(static_cast<uint8_t>(Current_(parser))) != 0) {
                Advance_(parser);
            } else if (Current_(parser) == '#') {
                while (!At_End_(parser) && Current_(parser) != '\r' && Current_(parser) != '\n') {
                    Advance_(parser);
                }
            } else {
                break;
            }
        }
    }

    bool Is_Identifier_Start_(char character) {
        return (character >= 'a' && character <= 'z') ||
               (character >= 'A' && character <= 'Z') || character == '_';
    }

    bool Is_Identifier_Continue_(char character) {
        return Is_Identifier_Start_(character) ||
               (character >= '0' && character <= '9') || character == '-' || character == '.';
    }

    bool Is_Identifier_(const std::string& value) {
        if (value.empty() || !Is_Identifier_Start_(value.front())) {
            return false;
        }
        for (char character : value) {
            if (!Is_Identifier_Continue_(character)) {
                return false;
            }
        }
        return true;
    }

    bool Is_Decimal_(const std::string& value, bool allow_negative) {
        size_t offset = 0;
        if (allow_negative && !value.empty() && value[0] == '-') {
            offset = 1;
        }
        if (offset == value.size()) {
            return false;
        }
        for (; offset < value.size(); ++offset) {
            if (value[offset] < '0' || value[offset] > '9') {
                return false;
            }
        }
        return true;
    }

    bool Is_Base_Integer_(const std::string& value, char prefix, const char* digits) {
        size_t offset = 0;
        if (!value.empty() && value[0] == '-') {
            offset = 1;
        }
        if (offset + 2 >= value.size() || value[offset] != '0' ||
            (value[offset + 1] != prefix && value[offset + 1] != static_cast<char>(prefix - 'a' + 'A'))) {
            return false;
        }
        offset += 2;
        for (; offset < value.size(); ++offset) {
            if (std::strchr(digits, value[offset]) == nullptr) {
                return false;
            }
        }
        return true;
    }

    bool Is_Integer_(const std::string& value) {
        return Is_Decimal_(value, true) || Is_Base_Integer_(value, 'x', "0123456789abcdefABCDEF") ||
               Is_Base_Integer_(value, 'b', "01");
    }

    bool Is_Ratio_(const std::string& value) {
        const size_t slash = value.find('/');
        return slash != std::string::npos && slash == value.rfind('/') &&
               Is_Decimal_(value.substr(0, slash), true) &&
               Is_Decimal_(value.substr(slash + 1), true);
    }

    bool Is_Mask_(const std::string& value) {
        size_t start = 0;
        bool has_comma = false;
        while (start <= value.size()) {
            const size_t comma = value.find(',', start);
            const size_t end = comma == std::string::npos ? value.size() : comma;
            if (!Is_Identifier_(value.substr(start, end - start))) {
                return false;
            }
            if (comma == std::string::npos) {
                return has_comma;
            }
            has_comma = true;
            start = comma + 1;
        }
        return false;
    }

    bool Looks_Numeric_(const std::string& value) {
        return !value.empty() && ((value[0] >= '0' && value[0] <= '9') || value[0] == '+' ||
                                  (value[0] == '-' && value.size() > 1));
    }

    bool Append_Utf8_(uint32_t codepoint, std::string* value) {
        if (codepoint > 0x10FFFF || (codepoint >= 0xD800 && codepoint <= 0xDFFF)) {
            return false;
        }
        if (codepoint <= 0x7F) {
            value->push_back(static_cast<char>(codepoint));
        } else if (codepoint <= 0x7FF) {
            value->push_back(static_cast<char>(0xC0 | codepoint >> 6));
            value->push_back(static_cast<char>(0x80 | (codepoint & 0x3F)));
        } else if (codepoint <= 0xFFFF) {
            value->push_back(static_cast<char>(0xE0 | codepoint >> 12));
            value->push_back(static_cast<char>(0x80 | (codepoint >> 6 & 0x3F)));
            value->push_back(static_cast<char>(0x80 | (codepoint & 0x3F)));
        } else {
            value->push_back(static_cast<char>(0xF0 | codepoint >> 18));
            value->push_back(static_cast<char>(0x80 | (codepoint >> 12 & 0x3F)));
            value->push_back(static_cast<char>(0x80 | (codepoint >> 6 & 0x3F)));
            value->push_back(static_cast<char>(0x80 | (codepoint & 0x3F)));
        }
        return true;
    }

    int Hex_Value_(char character) {
        if (character >= '0' && character <= '9') {
            return character - '0';
        }
        if (character >= 'a' && character <= 'f') {
            return character - 'a' + 10;
        }
        if (character >= 'A' && character <= 'F') {
            return character - 'A' + 10;
        }
        return -1;
    }

    bool Read_Unicode_Escape_(s_parser* parser, uint32_t* codepoint,
                              const s_source_location& string_location) {
        uint32_t value = 0;
        for (size_t index = 0; index < 4; ++index) {
            if (At_End_(parser)) {
                return Fail_(parser, string_location, "unterminated Unicode escape");
            }
            const int digit = Hex_Value_(Current_(parser));
            if (digit < 0) {
                return Fail_(parser, Location_(parser), "invalid Unicode escape");
            }
            value = value << 4 | static_cast<uint32_t>(digit);
            Advance_(parser);
        }

        if (value >= 0xD800 && value <= 0xDBFF) {
            if (At_End_(parser) || Current_(parser) != '\\') {
                return Fail_(parser, Location_(parser), "high surrogate requires a low surrogate");
            }
            Advance_(parser);
            if (At_End_(parser) || Current_(parser) != 'u') {
                return Fail_(parser, Location_(parser), "high surrogate requires a low surrogate");
            }
            Advance_(parser);
            uint32_t low = 0;
            for (size_t index = 0; index < 4; ++index) {
                if (At_End_(parser)) {
                    return Fail_(parser, string_location, "unterminated Unicode escape");
                }
                const int digit = Hex_Value_(Current_(parser));
                if (digit < 0) {
                    return Fail_(parser, Location_(parser), "invalid Unicode escape");
                }
                low = low << 4 | static_cast<uint32_t>(digit);
                Advance_(parser);
            }
            if (low < 0xDC00 || low > 0xDFFF) {
                return Fail_(parser, Location_(parser), "high surrogate requires a low surrogate");
            }
            value = 0x10000 + ((value - 0xD800) << 10) + (low - 0xDC00);
        } else if (value >= 0xDC00 && value <= 0xDFFF) {
            return Fail_(parser, Location_(parser), "low surrogate requires a high surrogate");
        }
        *codepoint = value;
        return true;
    }

    bool Read_String_(s_parser* parser, s_cfg_token* token) {
        token->kind = CFG_TOKEN_STRING;
        token->location = Location_(parser);
        const size_t start = parser->offset;
        Advance_(parser);
        while (!At_End_(parser) && Current_(parser) != '"') {
            const uint8_t byte = static_cast<uint8_t>(Current_(parser));
            if (byte < 0x20) {
                return Fail_(parser, Location_(parser), "unescaped control character in string");
            }
            if (Current_(parser) != '\\') {
                const size_t character_start = parser->offset;
                Advance_(parser);
                token->value.append(parser->source + character_start,
                                    parser->offset - character_start);
                continue;
            }

            Advance_(parser);
            if (At_End_(parser)) {
                return Fail_(parser, token->location, "unterminated string");
            }
            const s_source_location escape_location = Location_(parser);
            const char escape = Current_(parser);
            Advance_(parser);
            switch (escape) {
            case '"': token->value.push_back('"'); break;
            case '\\': token->value.push_back('\\'); break;
            case '/': token->value.push_back('/'); break;
            case 'b': token->value.push_back('\b'); break;
            case 'f': token->value.push_back('\f'); break;
            case 'n': token->value.push_back('\n'); break;
            case 'r': token->value.push_back('\r'); break;
            case 't': token->value.push_back('\t'); break;
            case 'u': {
                uint32_t codepoint = 0;
                if (!Read_Unicode_Escape_(parser, &codepoint, token->location)) {
                    return false;
                }
                if (!Append_Utf8_(codepoint, &token->value)) {
                    return Fail_(parser, token->location, "invalid Unicode escape");
                }
                break;
            }
            default:
                return Fail_(parser, escape_location, "invalid string escape");
            }
        }
        if (At_End_(parser)) {
            return Fail_(parser, token->location, "unterminated string");
        }
        Advance_(parser);
        token->raw.assign(parser->source + start, parser->offset - start);
        if (!At_End_(parser) && std::isspace(static_cast<uint8_t>(Current_(parser))) == 0 &&
            Current_(parser) != ';' && Current_(parser) != '#') {
            return Fail_(parser, Location_(parser), "expected whitespace after string");
        }
        return true;
    }

    bool Read_Bare_(s_parser* parser, s_cfg_token* token) {
        token->location = Location_(parser);
        const size_t start = parser->offset;
        while (!At_End_(parser) && std::isspace(static_cast<uint8_t>(Current_(parser))) == 0 &&
               Current_(parser) != ';' && Current_(parser) != '#' && Current_(parser) != '=') {
            Advance_(parser);
        }
        token->raw.assign(parser->source + start, parser->offset - start);
        token->value = token->raw;
        if (token->raw.empty()) {
            return Fail_(parser, token->location, "expected a token");
        }
        if (token->raw == "-") {
            token->kind = CFG_TOKEN_SKIP;
        } else if (Is_Integer_(token->raw)) {
            token->kind = CFG_TOKEN_INTEGER;
        } else if (Is_Ratio_(token->raw)) {
            token->kind = CFG_TOKEN_RATIO;
        } else if (Is_Mask_(token->raw)) {
            token->kind = CFG_TOKEN_MASK;
        } else if (Looks_Numeric_(token->raw)) {
            return Fail_(parser, token->location, "malformed numeric value '" + token->raw + "'");
        } else if (Is_Identifier_(token->raw)) {
            token->kind = CFG_TOKEN_IDENTIFIER;
        } else {
            return Fail_(parser, token->location, "invalid token '" + token->raw + "'");
        }
        return true;
    }

    bool Read_Value_(s_parser* parser, s_cfg_token* token) {
        if (Current_(parser) == '"') {
            return Read_String_(parser, token);
        }
        return Read_Bare_(parser, token);
    }

    bool Read_Semicolon_(s_parser* parser, const s_source_location& statement_location) {
        Skip_Whitespace_And_Comments_(parser);
        if (At_End_(parser)) {
            return Fail_(parser, statement_location, "unterminated statement; expected ';'");
        }
        if (Current_(parser) != ';') {
            return Fail_(parser, Location_(parser), "expected ';'");
        }
        Advance_(parser);
        return true;
    }

    bool Read_Stop_(s_parser* parser, s_cfg_statement* statement) {
        statement->kind = CFG_STATEMENT_STOP;
        return Read_Semicolon_(parser, statement->location);
    }

    bool Read_Include_(s_parser* parser, s_cfg_statement* statement) {
        statement->kind = CFG_STATEMENT_INCLUDE;
        Skip_Whitespace_And_Comments_(parser);
        if (At_End_(parser) || Current_(parser) == ';') {
            return Fail_(parser, statement->location, "include requires a path");
        }
        if (Current_(parser) == '"') {
            if (!Read_String_(parser, &statement->argument)) {
                return false;
            }
        } else {
            statement->argument.kind = CFG_TOKEN_PATH;
            statement->argument.location = Location_(parser);
            const size_t start = parser->offset;
            while (!At_End_(parser) &&
                   std::isspace(static_cast<uint8_t>(Current_(parser))) == 0 &&
                   Current_(parser) != ';' && Current_(parser) != '#') {
                if (Current_(parser) == '=') {
                    return Fail_(parser, Location_(parser), "invalid character in include path");
                }
                Advance_(parser);
            }
            statement->argument.raw.assign(parser->source + start, parser->offset - start);
            statement->argument.value = statement->argument.raw;
            if (statement->argument.raw.empty()) {
                return Fail_(parser, statement->argument.location, "include requires a path");
            }
        }
        return Read_Semicolon_(parser, statement->location);
    }

    bool Read_Enable_(s_parser* parser, s_cfg_statement* statement) {
        statement->kind = CFG_STATEMENT_ENABLE;
        Skip_Whitespace_And_Comments_(parser);
        if (At_End_(parser) || Current_(parser) == ';') {
            return Fail_(parser, statement->location, "enable requires a mod ID");
        }
        if (!Read_Bare_(parser, &statement->argument)) {
            return false;
        }
        if (statement->argument.kind != CFG_TOKEN_IDENTIFIER) {
            return Fail_(parser, statement->argument.location, "enable requires a symbolic mod ID");
        }
        return Read_Semicolon_(parser, statement->location);
    }

    bool Is_Path_Token_(const s_cfg_token& token) {
        return token.kind == CFG_TOKEN_IDENTIFIER ||
               (token.kind == CFG_TOKEN_INTEGER && Is_Decimal_(token.raw, false));
    }

    bool Read_Assignment_(s_parser* parser, s_cfg_statement* statement, s_cfg_token&& first) {
        statement->kind = CFG_STATEMENT_ASSIGNMENT;
        if (!Is_Path_Token_(first)) {
            return Fail_(parser, first.location, "path tokens must be names or decimal indexes");
        }
        statement->path.push_back(std::move(first));

        while (true) {
            Skip_Whitespace_And_Comments_(parser);
            if (At_End_(parser)) {
                return Fail_(parser, statement->location, "unterminated assignment; expected '='");
            }
            if (Current_(parser) == '=') {
                Advance_(parser);
                break;
            }
            if (Current_(parser) == ';') {
                return Fail_(parser, Location_(parser), "assignment is missing '='");
            }
            s_cfg_token token;
            if (!Read_Bare_(parser, &token)) {
                return false;
            }
            if (!Is_Path_Token_(token)) {
                return Fail_(parser, token.location, "path tokens must be names or decimal indexes");
            }
            statement->path.push_back(std::move(token));
        }

        while (true) {
            Skip_Whitespace_And_Comments_(parser);
            if (At_End_(parser)) {
                return Fail_(parser, statement->location, "unterminated assignment; expected ';'");
            }
            if (Current_(parser) == ';') {
                if (statement->values.empty()) {
                    return Fail_(parser, Location_(parser), "assignment requires at least one value");
                }
                Advance_(parser);
                return true;
            }
            if (Current_(parser) == '=') {
                return Fail_(parser, Location_(parser), "unexpected '=' in assignment values");
            }
            statement->values.emplace_back();
            if (!Read_Value_(parser, &statement->values.back())) {
                return false;
            }
        }
    }

    bool Read_Statement_(s_parser* parser, s_cfg_statement* statement) {
        statement->location = Location_(parser);
        if (Current_(parser) == ';') {
            return Fail_(parser, statement->location, "empty statements are not supported");
        }
        if (Current_(parser) == '"') {
            return Fail_(parser, statement->location, "a statement must begin with a name");
        }

        s_cfg_token first;
        if (!Read_Bare_(parser, &first)) {
            return false;
        }
        if (first.kind != CFG_TOKEN_IDENTIFIER) {
            return Fail_(parser, first.location, "a statement must begin with a name");
        }
        if (first.raw == "stop") {
            return Read_Stop_(parser, statement);
        }
        if (first.raw == "enable") {
            return Read_Enable_(parser, statement);
        }
        if (first.raw == "include") {
            return Read_Include_(parser, statement);
        }
        return Read_Assignment_(parser, statement, std::move(first));
    }
}

bool Parse_Cfg(const char* source_name, const char* source, size_t source_size,
               s_cfg_document* document, s_cfg_diagnostic* diagnostic) {
    if (source_name == nullptr || source == nullptr || document == nullptr || diagnostic == nullptr) {
        return false;
    }

    document->statements.clear();
    *diagnostic = {};
    s_parser parser;
    parser.source_name = source_name;
    parser.source = source;
    parser.source_size = source_size;
    parser.diagnostic = diagnostic;
    if (!Validate_Utf8_(&parser)) {
        return false;
    }

    while (true) {
        Skip_Whitespace_And_Comments_(&parser);
        if (At_End_(&parser)) {
            return true;
        }
        document->statements.emplace_back();
        document->statements.back().statement_number =
            static_cast<uint32_t>(document->statements.size());
        if (!Read_Statement_(&parser, &document->statements.back())) {
            document->statements.clear();
            return false;
        }
    }
}

}