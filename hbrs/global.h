#pragma once
//hisi sdk
#include <hifb.h>
#include <hi_comm_aenc.h>
#include <hi_comm_aio.h>
#include <hi_comm_hdmi.h>
#include <hi_comm_pciv.h>
#include <hi_comm_vb.h>
#include <hi_comm_vdec.h>
#include <hi_comm_venc.h>
#include <hi_comm_vi.h>
#include <hi_comm_vo.h>
#include <hi_comm_vpss.h>
#include <hi_common.h>
#include <hi_debug.h>
#include <hi_mcc_usrdev.h>
#include <mpi_aenc.h>
#include <mpi_ai.h>
#include <mpi_ao.h>
#include <mpi_hdmi.h>
#include <mpi_pciv.h>
#include <mpi_sys.h>
#include <mpi_vb.h>
#include <mpi_vdec.h>
#include <mpi_venc.h>
#include <mpi_vi.h>
#include <mpi_vo.h>
#include <mpi_vpss.h>
//system
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <fcntl.h>
#include <unistd.h>
//stl
#include <vector>
#include <map>
#include <mutex>
#include <atomic>
#include <thread>
#include <memory>
#include <chrono>
#include <system_error>
//stdlib
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
//drive
#include <tw6874_ioctl_cmd.h>
//logger
#include <elog.h>

#define RS_ALIGN_WIDTH 64                               //图像对齐大小
#define RS_PIXEL_FORMAT PIXEL_FORMAT_YUV_SEMIPLANAR_420 //图像像素格式
#define RS_PCIV_WINDOW_SIZE 7340032                     //PCIV窗口大小
#define RS_PCIV_SLAVE1_ID 1                             //PCIV从片1地址
#define RS_PCIV_SLAVE3_ID 3                             //PCIV从片3地址
#define RS_MAX_WIDTH 1920                               //最大支持的视频宽度
#define RS_MAX_HEIGHT 1080                              //最大支持的视频长度
#define RS_PCIV_CMD_PORT 0                              //PCIV命令端口
#define RS_PCIV_TRANS_READ_PORT 1                       //PCIV传输读端口
#define RS_PCIV_TRANS_WRITE_PORT 2                      //PCIV传输写端口
#define RS_MEM_BLK_NUM 20                               //系统VB内存块分配数量
#define RS_TOTAL_SCENE_NUM 6                            //总场景数量
#define RS_MASTER_SDI_BASE 0                            //主片SDI通道起始偏移
#define RS_MASTER_SDI_NUM 2                             //主片SDI通道的数量
#define RS_SLAVE_SDI_BASE 1                             //从片SDI通道起始偏移
#define RS_SLAVE_SDI_NUM 4                              //从片SDI通道的数量

#define RS_ASSERT(cond)     \
    while (!(cond))         \
    {                       \
        log_e("%s", #cond); \
        exit(1);            \
    }

enum RS_SCENE
{
    TEA_FEATURE = 0,
    STU_FEATURE,
    TEA_FULL_VIEW,
    STU_FULL_VIEW,
    BLACK_BOARD_FEATURE,
    PC_CAPTURE
};