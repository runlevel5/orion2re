# Custom Text Formatting Engine (`fmtpara.cpp`)

## Introduction
The formatting engine implemented in `fmtpara.cpp` is a highly customized, inline control-code-based text rendering system. It processes standard ASCII control characters, proprietary binary formatting codes, and complex function sequences to provide robust layout management, dynamic string insertion, conditional logic, and styling.

This documentation covers all available styling constructs, standard tags, advanced modifiers, and inline functions.

---

## Standard Control Characters
The engine responds to standard ASCII control codes (values under `0x17`) to manage basic layout and text flow:

* **`\0` (0x00) NUL**: Ends the string or pops the current nesting level.
  * *Example*: `"Text\0"`
* **`\a` (0x07) Bell**: Initiates a function sequence.
  * *Example*: `"\aF1.Text"`
* **`\b` (0x08) Backspace**: Registers a valid wrap point within a word, rendering a hyphen (`-`) if wrapped to the next line.
  * *Example*: `"Hyphen\bated"`
* **`\t` (0x09) Tab**: Advances the cursor horizontally to the next defined tab stop.
  * *Example*: `"Column 1\tColumn 2"`
* **`\n` (0x0A) Line Feed** & **`\r` (0x0D) Carriage Return**: Completes the current line and forces a paragraph break.
  * *Example*: `"Line 1\nLine 2"`
* **`\v` (0x0B) Vertical Tab**: Moves the text cursor down by the current line height.
  * *Example*: `"Line 1\vLine 2"`
* **`\f` (0x0C) Form Feed**: Moves the cursor down by the paragraph advance height value.
  * *Example*: `"Paragraph 1\fParagraph 2"`

---

## Extended Binary Commands

Commands starting at `0x17` (`\x17`) up to `0x1F` (`\x1F`) provide advanced inline state management, formatting, and dynamic macro evaluation.

### Set Value Modifiers (`\x17`)
Adjusts paragraph and line indents based on the current text cursor position. The command is followed by a specific character.

* **`\x17I`**: Sets first-line indent relative to the current X position.
  * *Example*: `"    \x17IText"`
* **`\x17L`**: Sets the left indent to the current X position.
  * *Example*: `"  \x17LText"`
* **`\x17R`**: Sets the right indent based on the remaining width.
  * *Example*: `"Text\x17R"`
* **`\x17T`**: Adds a tab stop at the current X position.
  * *Example*: `"    \x17T"`
* **`\x17i`**: Clears first-line indent.
  * *Example*: `"\x17iText"`
* **`\x17l`** / **`\x17m`**: Clears left indent (and right indent for `m`).
  * *Example*: `"\x17lText"`
* **`\x17r`**: Clears right indent.
  * *Example*: `"\x17rText"`
* **`\x17t`**: Clears all tab stops.
  * *Example*: `"\x17tText"`

### Dynamic Items and Macros (`\x18`)
Injects variables, ordinals, and handles conditional capitalization. Depending on the letter, it might require a hex argument `0-F` referring to a variable index.

* **`\x18A<arg>`**: Inserts a string argument.
  * *Example*: `"Hello \x18A0!"`
* **`\x18B<arg>`**: Inserts a number argument.
  * *Example*: `"You have \x18B1 coins."`
* **`\x18C<arg>`**: Inserts an ordinal number (e.g., "1st", "2nd").
  * *Example*: `"You are in \x18C0 place."`
* **`\x18D<arg>`**: Inserts an ordinal word (e.g., "First", "Second").
  * *Example*: `"The \x18D0 stage."`
* **`\x18E`**: Inserts "s" if the global plural flag is set.
  * *Example*: `"Apple\x18E"`
* **`\x18F<arg>`**: Inserts "s" if the referenced argument is not exactly 1.
  * *Example*: `"Car\x18F0"`
* **`\x18G`**: Inserts "es" if the global plural flag is set.
  * *Example*: `"Box\x18G"`
* **`\x18H<arg>`**: Inserts "es" if the referenced argument is not exactly 1.
  * *Example*: `"Match\x18H0"`
* **`\x18I<arg>`**: Inserts a string argument if the global plural flag is set.
  * *Example*: `"\x18I0"`
* **`\x18J<arg>`**: Inserts the next argument string if the referenced argument is not exactly 1.
  * *Example*: `"\x18J0"`
* **`\x18K<arg>`**: Inserts a string argument if the global plural flag is NOT set.
  * *Example*: `"\x18K0"`
* **`\x18L<arg>`**: Inserts the next argument string if the referenced argument equals 1.
  * *Example*: `"\x18L0"`
* **`\x18M<arg>`**: Inserts a string argument if the conditional flag is true.
  * *Example*: `"\x18M0"`
* **`\x18N<arg>`**: Inserts the next argument string if the referenced argument is not 0.
  * *Example*: `"\x18N0"`
* **`\x18O<arg>`**: Inserts a string argument if the conditional flag is false.
  * *Example*: `"\x18O0"`
* **`\x18P<arg>`**: Inserts the next argument string if the referenced argument is 0.
  * *Example*: `"\x18P0"`
* **`\x18Q`**: Switches the text mode to UPPERCASE.
  * *Example*: `"\x18Qshouting\x18T"`
* **`\x18R`**: Switches the text mode to lowercase.
  * *Example*: `"\x18RWHISPERING\x18T"`
* **`\x18S`**: Switches the text mode to Title Case.
  * *Example*: `"\x18Stitle casing test\x18T"`
* **`\x18T`**: Restores normal casing.
  * *Example*: `"\x18QBIG \x18TNormal"`

### Conditionals (`\x19`)
Evaluates variables and sets a global `cond_flag`. The command is followed by a condition operator (`0`-`A`).

* **`\x190<arg>`**: `cond_flag = (val1 != 0)`
  * *Example*: `"\x1900"`
* **`\x191`**: `cond_flag = use_plural`
  * *Example*: `"\x191"`
* **`\x192`**: `cond_flag = !cond_flag` (Inverts the flag)
  * *Example*: `"\x192"`
* **`\x193<arg>`**: `Set_Item_Value(arg, cond_flag)`
  * *Example*: `"\x1930"`
* **`\x194<arg>`**: `Set_Item_Value(arg, val1)`
  * *Example*: `"\x1940"`
* **`\x195<arg>`**: `cond_flag = (val2 == val1)`
  * *Example*: `"\x1950"`
* **`\x196<arg>`**: `cond_flag = (val2 != val1)`
  * *Example*: `"\x1960"`
* **`\x197<arg>`**: `cond_flag = (val2 > val1)`
  * *Example*: `"\x1970"`
* **`\x198<arg>`**: `cond_flag = (val2 < val1)`
  * *Example*: `"\x1980"`
* **`\x199<arg>`**: `cond_flag = (val2 >= val1)`
  * *Example*: `"\x1990"`
* **`\x19A<arg>`**: `cond_flag = (val2 <= val1)`
  * *Example*: `"\x19A0"`

### Text Justification (`\x1A`)
Changes inline paragraph justification mode. Followed by a numeric character `0-3`.

* **`\x1A0`**: Left Align
  * *Example*: `"\x1A0Left aligned"`
* **`\x1A1`**: Right Align
  * *Example*: `"\x1A1Right aligned"`
* **`\x1A2`**: Center Align
  * *Example*: `"\x1A2Centered"`
* **`\x1A3`**: Full Justification
  * *Example*: `"\x1A3Fully justified paragraph"`

### Color Attributes (`\x1B`)
Sets the current color attribute by reading the subsequent byte and bit-shifting it left by 5.

* *Example*: `"\x1B\x01Text"`

### Color Styles (`\x1C`)
Sets the text foreground and background color based on an index `0-5`.

* **`\x1C<0-5>`**: Sets the foreground color to the specific index.
  * *Example*: `"\x1C2Red Text"`
* **`\x1C6<0-7>`**: Sets the background color to the specific index.
  * *Example*: `"\x1C61Blue Background"`

### Reset Parameters (`\x1D`)
Resets specific layout/font parameters based on a lookup table indexed by the next character.

* *Example*: `"\x1D\x01Text"`

### Quit Context (`\x1F`)
Allows early exit from paragraphs or nesting based on conditional logic.

* **`\x1FL`**: Exits the current nesting level if `cond_flag` is true.
  * *Example*: `"\x1FL"`
* **`\x1FP`**: Quits processing the current paragraph entirely.
  * *Example*: `"\x1FP"`

---

## Function Sequences (`\a`)
Function sequences are parsed when the `\a` (bell) character is encountered. Commands are identified by letters and can take numeric arguments, absolute or relative (`+` / `-`). Multiple arguments are separated by commas `,`. The sequence ends with a period `.` or any illegal function character.

### `FN_Set_Variable_` (`=`)
Sets or updates internal item variables. Argument 1 sets the variable index, Argument 2 sets or updates the value.
* *Example*: `"\a=0,=5.Text"` (Sets variable 0 to 5)
* *Example*: `"\a=0,=+2.Text"` (Adds 2 to variable 0)

### `FN_User_Call_` (`C`)
Triggers a custom user callback in the game's code. Argument 1 sets an auxiliary user arg, Argument 2 invokes the function.
* *Example*: `"\aC1,C100.Text"`

### `FN_Set_Font_` (`F`, `R`)
Sets the active standard font (`F`) or highlighted font (`R`) to an ID between 0 and 5.
* *Example*: `"\aF2.Text"` (Sets standard font to 2)
* *Example*: `"\aR3.Text"` (Sets highlighted font to 3)

### `FN_Set_H_Spacing_` (`H`)
Adjusts the horizontal font spacing value (bounds: 0-63).
* *Example*: `"\aH+2.Text"` (Increases horizontal spacing by 2)

### `FN_Set_Indent_` (`I`)
Sets the first line indent relative to the base left indent.
* *Example*: `"\aI10.Text"` (Sets first line indent to 10 pixels)

### `FN_Set_Margins_` (`M`)
Sets paragraph margins. Argument specifies the value, followed by the side indicator (`1` for left, otherwise right).
* *Example*: `"\aM15,1.Text"` (Sets left margin to 15)

### `FN_Set_Origin_` (`O`)
Modifies the bounding box parameters. Takes a value and an axis (`1`=X, `2`=Y, `3`=Width).
* *Example*: `"\aO200,3.Text"` (Sets maximum width to 200)

### `FN_Set_FF_Size_` (`P`)
Sets the form-feed vertical advance size (bounds: 0-63).
* *Example*: `"\aP20.Text"`

### `FN_Set_Space_Size_` (`S`)
Sets the width of the spacebar character (bounds: 0-63).
* *Example*: `"\aS5.Text"`

### `FN_Set_Tabs_` (`T`)
Creates a new tab stop at the defined coordinate. Passing `T` with no arguments clears all tabs.
* *Example*: `"\aT.Text"` (Clears tabs)
* *Example*: `"\aT50,T100.Text"` (Sets tabs at X=50 and X=100)

### `FN_Set_V_Spacing_` (`V`)
Sets the vertical spacing (line advance Y) (bounds: 0-63).
* *Example*: `"\aV12.Text"`

### `FN_Set_X_Pos_` (`X`)
Adjusts the text cursor's X position.
* *Example*: `"\aX+10.Text"` (Moves cursor forward 10 pixels)

### `FN_Set_Y_Pos_` (`Y`)
Adjusts the paragraph's Y position within the origin bounds.
* *Example*: `"\aY50.Text"`

### `FN_Superscript_` (`^`)
Applies a superscript offset by shifting the font ID downwards. Passing `0` restores the previous style.
* *Example*: `"Base\a^1.Superscript\a^0.Base"`

---

## Professional Summary

The `fmtpara.cpp` formatting engine offers an incredibly comprehensive, binary-compact markup language designed for real-time text rendering. It provides all standard typographic features such as varied alignments, tab stops, indentation, and letter spacing, whilst simultaneously supporting complex macro-evaluations for conditional pluralization and dynamic value injection. 

Its unique blend of standard control sequences (`\n`, `\t`), ASCII-driven structural functions (`\a`), and binary byte-modifiers (`\x17` - `\x1F`) ensures that memory footprint remains minimal while layout precision is maximized. This single-pass system bridges the gap between text layout and game logic seamlessly via user callbacks and variable handling.
