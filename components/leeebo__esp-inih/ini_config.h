/*
 * SPDX-FileCopyrightText: 2022-2023 libo_go@163.com
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once
#ifdef CONFIG_IDF_TARGET
#include "sdkconfig.h"

/* Nonzero if ini_handler callback should accept lineno parameter. */
#ifdef CONFIG_INI_HANDLER_LINENO
#define INI_HANDLER_LINENO CONFIG_INI_HANDLER_LINENO
#else
#define INI_HANDLER_LINENO 0
#endif

/* Nonzero to allow multi-line value parsing, in the style of Python's
   configparser. If allowed, ini_parse() will call the handler with the same
   name for each subsequent line parsed. */
#ifdef CONFIG_INI_ALLOW_MULTILINE
#define INI_ALLOW_MULTILINE CONFIG_INI_ALLOW_MULTILINE
#else
#define INI_ALLOW_MULTILINE 0
#endif

/* Nonzero to allow a UTF-8 BOM sequence (0xEF 0xBB 0xBF) at the start of
   the file. See https://github.com/benhoyt/inih/issues/21 */
#ifdef CONFIG_INI_ALLOW_BOM
#define INI_ALLOW_BOM CONFIG_INI_ALLOW_BOM
#else
#define INI_ALLOW_BOM 0
#endif

/* Chars that begin a start-of-line comment. Per Python configparser, allow
   both ; and # comments at the start of a line by default. */
#ifdef CONFIG_INI_START_COMMENT_PREFIXES
#define INI_START_COMMENT_PREFIXES CONFIG_INI_START_COMMENT_PREFIXES
#else
#define INI_START_COMMENT_PREFIXES ";#"
#endif

/* Nonzero to allow inline comments (with valid inline comment characters
   specified by INI_INLINE_COMMENT_PREFIXES). Set to 0 to turn off and match
   Python 3.2+ configparser behaviour. */
#ifdef CONFIG_INI_ALLOW_INLINE_COMMENTS
#define INI_ALLOW_INLINE_COMMENTS CONFIG_INI_ALLOW_INLINE_COMMENTS
#else
#define INI_ALLOW_INLINE_COMMENTS 0
#endif
#ifdef CONFIG_INI_INLINE_COMMENT_PREFIXES
#define INI_INLINE_COMMENT_PREFIXES CONFIG_INI_INLINE_COMMENT_PREFIXES
#else
#define INI_INLINE_COMMENT_PREFIXES ";"
#endif

/* Nonzero to use stack for line buffer, zero to use heap (malloc/free). */
#ifdef CONFIG_INI_USE_STACK
#define INI_USE_STACK CONFIG_INI_USE_STACK
#else
#define INI_USE_STACK 0
#endif

/* Maximum line length for any line in INI file (stack or heap). Note that
   this must be 3 more than the longest line (due to '\r', '\n', and '\0'). */
#ifdef CONFIG_INI_MAX_LINE
#define INI_MAX_LINE CONFIG_INI_MAX_LINE
#else
#define INI_MAX_LINE 200
#endif

/* Nonzero to allow heap line buffer to grow via realloc(), zero for a
   fixed-size buffer of INI_MAX_LINE bytes. Only applies if INI_USE_STACK is
   zero. */
#ifdef CONFIG_INI_ALLOW_REALLOC
#define INI_ALLOW_REALLOC CONFIG_INI_ALLOW_REALLOC
#else
#define INI_ALLOW_REALLOC 0
#endif

/* Initial size in bytes for heap line buffer. Only applies if INI_USE_STACK
   is zero. */
#ifdef CONFIG_INI_INITIAL_ALLOC
#define INI_INITIAL_ALLOC CONFIG_INI_INITIAL_ALLOC
#else
#define INI_INITIAL_ALLOC 200
#endif

/* Stop parsing on first error (default is to keep parsing). */
#ifdef CONFIG_INI_STOP_ON_FIRST_ERROR
#define INI_STOP_ON_FIRST_ERROR CONFIG_INI_STOP_ON_FIRST_ERROR
#else
#define INI_STOP_ON_FIRST_ERROR 0
#endif

/* Nonzero to call the handler at the start of each new section (with
   name and value NULL). Default is to only call the handler on
   each name=value pair. */
#ifdef CONFIG_INI_CALL_HANDLER_ON_NEW_SECTION
#define INI_CALL_HANDLER_ON_NEW_SECTION CONFIG_INI_CALL_HANDLER_ON_NEW_SECTION
#else
#define INI_CALL_HANDLER_ON_NEW_SECTION 0
#endif

/* Nonzero to allow a name without a value (no '=' or ':' on the line) and
   call the handler with value NULL in this case. Default is to treat
   no-value lines as an error. */
#ifdef CONFIG_INI_ALLOW_NO_VALUE
#define INI_ALLOW_NO_VALUE CONFIG_INI_ALLOW_NO_VALUE
#else
#define INI_ALLOW_NO_VALUE 0
#endif

/* Nonzero to use custom ini_malloc, ini_free, and ini_realloc memory
   allocation functions (INI_USE_STACK must also be 0). These functions must
   have the same signatures as malloc/free/realloc and behave in a similar
   way. ini_realloc is only needed if INI_ALLOW_REALLOC is set. */
#ifdef CONFIG_INI_CUSTOM_ALLOCATOR
#define INI_CUSTOM_ALLOCATOR CONFIG_INI_CUSTOM_ALLOCATOR
#else
#define INI_CUSTOM_ALLOCATOR 0
#endif
#endif