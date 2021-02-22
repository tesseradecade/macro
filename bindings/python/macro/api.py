import cmacro
import typing


class Pattern:
    def __init__(self, pattern: str):
        self.pattern = cmacro.macro_pattern(pattern)

    def check(self, real: str, as_json: bool = False) -> typing.Union[str, dict, None]:
        return cmacro.macro_parse(real, self.pattern, as_json)


def match(
        pattern: str, real: str, as_json: bool = False
) -> typing.Optional[str]:
    return cmacro.macro_match(pattern, real, as_json)
