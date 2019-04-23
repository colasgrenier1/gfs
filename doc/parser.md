#PARSING GFS

There are 6 token types in GFS:

* `EMPTY`
* `NEWLINE`
* `SPACE`
* `WORD`
* `COMMAND`
* `BLOCK`

## The `EMPTY` Token

This token is an artfact of parsing, it should always be discarded.

## The `NEWLINE` Token

This token contains a single newline, which can be `\n` or `\r\n`. The actual value is contained in `token.content`.

## The `SPACE` Token

This token contains spaces: spaces and tabs.

## The `WORD` Token

This token contains all non-space, non-newline characters which are not part of a command. They contain punctuation.

##The `COMMAND` Token

The command token contains the name of a command. So for example in `@italic<Allo!>` the token will have `italic` in its `content` member.

## The `BLOCK` Token

Blocks are contained within angle brackets (`<` and `>`). The parsed tokens contained within it are avaiable as a `DList!Token` through its `contents` member.