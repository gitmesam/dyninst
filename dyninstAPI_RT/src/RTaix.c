/*
 * Copyright (c) 1996-2004 Barton P. Miller
 * 
 * We provide the Paradyn Parallel Performance Tools (below
 * described as "Paradyn") on an AS IS basis, and do not warrant its
 * validity or performance.  We reserve the right to update, modify,
 * or discontinue this software at any time.  We shall have no
 * obligation to supply such updates or modifications or any other
 * form of support to you.
 * 
 * This license is for research uses.  For such uses, there is no
 * charge. We define "research use" to mean you may freely use it
 * inside your organization for whatever purposes you see fit. But you
 * may not re-distribute Paradyn or parts of Paradyn, in any form
 * source or binary (including derivatives), electronic or otherwise,
 * to any other organization or entity without our permission.
 * 
 * (for other uses, please contact us at paradyn@cs.wisc.edu)
 * 
 * All warranties, including without limitation, any warranty of
 * merchantability or fitness for a particular purpose, are hereby
 * excluded.
 * 
 * By your use of Paradyn, you understand and agree that we (or any
 * other person or entity with proprietary rights in Paradyn) are
 * under no obligation to provide either maintenance services,
 * update services, notices of latent defects, or correction of
 * defects for Paradyn.
 * 
 * Even if advised of the possibility of such damages, under no
 * circumstances shall we (or any other person or entity with
 * proprietary rights in the software licensed hereunder) be liable
 * to you or any third party for direct, indirect, or consequential
 * damages of any character regardless of type of action, including,
 * without limitation, loss of profits, loss of use, loss of good
 * will, or computer failure or malfunction.  You agree to indemnify
 * us (and any other person or entity with proprietary rights in the
 * software licensed hereunder) for any and all liability it may
 * incur to third parties resulting from your use of Paradyn.
 */

/************************************************************************
 * RTaix.c: mutatee-side library function specific to AIX
************************************************************************/

#if !defined (EXPORT_SPINLOCKS_AS_HEADER)
/* everything should be under this flag except for the assembly code
   that handles the runtime spinlocks  -- this is imported into the
   test suite for direct testing */

#include <dlfcn.h> /* dlopen constants */
#include <stdio.h>
#include "dyninstAPI_RT/h/dyninstAPI_RT.h"
/************************************************************************
 * void DYNINSTos_init(void)
 *
 * os initialization function
************************************************************************/

void DYNINSTstaticHeap_1048576_textHeap_libSpace(void);
void
DYNINSTos_init(int calledByFork, int calledByAttach)
{
/* Text heap initialization. Call this to force the library to
   be included by the linker */
#ifdef USES_LIB_TEXT_HEAP
    /* Dummy call to get the library space actually included
       (not pruned by an optimizing linker) */
    DYNINSTstaticHeap_1048576_textHeap_libSpace();
#endif

}

char gLoadLibraryErrorString[ERROR_STRING_LENGTH];
int DYNINSTloadLibrary(char *libname)
{
  void *res;
  char *err_str;
  gLoadLibraryErrorString[0]='\0';
  
  if (NULL == (res = dlopen(libname, RTLD_NOW | RTLD_GLOBAL))) {
    // An error has occurred
    perror( "DYNINSTloadLibrary -- dlopen" );
    
    if (NULL != (err_str = dlerror()))
      strncpy(gLoadLibraryErrorString, err_str, ERROR_STRING_LENGTH);
    else 
      sprintf(gLoadLibraryErrorString,"unknown error with dlopen");
    
    //fprintf(stderr, "%s[%d]: %s\n",__FILE__,__LINE__,gLoadLibraryErrorString);
    return 0;  
  } else
    return 1;
}

#endif /* EXPORT_SPINLOCKS_AS_HEADER */

void DYNINSTlock_spinlock(dyninst_spinlock *mut)
{

 asm (
         "  .Loop:\n"
         " lwarx        4,0,3    # &lock in R3, reserves addr in R3 for future atomic store\n"
         "                       # R4 <- *R3         \n"
         " cmpwi        4,1      # if lock set, spin.\n"
         " beq-         .Loop                        \n"
         " lil          5,1      # R5 <- 1 (indicate mutex is locked)  \n"
         " stwcx.       5,0,3    # atomic store R5 in memory given by R3 (&lock)\n"
         " bne-         .Loop    # atomic store failed, try again   \n"
         " isync                 # memory barrier, ensures lock obtained before cont\n"
     );

}
