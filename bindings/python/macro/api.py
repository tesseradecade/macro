import cmacro
import typing


class Pattern:
    def __init__(self, pattern: str):
        args = cmacro.macro_pattern(pattern)
        self.pattern: str = args[0]
        self.args: typing.Tuple[typing.Tuple[str, str, int, str]] = args[1]
        self.length: int = args[2]

    def check(self, real: str, as_json: bool = False) -> typing.Union[str, dict, None]:
        return cmacro.macro_parse(real, self.pattern, self.args, self.length, as_json)


def match(
        pattern: str, real: str, as_json: bool = False
) -> typing.Union[str, dict, None]:
    return cmacro.macro_match(pattern, real, as_json)
