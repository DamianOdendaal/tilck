
#include "devshell.h"

#define DECL_CMD(name) int cmd_##name(int argc, char **argv)
#define CMD_ENTRY(name, len, enabled) {#name, cmd_ ## name, len, enabled}
#define CMD_END() {NULL, NULL, 0, 0}

DECL_CMD(help);
DECL_CMD(selftest);
DECL_CMD(runall);
DECL_CMD(loop);
DECL_CMD(fork);
DECL_CMD(sysenter);
DECL_CMD(fork_se);
DECL_CMD(bad_read);
DECL_CMD(bad_write);
DECL_CMD(fork_perf);
DECL_CMD(syscall_perf);
DECL_CMD(fpu);
DECL_CMD(fpu_loop);
DECL_CMD(brk);
DECL_CMD(mmap);
DECL_CMD(kcow);
DECL_CMD(wpid1);
DECL_CMD(wpid2);
DECL_CMD(wpid3);
DECL_CMD(wpid4);
DECL_CMD(sigsegv1);
DECL_CMD(sigsegv2);
DECL_CMD(sigill);
DECL_CMD(sigfpe);
DECL_CMD(sigabrt);
DECL_CMD(sig1);
DECL_CMD(select1);
DECL_CMD(select2);
DECL_CMD(poll1);
DECL_CMD(poll2);
DECL_CMD(poll3);
DECL_CMD(bigargv);
DECL_CMD(cloexec);

static struct test_cmd_entry _cmds_table[] =
{
   CMD_ENTRY(help, TT_SHORT, false),
   CMD_ENTRY(selftest, TT_LONG, false),
   CMD_ENTRY(runall, TT_LONG, false),
   CMD_ENTRY(loop, TT_MED, false),

   CMD_ENTRY(fork, TT_MED, true),
   CMD_ENTRY(sysenter, TT_SHORT, true),
   CMD_ENTRY(fork_se, TT_MED, true),
   CMD_ENTRY(bad_read, TT_SHORT, true),
   CMD_ENTRY(bad_write, TT_SHORT, true),
   CMD_ENTRY(fork_perf, TT_LONG, true),
   CMD_ENTRY(syscall_perf, TT_SHORT, true),
   CMD_ENTRY(fpu, TT_SHORT, true),
   CMD_ENTRY(fpu_loop, TT_LONG, false),
   CMD_ENTRY(brk, TT_SHORT, true),
   CMD_ENTRY(mmap, TT_MED, true),
   CMD_ENTRY(kcow, TT_SHORT, true),
   CMD_ENTRY(wpid1, TT_SHORT, true),
   CMD_ENTRY(wpid2, TT_SHORT, true),
   CMD_ENTRY(wpid3, TT_SHORT, true),
   CMD_ENTRY(wpid4, TT_SHORT, true),
   CMD_ENTRY(sigsegv1, TT_SHORT, true),
   CMD_ENTRY(sigsegv2, TT_SHORT, true),
   CMD_ENTRY(sigill, TT_SHORT, true),
   CMD_ENTRY(sigfpe, TT_SHORT, true),
   CMD_ENTRY(sigabrt, TT_SHORT, true),
   CMD_ENTRY(sig1, TT_SHORT, true),
   CMD_ENTRY(bigargv, TT_SHORT, true),
   CMD_ENTRY(cloexec, TT_SHORT, true),

   CMD_ENTRY(select1, TT_SHORT, false),
   CMD_ENTRY(select2, TT_SHORT, false),
   CMD_ENTRY(poll1, TT_SHORT, false),
   CMD_ENTRY(poll2, TT_SHORT, false),
   CMD_ENTRY(poll3, TT_SHORT, false),

   CMD_END(),
};

struct test_cmd_entry *cmds_table = _cmds_table;
