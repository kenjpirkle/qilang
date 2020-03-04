import shutil as sh
import os

states = [
    'start_declaration',
    'seen_hash',
    'start_statement',
    'cr_state',
    'seen_forward_slash',
    'comment',
    'multi_comment',
    'multi_comment_lf',
    'multi_comment_cr',
    'multi_comment_asterisk',
    'seen_id',
    'id_list',
    'comparison',
    'dereference',
    'bitwise_comparison',
    'arithmetic_binary',
    'bitwise',
    'assign',
    'bitwise_assign',
    'member_access',
    'range_inclusive',
    'range_exclusive',
    'id',
    'underscore_id',
    'seen_close_parens',
    'seen_colon',
    'function_declaration'
]

break_out_states = [
    'lf_state',
    'declaration',
    'variable_declaration',
    'constant_declaration',
    'numeric_literal',
    'seen_symbol',
    'divide',
    'divide_assign',
    'string_literal',
    'char_literal',
    'invalid_character',
    'unexpected_indentation',
    'incomplete_expression',
    'error',
    'eof'
]

char_types = [
    'null',
    'unused',
    'lf_char',
    'cr_char',
    'space',
    'digit',
    'letter',
    'double_quote',
    'hash',
    'dollar',
    'single_quote',
    'open_parens',
    'close_parens',
    'comma',
    'colon',
    'question_mark',
    'at_symbol',
    'open_bracket',
    'close_bracket',
    'underscore',
    'open_brace',
    'close_brace',
    'tilde',
    'forward_slash',
    'asterisk',
    'symbol',
    'equals'
]

transitions = {
    # may be able to get rid of start_declaration state in exchange for a conditional move
    'start_declaration': {
        'null': 'eof',
        'unused': 'invalid_character',
        'lf_char': 'lf_state',
        'cr_char':'cr_state',
        'space': 'unexpected_indentation',
        'digit': 'invalid_character',
        'letter': 'id',
        'double_quote': 'invalid_character',
        'hash': 'seen_hash', # meta
        'dollar': 'invalid_character',
        'single_quote': 'invalid_character',
        'open_parens': 'invalid_character',
        'close_parens': 'invalid_character',
        'comma': 'invalid_character',
        'question_mark': 'invalid_character',
        'at_symbol': 'invalid_character',
        'open_bracket': 'invalid_character',
        'close_bracket': 'invalid_character',
        'underscore': 'underscore_id',
        'open_brace': 'invalid_character',
        'close_brace': 'invalid_character',
        'tilde': 'invalid_character',
        'forward_slash': 'seen_forward_slash',
        'asterisk': 'invalid_character',
        'symbol': 'invalid_character',
        'equals': 'invalid_character',
        'colon': 'invalid_character'
    },
    'start_statement': {
        'null': 'eof',
        'unused': 'invalid_character',
        'lf_char': 'lf_state',
        'cr_char':'cr_state',
        'space': 'invalid_character',
        'digit': 'numeric_literal',
        'letter': 'id',
        'double_quote': 'string_literal',
        'hash': 'seen_hash', # meta
        'dollar': 'error',
        'single_quote': 'char_literal',
        'open_parens': 'error',
        'close_parens': 'error',
        'comma': 'invalid_character',
        'question_mark': 'invalid_character',
        'at_symbol': 'invalid_character',
        'open_bracket': 'error',
        'close_bracket': 'error',
        'underscore': 'underscore_id',
        'open_brace': 'invalid_character',
        'close_brace': 'invalid_character',
        'tilde': 'invalid_character',
        'forward_slash': 'seen_forward_slash',
        'asterisk': 'invalid_character',
        'symbol': 'invalid_character',
        'equals': 'invalid_character',
        'colon': 'invalid_character'
    },
    'seen_hash': {
        'null': 'error',
        'unused': 'invalid_character',
        'lf_char': 'error',
        'cr_char':'error',
        'space': 'error',
        'digit': 'error',
        'letter': 'id',
        'double_quote': 'error',
        'hash': 'error', # meta
        'dollar': 'error',
        'single_quote': 'error',
        'open_parens': 'error',
        'close_parens': 'error',
        'comma': 'error',
        'question_mark': 'error',
        'at_symbol': 'error',
        'open_bracket': 'error',
        'close_bracket': 'error',
        'underscore': 'underscore_id',
        'open_brace': 'error',
        'close_brace': 'error',
        'tilde': 'error',
        'forward_slash': 'error',
        'asterisk': 'error',
        'symbol': 'invalid_character',
        'equals': 'invalid_character',
        'colon': 'invalid_character'
    },
    'cr_state':{
        'null': 'error',
        'unused': 'invalid_character',
        'lf_char': 'lf_state',
        'cr_char':'error',
        'space': 'error',
        'digit': 'error',
        'letter': 'error',
        'double_quote': 'error',
        'hash': 'error',
        'dollar': 'error',
        'single_quote': 'error',
        'open_parens': 'error',
        'close_parens': 'error',
        'comma': 'error',
        'question_mark': 'error',
        'at_symbol': 'error',
        'open_bracket': 'error',
        'close_bracket': 'error',
        'underscore': 'error',
        'open_brace': 'error',
        'close_brace': 'error',
        'tilde': 'error',
        'forward_slash': 'error',
        'asterisk': 'error',
        'symbol': 'invalid_character',
        'equals': 'invalid_character',
        'colon': 'invalid_character'
    },
    'seen_forward_slash': {
        'null': 'error',
        'unused': 'error',
        'lf_char': 'lf_state',
        'cr_char':'cr_state',
        'space': 'divide',
        'digit': 'error',
        'letter': 'error',
        'double_quote': 'error',
        'hash': 'error',
        'dollar': 'error',
        'single_quote': 'error',
        'open_parens': 'error',
        'close_parens': 'error',
        'comma': 'error',
        'question_mark': 'error',
        'at_symbol': 'error',
        'open_bracket': 'error',
        'close_bracket': 'error',
        'underscore': 'error',
        'open_brace': 'error',
        'close_brace': 'error',
        'tilde': 'error',
        'forward_slash': 'comment',
        'asterisk': 'multi_comment',
        'symbol': 'invalid_character',
        'equals': 'divide_assign',
        'colon': 'invalid_character'
    },
    'seen_colon': {
        'null': 'eof',
        'unused': 'invalid_character',
        'lf_char': 'lf_state',
        'cr_char':'cr_state',
        'space': 'start_declaration',
        'digit': 'error',
        'letter': 'id',
        'double_quote': 'error',
        'hash': 'error',
        'dollar': 'error',
        'single_quote': 'error',
        'open_parens': 'error',
        'close_parens': 'error',
        'comma': 'error',
        'question_mark': 'error',
        'at_symbol': 'error',
        'open_bracket': 'error',
        'close_bracket': 'error',
        'underscore': 'underscore_id',
        'open_brace': 'error',
        'close_brace': 'error',
        'tilde': 'error',
        'forward_slash': 'seen_forward_slash',
        'asterisk': 'error',
        'symbol': 'error',
        'equals': 'variable_declaration',
        'colon': 'constant_declaration'
    },
    'comment': {
        'null': 'eof',
        'unused': 'comment',
        'lf_char': 'lf_state',
        'cr_char':'cr_state',
        'space': 'comment',
        'digit': 'comment',
        'letter': 'comment',
        'double_quote': 'comment',
        'hash': 'comment',
        'dollar': 'comment',
        'single_quote': 'comment',
        'open_parens': 'comment',
        'close_parens': 'comment',
        'comma': 'comment',
        'question_mark': 'comment',
        'at_symbol': 'comment',
        'open_bracket': 'comment',
        'close_bracket': 'comment',
        'underscore': 'comment',
        'open_brace': 'comment',
        'close_brace': 'comment',
        'tilde': 'comment',
        'forward_slash': 'comment',
        'asterisk': 'comment',
        'symbol': 'comment',
        'equals': 'comment',
        'colon': 'comment'
    },
    'multi_comment': {
        'null': 'eof',
        'unused': 'multi_comment',
        'lf_char': 'multi_comment_lf',
        'cr_char':'multi_comment_cr',
        'space': 'multi_comment',
        'digit': 'multi_comment',
        'letter': 'multi_comment',
        'double_quote': 'multi_comment',
        'hash': 'multi_comment',
        'dollar': 'multi_comment',
        'single_quote': 'multi_comment',
        'open_parens': 'multi_comment',
        'close_parens': 'multi_comment',
        'comma': 'multi_comment',
        'question_mark': 'multi_comment',
        'at_symbol': 'multi_comment',
        'open_bracket': 'multi_comment',
        'close_bracket': 'multi_comment',
        'underscore': 'multi_comment',
        'open_brace': 'multi_comment',
        'close_brace': 'multi_comment',
        'tilde': 'multi_comment',
        'forward_slash': 'multi_comment',
        'asterisk': 'multi_comment_asterisk',
        'symbol': 'multi_comment',
        'equals': 'multi_comment',
        'colon': 'multi_comment'
    },
    'multi_comment_lf': {
        'null': 'eof',
        'unused': 'multi_comment',
        'lf_char': 'multi_comment_lf',
        'cr_char':'multi_comment_cr',
        'space': 'multi_comment',
        'digit': 'multi_comment',
        'letter': 'multi_comment',
        'double_quote': 'multi_comment',
        'hash': 'multi_comment',
        'dollar': 'multi_comment',
        'single_quote': 'multi_comment',
        'open_parens': 'multi_comment',
        'close_parens': 'multi_comment',
        'comma': 'multi_comment',
        'question_mark': 'multi_comment',
        'at_symbol': 'multi_comment',
        'open_bracket': 'multi_comment',
        'close_bracket': 'multi_comment',
        'underscore': 'multi_comment',
        'open_brace': 'multi_comment',
        'close_brace': 'multi_comment',
        'tilde': 'multi_comment',
        'forward_slash': 'multi_comment',
        'asterisk': 'multi_comment_asterisk',
        'symbol': 'multi_comment',
        'equals': 'multi_comment',
        'colon': 'multi_comment'
    },
    'multi_comment_cr': {
        'null': 'eof',
        'unused': 'multi_comment',
        'lf_char': 'multi_comment_lf',
        'cr_char':'multi_comment_cr',
        'space': 'multi_comment',
        'digit': 'multi_comment',
        'letter': 'multi_comment',
        'double_quote': 'multi_comment',
        'hash': 'multi_comment',
        'dollar': 'multi_comment',
        'single_quote': 'multi_comment',
        'open_parens': 'multi_comment',
        'close_parens': 'multi_comment',
        'comma': 'multi_comment',
        'question_mark': 'multi_comment',
        'at_symbol': 'multi_comment',
        'open_bracket': 'multi_comment',
        'close_bracket': 'multi_comment',
        'underscore': 'multi_comment',
        'open_brace': 'multi_comment',
        'close_brace': 'multi_comment',
        'tilde': 'multi_comment',
        'forward_slash': 'multi_comment',
        'asterisk': 'multi_comment_asterisk',
        'symbol': 'multi_comment',
        'equals': 'multi_comment',
        'colon': 'multi_comment'
    },
    'multi_comment_asterisk': {
        'null': 'eof',
        'unused': 'multi_comment',
        'lf_char': 'multi_comment_lf',
        'cr_char':'multi_comment_cr',
        'space': 'multi_comment',
        'digit': 'multi_comment',
        'letter': 'multi_comment',
        'double_quote': 'multi_comment',
        'hash': 'multi_comment',
        'dollar': 'multi_comment',
        'single_quote': 'multi_comment',
        'open_parens': 'multi_comment',
        'close_parens': 'multi_comment',
        'comma': 'multi_comment',
        'question_mark': 'multi_comment',
        'at_symbol': 'multi_comment',
        'open_bracket': 'multi_comment',
        'close_bracket': 'multi_comment',
        'underscore': 'multi_comment',
        'open_brace': 'multi_comment',
        'close_brace': 'multi_comment',
        'tilde': 'multi_comment',
        'forward_slash': 'start_declaration',
        'asterisk': 'multi_comment_asterisk',
        'symbol': 'multi_comment',
        'equals': 'multi_comment',
        'colon': 'multi_comment'
    },
    'seen_id': {
        'null': 'eof',
        'unused': 'invalid_character',
        'lf_char': 'lf_state',
        'cr_char':'cr_state',
        'space': 'seen_id',
        'digit': 'error',
        'letter': 'error',
        'double_quote': 'error',
        'hash': 'error',
        'dollar': 'error',
        'single_quote': 'error',
        'open_parens': 'function_declaration',
        'close_parens': 'seen_close_parens',
        'comma': 'id_list',
        'question_mark': 'error',
        'at_symbol': 'error',
        'open_bracket': 'error',
        'close_bracket': 'error',
        'underscore': 'error',
        'open_brace': 'error',
        'close_brace': 'error',
        'tilde': 'error',
        'forward_slash': 'error',
        'asterisk': 'error',
        'symbol': 'seen_symbol',
        'equals': 'seen_symbol',
        'colon': 'seen_colon'
    },
    'id_list': {
        'null': 'error',
        'unused': 'invalid_character',
        'lf_char': 'error',
        'cr_char':'error',
        'space': 'id_list',
        'digit': 'error',
        'letter': 'id',
        'double_quote': 'error',
        'hash': 'error',
        'dollar': 'error',
        'single_quote': 'error',
        'open_parens': 'error',
        'close_parens': 'error',
        'comma': 'error',
        'question_mark': 'error',
        'at_symbol': 'error',
        'open_bracket': 'error',
        'close_bracket': 'error',
        'underscore': 'underscore_id',
        'open_brace': 'error',
        'close_brace': 'error',
        'tilde': 'error',
        'forward_slash': 'error',
        'asterisk': 'error',
        'symbol': 'invalid_character',
        'equals': 'invalid_character',
        'colon': 'invalid_character'
    },
    'comparison': {
        'null': 'eof',
        'unused': 'invalid_character',
        'lf_char': 'lf_state',
        'cr_char':'cr_state',
        'space': 'comparison',
        'digit': 'numeric_literal',
        'letter': 'id',
        'double_quote': 'string_literal',
        'hash': 'error',
        'dollar': 'error',
        'single_quote': 'char_literal',
        'open_parens': 'error',
        'close_parens': 'error',
        'comma': 'error',
        'question_mark': 'error',
        'at_symbol': 'error',
        'open_bracket': 'error',
        'close_bracket': 'error',
        'underscore': 'underscore_id',
        'open_brace': 'error',
        'close_brace': 'error',
        'tilde': 'error',
        'forward_slash': 'seen_forward_slash',
        'asterisk': 'error',
        'symbol': 'invalid_character',
        'equals': 'invalid_character',
        'colon': 'invalid_character'
    },
    'dereference': {
        'null': 'eof',
        'unused': 'invalid_character',
        'lf_char': 'lf_state',
        'cr_char':'cr_state',
        'space': 'dereference',
        'digit': 'invalid_character',
        'letter': 'id',
        'double_quote': 'invalid_character',
        'hash': 'error',
        'dollar': 'error',
        'single_quote': 'invalid_character',
        'open_parens': 'error',
        'close_parens': 'error',
        'comma': 'error',
        'question_mark': 'error',
        'at_symbol': 'error',
        'open_bracket': 'error',
        'close_bracket': 'error',
        'underscore': 'underscore_id',
        'open_brace': 'error',
        'close_brace': 'error',
        'tilde': 'error',
        'forward_slash': 'seen_forward_slash',
        'asterisk': 'error',
        'symbol': 'seen_symbol',
        'equals': 'seen_symbol',
        'colon': 'invalid_character'
    },
    'bitwise_comparison': {
        'null': 'eof',
        'unused': 'invalid_character',
        'lf_char': 'lf_state',
        'cr_char':'cr_state',
        'space': 'bitwise_comparison',
        'digit': 'numeric_literal',
        'letter': 'id',
        'double_quote': 'string_literal',
        'hash': 'error',
        'dollar': 'error',
        'single_quote': 'char_literal',
        'open_parens': 'error',
        'close_parens': 'error',
        'comma': 'error',
        'question_mark': 'error',
        'at_symbol': 'error',
        'open_bracket': 'error',
        'close_bracket': 'error',
        'underscore': 'underscore_id',
        'open_brace': 'error',
        'close_brace': 'error',
        'tilde': 'error',
        'forward_slash': 'seen_forward_slash',
        'asterisk': 'error',
        'symbol': 'error',
        'equals': 'error',
        'colon': 'invalid_character'
    },
    'arithmetic_binary': {
        'null': 'eof',
        'unused': 'invalid_character',
        'lf_char': 'lf_state',
        'cr_char':'cr_state',
        'space': 'arithmetic_binary',
        'digit': 'numeric_literal',
        'letter': 'id',
        'double_quote': 'string_literal',
        'hash': 'error',
        'dollar': 'error',
        'single_quote': 'char_literal',
        'open_parens': 'error',
        'close_parens': 'error',
        'comma': 'error',
        'question_mark': 'error',
        'at_symbol': 'error',
        'open_bracket': 'error',
        'close_bracket': 'error',
        'underscore': 'underscore_id',
        'open_brace': 'error',
        'close_brace': 'error',
        'tilde': 'error',
        'forward_slash': 'seen_forward_slash',
        'asterisk': 'error',
        'symbol': 'error',
        'equals': 'error',
        'colon': 'invalid_character'
    },
    'bitwise': {
        'null': 'eof',
        'unused': 'invalid_character',
        'lf_char': 'lf_state',
        'cr_char':'cr_state',
        'space': 'bitwise',
        'digit': 'numeric_literal',
        'letter': 'id',
        'double_quote': 'string_literal',
        'hash': 'error',
        'dollar': 'error',
        'single_quote': 'char_literal',
        'open_parens': 'error',
        'close_parens': 'error',
        'comma': 'error',
        'question_mark': 'error',
        'at_symbol': 'error',
        'open_bracket': 'error',
        'close_bracket': 'error',
        'underscore': 'underscore_id',
        'open_brace': 'error',
        'close_brace': 'error',
        'tilde': 'error',
        'forward_slash': 'seen_forward_slash',
        'asterisk': 'error',
        'symbol': 'error',
        'equals': 'error',
        'colon': 'invalid_character'
    },
    'assign': {
        'null': 'eof',
        'unused': 'invalid_character',
        'lf_char': 'lf_state',
        'cr_char':'cr_state',
        'space': 'assign',
        'digit': 'numeric_literal',
        'letter': 'id',
        'double_quote': 'string_literal',
        'hash': 'error',
        'dollar': 'error',
        'single_quote': 'char_literal',
        'open_parens': 'error',
        'close_parens': 'error',
        'comma': 'error',
        'question_mark': 'error',
        'at_symbol': 'error',
        'open_bracket': 'error',
        'close_bracket': 'error',
        'underscore': 'underscore_id',
        'open_brace': 'error',
        'close_brace': 'error',
        'tilde': 'error',
        'forward_slash': 'seen_forward_slash',
        'asterisk': 'error',
        'symbol': 'error',
        'equals': 'error',
        'colon': 'invalid_character'
    },
    'bitwise_assign': {
        'null': 'eof',
        'unused': 'invalid_character',
        'lf_char': 'lf_state',
        'cr_char':'cr_state',
        'space': 'bitwise_assign',
        'digit': 'numeric_literal',
        'letter': 'id',
        'double_quote': 'string_literal',
        'hash': 'error',
        'dollar': 'error',
        'single_quote': 'char_literal',
        'open_parens': 'error',
        'close_parens': 'error',
        'comma': 'error',
        'question_mark': 'error',
        'at_symbol': 'error',
        'open_bracket': 'error',
        'close_bracket': 'error',
        'underscore': 'underscore_id',
        'open_brace': 'error',
        'close_brace': 'error',
        'tilde': 'error',
        'forward_slash': 'seen_forward_slash',
        'asterisk': 'error',
        'symbol': 'error',
        'equals': 'error',
        'colon': 'invalid_character'
    },
    'function_declaration': {
        'null': 'eof',
        'unused': 'invalid_character',
        'lf_char': 'lf_state',
        'cr_char':'cr_state',
        'space': 'function_declaration',
        'digit': 'error',
        'letter': 'id',
        'double_quote': 'error',
        'hash': 'error',
        'dollar': 'error',
        'single_quote': 'error',
        'open_parens': 'error',
        'close_parens': 'error',
        'comma': 'error',
        'question_mark': 'error',
        'at_symbol': 'error',
        'open_bracket': 'error',
        'close_bracket': 'error',
        'underscore': 'underscore_id',
        'open_brace': 'error',
        'close_brace': 'error',
        'tilde': 'error',
        'forward_slash': 'seen_forward_slash',
        'asterisk': 'error',
        'symbol': 'error',
        'equals': 'error',
        'colon': 'error'
    },
    'member_access': {
        'null': 'eof',
        'unused': 'invalid_character',
        'lf_char': 'lf_state',
        'cr_char':'cr_state',
        'space': 'member_access', # should I allow space after member access operator?
        'digit': 'error',
        'letter': 'id',
        'double_quote': 'error',
        'hash': 'error',
        'dollar': 'error',
        'single_quote': 'error',
        'open_parens': 'error',
        'close_parens': 'error',
        'comma': 'error',
        'question_mark': 'error',
        'at_symbol': 'error',
        'open_bracket': 'error',
        'close_bracket': 'error',
        'underscore': 'underscore_id',
        'open_brace': 'error',
        'close_brace': 'error',
        'tilde': 'error',
        'forward_slash': 'seen_forward_slash',
        'asterisk': 'error',
        'symbol': 'error',
        'equals': 'error',
        'colon': 'invalid_character'
    },
    'range_inclusive': {
        'null': 'eof',
        'unused': 'invalid_character',
        'lf_char': 'lf_state',
        'cr_char':'cr_state',
        'space': 'range_inclusive', # should I allow spaces after range operators?
        'digit': 'numeric_literal',
        'letter': 'id',
        'double_quote': 'string_literal',
        'hash': 'error',
        'dollar': 'error',
        'single_quote': 'char_literal',
        'open_parens': 'error',
        'close_parens': 'error',
        'comma': 'error',
        'question_mark': 'error',
        'at_symbol': 'error',
        'open_bracket': 'error',
        'close_bracket': 'error',
        'underscore': 'underscore_id',
        'open_brace': 'error',
        'close_brace': 'error',
        'tilde': 'error',
        'forward_slash': 'seen_forward_slash',
        'asterisk': 'error',
        'symbol': 'error',
        'equals': 'error',
        'colon': 'invalid_character'
    },
    'range_exclusive': {
        'null': 'eof',
        'unused': 'invalid_character',
        'lf_char': 'lf_state',
        'cr_char':'cr_state',
        'space': 'range_exclusive', # should I allow spaces after range operators?
        'digit': 'numeric_literal',
        'letter': 'id',
        'double_quote': 'string_literal',
        'hash': 'error',
        'dollar': 'error',
        'single_quote': 'char_literal',
        'open_parens': 'error',
        'close_parens': 'error',
        'comma': 'error',
        'question_mark': 'error',
        'at_symbol': 'error',
        'open_bracket': 'error',
        'close_bracket': 'error',
        'underscore': 'underscore_id',
        'open_brace': 'error',
        'close_brace': 'error',
        'tilde': 'error',
        'forward_slash': 'seen_forward_slash',
        'asterisk': 'error',
        'symbol': 'error',
        'equals': 'error',
        'colon': 'invalid_character'
    },
    'id': {
        'null': 'eof',
        'unused': 'invalid_character',
        'lf_char': 'lf_state',
        'cr_char':'cr_state',
        'space': 'seen_id',
        'digit': 'id',
        'letter': 'id',
        'double_quote': 'error',
        'hash': 'error',
        'dollar': 'error',
        'single_quote': 'error',
        'open_parens': 'function_declaration',
        'close_parens': 'seen_close_parens',
        'comma': 'id_list',
        'question_mark': 'error', # I need to do null operators
        'at_symbol': 'error',
        'open_bracket': 'error',
        'close_bracket': 'error',
        'underscore': 'id',
        'open_brace': 'seen_id',
        'close_brace': 'error',
        'tilde': 'error',
        'forward_slash': 'seen_forward_slash',
        'asterisk': 'seen_symbol',
        'symbol': 'seen_symbol',
        'equals': 'seen_symbol',
        'colon': 'seen_colon'
    },
    'underscore_id': {
        'null': 'eof',
        'unused': 'invalid_character',
        'lf_char': 'error',
        'cr_char':'error',
        'space': 'error',
        'digit': 'error',
        'letter': 'id',
        'double_quote': 'error',
        'hash': 'error',
        'dollar': 'error',
        'single_quote': 'error',
        'open_parens': 'error',
        'close_parens': 'seen_close_parens',
        'comma': 'error',
        'question_mark': 'error',
        'at_symbol': 'error',
        'open_bracket': 'error',
        'close_bracket': 'error',
        'underscore': 'underscore_id',
        'open_brace': 'error',
        'close_brace': 'error',
        'tilde': 'error',
        'forward_slash': 'error',
        'asterisk': 'error',
        'symbol': 'error',
        'equals': 'error',
        'colon': 'invalid_character'
    },
    'seen_close_parens': {
        'null': 'eof',
        'unused': 'invalid_character',
        'lf_char': 'lf_state',
        'cr_char':'cr_state',
        'space': 'seen_id',
        'digit': 'error',
        'letter': 'error',
        'double_quote': 'error',
        'hash': 'error',
        'dollar': 'error',
        'single_quote': 'error',
        'open_parens': 'function_declaration',
        'close_parens': 'seen_close_parens',
        'comma': 'id_list',
        'question_mark': 'error',
        'at_symbol': 'error',
        'open_bracket': 'error',
        'close_bracket': 'error',
        'underscore': 'error',
        'open_brace': 'error',
        'close_brace': 'error',
        'tilde': 'error',
        'forward_slash': 'error',
        'asterisk': 'error',
        'symbol': 'seen_symbol',
        'equals': 'seen_symbol',
        'colon': 'seen_colon'
    }
}

max_state_length = len(max(states, key=len)) + 1
max_char_type_length = len(max(char_types, key=len)) + 1


# state.hpp

state_file = 'includes/state.hpp'
state_file_copy = 'includes/backup/state.hpp'

if os.path.exists(state_file):
    sh.copyfile(state_file, state_file_copy)

state_file_start = '#pragma once\n\nenum state : char {'
state_file_end   = '\n};'

with open(state_file, 'w+') as f:
    try:
        f.write(state_file_start)
        for state in states:
            f.write('\n    ' + state + ',')
        for state in break_out_states:
            f.write('\n    ' + state + ',')
        f.write(state_file_end)
    except:
        print('something went wrong generating the state.hpp file')
        os.remove(state_file)

        if os.path.exists(state_file_copy):
            os.rename(state_file_copy, state_file)

        quit()


# char_type.hpp

char_type_file = 'includes/char_type.hpp'
char_type_file_copy = 'includes/backup/char_type.hpp'

if os.path.exists(char_type_file):
    sh.copyfile(char_type_file, char_type_file_copy)

char_type_file_start = '#pragma once\n\nenum char_type : short {'
char_type_file_end   = '\n};'

with open(char_type_file, 'w+') as f:
    try:
        f.write(char_type_file_start)
        for i, char_type in enumerate(char_types):
            f.write('\n    ' + char_type +
                (' ' * (max_char_type_length - len(char_type))) +
                '= ' +
                str(i * len(states)) +
                ',')
        f.write(char_type_file_end)
    except:
        print('something went wrong generating the char_type.hpp file')
        os.remove(char_type_file)

        if os.path.exists(state_file_copy):
            os.rename(state_file_copy, state_file)

        if os.path.exists(char_type_file_copy):
            os.rename(char_type_file_copy, char_type_file)

        quit()


# transitions.hpp

def create_states_array_string():
    transition_array_string = ''
    array_index = 0
    index_digits = most_array_index_digits()

    for char_type in char_types:
        for state in states:
            try:
                transition_array_string += ('\n    /* ' +
                    str(array_index) + 
                    (' ' * (index_digits - len(str(array_index))) + ' ' +
                    state +
                    (' ' * (max_state_length - len(state))) +
                    char_type +
                    (' ' * (max_char_type_length - len(char_type))) +
                    '*/' +
                    ' state::' + transitions[state][char_type] + ','))
                array_index += 1
            except:
                print(state + ': ' + char_type)

    return transition_array_string

def create_bitset_array_string():
    bitset_array_string = ''
    multi_comment_lf_state_ind = states.index('multi_comment_lf')
    lf_state_ind = states.index('lf_state')

    for _ in range(0, multi_comment_lf_state_ind):
        bitset_array_string += '0,'
    bitset_array_string += '1,'
    for _ in range(multi_comment_lf_state_ind + 1, len(states)):
        bitset_array_string += '0,'

    for _ in range(0, lf_state_ind):
        bitset_array_string += '0,'
    bitset_array_string += '1,'
    for _ in range(lf_state_ind + 1, len(break_out_states)):
        bitset_array_string += '0,'

    return bitset_array_string

def most_array_index_digits():
    total_entries = len(char_types) * len(states)
    if total_entries < 11:
        return 1
    elif total_entries < 101:
        return 2
    else:
        return 3

transitions_file = 'includes/transitions.hpp'
transitions_file_copy = 'includes/backup/transitions.hpp'

if os.path.exists(transitions_file):
    sh.copyfile(transitions_file, transitions_file_copy)

with open(transitions_file, 'w+') as f:
    text = f.read()
    try:
        f.seek(0)
        f.truncate()
        f.write('#pragma once\n\n#include "state.hpp"\n#include "char_type.hpp"\n#include <array>\n#include <bitset>\n\n')
        f.write('static const std::array<state, ' + str(len(states) * len(char_types)) + '> states = {')
        f.write(create_states_array_string())
        f.write('\n};')
        #f.write('\nstatic const std::bitset<')
        #f.write(str(len(states) + len(break_out_states)))
        #f.write('> is_first_char_of_new_line = ')
        #f.write('{\n    ')
        #f.write(create_bitset_array_string())
        #f.write('\n};')
    except:
        print('something went wrong generating the transitions.hpp file')
        os.remove(transitions_file)
        
        if os.path.exists(state_file_copy):
            os.rename(state_file_copy, state_file)
            
        if os.path.exists(char_type_file_copy):
            os.rename(char_type_file_copy, char_type_file)

        if os.path.exists(transitions_file_copy):
            os.rename(transitions_file_copy, transitions_file)

        quit()