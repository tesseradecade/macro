# Macro Argument Type

## Ordinary Argument

```text
hello <name>!
```

Adds the record (name:data) to container, no boundaries

## Anything argument

```text
hello <1>!
```

Checks for at least one symbol for the argument, no data storage. Examples: \[`hello macro!` -> `1`\], \[`hello !` -> `0`\]

## Ignore argument

```text
hello <!>!
```

Argument existence is not required, no data storage. Examples: \[`hello macro!` -> `1`\], \[`hello !` -> `1`\], \[`hello!` -> `0`\]