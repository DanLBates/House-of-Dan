# Linux.s
#
# Include to symbolize the assembly interface to linux
#
#
# Warning not all of these have been fully tested. In fact only a few
#  have been fully tested. Use these at your own risc.
# It is however easier to check than it was for me to type them all
# in. Please inform me of any faults and I will investigate & correct them.
#
#system call numbers X86 64
        .equ    sys_read,             0
        .equ    sys_write,            1
        .equ    sys_open,             2
        .equ    sys_close,            3
        .equ    sys_stat,             4
        .equ    sys_fstat,            5
        .equ    sys_lstat,            6
        .equ    sys_poll,             7
        .equ    sys_lseek,            8
        .equ    sys_mmap,             9
        .equ    sys_mprotect,        10
        .equ    sys_munmap,          11
        .equ    sys_brk,             12
        .equ    sys_rt_sigaction,    13
        .equ    sys_rt_sigprocmask,  14
        .equ    sys_rt_sigreturn,    15
        .equ    sys_ioctl,           16
        .equ    sys_pread64,         17
        .equ    sys_pwrite64,        18
        .equ    sys_readv,           19
        .equ    sys_writev,          20
        .equ    sys_access,          21
        .equ    sys_pipe,            22
        .equ    sys_select,          23
        .equ    sys_sched_yield,     24
        .equ    sys_mremap,          25
        .equ    sys_msync,           26
        .equ    sys_mincore,         27
        .equ    sys_madvise,         28
        .equ    sys_shmget,          29
        .equ    sys_shmat,           30
        .equ    sys_shmtcl,          31
        .equ    sys_dup,             32
        .equ    sys_dup2,            33
        .equ    sys_pause,           34
        .equ    sys_nanosleep,       35
        .equ    sys_getitimer,       36
        .equ    sys_alarm,           37
        .equ    sys_setitimer,       38
        .equ    sys_getpid,          39
        .equ    sys_sendfile,        40
        .equ    sys_socket,          41
        .equ    sys_connect,         42
        .equ    sys_accept,          43
        .equ    sys_sendto,          44
        .equ    sys_recvfrom,        45
        .equ    sys_sendmsg,         46
        .equ    sys_recvmsg,         47
        .equ    sys_shutdown,        48
        .equ    sys_bind,            49
        .equ    sys_listen,          50
        .equ    sys_getsockname,     51
        .equ    sys_getpeername,     52
        .equ    sys_socketpair,      53
        .equ    sys_setsockopt,      54
        .equ    sys_getsockopt,      55
        .equ    sys_clone,           56
        .equ    sys_fork,            57
        .equ    sys_vfork,           58
        .equ    sys_execve,          59
        .equ    sys_exit,            60
        .equ    sys_wait4,           61
        .equ    sys_kill,            62
        .equ    sys_uname,           63
        .equ    sys_semget,          64
        .equ    sys_semop,           65
        .equ    sys_semctl,          66
        .equ    sys_shmdt,           67
        .equ    sys_msgget,          68
        .equ    sys_msgsnd,          69
        .equ    sys_msgrcv,          70
        .equ    sys_msgctl,          71
        .equ    sys_fcntl,           72
        .equ    sys_flock,           73
        .equ    sys_fsync,           74
        .equ    sys_fdatasync,       75
        .equ    sys_truncate,        76
        .equ    sys_ftruncate,       77
        .equ    sys_getdents,        78
        .equ    sys_getcwd,          79
        .equ    sys_chdir,           80
        .equ    sys_fchdir,          81
        .equ    sys_rename,          82
        .equ    sys_mkdir,           83
        .equ    sys_rmdir,           84
        .equ    sys_creat,           85
        .equ    sys_link,            86
        .equ    sys_unlink,          87
        .equ    sys_symlink,         88
        .equ    sys_readlink,        89
        .equ    sys_chmod,           90
        .equ    sys_fchmod,          91
        .equ    sys_chown,           92
        .equ    sys_fchown,          93
        .equ    sys_lchown,          94
        .equ    sys_umask,           95
        .equ    sys_gettimeofday,    96
        .equ    sys_getrlimit,       97
        .equ    sys_getrusage,       98
        .equ    sys_sysinfo,         99
        .equ    sys_times,          100
        .equ    sys_ptrace,         101
        .equ    sys_getuid,         102
        .equ    sys_syslog,         103
        .equ    sys_getgid,         104
        .equ    sys_setuid,         105
        .equ    sys_setgid,         106
        .equ    sys_geteuid,        107
        .equ    sys_getegid,        108
        .equ    sys_setpgid,        109
        .equ    sys_getppid,        110
        .equ    sys_getpgrp,        111
        .equ    sys_setsid,         112
        .equ    sys_setreuid,       113
        .equ    sys_setregid,       114
        .equ    sys_getgroups,      115
        .equ    sys_setgroups,      116
        .equ    sys_setreuid,       117
        .equ    sys_getresuid,      118
        .equ    sys_setresgid,      119
        .equ    sys_getresgid,      120
        .equ    sys_getpgid,        121
        .equ    sys_setfsuid,       122
        .equ    sys_setfsgid,       123
        .equ    sys_getsid,         124
        .equ    sys_capget,         125
        .equ    sys_capset,         126
        .equ    sys_rt_sigpending,  127
        .equ    sys_rt_sigtimedwait,128
        .equ    sys_rt_sigqueueinfo,129
        .equ    sys_rt_sigsuspend,  130
        .equ    sys_sig_altstack,   131
        .equ    sys_sysutime,       132
        .equ    sys_mknod,          133
        .equ    sys_uselib,         134
        .equ    sys_personality,    135
        .equ    sys_ustat,          136
        .equ    sys_statfs,         137
        .equ    sys_fstatfs,        138
        .equ    sys_sysfs,          139
        .equ    sys_getpriority,    140
        .equ    sys_setpriority,    141
        .equ    sys_sched_setparam, 142
        .equ    sys_sched_getparam, 143
        .equ    sys_sched_setscheduler,     144
        .equ    sys_sched_getscheduler,     145
        .equ    sys_sched_get_priority_max, 146
        .equ    sys_sched_get_priority_min, 147
        .equ    sys_sched_rr_get_interval,  148
        .equ    sys_mlock,          149
        .equ    sys_munlock,        150
        .equ    sys_mlockall,       151
        .equ    sys_munlockall,     152
        .equ    sys_vhangup,        153
        .equ    sys_modify_ldt,     154
        .equ    sys_pivot_root,     155
        .equ    sys__sysctl,        156
        .equ    sys_prctl,          157
        .equ    sys_arch_prctl,     158
        .equ    sys_adjtimex,       159
        .equ    sys_setrlimit,      160
        .equ    sys_chroot,         161
        .equ    sys_sync,           162
        .equ    sys_acct,           163
        .equ    sys_settimeofday,   164
        .equ    sys_mount,          165
        .equ    sys_umount2,        166
        .equ    sys_swapon,         167
        .equ    sys_swapoff,        168
        .equ    sys_reboot,         169
        .equ    sys_sethostname,    170
        .equ    sys_setdomainname,  171
        .equ    sys_iopl,           172
        .equ    sys_ioperm,         173
        .equ    sys_init_module,    175
        .equ    sys_delete_module,  176
        .equ    sys_quotactl,       179
        .equ    sys_gettid,         186
        .equ    sys_readahead,      187
        .equ    sys_setxattr,       188
        .equ    sys_lsetxattr,      189
        .equ    sys_fsetxattr,      190
        .equ    sys_getxattr,       191
        .equ    sys_lgetxattr,      192
        .equ    sys_fgetxattr,      193
        .equ    sys_listxattr,      194
        .equ    sys_llistxattr,     195
        .equ    sys_flistxattr,     196
        .equ    sys_removexattr,    197
        .equ    sys_lremovexattr,   198
        .equ    sys_fremovexattr,   199
        .equ    sys_tkill,          200
        .equ    sys_time,           201
        .equ    sys_futex,          202
        .equ    sys_sched_setaffinity,203
        .equ    sys_sched_getaffinity,204
        .equ    sys_io_setup,       206
        .equ    sys_io_destroy,     207
        .equ    sys_io_getevents,   208
        .equ    sys_io_submit,      209
        .equ    sys_io_cancel,      210
        .equ    sys_get_thread_area,211
        .equ    sys_lookup_dcookie, 212
        .equ    sys_epoll_create,   213
        .equ    sys_epoll_ctl_old,  214
        .equ    sys_epoll_wait_old, 215
        .equ    sys_remap_file_pages,216
        .equ    sys_getdents64,     217
        .equ    sys_set_tid_address,218
        .equ    sys_restart_syscall,219
        .equ    sys_semtimedop,     220
        .equ    sys_fadvise64,      221
        .equ    sys_timer_create,   222
        .equ    sys_timer_settime,  223
        .equ    sys_timer_gettime,  224
        .equ    sys_timer_getoverrun,225
        .equ    sys_timer_delete,   226
        .equ    sys_clock_settime,  227
        .equ    sys_clock_gettime,  228
        .equ    sys_clock_getres,   229
        .equ    sys_clock_nanosleep,230
        .equ    sys_exit_group,     231
        .equ    sys_epoll_wait,     232
        .equ    sys_epoll_ctl,      233
        .equ    sys_tgkill,         234
        .equ    sys_utimes,         235
        .equ    sys_vserver,        236
        .equ    sys_mbind,          237
        .equ    sys_set_mempolicy,  238
        .equ    sys_get_mempolicy,  239
        .equ    sys_mq_open,        240
        .equ    sys_mq_unlink,      241
        .equ    sys_mq_timedsend,   242
        .equ    sys_mq_timedrecieve,243
        .equ    sys_mq_notify,      244
        .equ    sys_mq_getsetattr,  245
        .equ    sys_kexec_load,     246
        .equ    sys_waitid,         247
        .equ    sys_add_key,        248
        .equ    sys_request_key,    249
        .equ    sys_keyctl,         250
        .equ    sys_ioprio_set,     251
        .equ    sys_ioprio_get,     252
        .equ    sys_inotify_init,   253
        .equ    sys_inotify_add_watch,254
        .equ    sys_inotify_rm_watch, 255
        .equ    sys_migrate_pages,  256
        .equ    sys_openat,         257
        .equ    sys_mkdirat,        258
        .equ    sys_mknodat,        259
        .equ    sys_fchownat,       260
        .equ    sys_futimesat,      261
        .equ    sys_newfstatat,     262
        .equ    sys_unlinkat,       263
        .equ    sys_renameat,       264
        .equ    sys_linkat,         265
        .equ    sys_symlinkat,      266
        .equ    sys_readlinkat,     267
        .equ    sys_fchmodat,       268
        .equ    sys_faccessat,      269
        .equ    sys_pselect6,       270
        .equ    sys_ppoll,          271
        .equ    sys_unshare,        272
        .equ    sys_set_robust_list,273
        .equ    sys_get_robust_list,274
        .equ    sys_splice,         275
        .equ    sys_tee,            276
        .equ    sys_sync_file_range,277
        .equ    sys_vmsplice,       278
        .equ    sys_move_pages,     279
        .equ    sys_utimensat,      280
        .equ    sys_epoll_pwait,    281
        .equ    sys_signalfd,       282
        .equ    sys_timerfd_create, 283
        .equ    sys_eventfd,        284
        .equ    sys_fallocate,      285
        .equ    sys_timerfd_settime,286
        .equ    sys_timerfd_gettime,287
        .equ    sys_accept4,        288
        .equ    sys_signalfd4,      289
        .equ    sys_eventfd2,       290
        .equ    sys_epoll_createl,  291
        .equ    sys_dup3,           292
        .equ    sys_pipe2,          293
        .equ    sys_inotify_initl,  294
        .equ    sys_preadv,         295
        .equ    sys_pwritev,        296
        .equ    sys_rt_tsigqueueinfo,297
        .equ    sys_perf_event_open,298
        .equ    sys_recvmmsg,       299
        .equ    sys_fanotify_init,  300
        .equ    sys_fanotify_mark,  301
        .equ    sys_prlimit64,      302
        .equ    sys_name_to_handle_at,303
        .equ    sys_open_by_handle_at,304
        .equ    sys_clock_adjtime,  305
        .equ    sys_syncfs,         306
        .equ    sys_sendmmsg,       307
        .equ    sys_setns,          308
        .equ    sys_getcpu,         309
        .equ    sys_process_vm_readv, 310
        .equ    sys_process_vm_writev,311
        .equ    sys_kemp,           312
        .equ    sys_finit_module,   313
        .equ    sys_sched_setattr,  314
        .equ    sys_sched_getattr,  315
        .equ    sys_renameat2,      316
        .equ    sys_seccomp,        317
        .equ    sys_getrandom,      318
        .equ    sys_memfd_create,   319
        .equ    sys_kexec_file_load,320
        .equ    sys_bpf,            321
        .equ    sys_stub_execveat,  322
        .equ    sys_userfaultfd,    323
        .equ    sys_membarrier,     324
        .equ    sys_mlock2,         325
        .equ    sys_copy_file_range,326
        .equ    sys_preadv2,        327
        .equ    sys_pwritev2,       328

#options for open
        .equ    O_RDONLY, 0
        .equ    O_CREAT_WRONLY_TRUNC, 03101

#system call interface is syscall a 64 bit instruction

#end of file
        .equ    END_OF_FILE, 0

#stdin, stdout, stderr
        .equ    STDIN, 0
        .equ    STDOUT,1
        .equ    STDERR,2

# offsets from initial %rsp (should be moved to %rbp)
        .equ    ARGC, 0         #count of Number of Arguments
        .equ    ARG0PNAME,8     #arg[0] is program name;
        .equ    ARG1,16         #arg[1] is first arg after program name;
        .equ    ARG2,24         #arg[2] is second arg after program name;
        .equ    ARG3,32         #arg[3] is third arg after program name;
        .equ    ARG4,40         #arg[4] is fourth arg after program name;
        .equ    ARG5,48         #arg[5] is fifth arg after program name;
        .equ    ARG6,56         #arg[6] is sixth arg after program name;
        .equ    ARG7,64         #arg[7] is seventh arg after program name;
        .equ    ARG8,72         #arg[8] is eighth arg after program name;
        .equ    ARG9,80         #arg[9] is ninth arg after program name;
        .equ    ARG10,88        #arg[10] is tenth arg after program name;
        .equ    ARG11,96        #arg[11] is eleventh arg after program name;
