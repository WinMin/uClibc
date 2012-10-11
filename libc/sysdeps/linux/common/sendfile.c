/* vi: set sw=4 ts=4: */
/*
 * sendfile() for uClibc
 *
 * Copyright (C) 2000-2006 Erik Andersen <andersen@uclibc.org>
 *
 * Licensed under the LGPL v2.1, see the file COPYING.LIB in this tarball.
 */

#include <sys/syscall.h>
#include <stddef.h>
#include <unistd.h>
#include <sys/sendfile.h>

#if defined(__NR_sendfile64) && !defined(__NR_sendfile)
ssize_t sendfile(int out_fd, int in_fd, __off_t *offset, size_t count)
{
	__off64_t off64, *off;
	ssize_t res;

	/*
	 * Check if valids fd and valid pointers were passed
	 * This does not prevent the user from passing
	 * an arbitrary pointer causing a segfault or
	 * other security issues
	 */

	if (in_fd < 0 || out_fd < 0) {
		__set_errno(EBADF);
		return -1;
	}

	if (offset == NULL || (int)offset < 0) {
		__set_errno(EFAULT);
		return -1;
	}

	if (offset) {
		off = &off64;
		off64 = *offset;
	} else {
		off = NULL;
	}

	res = INLINE_SYSCALL(sendfile64, 4, out_fd, in_fd, off, count);

	if (res >= 0)
		*offset = off64;

	return res;
}

#elif defined(__NR_sendfile)
_syscall4(ssize_t, sendfile, int, out_fd, int, in_fd, __off_t *, offset,
		  size_t, count)

#if ! defined(__NR_sendfile64) && defined(__UCLIBC_HAS_LFS__)
strong_alias(sendfile,sendfile64)
#endif

#endif
