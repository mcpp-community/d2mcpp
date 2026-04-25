-- Reference solutions root.
-- Mirrors dslings/ structure but only contains the canonical "filled-in"
-- answers, used purely for build / run CI. Each target carries the "-ref"
-- suffix to coexist with the dslings/ targets that share the same logical
-- name (e.g. cpp11-13-long-long-0  vs  cpp11-13-long-long-0-ref).
--
-- Solutions are language-neutral by design: the comments matter only to
-- contributors, not the compiler, so we keep a single zh-style copy.

includes("cpp11/xmake.lua")
