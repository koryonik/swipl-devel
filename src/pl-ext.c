/*  $Id$

    Copyright (c) 1990 Jan Wielemaker. All rights reserved.
    See ../LICENCE to find out about your rights.
    jan@swi.psy.uva.nl

    Purpose: link built_in predicates
*/

#include "pl-incl.h"

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
Link all foreign language predicates.  The arguments to FRG are:

	FRG(name, arity, function, flags).

Flags almost always is TRACE_ME.  Additional common flags:

	TRANSPARENT		Predicate is module transparent
	NONDETERMINISTIC	Predicate can be resatisfied
- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#define NDET NONDETERMINISTIC		/* make a bit shorter */
#define META TRANSPARENT		/* same */

#define FRG(n, a, f, flags) { n, f, flags, a }

static struct foreign {
  char		*name;
  Func		function;
  unsigned long flags;
  int		arity;
} foreigns[] = {
  FRG("nl",			0, pl_nl,			TRACE_ME),
  FRG("put",			1, pl_put,			TRACE_ME),
  FRG("get0",			1, pl_get0,			TRACE_ME),
  FRG("get",			1, pl_get,			TRACE_ME),
  FRG("skip",			1, pl_skip,			TRACE_ME),
  FRG("skip",			2, pl_skip2,			TRACE_ME),
  FRG("get_single_char",	1, pl_get_single_char,		TRACE_ME),
  FRG("seeing",			1, pl_seeing,			TRACE_ME),
  FRG("telling",		1, pl_telling,			TRACE_ME),
  FRG("seen",			0, pl_seen,			TRACE_ME),
  FRG("tab",			1, pl_tab,			TRACE_ME),
  FRG("tmp_file",		2, pl_tmp_file,			TRACE_ME),
  FRG("delete_file",		1, pl_delete_file,		TRACE_ME),
  FRG("access_file",		2, pl_access_file,		TRACE_ME),
  FRG("read_link",		3, pl_read_link,		TRACE_ME),
  FRG("exists_file",		1, pl_exists_file,		TRACE_ME),
  FRG("exists_directory",	1, pl_exists_directory,		TRACE_ME),
  FRG("rename_file",		2, pl_rename_file,		TRACE_ME),
  FRG("same_file",		2, pl_same_file,		TRACE_ME),
  FRG("time_file",		2, pl_time_file,		TRACE_ME),
  FRG("told",			0, pl_told,			TRACE_ME),
  FRG("see",			1, pl_see,			TRACE_ME),
  FRG("tell",			1, pl_tell,			TRACE_ME),
  FRG("append",			1, pl_append,			TRACE_ME),
  FRG("ttyflush",		0, pl_ttyflush,			TRACE_ME),
  FRG("flush",			0, pl_flush,			TRACE_ME),
  FRG("prompt",			2, pl_prompt,			TRACE_ME),
  FRG("prompt1",		1, pl_prompt1,			TRACE_ME),
  FRG("expand_file_name",	2, pl_expand_file_name,		TRACE_ME),
  FRG("$absolute_file_name",	2, pl_absolute_file_name,	TRACE_ME),
  FRG("is_absolute_file_name",	1, pl_is_absolute_file_name,	TRACE_ME),
  FRG("file_base_name",		2, pl_file_base_name,		TRACE_ME),
  FRG("file_directory_name",	2, pl_file_dir_name,		TRACE_ME),
  FRG("prolog_to_os_filename",	2, pl_prolog_to_os_filename,	TRACE_ME),
#if defined(O_XOS) && defined(__WIN32__)
  FRG("make_fat_filemap",	1, pl_make_fat_filemap,		TRACE_ME),
  FRG("window_title",		2, pl_window_title,		TRACE_ME),
#endif
  FRG("fileerrors",		2, pl_fileerrors,		TRACE_ME),
  FRG("$syntaxerrors",		2, pl_syntaxerrors,		TRACE_ME),
  FRG("chdir",			1, pl_chdir,			TRACE_ME),

  FRG("halt",			1, pl_halt,			TRACE_ME),
  FRG("$shell",			2, pl_shell,			TRACE_ME),
  FRG("getenv",			2, pl_getenv,			TRACE_ME),
  FRG("setenv",			2, pl_setenv,			TRACE_ME),
  FRG("unsetenv",		1, pl_unsetenv,			TRACE_ME),
  FRG("wildcard_match",		2, pl_wildcard_match,		TRACE_ME),
  FRG("$apropos_match",		2, pl_apropos_match,		TRACE_ME),
  FRG("$argv",			1, pl_argv,			TRACE_ME),
  FRG("$option",		3, pl_option,			TRACE_ME),
  FRG("convert_time",		8, pl_convert_time,		TRACE_ME),
  FRG("sleep",			1, pl_sleep,			TRACE_ME),
  FRG("break",			0, pl_break,			TRACE_ME),
  FRG("$break",			1, pl_break1,			TRACE_ME),
  FRG("notrace",		1, pl_notrace1,			META),

  FRG("display",		1, pl_display,			TRACE_ME),
  FRG("displayq",		1, pl_displayq,			TRACE_ME),
  FRG("write",			1, pl_write,			TRACE_ME),
  FRG("writeq",			1, pl_writeq,			TRACE_ME),
  FRG("print",			1, pl_print,			TRACE_ME),
  FRG("$print",			2, pl_dprint,		META|TRACE_ME),

  FRG("read_variables",		2, pl_read_variables,		TRACE_ME),
  FRG("read_variables",		3, pl_read_variables3,		TRACE_ME),
  FRG("read",			1, pl_read,			TRACE_ME),
  FRG("read_clause",		1, pl_read_clause,		TRACE_ME),
  FRG("read_clause",		2, pl_read_clause2,		TRACE_ME),
  FRG("$raw_read",		1, pl_raw_read,			TRACE_ME),
  FRG("current_op",		3, pl_current_op,	   NDET|TRACE_ME),
  FRG("current_atom",		1, pl_current_atom,	   NDET|TRACE_ME),
  FRG("current_functor",	2, pl_current_functor,	   NDET|TRACE_ME),
  FRG("$complete_atom",		3, pl_complete_atom,		TRACE_ME),
  FRG("$atom_completions",	2, pl_atom_completions,		TRACE_ME),
  FRG("$op",			3, pl_op1,			TRACE_ME),
  FRG("$reset_operators",	0, pl_reset_operators,		TRACE_ME),

  FRG("!",			0, pl_metacut,			TRACE_ME),
  FRG("functor",		3, pl_functor,			TRACE_ME),
  FRG("arg",			3, pl_arg,		   NDET|TRACE_ME),
  FRG("setarg",			3, pl_setarg,			TRACE_ME),
  FRG("=..",			2, pl_univ,			TRACE_ME),
  FRG("name",			2, pl_name,			TRACE_ME),
  FRG("atom_chars",		2, pl_atom_chars,		TRACE_ME),
  FRG("atom_char",		2, pl_atom_char,		TRACE_ME),
  FRG("number_chars",		2, pl_number_chars,		TRACE_ME),
  FRG("int_to_atom",		3, pl_int_to_atom,		TRACE_ME),
  FRG("$format_number",		3, pl_format_number,		TRACE_ME),
  FRG("concat",			3, pl_concat,			TRACE_ME),
  FRG("$concat_atom",		2, pl_concat_atom,		TRACE_ME),
  FRG("atom_length",		2, pl_atom_length,		TRACE_ME),
  FRG("$term_to_atom",		4, pl_term_to_atom,		TRACE_ME),
  FRG("numbervars",		4, pl_numbervars,		TRACE_ME),
  FRG("free_variables",		2, pl_free_variables,		TRACE_ME),
  FRG("$e_free_variables",	2, pl_e_free_variables,		TRACE_ME),

  FRG("$open_wic",		2, pl_open_wic,			TRACE_ME),
  FRG("$close_wic",		0, pl_close_wic,		TRACE_ME),
  FRG("$add_directive_wic",	1, pl_add_directive_wic,	TRACE_ME),
  FRG("$import_wic",		2, pl_import_wic,		TRACE_ME),

  FRG("$qlf_put_states",	0, pl_qlf_put_states,		TRACE_ME),
  FRG("$qlf_start_module",	1, pl_qlf_start_module,		TRACE_ME),
  FRG("$qlf_start_sub_module",	1, pl_qlf_start_sub_module,	TRACE_ME),
  FRG("$qlf_start_file",	1, pl_qlf_start_file,		TRACE_ME),
  FRG("$qlf_end_part",		0, pl_qlf_end_part,		TRACE_ME),
  FRG("$qlf_open",		1, pl_qlf_open,			TRACE_ME),
  FRG("$qlf_close",		0, pl_qlf_close,		TRACE_ME),
  FRG("$qlf_load",		2, pl_qlf_load,		   META|TRACE_ME),
  FRG("$qlf_assert_clause",	1, pl_qlf_assert_clause,	TRACE_ME),
  FRG("$qlf_info",		4, pl_qlf_info,			TRACE_ME),

  FRG("abolish",    		2, pl_abolish,		   META|TRACE_ME),
  FRG("$clause",    		3, pl_clause,	      NDET|META|TRACE_ME),
  FRG("nth_clause", 		3, pl_nth_clause,     NDET|META|TRACE_ME),
  FRG("retract",    		1, pl_retract,        NDET|META|TRACE_ME),
  FRG("retractall",		1, pl_retractall,	   META|TRACE_ME),
  FRG("$xr_member",		2, pl_xr_member,	   NDET|TRACE_ME),
  FRG("$wam_list",		1, pl_wam_list,			TRACE_ME),
  
  FRG("flag",			3, pl_flag,			TRACE_ME),
  FRG("recorda",		3, pl_recorda,			TRACE_ME),
  FRG("recordz",		3, pl_recordz,			TRACE_ME),
  FRG("recorded",		3, pl_recorded,		   NDET|TRACE_ME),
  FRG("erase",			1, pl_erase,			TRACE_ME),
  FRG("assert",			1, pl_assertz,		   META|TRACE_ME),
  FRG("asserta",		1, pl_asserta,		   META|TRACE_ME),
  FRG("assertz",		1, pl_assertz,		   META|TRACE_ME),
  FRG("assert",			2, pl_assertz2,		   META|TRACE_ME),
  FRG("asserta",		2, pl_asserta2,		   META|TRACE_ME),
  FRG("assertz",		2, pl_assertz2,		   META|TRACE_ME),
  FRG("$record_clause",		3, pl_record_clause,		TRACE_ME),
  FRG("redefine_system_predicate", 1, pl_redefine_system_predicate,
							   META|TRACE_ME),

  FRG("$c_current_predicate",	2, pl_current_predicate,  NDET|META|TRACE_ME),
  FRG("$set_predicate_attribute", 3, pl_set_predicate_attribute,META|TRACE_ME),
  FRG("$get_predicate_attribute", 3, pl_get_predicate_attribute,META|TRACE_ME),
  FRG("$get_clause_attribute",  3, pl_get_clause_attribute,	TRACE_ME),
  FRG("$require",		1, pl_require,		   META|TRACE_ME),
  FRG("$source_file",		2, pl_source_file,	   META|TRACE_ME),
  FRG("$time_source_file",	2, pl_time_source_file,	   NDET|TRACE_ME),
  FRG("$start_consult",		1, pl_start_consult,		TRACE_ME),
  FRG("$make_system_source_files",0,pl_make_system_source_files,TRACE_ME),
  FRG("$default_predicate",	2, pl_default_predicate,   META|TRACE_ME),

  FRG("var",			1, pl_var,			TRACE_ME),
  FRG("nonvar",			1, pl_nonvar,			TRACE_ME),
  FRG("integer",		1, pl_integer,			TRACE_ME),
  FRG("float",			1, pl_float,			TRACE_ME),
  FRG("number",			1, pl_number,			TRACE_ME),
  FRG("atom",			1, pl_atom,			TRACE_ME),
  FRG("atomic",			1, pl_atomic,			TRACE_ME),
  FRG("ground",			1, pl_ground,			TRACE_ME),
  FRG("compound",		1, pl_compound,			TRACE_ME),

  FRG("==",			2, pl_equal,			TRACE_ME),
  FRG("\\==",			2, pl_nonequal,			TRACE_ME),
  FRG("=",			2, pl_unify,			TRACE_ME),
  FRG("\\=",			2, pl_notunify,			TRACE_ME),
  FRG("compare",		3, pl_compare,			TRACE_ME),
  FRG("@<",			2, pl_lessStandard,		TRACE_ME),
  FRG("@=<",			2, pl_lessEqualStandard,	TRACE_ME),
  FRG("@>",			2, pl_greaterStandard,		TRACE_ME),
  FRG("@>=",			2, pl_greaterEqualStandard,	TRACE_ME),
  FRG("=@=",			2, pl_structural_equal,		TRACE_ME),
  FRG("\\=@=",			2, pl_structural_nonequal,	TRACE_ME),

  FRG("repeat",			0, pl_repeat,		   NDET|TRACE_ME),
  FRG("fail",			0, pl_fail,			TRACE_ME),
  FRG("true",			0, pl_true,			TRACE_ME),
  FRG("$fail",			0, pl_fail,			0),
  FRG("abort",			0, pl_abort,			TRACE_ME),

  FRG("statistics",		2, pl_statistics,		TRACE_ME),

  FRG("between",		3, pl_between,		   NDET|TRACE_ME),
  FRG("succ",			2, pl_succ,			TRACE_ME),
  FRG("plus",			3, pl_plus,			TRACE_ME),
  FRG("<",			2, pl_lessNumbers,		TRACE_ME),
  FRG(">",			2, pl_greaterNumbers,		TRACE_ME),
  FRG("=<",			2, pl_lessEqualNumbers,		TRACE_ME),
  FRG(">=",			2, pl_greaterEqualNumbers,	TRACE_ME),
  FRG("=\\=",			2, pl_nonEqualNumbers,		TRACE_ME),
  FRG("=:=",			2, pl_equalNumbers,		TRACE_ME),
  FRG("is",			2, pl_is,		   META|TRACE_ME),

  FRG("trace",			0, pl_trace,			0),
  FRG("notrace",		0, pl_notrace,			0),
  FRG("tracing",		0, pl_tracing,			0),
  FRG("debug",			0, pl_debug,			0),
  FRG("nodebug",		0, pl_nodebug,			0),
  FRG("$debugging",		0, pl_debugging,		0),
  FRG("$spy",			1, pl_spy,		   META|TRACE_ME),
  FRG("$nospy",			1, pl_nospy,		   META|TRACE_ME),
  FRG("$leash",			2, pl_leash, 			0),
  FRG("$visible",		2, pl_visible,			0),
  FRG("$debuglevel",		2, pl_debuglevel,		TRACE_ME),
  FRG("unknown",		2, pl_unknown,		   META|TRACE_ME),
  FRG("$style_check",		2, pl_style_check,		TRACE_ME),

#if COUNTING
  FRG("$count",			0, pl_count,			TRACE_ME),
#endif /* COUNTING */

  FRG("$profile",		2, pl_profile,			TRACE_ME),
  FRG("reset_profiler",		0, pl_reset_profiler,		TRACE_ME),
  FRG("profile_count",		3, pl_profile_count,	   META|TRACE_ME),
  FRG("profile_box",		5, pl_profile_box,	   META|TRACE_ME),

  FRG("prolog_current_frame",	1, pl_prolog_current_frame,	TRACE_ME),
  FRG("prolog_frame_attribute",	3, pl_prolog_frame_attribute,	TRACE_ME),
  FRG("$trace_continuation",	1, pl_trace_continuation,	TRACE_ME),
  FRG("prolog_skip_level",	2, pl_skip_level,		0),

  FRG("$write_on_atom",		2, pl_write_on_atom,		TRACE_ME),
#if O_STRING
  FRG("$write_on_string",	2, pl_write_on_string,		TRACE_ME),
#endif
  FRG("$write_on_list",		2, pl_write_on_list, 		TRACE_ME),
  FRG("dwim_match",		3, pl_dwim_match,		TRACE_ME),
  FRG("$dwim_predicate",	2, pl_dwim_predicate,	   NDET|TRACE_ME),

  FRG("$novice",		2, pl_novice,			TRACE_ME),
  FRG("protocol",		1, pl_protocol,			TRACE_ME),
  FRG("protocola",		1, pl_protocola,		TRACE_ME),
  FRG("noprotocol",		0, pl_noprotocol,		TRACE_ME),
  FRG("protocolling",		1, pl_protocolling,		TRACE_ME),

  FRG("$current_module",	2, pl_current_module,	   NDET|TRACE_ME),
  FRG("$module",		2, pl_module,			TRACE_ME),
  FRG("$set_source_module",	2, pl_set_source_module,	TRACE_ME),
  FRG("$declare_module",	2, pl_declare_module,		TRACE_ME),
  FRG("context_module",		1, pl_context_module,	   META|TRACE_ME),
  FRG("$strip_module",		3, pl_strip_module,	   META|TRACE_ME),
  FRG("import",			1, pl_import,		   META|TRACE_ME),
  FRG("export",			1, pl_export,		   META|TRACE_ME),
  FRG("$check_export",		0, pl_check_export,	   META|TRACE_ME),
  FRG("export_list",		2, pl_export_list,		TRACE_ME),
  FRG("index",			1, pl_index,		   META|TRACE_ME),
  FRG("hash",			1, pl_hash,		   META|TRACE_ME),
#ifdef O_HASHTERM
  FRG("hash_term",		2, pl_hash_term,		TRACE_ME),
#endif
#if O_AIX_FOREIGN
  FRG("$load_foreign",		1, pl_load_foreign1,	   META|TRACE_ME),
#else
  FRG("$load_foreign",		5, pl_load_foreign,	   META|TRACE_ME),
#endif
  FRG("$open_shared_object",	3, pl_open_shared_object,	TRACE_ME),
#if defined(HAVE_DLOPEN) || defined(HAVE_SHL_LOAD)
  FRG("close_shared_object",	1, pl_close_shared_object,	TRACE_ME),
  FRG("call_shared_object_function",
				2, pl_call_shared_object_function,
							   META|TRACE_ME),
#endif /*HAVE_DLOPEN*/

#if O_DDE
  FRG("open_dde_conversation",	3, pl_open_dde_conversation,	TRACE_ME),
  FRG("close_dde_conversation",	1, pl_close_dde_conversation,	TRACE_ME),
  FRG("dde_request",		4, pl_dde_request,		TRACE_ME),
  FRG("dde_execute",		3, pl_dde_execute,		TRACE_ME),
  FRG("$dde_register_service",	2, pl_dde_register_service,	TRACE_ME),
#endif /*O_DDE*/

#ifdef O_DLL
  FRG("open_dll",		2, pl_open_dll,			TRACE_ME),
  FRG("close_dll",		1, pl_close_dll,		TRACE_ME),
  FRG("call_dll_function",	2, pl_call_dll_function,    META|TRACE_ME),
#endif /*O_DLL*/

#if O_STRING
  FRG("string",			1, pl_string,			TRACE_ME),
  FRG("string_length",		2, pl_string_length,		TRACE_ME),
  FRG("string_to_atom",		2, pl_string_to_atom,		TRACE_ME),
  FRG("string_to_list",		2, pl_string_to_list,		TRACE_ME),
  FRG("substring",		4, pl_substring,		TRACE_ME),
#endif /* O_STRING */

  FRG("save",			2, pl_save,			TRACE_ME),
  FRG("restore",		1, pl_restore,			TRACE_ME),
  FRG("$save_program",		2, pl_save_program,		TRACE_ME),

  FRG("is_list",		1, pl_is_list,			TRACE_ME),
  FRG("proper_list",		1, pl_proper_list,		TRACE_ME),
  FRG("$length",		2, pl_length,			TRACE_ME),
  FRG("memberchk",		2, pl_memberchk,		TRACE_ME),
  FRG("msort",			2, pl_msort,			TRACE_ME),
  FRG("sort",			2, pl_sort,			TRACE_ME),
  FRG("format",			2, pl_format,			TRACE_ME),
  FRG("$collect_bag",		2, pl_collect_bag,		TRACE_ME),
  FRG("$record_bag",		1, pl_record_bag,		TRACE_ME),
  FRG("$please",		3, pl_please,			TRACE_ME),
  FRG("$check_definition",	1, pl_check_definition,    META|TRACE_ME),

#if O_COMPILE_OR
  FRG("$alt",			1, pl_alt,			NDET),
#endif /* O_COMPILE_OR */
  FRG("$atom_hashstat",		2, pl_atom_hashstat,		TRACE_ME),
  FRG("$tty",			0, pl_tty,			TRACE_ME),
  FRG("feature",		2, pl_feature,		   NDET|TRACE_ME),
  FRG("set_feature",		2, pl_set_feature,		TRACE_ME),
  FRG("limit_stack",		2, pl_limit_stack,		TRACE_ME),
  FRG("trim_stacks",		0, pl_trim_stacks,		TRACE_ME),
#if O_SHIFT_STACKS
  FRG("stack_parameter",	4, pl_stack_parameter,		TRACE_ME),
#endif
  FRG("$garbage_collect",	1, pl_garbage_collect,		TRACE_ME),
  FRG("copy_term",		2, pl_copy_term,		TRACE_ME),
  FRG("current_key",		1, pl_current_key,	   NDET|TRACE_ME),
  FRG("current_flag",		1, pl_current_flag,	   NDET|TRACE_ME),

  FRG("open",			3, pl_open,			TRACE_ME),
  FRG("open",			4, pl_open4,			TRACE_ME),
  FRG("open_null_stream",	1, pl_open_null_stream,		TRACE_ME),
  FRG("close",			1, pl_close,			TRACE_ME),
  FRG("current_stream",		3, pl_current_stream,	   NDET|TRACE_ME),
  FRG("flush_output",		1, pl_flush_output,		TRACE_ME),
  FRG("stream_position",	3, pl_stream_position,		TRACE_ME),
  FRG("set_input",		1, pl_set_input,		TRACE_ME),
  FRG("set_output",		1, pl_set_output,		TRACE_ME),
  FRG("current_input",		1, pl_current_input,		TRACE_ME),
  FRG("current_output",		1, pl_current_output,		TRACE_ME),
  FRG("character_count",	2, pl_character_count,		TRACE_ME),
  FRG("line_count",		2, pl_line_count,		TRACE_ME),
  FRG("line_position",		2, pl_line_position,		TRACE_ME),
  FRG("source_location",	2, pl_source_location,		TRACE_ME),

  FRG("nl",			1, pl_nl1,			TRACE_ME),
  FRG("tab",			2, pl_tab2,			TRACE_ME),
  FRG("put",			2, pl_put2,			TRACE_ME),
  FRG("get",			2, pl_get2,			TRACE_ME),
  FRG("get0",			2, pl_get02,			TRACE_ME),
  FRG("read",			2, pl_read2,			TRACE_ME),
  FRG("write",			2, pl_write2,			TRACE_ME),
  FRG("writeq",			2, pl_writeq2,			TRACE_ME),
  FRG("print",			2, pl_print2,			TRACE_ME),
  FRG("display",		2, pl_display2,			TRACE_ME),
  FRG("displayq",		2, pl_displayq2,		TRACE_ME),
  FRG("format",			3, pl_format3,			TRACE_ME),

  FRG("tty_get_capability",	3, pl_tty_get_capability,	TRACE_ME),
  FRG("tty_goto",		2, pl_tty_goto,			TRACE_ME),
  FRG("tty_put",		2, pl_tty_put,			TRACE_ME),
  FRG("format_predicate",	2, pl_format_predicate,	   META|TRACE_ME),
  FRG("set_tty",		2, pl_set_tty,			TRACE_ME),
  FRG("wait_for_input",		3, pl_wait_for_input,		TRACE_ME),
  FRG("get_time",		1, pl_get_time,			TRACE_ME),
  FRG("size_file",		2, pl_size_file,		TRACE_ME),
  FRG("$default_module",	3, pl_default_module,	   META|TRACE_ME),
#if O_PROLOG_FUNCTIONS
  FRG("$arithmetic_function",   1, pl_arithmetic_function, META|TRACE_ME),
  FRG("current_arithmetic_function", 1, pl_current_arithmetic_function,
						      NDET|META|TRACE_ME),
#endif

  /* DO NOT ADD ENTRIES BELOW THIS ONE */
  FRG((char *)NULL,		0, (Func)NULL,			0)
};

void
initBuildIns(void)
{ struct foreign *f;
  register Definition def;
  PL_extension *e;

  for(f = &foreigns[0]; f->name; f++)
  { FunctorDef fdef = lookupFunctorDef(lookupAtom(f->name), f->arity);

    def = lookupProcedure(fdef, MODULE_system)->definition;
    set(def, FOREIGN|SYSTEM|LOCKED);
    clear(def, TRACE_ME);
    set(def, f->flags);
    def->definition.function = f->function;
    def->indexPattern = 0;
    def->indexCardinality = 0;
    if ( false(def, NONDETERMINISTIC) && 
	 f->arity <= 2 )
      set(fdef, INLINE_F);
  }

  PROCEDURE_alt1 = lookupProcedure(FUNCTOR_alt1, MODULE_system);
  PROCEDURE_garbage_collect0 = lookupProcedure(FUNCTOR_garbage_collect0,
					       MODULE_system);
  PROCEDURE_block3 = lookupProcedure(FUNCTOR_block3, MODULE_system);
  PROCEDURE_true0  = lookupProcedure(FUNCTOR_true0, MODULE_system);
  PROCEDURE_fail0  = lookupProcedure(FUNCTOR_fail0, MODULE_system);

  for(e = &PL_extensions[0]; e->predicate_name; e++)
  { short flags = TRACE_ME;

    if ( e->flags & PL_FA_NOTRACE )	     flags &= ~TRACE_ME;
    if ( e->flags & PL_FA_TRANSPARENT )	     flags |= TRANSPARENT;
    if ( e->flags & PL_FA_NONDETERMINISTIC ) flags |= NONDETERMINISTIC;

    def = lookupProcedure(lookupFunctorDef(lookupAtom(e->predicate_name),
					   e->arity), 
			  MODULE_user)->definition;
    set(def, FOREIGN);
    set(def, flags);
    def->definition.function = e->function;
    def->indexPattern = 0;
    def->indexCardinality = 0;
  }    
}
