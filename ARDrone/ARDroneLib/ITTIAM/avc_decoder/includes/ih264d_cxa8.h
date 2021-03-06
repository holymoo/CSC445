/*****************************************************************************/
/*                                                                           */
/*                 H264 ASP Decoder on Cortex A8 Ittiam APIs                */
/*                     ITTIAM SYSTEMS PVT LTD, BANGALORE                     */
/*                             COPYRIGHT(C) 2010                             */
/*                                                                           */
/*  This program  is  proprietary to  Ittiam  Systems  Private  Limited  and */
/*  is protected under Indian  Copyright Law as an unpublished work. Its use */
/*  and  disclosure  is  limited by  the terms  and  conditions of a license */
/*  agreement. It may not be copied or otherwise  reproduced or disclosed to */
/*  persons outside the licensee's organization except in accordance with the*/
/*  terms  and  conditions   of  such  an  agreement.  All  copies  and      */
/*  reproductions shall be the property of Ittiam Systems Private Limited and*/
/*  must bear this notice in its entirety.                                   */
/*                                                                           */
/*****************************************************************************/
/*****************************************************************************/
/*                                                                           */
/*  File Name         : ih264d_cxa8_cxa8.h                                    */
/*                                                                           */
/*  Description       : This file contains all the necessary structure and   */
/*                      enumeration definitions needed for the Application   */
/*                      Program Interface(API) of the Ittiam H264 ASP       */
/*                      Decoder on Cortex A8 - Neon platform                 */
/*                                                                           */
/*  List of Functions : ih264d_cxa8_api_function                              */
/*                                                                           */
/*  Issues / Problems : None                                                 */
/*                                                                           */
/*  Revision History  :                                                      */
/*                                                                           */
/*         DD MM YYYY   Author(s)       Changes (Describe the changes made)  */
/*         26 08 2010   100239(RCY)     Draft                                */
/*                                                                           */
/*****************************************************************************/

#ifndef _IH264D_CXA8_H
#define _IH264D_CXA8_H

#include "iv.h"
#include "ivd.h"


/*****************************************************************************/
/* Constant Macros                                                           */
/*****************************************************************************/

#if DSP
#define H264DEC_MEMRECS   7
#else
#define H264DEC_MEMRECS   5
#endif
#ifdef APPLY_CONCEALMENT
#define NUMBER_OF_MEM_REC					(H264DEC_MEMRECS + 2)
#else //APPLY_CONCEALMENT
#define NUMBER_OF_MEM_REC					H264DEC_MEMRECS
#endif //APPLY_CONCEALMENT

/*****************************************************************************/
/* Function Macros                                                           */
/*****************************************************************************/
#define IS_IVD_CONCEALMENT_APPLIED(x)       (x & (1 << IVD_APPLIEDCONCEALMENT))
#define IS_IVD_INSUFFICIENTDATA_ERROR(x)    (x & (1 << IVD_INSUFFICIENTDATA))
#define IS_IVD_CORRUPTEDDATA_ERROR(x)       (x & (1 << IVD_CORRUPTEDDATA))
#define IS_IVD_CORRUPTEDHEADER_ERROR(x)     (x & (1 << IVD_CORRUPTEDHEADER))
#define IS_IVD_UNSUPPORTEDINPUT_ERROR(x)    (x & (1 << IVD_UNSUPPORTEDINPUT))
#define IS_IVD_UNSUPPORTEDPARAM_ERROR(x)    (x & (1 << IVD_UNSUPPORTEDPARAM))
#define IS_IVD_FATAL_ERROR(x)               (x & (1 << IVD_FATALERROR))
#define IS_IVD_INVALID_BITSTREAM_ERROR(x)   (x & (1 << IVD_INVALID_BITSTREAM))
#define IS_IVD_INCOMPLETE_BITSTREAM_ERROR(x) (x & (1 << IVD_INCOMPLETE_BITSTREAM))


/*****************************************************************************/
/* API Function Prototype                                                    */
/*****************************************************************************/
IV_API_CALL_STATUS_T ih264d_cxa8_api_function(iv_obj_t *ps_handle, void *pv_api_ip,void *pv_api_op);

/*****************************************************************************/
/* Enums                                                                     */
/*****************************************************************************/
/* Codec Error codes for H264 ASP Decoder                                   */

typedef enum {

    IH264D_CXA8_VID_HDR_DEC_NUM_FRM_BUF_NOT_SUFFICIENT   = IVD_DUMMY_ELEMENT_FOR_CODEC_EXTENSIONS + 1,

}IH264D_CXA8_ERROR_CODES_T;

/*****************************************************************************/
/* Extended Structures                                                       */
/*****************************************************************************/

/*****************************************************************************/
/*  Get Number of Memory Records                                             */
/*****************************************************************************/


typedef struct {
    iv_num_mem_rec_ip_t                    s_ivd_num_rec_ip_t;
}ih264d_cxa8_num_mem_rec_ip_t;


typedef struct{
    iv_num_mem_rec_op_t                    s_ivd_num_mem_rec_op_t;
}ih264d_cxa8_num_mem_rec_op_t;


/*****************************************************************************/
/*  Fill Memory Records                                                      */
/*****************************************************************************/


typedef struct {
    iv_fill_mem_rec_ip_t                	s_ivd_fill_mem_rec_ip_t;
    WORD32									s_level;
    UWORD32									s_num_reorder_frames;
    UWORD32									s_num_ref_frames;
}ih264d_cxa8_fill_mem_rec_ip_t;


typedef struct{
    iv_fill_mem_rec_op_t                   s_ivd_fill_mem_rec_op_t;
}ih264d_cxa8_fill_mem_rec_op_t;

/*****************************************************************************/
/*  Retrieve Memory Records                                                  */
/*****************************************************************************/


typedef struct {
    iv_retrieve_mem_rec_ip_t               s_ivd_retrieve_mem_rec_ip_t;
}ih264d_cxa8_retrieve_mem_rec_ip_t;


typedef struct{
    iv_retrieve_mem_rec_op_t               s_ivd_retrieve_mem_rec_op_t;
}ih264d_cxa8_retrieve_mem_rec_op_t;


/*****************************************************************************/
/*   Initialize decoder                                                      */
/*****************************************************************************/


typedef struct {
    ivd_init_ip_t                           s_ivd_init_ip_t;
    WORD32									s_level;
    UWORD32									s_num_reorder_frames;
    UWORD32									s_num_ref_frames;
}ih264d_cxa8_init_ip_t;


typedef struct{
    ivd_init_op_t                           s_ivd_init_op_t;
}ih264d_cxa8_init_op_t;


/*****************************************************************************/
/*   Video Decode                                                            */
/*****************************************************************************/


typedef struct {
    ivd_video_decode_ip_t                   s_ivd_video_decode_ip_t;
}ih264d_cxa8_video_decode_ip_t;


typedef struct{
    ivd_video_decode_op_t                   s_ivd_video_decode_op_t;
}ih264d_cxa8_video_decode_op_t;


/*****************************************************************************/
/*   Get Display Frame                                                       */
/*****************************************************************************/


typedef struct
{
    ivd_get_display_frame_ip_t              s_ivd_get_display_frame_ip_t;
}ih264d_cxa8_get_display_frame_ip_t;


typedef struct
{
    ivd_get_display_frame_op_t              s_ivd_get_display_frame_op_t;
}ih264d_cxa8_get_display_frame_op_t;


/*****************************************************************************/
/*   Release Display Buffers                                                 */
/*****************************************************************************/


typedef struct
{
    ivd_rel_disp_buff_ip_t                  s_ivd_rel_disp_buff_ip_t;
}ih264d_cxa8_rel_disp_buff_ip_t;


typedef struct
{
    ivd_rel_disp_buff_op_t                  s_ivd_rel_disp_buff_op_t;
}ih264d_cxa8_rel_disp_buff_op_t;

/*****************************************************************************/
/*   Video control  Flush                                                    */
/*****************************************************************************/


typedef struct{
    ivd_ctl_flush_ip_t                      s_ivd_ctl_flush_ip_t;
}ih264d_cxa8_ctl_flush_ip_t;


typedef struct{
    ivd_ctl_flush_op_t                      s_ivd_ctl_flush_op_t;
}ih264d_cxa8_ctl_flush_op_t;

/*****************************************************************************/
/*   Video control reset                                                     */
/*****************************************************************************/


typedef struct{
    ivd_ctl_reset_ip_t                      s_ivd_ctl_reset_ip_t;
}ih264d_cxa8_ctl_reset_ip_t;


typedef struct{
    ivd_ctl_reset_op_t                      s_ivd_ctl_reset_op_t;
}ih264d_cxa8_ctl_reset_op_t;


/*****************************************************************************/
/*   Video control  Set Params                                               */
/*****************************************************************************/


typedef struct {
    ivd_ctl_set_config_ip_t             s_ivd_ctl_set_config_ip_t;
}ih264d_cxa8_ctl_set_config_ip_t;


typedef struct{
    ivd_ctl_set_config_op_t             s_ivd_ctl_set_config_op_t;
}ih264d_cxa8_ctl_set_config_op_t;

/*****************************************************************************/
/*   Video control:Get Buf Info                                              */
/*****************************************************************************/


typedef struct{
    ivd_ctl_getbufinfo_ip_t             s_ivd_ctl_getbufinfo_ip_t;
}ih264d_cxa8_ctl_getbufinfo_ip_t;



typedef struct{
    ivd_ctl_getbufinfo_op_t             s_ivd_ctl_getbufinfo_op_t;
}ih264d_cxa8_ctl_getbufinfo_op_t;


/*****************************************************************************/
/*   Video control:Getstatus Call                                            */
/*****************************************************************************/


typedef struct{
    ivd_ctl_getstatus_ip_t                  s_ivd_ctl_getstatus_ip_t;
}ih264d_cxa8_ctl_getstatus_ip_t;



typedef struct{
    ivd_ctl_getstatus_op_t                  s_ivd_ctl_getstatus_op_t;
}ih264d_cxa8_ctl_getstatus_op_t;


/*****************************************************************************/
/*   Video control:Get Version Info                                          */
/*****************************************************************************/


typedef struct{
    ivd_ctl_getversioninfo_ip_t         s_ivd_ctl_getversioninfo_ip_t;
}ih264d_cxa8_ctl_getversioninfo_ip_t;



typedef struct{
    ivd_ctl_getversioninfo_op_t         s_ivd_ctl_getversioninfo_op_t;
}ih264d_cxa8_ctl_getversioninfo_op_t;


#endif /* _IH264D_CXA8_H */
