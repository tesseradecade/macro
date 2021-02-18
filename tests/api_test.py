from macro import Pattern, match
import json


def test_match():
    assert match("a <b>", "") is None
    assert match("a <b>", "a") is None
    assert match("a <b>", "a", as_json=True) is None
    assert match("a <b>!", "") is None
    assert match("a <b>!", "text?") is None
    assert (
        match("a <b>!..<c>", "a oh!..hello!")
        == {"b": "oh", "c": "hello!"}
        == json.loads(match("a <b>!..<c>", "a oh!..hello!", as_json=True))
    )


def test_pattern():
    assert Pattern("a <b>").length == 1
    assert Pattern("a <b> <c>").length == 2
    assert Pattern("a <b>").check("") is None
    assert Pattern("hello <name> lets <action>").check("hello @ lets code") == {"name": "@", "action": "code"}
