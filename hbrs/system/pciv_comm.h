#pragma once

#include <vector>
#include <map>

#include <json/json.h>

#include "common/global.h"

namespace rs
{
namespace pciv
{

struct Msg
{
    enum Type
    {
        READ_DONE = 0,
        WRITE_DONE
    };

    static const int32_t MaxDataLen = 32;

    int32_t type;
    uint8_t data[MaxDataLen];

} __attribute__((aligned(1)));

struct MemoryInfo
{
    uint32_t phy_addr;
} __attribute__((aligned(1)));

struct PosInfo
{
    int32_t start_pos;
    int32_t end_pos;
} __attribute__((aligned(1)));

struct StreamInfo
{
    int32_t align_len;
    int32_t len;
    uint64_t pts;
    int32_t vdec_chn;
} __attribute__((aligned(1)));

class Context
{
public:
    virtual ~Context() {}

    virtual const std::vector<int32_t> &GetRemoteIds() = 0;

    virtual int32_t Send(int32_t remote_id, int32_t port, uint8_t *data, int32_t len) = 0;

    virtual int32_t Recv(int32_t remote_id, int32_t port, uint8_t *data, int32_t len) = 0;

    virtual int32_t GetTransReadPort() = 0;

    virtual int32_t GetTransWritePort() = 0;

    virtual int32_t GetCMDPort() = 0;
};
} // namespace pciv

class PCIVComm : public pciv::Context
{
public:
    virtual ~PCIVComm();

    static PCIVComm *Instance();

    int32_t Initialize();

    void Close();

    const std::vector<int32_t> &GetRemoteIds() override;

    virtual int32_t Send(int32_t remote_id, int32_t port, uint8_t *data, int32_t len) override;

    virtual int32_t Recv(int32_t remote_id, int32_t port, uint8_t *data, int32_t len) override;

    int32_t GetTransReadPort() override;

    int32_t GetTransWritePort() override;

    int32_t GetCMDPort() override;

protected:
    static int32_t EnumChip(int32_t &local_id, std::vector<int32_t> &remote_ids);

    static int32_t OpenPort(int32_t remote_id, int32_t port, std::vector<std::vector<int32_t>> &remote_fds);

    static int32_t WaitConn(int32_t remote_id);

    explicit PCIVComm();

private:
    int32_t local_id_;
    std::vector<std::vector<int32_t>> remote_fds_;
    std::vector<int32_t> remote_ids_;
    bool init_;

    static const int32_t CommCMDPort;
    static const int32_t TransReadPort;
    static const int32_t TransWritePort;
    static const int32_t MsgPortBase;
    static const int32_t MaxPortNum;
};
}; // namespace rs