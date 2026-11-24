"""Splits a rebuild source file into its top-level items (for tools/regroup.py).

An item is one top-level construct inside `namespace rt3 { ... }` (or at file
level): a function definition together with the RT3_REPLACE lines that follow
it, a declaration, a static helper, a type, a macro, an anonymous namespace, ...
Comments and blank lines directly above an item belong to it.
"""
import re

REPLACE = re.compile(r"^[ \t]*RT3_REPLACE\(\s*0x([0-9a-fA-F]+)\s*,\s*(\w+)\s*\)\s*;?[ \t]*$", re.M)


def _skip_ws_comments(s, i):
    n = len(s)
    while i < n:
        if s.startswith("//", i):
            j = s.find("\n", i)
            i = n if j < 0 else j + 1
        elif s.startswith("/*", i):
            j = s.find("*/", i + 2)
            i = n if j < 0 else j + 2
        elif s[i] in " \t\r\n":
            i += 1
        else:
            break
    return i


def _scan_to_end(s, i):
    """From the start of an item, return the end index of the construct."""
    n = len(s)
    if s[i] == "#":  # preprocessor line (with continuations)
        while True:
            j = s.find("\n", i)
            if j < 0:
                return n
            if s[j - 1] == "\\" or (s[j - 1] == "\r" and s[j - 2] == "\\"):
                i = j + 1
                continue
            return j + 1
    depth = 0
    paren = 0
    while i < n:
        c = s[i]
        if s.startswith("//", i):
            i = s.find("\n", i)
            if i < 0:
                return n
            continue
        if s.startswith("/*", i):
            i = s.find("*/", i + 2) + 2
            continue
        if c == '"' or c == "'":
            q = c
            i += 1
            while i < n and s[i] != q:
                i += 2 if s[i] == "\\" else 1
            i += 1
            continue
        if c == "(":
            paren += 1
        elif c == ")":
            paren -= 1
        elif c == "{":
            depth += 1
        elif c == "}":
            depth -= 1
            if depth == 0 and paren == 0:
                j = i + 1
                k = _skip_ws_comments_inline(s, j)
                if k < n and s[k] == ";":  # struct / class / array initialiser
                    return k + 1
                return j
        elif c == ";" and depth == 0 and paren == 0:
            return i + 1
        i += 1
    return n


def _skip_ws_comments_inline(s, i):
    n = len(s)
    while i < n and s[i] in " \t":
        i += 1
    return i


def items(text):
    """Yields (kind, start, end, body_start) for the top-level items of text.

    Namespace wrappers `namespace rt3 {` / `}` are reported as kind 'ns_open' /
    'ns_close'; everything else as 'item'.
    """
    out = []
    i = 0
    n = len(text)
    while i < n:
        lead = i
        i = _skip_ws_comments(text, i)
        if i >= n:
            out.append(("tail", lead, n, n))
            break
        if re.match(r"namespace\s+rt3\s*\{", text[i:]):
            j = text.index("{", i) + 1
            out.append(("ns_open", lead, j, i))
            i = j
            continue
        if text[i] == "}" and _top_close(text, i):
            out.append(("ns_close", lead, i + 1, i))
            i += 1
            continue
        mac = re.match(r"RT3_(?!REPLACE\b|NOINLINE\b)[A-Z0-9_]+\(", text[i:])
        if mac:  # macro invocation, maybe without ';'
            j = i + mac.end()
            depth = 1
            while depth:
                depth += {"(": 1, ")": -1}.get(text[j], 0)
                j += 1
            m = re.match(r"[ \t]*;", text[j:])
            end = j + (m.end() if m else 0)
        else:
            end = _scan_to_end(text, i)
        # RT3_REPLACE / RT3_* registration lines right after belong to the item
        while True:
            m = re.match(r"[ \t]*\r?\n?[ \t]*RT3_REPLACE\([^)]*\)\s*;[ \t]*\r?\n?", text[end:])
            if not m:
                break
            end += m.end()
        out.append(("item", lead, end, i))
        i = end
    return out


def _top_close(text, i):
    # A '}' at line start closing the rt3 namespace.
    j = text.rfind("\n", 0, i)
    return text[j + 1:i].strip() == ""
