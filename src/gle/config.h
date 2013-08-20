/* config.h.  Generated from config.h.in by configure.  */
/* config.h.in.  Generated from configure.in by autoheader.  */
/********************************************************************\
 * acconfig.h -- configuration defines for gle                      *
 * Copyright (C) 2000 Linas Vepstas (linas@linas.org)               *
\********************************************************************/


#ifndef __GLE_CONFIG_H__
#define __GLE_CONFIG_H__

/* Package name and version number */
#define PACKAGE "gle"
#define VERSION "3.1.0"

/* Standard C headers present */
#define STDC_HEADERS 1

/* limits.h header present */
/* #undef HAVE_LIMITS_H */

/* Build for OpenGL by default, and not for old IrisGL aka GL 3.2 */
#define OPENGL_10 1
/* #undef GL_32 */

/* Disable debugging stuff (debugging replaces GL output with printfs) */
/* #undef DEBUG_OUTPUT */

/* Do we have a lenient tesselator? */
/* #undef LENIENT_TESSELATOR */
#define DELICATE_TESSELATOR 1

/* Enable texture mapping by default. */
#define AUTO_TEXTURE 1

/* assume a modern C compiler */
/* #undef FUNKY_C */


#endif

/* Define to 1 if you have the <dlfcn.h> header file. */
#define HAVE_DLFCN_H 1

/* Define to 1 if you have the <inttypes.h> header file. */
#define HAVE_INTTYPES_H 1

/* Define to 1 if you have the `memcpy' function. */
#define HAVE_MEMCPY 1

/* Define to 1 if you have the <memory.h> header file. */
#define HAVE_MEMORY_H 1

/* Define to 1 if you have the <stdint.h> header file. */
#define HAVE_STDINT_H 1

/* Define to 1 if you have the <stdlib.h> header file. */
#define HAVE_STDLIB_H 1

/* Define to 1 if you have the <strings.h> header file. */
#define HAVE_STRINGS_H 1

/* Define to 1 if you have the <string.h> header file. */
#define HAVE_STRING_H 1

/* Define to 1 if you have the <sys/stat.h> header file. */
#define HAVE_SYS_STAT_H 1

/* Define to 1 if you have the <sys/types.h> header file. */
#define HAVE_SYS_TYPES_H 1

/* Define to 1 if you have the <unistd.h> header file. */
#define HAVE_UNISTD_H 1

/* Define to the sub-directory in which libtool stores uninstalled libraries.
   */
#define LT_OBJDIR ".libs/"

/* Name of package */
#define PACKAGE "gle"

/* Define to the address where bug reports for this package should be sent. */
#define PACKAGE_BUGREPORT ""

/* Define to the full name of this package. */
#define PACKAGE_NAME ""

/* Define to the full name and version of this package. */
#define PACKAGE_STRING ""

/* Define to the one symbol short name of this package. */
#define PACKAGE_TARNAME ""

/* Define to the home page for this package. */
#define PACKAGE_URL ""

/* Define to the version of this package. */
#define PACKAGE_VERSION ""

/* Define to 1 if you have the ANSI C header files. */
#define STDC_HEADERS 1

/* Version number of package */
#define VERSION "3.1.0"

/* Define to 1 if the X Window System is missing or not being used. */
/* #undef X_DISPLAY_MISSING */
