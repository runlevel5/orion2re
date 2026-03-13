import re
with open("src/game/combat1.cpp") as f:
    data = f.read()

start = data.find("void __cdecl Do_Combat_Turn_")
if start != -1:
    end = data.find("\n}", start)
    # this will miss the closing bracket, wait, let's use a simple counter
    count = 0
    in_func = False
    out = []
    for line in data[start:].split("\n"):
        if "{" in line: count += line.count("{")
        if "}" in line: count -= line.count("}")
        out.append(line)
        if count == 0 and "}" in line: break
    print("\n".join(out))
