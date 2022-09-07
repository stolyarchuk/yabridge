// yabridge: a Wine plugin bridge
// Copyright (C) 2020-2022 Robbert van der Helm
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#pragma once

#include <clap/host.h>

#include "../../common/serialization/clap/plugin-factory.h"

// Forward declaration to avoid circular includes
class ClapBridge;

/**
 * A proxy for a plugin's `clap_host`.
 *
 * Because the plugin may not query host extensions until `init()` is called,
 * the available host extensions will only be populated at that point.
 */
class clap_host_proxy {
   public:
    /**
     * Construct a host proxy based for a plugin. The available extensions will
     * be populated when the host calls `clap_plugin::init()` as mentioned
     * above.
     */
    clap_host_proxy(ClapBridge& bridge,
                    size_t owner_instance_id,
                    clap::host::Host host_args);

    clap_host_proxy(const clap_host_proxy&) = delete;
    clap_host_proxy& operator=(const clap_host_proxy&) = delete;
    clap_host_proxy(clap_host_proxy&&) = delete;
    clap_host_proxy& operator=(clap_host_proxy&&) = delete;

    /**
     * Get a `clap_host` vtable that can be passed to the plugin.
     */
    inline const clap_host_t* host_vtable() const { return &host_vtable_; }

    static const void* CLAP_ABI host_get_extension(const struct clap_host* host,
                                                   const char* extension_id);
    static void CLAP_ABI host_request_restart(const struct clap_host* host);
    static void CLAP_ABI host_request_process(const struct clap_host* host);
    static void CLAP_ABI host_request_callback(const struct clap_host* host);

   private:
    ClapBridge& bridge_;
    size_t owner_instance_id_;
    clap::host::Host host_args_;

    /**
     * The vtable for `clap_host`, requires that this object is never moved or
     * copied. We'll use the host data pointer instead of placing this vtable at
     * the start of the struct and directly casting the `clap_host_t*`.
     */
    const clap_host_t host_vtable_;
};
