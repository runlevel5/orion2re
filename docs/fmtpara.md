# Custom Text Formatting Engine (`fmtpara.cpp`)

## Introduction
The formatting engine implemented in `fmtpara.cpp` is a highly customized, inline control-code-based text rendering system. It processes standard ASCII control characters, proprietary binary formatting codes, and complex function sequences to provide robust layout management, dynamic string insertion, conditional logic, and styling.

This documentation covers all available styling constructs, standard tags, advanced modifiers, and inline functions.

---

## Standard Control Characters
The engine responds to standard ASCII control codes (values under `0x17`) to manage basic layout and text flow.

* **`\0` (0x00) NUL**: Ends the string or pops the current nesting level.
  * *Effect*: Stops rendering the current text block immediately.
  * *Example*: `"Text\0"`
* **`\a` (0x07) Bell**: Initiates a function sequence.
  * *Effect*: Signals the engine to parse the following characters as layout or styling commands rather than printable text.
  * *Example*: `"\aF1.Text"`
* **`\b` (0x08) Backspace**: Registers a valid wrap point within a word.
  * *Effect*: Allows a long word to be split across two lines. If the word wraps at this point, a hyphen (`-`) is automatically inserted.
  * *Example*: `"Hyphen\bated"`
* **`\t` (0x09) Tab**: Advances the cursor horizontally.
  * *Effect*: Pushes the text cursor to the next predefined tab coordinate, aligning text into neat columns.
  * *Example*: `"Column 1\tColumn 2"`
* **`\n` (0x0A) Line Feed** & **`\r` (0x0D) Carriage Return**: Paragraph break.
  * *Effect*: Immediately moves the cursor to the beginning of the next line, resetting alignment and starting a new paragraph.
  * *Example*: `"Line 1\nLine 2"`
* **`\v` (0x0B) Vertical Tab**: Moves the text cursor down by the current line height.
  * *Effect*: Drops the text down by one line without resetting paragraph-level settings like indentation.
  * *Example*: `"Line 1\vLine 2"`
* **`\f` (0x0C) Form Feed**: Moves the cursor down by the paragraph advance height value.
  * *Effect*: Creates a large vertical gap, separating distinct blocks of text.
  * *Example*: `"Paragraph 1\fParagraph 2"`

---

## Extended Binary Commands

Commands starting at `0x17` (`\x17`) up to `0x1F` (`\x1F`) provide advanced inline state management, formatting, and dynamic macro evaluation. These are typically embedded as raw hexadecimal bytes.

### Set Value Modifiers (`\x17`)
Adjusts paragraph constraints based on where the text cursor currently is. Followed by a specific character.

* **`\x17I`**: Sets first-line indent.
  * *Effect*: All subsequent paragraphs will automatically push their very first line inward to match the cursor's current horizontal position.
  * *Example*: `"    \x17IText"`
* **`\x17L`**: Sets the left indent.
  * *Effect*: Creates a permanent left margin. All lines of text after this point will align their left edge to the cursor's current position.
  * *Example*: `"  \x17LText"`
* **`\x17R`**: Sets the right indent.
  * *Effect*: Restricts text from flowing past the cursor's current horizontal position, effectively narrowing the text column and forcing earlier line wraps.
  * *Example*: `"Text\x17R"`
* **`\x17T`**: Adds a tab stop.
  * *Effect*: Memorizes the current horizontal position so future `\t` characters can align text exactly to this spot.
  * *Example*: `"    \x17T"`
* **`\x17i`**: Clears first-line indent.
  * *Effect*: Reverts the first line of future paragraphs to start flush with the left margin.
  * *Example*: `"\x17iText"`
* **`\x17l`** / **`\x17m`**: Clears left indent (and right indent for `m`).
  * *Effect*: Removes custom left margins, allowing text to stretch back to the original left boundary.
  * *Example*: `"\x17lText"`
* **`\x17r`**: Clears right indent.
  * *Effect*: Removes custom right margins, allowing text to fill the full original width.
  * *Example*: `"\x17rText"`
* **`\x17t`**: Clears all tab stops.
  * *Effect*: Removes all memorized tab positions.
  * *Example*: `"\x17tText"`

### Dynamic Items and Macros (`\x18`)
Injects variables, ordinals, and handles conditional capitalization. Depending on the letter, it requires a hex argument `0-F` referring to a variable index.

* **`\x18A<arg>`**: Inserts a string argument.
  * *Effect*: Replaces the command with a text string stored in the engine's variables.
  * *Example*: `"Hello \x18A0!"` (Becomes "Hello Player!")
* **`\x18B<arg>`**: Inserts a number argument.
  * *Effect*: Prints the numeric value of the variable.
  * *Example*: `"You have \x18B1 coins."` (Becomes "You have 5 coins.")
* **`\x18C<arg>`**: Inserts an ordinal number.
  * *Effect*: Converts a number into its shorthand ordinal form (e.g., 1 becomes "1st", 2 becomes "2nd").
  * *Example*: `"You are in \x18C0 place."`
* **`\x18D<arg>`**: Inserts an ordinal word.
  * *Effect*: Converts a number into its fully spelled-out ordinal form (e.g., 1 becomes "First").
  * *Example*: `"The \x18D0 stage."`
* **`\x18E`**: Inserts "s" conditionally.
  * *Effect*: Appends an "s" only if the global plural flag is currently active. Useful for dynamic grammar.
  * *Example*: `"Apple\x18E"` (Becomes "Apple" or "Apples")
* **`\x18F<arg>`**: Inserts "s" conditionally based on argument.
  * *Effect*: Appends an "s" if the specified variable is anything other than exactly 1.
  * *Example*: `"Car\x18F0"`
* **`\x18G`**: Inserts "es" conditionally.
  * *Effect*: Appends "es" if the global plural flag is active.
  * *Example*: `"Box\x18G"` (Becomes "Box" or "Boxes")
* **`\x18H<arg>`**: Inserts "es" conditionally based on argument.
  * *Effect*: Appends "es" if the specified variable is not exactly 1.
  * *Example*: `"Match\x18H0"`
* **`\x18I<arg>`**: Inserts a string conditionally (plural).
  * *Effect*: Prints the string variable only if the plural flag is active.
  * *Example*: `"\x18I0"`
* **`\x18Q`**: Switches the text mode to UPPERCASE.
  * *Effect*: Forces all subsequent text to render in capital letters, regardless of how it was typed.
  * *Example*: `"\x18Qshouting\x18T"`
* **`\x18R`**: Switches the text mode to lowercase.
  * *Effect*: Forces all subsequent text to render in lowercase letters.
  * *Example*: `"\x18RWHISPERING\x18T"`
* **`\x18S`**: Switches the text mode to Title Case.
  * *Effect*: Capitalizes the first letter of subsequent words.
  * *Example*: `"\x18Stitle casing test\x18T"`
* **`\x18T`**: Restores normal casing.
  * *Effect*: Disables `\x18Q`, `\x18R`, or `\x18S`, returning text to its original typed casing.
  * *Example*: `"\x18QBIG \x18TNormal"`

### Conditionals (`\x19`)
Evaluates variables and sets a global `cond_flag`. The command is followed by a condition operator (`0`-`A`).

* **`\x190<arg>`**: `cond_flag = (val1 != 0)`
  * *Effect*: Checks if a variable is non-zero, saving the result (true/false) to be used by later commands (like `\x1FL` to exit text).
  * *Example*: `"\x1900"`
* **`\x192`**: `cond_flag = !cond_flag`
  * *Effect*: Flips the current true/false state of the conditional flag.
  * *Example*: `"\x192"`

### Text Justification (`\x1A`)
Changes inline paragraph justification mode. Followed by a numeric character `0-3`.

* **`\x1A0`**: Left Align
  * *Effect*: Aligns all text to the left margin, leaving a ragged right edge.
  * *Example*: `"\x1A0Left aligned"`
* **`\x1A1`**: Right Align
  * *Effect*: Aligns all text flush against the right margin, leaving a ragged left edge.
  * *Example*: `"\x1A1Right aligned"`
* **`\x1A2`**: Center Align
  * *Effect*: Centers each line of text evenly between the left and right margins.
  * *Example*: `"\x1A2Centered"`
* **`\x1A3`**: Full Justification
  * *Effect*: Adjusts spacing between words so that text sits flush against both the left and right margins simultaneously.
  * *Example*: `"\x1A3Fully justified paragraph"`

### Color Attributes (`\x1B`)
Sets the current color attribute by reading the subsequent byte and bit-shifting it left by 5.

* *Effect*: Applies specific palette highlights or visual states (like disabled/grayed-out text) depending on the font's color definitions.
* *Example*: `"\x1B\x01Text"`

### Color Styles (`\x1C`)
Sets the text foreground and background color based on an index `0-5`.

* **`\x1C<0-5>`**: Sets the foreground color to the specific index.
  * *Effect*: Instantly changes the color of the text being drawn (e.g., from white to red).
  * *Example*: `"\x1C2Red Text"`
* **`\x1C6<0-7>`**: Sets the background color to the specific index.
  * *Effect*: Draws a colored highlight box behind the text.
  * *Example*: `"\x1C61Blue Background"`

### Quit Context (`\x1F`)
Allows early exit from paragraphs or nesting based on conditional logic.

* **`\x1FL`**: Exits the current nesting level if `cond_flag` is true.
  * *Effect*: Aborts drawing the current string and returns to the parent string, acting as an `if` statement for text rendering.
  * *Example*: `"\x1FL"`
* **`\x1FP`**: Quits processing the current paragraph entirely.
  * *Effect*: Immediately stops rendering any further text in the current block.
  * *Example*: `"\x1FP"`

---

## Function Sequences (`\a`)
Function sequences are parsed when the `\a` (bell) character is encountered. Commands are identified by letters and can take numeric arguments, absolute or relative (`+` / `-`). Multiple arguments are separated by commas `,`. The sequence ends with a period `.` or any illegal function character.

### `FN_Set_Variable_` (`=`)
Sets or updates internal item variables. Argument 1 sets the variable index, Argument 2 sets or updates the value.
* *Effect*: Allows the text string to write data back into the game engine's variables, storing numbers for later conditional checks.
* *Example*: `"\a=0,=5.Text"` (Sets variable 0 to 5)
* *Example*: `"\a=0,=+2.Text"` (Adds 2 to variable 0)

### `FN_User_Call_` (`C`)
Triggers a custom user callback in the game's code. Argument 1 sets an auxiliary user arg, Argument 2 invokes the function.
* *Effect*: Allows text rendering to trigger physical game events (e.g., playing a sound effect when a specific word appears on screen).
* *Example*: `"\aC1,C100.Text"`

### `FN_Set_Font_` (`F`, `R`)
Sets the active standard font (`F`) or highlighted font (`R`) to an ID between 0 and 5.
* *Effect*: Instantly switches the typeface, size, or style (e.g., from normal text to a bold header font).
* *Example*: `"\aF2.Text"` (Sets standard font to 2)

### `FN_Set_H_Spacing_` (`H`)
Adjusts the horizontal font spacing value (bounds: 0-63).
* *Effect*: Increases or decreases the blank pixel padding between every individual letter (tracking/letter-spacing).
* *Example*: `"\aH+2.Text"` (Increases horizontal spacing by 2)

### `FN_Set_Indent_` (`I`)
Sets the first line indent relative to the base left indent.
* *Effect*: Pushes the very first line of a paragraph inward to create a traditional paragraph indent.
* *Example*: `"\aI10.Text"` (Sets first line indent to 10 pixels)

### `FN_Set_Margins_` (`M`)
Sets paragraph margins. Argument specifies the value, followed by the side indicator (`1` for left, otherwise right).
* *Effect*: Redefines the invisible left and right walls that constrain where text can wrap.
* *Example*: `"\aM15,1.Text"` (Sets left margin to 15)

### `FN_Set_Origin_` (`O`)
Modifies the bounding box parameters. Takes a value and an axis (`1`=X, `2`=Y, `3`=Width).
* *Effect*: Moves the entire text block globally, or changes the maximum allowed width of the text area.
* *Example*: `"\aO200,3.Text"` (Sets maximum width to 200)

### `FN_Set_FF_Size_` (`P`)
Sets the form-feed vertical advance size (bounds: 0-63).
* *Effect*: Changes exactly how many pixels the cursor drops down when encountering a `\f` (Form Feed) character.
* *Example*: `"\aP20.Text"`

### `FN_Set_Space_Size_` (`S`)
Sets the width of the spacebar character (bounds: 0-63).
* *Effect*: Adjusts how wide the gap is between words.
* *Example*: `"\aS5.Text"`

### `FN_Set_Tabs_` (`T`)
Creates a new tab stop at the defined coordinate. Passing `T` with no arguments clears all tabs.
* *Effect*: Defines explicit horizontal column locations for aligning text via `\t`.
* *Example*: `"\aT.Text"` (Clears tabs)
* *Example*: `"\aT50,T100.Text"` (Sets tabs at X=50 and X=100)

### `FN_Set_V_Spacing_` (`V`)
Sets the vertical spacing (line advance Y) (bounds: 0-63).
* *Effect*: Increases or decreases the blank pixel padding between horizontal lines of text (leading).
* *Example*: `"\aV12.Text"`

### `FN_Set_X_Pos_` (`X`)
Adjusts the text cursor's X position.
* *Effect*: Instantly teleports the text cursor horizontally, creating gaps or overlapping text.
* *Example*: `"\aX+10.Text"` (Moves cursor forward 10 pixels)

### `FN_Set_Y_Pos_` (`Y`)
Adjusts the paragraph's Y position within the origin bounds.
* *Effect*: Instantly teleports the text cursor vertically, moving text up or down relative to its normal baseline.
* *Example*: `"\aY50.Text"`

### `FN_Superscript_` (`^`)
Applies a superscript offset by shifting the font ID downwards. Passing `0` restores the previous style.
* *Effect*: Renders text smaller and higher up, typically used for exponents, footnotes, or trademark symbols.
* *Example*: `"Base\a^1.Superscript\a^0.Base"`

---

## Professional Summary

The `fmtpara.cpp` formatting engine offers an incredibly comprehensive, binary-compact markup language designed for real-time text rendering. It provides all standard typographic features such as varied alignments, tab stops, indentation, and letter spacing, whilst simultaneously supporting complex macro-evaluations for conditional pluralization and dynamic value injection. 

Its unique blend of standard control sequences (`\n`, `\t`), ASCII-driven structural functions (`\a`), and binary byte-modifiers (`\x17` - `\x1F`) ensures that memory footprint remains minimal while layout precision is maximized. This single-pass system bridges the gap between text layout and game logic seamlessly via user callbacks and variable handling.
