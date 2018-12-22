/* SPDX-License-Identifier: BSD-2-Clause */

#include <tilck/common/basic_defs.h>

typedef u64 gcov_type;
typedef u32 gcov_unsigned_t;

/*
 * Unfortunately, GCOV_COUNTERS changes with GCC's versions. Hopefully, the
 * following logic is correct and we've missed no cases.
 */

#if (__GNUC__ >= 7)
   #define GCOV_COUNTERS 9
#elif (__GNUC__ > 5) || (__GNUC__ == 5 && __GNUC_MINOR__ >= 1)
   #define GCOV_COUNTERS 10
#elif __GNUC__ == 4 && __GNUC_MINOR__ >= 9
   #define GCOV_COUNTERS 9
#else
   #define GCOV_COUNTERS 8
#endif

/*
 * --------------------------------------------------------------------------
 * Data structures (w/ comments) copied from GCC's source: libgcc/libgcov.h
 * --------------------------------------------------------------------------
 */


/* Information about counters for a single function.  */
struct gcov_ctr_info
{
  gcov_unsigned_t num;          /* number of counters.  */
  gcov_type *values;            /* their values.  */
};

/* Information about a single function.  This uses the trailing array
   idiom. The number of counters is determined from the merge pointer
   array in gcov_info.  The key is used to detect which of a set of
   comdat functions was selected -- it points to the gcov_info object
   of the object file containing the selected comdat function.  */

struct gcov_fn_info
{
  const struct gcov_info *key;          /* comdat key */
  gcov_unsigned_t ident;                /* unique ident of function */
  gcov_unsigned_t lineno_checksum;      /* function lineo_checksum */
  gcov_unsigned_t cfg_checksum;         /* function cfg checksum */
  struct gcov_ctr_info ctrs[1];         /* instrumented counters */
};

/* Type of function used to merge counters.  */
typedef void (*gcov_merge_fn) (gcov_type *, gcov_unsigned_t);

/* Information about a single object file.  */
struct gcov_info
{
  gcov_unsigned_t version;      /* expected version number */
  struct gcov_info *next;       /* link to next, used by libgcov */
  gcov_unsigned_t stamp;        /* uniquifying time stamp */
  const char *filename;         /* output file name */
  gcov_merge_fn merge[GCOV_COUNTERS];  /* merge functions (null for unused) */
  unsigned n_functions;         /* number of functions */
  const struct gcov_fn_info *const *functions; /* pointer to pointers
                                                  to function information  */
};

/*
 * --------------------------------------------------------------------------
 * END
 * --------------------------------------------------------------------------
 */


void __gcov_merge_add(gcov_type *counters, u32 n) { }
void __gcov_exit(void) { }

static struct gcov_info *gi_list;

void __gcov_init(struct gcov_info *info)
{
   if (!gi_list) {
      gi_list = info;
   } else {
      info->next = gi_list;
      gi_list = info;
   }
}
