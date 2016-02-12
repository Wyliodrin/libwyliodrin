#ifndef _VERSION_H
#define _VERSION_H



/*** API *****************************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Return version major
 */
int get_version_major();

/**
 * Return version minor
 */
int get_version_minor();

#ifdef __cplusplus
}
#endif

/*************************************************************************************************/



#endif /* _VERSION_H */
