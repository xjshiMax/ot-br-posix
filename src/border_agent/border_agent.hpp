/*
 *    Copyright (c) 2017, The OpenThread Authors.
 *    All rights reserved.
 *
 *    Redistribution and use in source and binary forms, with or without
 *    modification, are permitted provided that the following conditions are met:
 *    1. Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *    2. Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *    3. Neither the name of the copyright holder nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 *    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *    AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *    IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 *    ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 *    LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 *    CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 *    SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 *    INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 *    CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *    ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *    POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * @file
 *   This file includes definition for Thread border agent.
 */

#ifndef OTBR_AGENT_BORDER_AGENT_HPP_
#define OTBR_AGENT_BORDER_AGENT_HPP_

#include "openthread-br/config.h"

#if !(OTBR_ENABLE_MDNS_AVAHI || OTBR_ENABLE_MDNS_MDNSSD || OTBR_ENABLE_MDNS_MOJO)
#error "Border Agent feature requires at least one `OTBR_MDNS` implementation"
#endif

#include <vector>

#include <stdint.h>

#include "backbone_router/backbone_agent.hpp"
#include "common/code_utils.hpp"
#include "common/mainloop.hpp"
#include "mdns/mdns.hpp"
#include "ncp/ncp_openthread.hpp"
#include "sdp_proxy/advertising_proxy.hpp"
#include "sdp_proxy/discovery_proxy.hpp"
#include "trel_dnssd/trel_dnssd.hpp"

#ifndef OTBR_VENDOR_NAME
#define OTBR_VENDOR_NAME "OpenThread"
#endif

#ifndef OTBR_PRODUCT_NAME
#define OTBR_PRODUCT_NAME "BorderRouter"
#endif

#ifndef OTBR_MESHCOP_SERVICE_INSTANCE_NAME
#define OTBR_MESHCOP_SERVICE_INSTANCE_NAME OTBR_VENDOR_NAME " " OTBR_PRODUCT_NAME
#endif

namespace otbr {

/**
 * @addtogroup border-router-border-agent
 *
 * @brief
 *   This module includes definition for Thread border agent
 *
 * @{
 */

/**
 * This class implements Thread border agent functionality.
 *
 */
class BorderAgent : private NonCopyable
{
public:
    /**
     * The constructor to initialize the Thread border agent.
     *
     * @param[in] aNcp  A reference to the NCP controller.
     * @param[in] aPublisher  A reference to the mDNS Publisher.
     *
     */
    BorderAgent(otbr::Ncp::ControllerOpenThread &aNcp, Mdns::Publisher &aPublisher);

    ~BorderAgent(void) = default;

    /**
     * This method enables/disables the Border Agent.
     *
     * @param[in] aIsEnabled  Whether to enable the Border Agent.
     *
     */
    void SetEnabled(bool aIsEnabled);

    /**
     * This method handles mDNS publisher's state changes.
     *
     * @param[in] aState  The state of mDNS publisher.
     *
     */
    void HandleMdnsState(Mdns::Publisher::State aState);

private:
    void Start(void);
    void Stop(void);
    bool IsEnabled(void) const { return mIsEnabled; }
    void PublishMeshCopService(void);
    void UpdateMeshCopService(void);
    void UnpublishMeshCopService(void);
#if OTBR_ENABLE_DBUS_SERVER
    void HandleUpdateVendorMeshCoPTxtEntries(std::map<std::string, std::vector<uint8_t>> aUpdate);
#endif

    void HandleThreadStateChanged(otChangedFlags aFlags);

    bool        IsThreadStarted(void) const;
    std::string BaseServiceInstanceName() const;
    std::string GetAlternativeServiceInstanceName() const;

    otbr::Ncp::ControllerOpenThread &mNcp;
    Mdns::Publisher                 &mPublisher;
    bool                             mIsEnabled;

#if OTBR_ENABLE_DBUS_SERVER
    std::map<std::string, std::vector<uint8_t>> mMeshCopTxtUpdate;
#endif

    std::string mServiceInstanceName;
};

/**
 * @}
 */

} // namespace otbr

#endif // OTBR_AGENT_BORDER_AGENT_HPP_
