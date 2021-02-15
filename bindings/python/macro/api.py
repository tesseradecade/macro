import cmacro
import typing


def match(
        pattern: str,
        real: str,
        as_json: bool = False
) -> typing.Union[str, dict]:
    return cmacro.macro_match(pattern, real, as_json)
