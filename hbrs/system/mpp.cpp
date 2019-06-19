#include "system/mpp.h"
#include "common/utils.h"

namespace rs
{

MPPSystem::MPPSystem() : init_(false) {}

MPPSystem::~MPPSystem()
{
    Close();
}

void MPPSystem::Close()
{
}

MPPSystem *MPPSystem::Instance()
{
    static MPPSystem *instance = new MPPSystem;
    return instance;
}

int MPPSystem::Initialize(const Params &params)
{
    if (init_)
        return KInitialized;

    int ret;

    params_ = params;

    HI_MPI_SYS_Exit();

    HI_MPI_VB_Exit();

    ConfigLogger();

    ret = ConfigVB(params.mode, params.block_num);
    if (ret != KSuccess)
        return ret;

    ret = ConfigSys(RS_ALIGN_WIDTH);
    if (ret != KSuccess)
        return ret;

    ret = ConfigMem();
    if (ret != KSuccess)
        return ret;

    init_ = true;

    return KSuccess;
}

int MPPSystem::ConfigSys(int align_width)
{
    int ret;

    MPP_SYS_CONF_S conf;
    memset(&conf, 0, sizeof(MPP_SYS_CONF_S));
    conf.u32AlignWidth = RS_ALIGN_WIDTH;
    ret = HI_MPI_SYS_SetConf(&conf);
    if (ret != KSuccess)
    {
        log_e("HI_MPI_SYS_SetConf failed with %#x", ret);
        return KSDKError;
    }

    ret = HI_MPI_SYS_Init();
    if (ret != KSuccess)
    {
        log_e("HI_MPI_SYS_Init failed with %#x", ret);
        return KSDKError;
    }

    return KSuccess;
}

int MPPSystem::ConfigVB(CaptureMode mode, int block_num)
{
    int ret;

    SIZE_S size = Utils::GetSize(mode);
    uint32_t blk_size = (CEILING_2_POWER(size.u32Width, RS_ALIGN_WIDTH) * CEILING_2_POWER(size.u32Height, RS_ALIGN_WIDTH) * 3) >> 1;
    log_d("MPP:VB block size:%u", blk_size);

    VB_CONF_S conf;
    memset(&conf, 0, sizeof(conf));
    conf.u32MaxPoolCnt = 128;
    conf.astCommPool[0].u32BlkSize = blk_size;
    conf.astCommPool[0].u32BlkCnt = block_num;
    conf.astCommPool[1].u32BlkSize = blk_size;
    conf.astCommPool[1].u32BlkCnt = block_num;
    strcpy(conf.astCommPool[1].acMmzName, "ddr1");

    ret = HI_MPI_VB_SetConf(&conf);
    if (ret != KSuccess)
    {
        log_e("HI_MPI_VB_SetConf failed with %#x", ret);
        return KSDKError;
    }

    ret = HI_MPI_VB_Init();
    if (ret != KSuccess)
    {
        log_e("HI_MPI_VB_Init failed with %#x", ret);
        return KSDKError;
    }

    return KSuccess;
}

void MPPSystem::ConfigLogger()
{
    setbuf(stdout, NULL);
    elog_init();
    elog_set_fmt(ELOG_LVL_ASSERT, ELOG_FMT_ALL);
    elog_set_fmt(ELOG_LVL_ERROR, ELOG_FMT_LVL | ELOG_FMT_TIME | ELOG_FMT_DIR |
                                     ELOG_FMT_LINE | ELOG_FMT_FUNC);
    elog_set_fmt(ELOG_LVL_WARN, ELOG_FMT_LVL | ELOG_FMT_TIME);
    elog_set_fmt(ELOG_LVL_INFO, ELOG_FMT_LVL | ELOG_FMT_TIME);
    elog_set_fmt(ELOG_LVL_DEBUG, ELOG_FMT_ALL & ~ELOG_FMT_FUNC);
    elog_set_fmt(ELOG_LVL_VERBOSE, ELOG_FMT_ALL & ~ELOG_FMT_FUNC);
    elog_set_text_color_enabled(true);
    elog_start();
}

int MPPSystem::ConfigMem()
{
    int ret;

    {
        //VI CHN 4
        MPP_CHN_S chn;
        chn.enModId = HI_ID_VIU;
        chn.s32DevId = 0;
        chn.s32ChnId = 4;
        ret = HI_MPI_SYS_SetMemConf(&chn, nullptr);
        if (ret != KSuccess)
        {
            log_e("HI_MPI_SYS_SetMemConf failed");
            return KSDKError;
        }
    }
    {
        //VI CHN 8
        MPP_CHN_S chn;
        chn.enModId = HI_ID_VIU;
        chn.s32DevId = 0;
        chn.s32ChnId = 8;
        ret = HI_MPI_SYS_SetMemConf(&chn, "ddr1");
        if (ret != KSuccess)
        {
            log_e("HI_MPI_SYS_SetMemConf failed");
            return KSDKError;
        }
    }

    return KSuccess;
}
} // namespace rs