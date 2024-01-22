/* alloca.h */

/*---------------------------------------------------------------------------
 * This is included here so that it will be included instead of the
 * SunOS /usr/include/alloca.h which defines __builtin_alloca on the
 * sparc machines.  Our library does not have __builtin_alloca, so
 * we want the user to provide their own.
 *---------------------------------------------------------------------------
 */
