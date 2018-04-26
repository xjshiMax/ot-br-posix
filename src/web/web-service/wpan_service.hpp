/*
 *  Copyright (c) 2017, The OpenThread Authors.
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 *  1. Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *  2. Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *  3. Neither the name of the copyright holder nor the
 *     names of its contributors may be used to endorse or promote products
 *     derived from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 *  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 *  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 *  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 *  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 *  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 *  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * @file
 *   This file implements the wpan controller service
 */

#ifndef WPAN_SERVICE
#define WPAN_SERVICE

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <jsoncpp/json/json.h>
#include <jsoncpp/json/writer.h>

#include "../pskc-generator/pskc.hpp"
#include "../wpan-controller/wpan_controller.hpp"
#include "../utils/encoding.hpp"
#include "common/logging.hpp"
#include "utils/hex.hpp"

/**
 * WPAN parameter constants
 *
 */

#define OT_EXTENDED_PANID_LENGTH 8
#define OT_HARDWARE_ADDRESS_LENGTH 8
#define OT_NETWORK_NAME_LENGTH 16
#define OT_PANID_LENGTH 2
#define OT_PSKC_MAX_LENGTH 16
#define OT_HEX_PREFIX_LENGTH 2
#define OT_PUBLISH_SERVICE_INTERVAL 20

namespace ot {
namespace Web {

/**
 * This class provides web service to manage WPAN.
 *
 */
class WpanService
{
public:

    /**
     * This method handles the http request to join network.
     *
     * @param[in]  aJoinRequest  A reference to the http request of joining network.
     *
     * @returns The string to the http response of joining network.
     *
     */
    std::string HandleJoinNetworkRequest(const std::string &aJoinRequest);

    /**
     * This method handles the http request to form network.
     *
     * @param[in]  aFormRequest  A reference to the http request of forming network.
     *
     * @returns The string to the http response of forming network.
     *
     */
    std::string HandleFormNetworkRequest(const std::string &aFormRequest);

    /**
     * This method handles the http request to add on-mesh prefix.
     *
     * @param[in]  aAddPrefixRequest  A reference to the http request of adding on-mesh prefix.
     *
     * @returns The string to the http response of adding on-mesh prefix.
     *
     */
    std::string HandleAddPrefixRequest(const std::string &aAddPrefixRequest);

    /**
     * This method handles the http request to delete on-mesh prefix http request.
     *
     * @param[in]  aDeleteRequest  A reference to the http request of deleting on-mesh prefix.
     *
     * @returns The string to the http response of deleting on-mesh prefix.
     *
     */
    std::string HandleDeletePrefixRequest(const std::string &aDeleteRequest);

    /**
     * This method handles http request to get netowrk status.
     *
     * @returns The string to the http response of getting status.
     *
     */
    std::string HandleStatusRequest(void);

    /**
     * This method handles http request to get available networks.
     *
     * @returns The string to the http response of getting available networks.
     *
     */
    std::string HandleAvailableNetworkRequest(void);

    /**
     * This method sets the interface name of the wpantund.
     *
     * @param[in]  aIfName  The pointer to the interface name of wpantund.
     *
     */
    void SetInterfaceName(const char *aIfName) { strncpy(mIfName, aIfName, sizeof(mIfName)); }

    /**
     * This method gets status of wpan service.
     *
     * @param[inout]  aNetworkName  The pointer to the network name.
     * @param[inout]  aIfName       The pointer to the extended PAN ID.
     *
     * @retval kWpanStatus_OK        Successfully started the wpan service.
     * @retval kWpanStatus_Offline   Not started the wpan service.
     * @retval kWpanStatus_Down      The wpantund was down.
     *
     */
    int GetWpanServiceStatus(std::string &aNetworkName, std::string &aExtPanId) const;

private:

    ot::Dbus::WpanNetworkInfo mNetworks[DBUS_MAXIMUM_NAME_LENGTH];
    int                       mNetworksCount;
    char                      mIfName[IFNAMSIZ];
    std::string               mNetworkName;
    std::string               mExtPanId;
    const char               *mResponseSuccess = "successful";
    const char               *mResponseFail = "failed";
    const char               *mServiceUp = "up";
    const char               *mServiceDown = "down";

    enum
    {
        kWpanStatus_OK = 0,
        kWpanStatus_Associating,
        kWpanStatus_Down,
        kWpanStatus_Offline,
        kWpanStatus_Uninitialized,
        kWpanStatus_ParseRequestFailed,
        kWpanStatus_GetPropertyFailed,
    };

    enum
    {
        kPropertyType_String = 0,
        kPropertyType_Data,
    };

};

} //namespace Web
} //namespace ot

#endif
