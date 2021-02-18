# Simple util to test macro in console

try:
    from macro import Pattern
except ImportError:
    raise RuntimeError("You must build Macro Python API firstly")

pattern = None

while True:
    cmd = input((f"Pattern({pattern.pattern!r})" if pattern else "Enter pattern") + " : ")

    if pattern is None:
        if cmd == ".":
            exit(0)
        pattern = Pattern(cmd)
        continue
    elif cmd == ".":
        pattern = None
        continue

    print("Response:", pattern.check(cmd, True))
