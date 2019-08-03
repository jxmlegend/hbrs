#include "live/rtmp_streamer.h"
#include "common/err_code.h"
#include "common/utils.h"

namespace rs
{

const int RTMPStreamer::DefaultTimeOut = 1000;

RTMPStreamer::RTMPStreamer() : rtmp_(nullptr),
                               init_(false)
{
}

RTMPStreamer::~RTMPStreamer()
{
    Close();
}

int RTMPStreamer::Initialize(const std::string &url, bool has_audio)
{
    if (init_)
        return KInitialized;

    int ret;

    log_d("[%s]start,has_audio:%d", url.c_str(), has_audio);

    rtmp_ = srs_rtmp_create(url.c_str());
    if (rtmp_ == nullptr)
    {
        log_e("srs_rtmp_create failed");
        return KSDKError;
    }

    ret = srs_rtmp_set_timeout(rtmp_, DefaultTimeOut, DefaultTimeOut);
    if (ret != KSuccess)
    {
        srs_rtmp_destroy(rtmp_);
        log_e("srs_rtmp_set_timeout failed with %#x", ret);
        return KSDKError;
    }

    ret = srs_rtmp_handshake(rtmp_);
    if (ret != KSuccess)
    {
        srs_rtmp_destroy(rtmp_);
        log_e("srs_rtmp_handshake failed with %#x", ret);
        return KSDKError;
    }

    ret = srs_rtmp_connect_app(rtmp_);
    if (ret != KSuccess)
    {
        srs_rtmp_destroy(rtmp_);
        log_e("srs_rtmp_connect_app failed with %#x", ret);
        return KSDKError;
    }

    ret = srs_rtmp_publish_stream(rtmp_);
    if (ret != KSuccess)
    {
        srs_rtmp_destroy(rtmp_);
        log_e("srs_rtmp_publish_stream failed with %#x", ret);
        return KSDKError;
    }

    init_ = true;
    return KSuccess;
}

void RTMPStreamer::Close()
{
    if (!init_)
        return;

    srs_rtmp_destroy(rtmp_);
    init_ = false;
}

int RTMPStreamer::WriteAudioFrame(const AENCFrame &frame)
{
    if (!init_)
        return KUnInitialized;
    int ret;

    ret = srs_audio_write_raw_frame(rtmp_, 10, 3, 1, 1, reinterpret_cast<char *>(frame.data), frame.len, frame.ts);
    if (ret != KSuccess)
    {
        log_e("srs_audio_write_raw_frame failed with %#x", ret);
        return KSDKError;
    }
    return KSuccess;
}

int RTMPStreamer::WriteVideoFrame(const VENCFrame &frame)
{
#define ERROR_H264_DROP_BEFORE_SPS_PPS 3043
#define ERROR_H264_DUPLICATED_SPS 3044
#define ERROR_H264_DUPLICATED_PPS 3045

    if (!init_)
        return KUnInitialized;

    int ret;

    ret = srs_h264_write_raw_frames(rtmp_, reinterpret_cast<char *>(frame.data), frame.len, frame.ts, frame.ts);
    if (ret != KSuccess)
    {
        if (ret == ERROR_H264_DROP_BEFORE_SPS_PPS ||
            ret == ERROR_H264_DUPLICATED_SPS ||
            ret == ERROR_H264_DUPLICATED_PPS)
            return KSuccess;
        log_e("srs_h264_write_raw_frames failed with %#x", ret);
        return KSDKError;
    }

    return KSuccess;
}

} // namespace rs