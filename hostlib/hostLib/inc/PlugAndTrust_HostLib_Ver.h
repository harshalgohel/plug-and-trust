/* Copyright 2019-2021 NXP
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 *
 */

#ifndef PLUGANDTRUST_HOSTLIB_VERSION_INFO_H_INCLUDED
#define PLUGANDTRUST_HOSTLIB_VERSION_INFO_H_INCLUDED


/* clang-format off */
#define PLUGANDTRUST_HOSTLIB_PROD_NAME          "PlugAndTrust_HostLib"
#define PLUGANDTRUST_HOSTLIB_VER_STRING_NUM     "v04.03.00_20221122"
#define PLUGANDTRUST_HOSTLIB_PROD_NAME_VER_FULL "PlugAndTrust_HostLib_v04.03.00_20221122"
#define PLUGANDTRUST_HOSTLIB_VER_MAJOR          (4u)
#define PLUGANDTRUST_HOSTLIB_VER_MINOR          (3u)
#define PLUGANDTRUST_HOSTLIB_VER_DEV            (0u)

/* v04.03 = 40003u */
#define PLUGANDTRUST_HOSTLIB_VER_MAJOR_MINOR ( 0 \
    | (PLUGANDTRUST_HOSTLIB_VER_MAJOR * 10000u)    \
    | (PLUGANDTRUST_HOSTLIB_VER_MINOR))

/* v04.03.00 = 400030000ULL */
#define PLUGANDTRUST_HOSTLIB_VER_MAJOR_MINOR_DEV ( 0 \
    | (PLUGANDTRUST_HOSTLIB_VER_MAJOR * 10000*10000u)    \
    | (PLUGANDTRUST_HOSTLIB_VER_MINOR * 10000u)    \
    | (PLUGANDTRUST_HOSTLIB_VER_DEV))

/* clang-format on */


/* Version Information:
 * Generated by:
 *     scripts\version_info.py (v2019.01.17_00)
 *
 * Do not edit this file. Update:
 *     hostlib/version_info.txt instead.
 *
 *
 * prod_name = "PlugAndTrust_HostLib"
 *
 * prod_desc = "Host Library"
 *
 * lang_c_prefix = prod_name.upper()
 *
 * lang_namespace = ""
 *
 * v_major  = "04"
 *
 * v_minor  = "03"
 *
 * v_dev    = "00"
 *
 * v_meta   = ""
 *
 * maturity = "B"
 *
 *
 */

#endif /* PLUGANDTRUST_HOSTLIB_VERSION_INFO_H_INCLUDED */
