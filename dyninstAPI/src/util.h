/*
 *  Copyright 1993 Jeff Hollingsworth.  All rights reserved.
 *
 */

/*
 * util.h - support functions.
 *
 * $Log: util.h,v $
 * Revision 1.4  1994/09/22 02:27:52  markc
 * Change signature to intComp
 *
 * Revision 1.3  1994/07/28  22:40:50  krisna
 * changed definitions/declarations of xalloc functions to conform to alloc.
 *
 * Revision 1.2  1994/06/27  18:57:20  hollings
 * removed printfs.  Now use logLine so it works in the remote case.
 * added internalMetric class.
 * added extra paramter to metric info for aggregation.
 *
 * Revision 1.1  1994/01/27  20:31:49  hollings
 * Iinital version of paradynd speaking dynRPC igend protocol.
 *
 * Revision 1.1  1993/03/19  22:51:05  hollings
 * Initial revision
 *
 *
 */
#ifndef UTIL_H
#define UTIL_H

extern "C" {
#include <sys/types.h>
}

extern "C" void *xmalloc(size_t);

extern "C" void *xcalloc(size_t, size_t);

extern "C" void *xrealloc(void *, size_t);

extern void logLine(const char *line);
extern char errorLine[];
extern int intComp(const void *, const void *);

#endif /* UTIL_H */
