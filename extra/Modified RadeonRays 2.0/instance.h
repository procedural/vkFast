//
// Copyright (c) 2016 Advanced Micro Devices, Inc. All rights reserved.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//

/** Implements a wrapper for a single Vulkan instance. Implemented in order to:
 *
 *  - manage life-time of Vulkan instances.
 *  - encapsulate all logic required to manipulate instances and children objects.
 *  - let ObjectTracker detect leaking Vulkan instance wrapper instances.
 *
 *  The wrapper is NOT thread-safe.
 **/
#ifndef WRAPPERS_INSTANCE_H
#define WRAPPERS_INSTANCE_H


#include "types.h"

namespace Anvil
{
    class Instance
    {
    public:
        /* Public functions */

        /** Creates a new Instance wrapper instance. This process is executed in the following steps:
         *
         *  1. If @param opt_pfn_validation_callback_proc is specified, available instance layers are
         *     enumerated. Layers which support VK_EXT_debug_report extension, are cached and used
         *     in step 2.
         *  2. A new Vulkan instance is created.
         *  3. Available physical devices are enumerated.
         *  4. Instance-level function pointers are extracted.
         *
         *  Only one Instance wrapper instance should be created during application's life-time.
         *
         *
         *  @param app_name                 Name of the application, to be passed in VkCreateInstanceInfo
         *                                  structure.
         *  @param engine_name              Name of the engine, to be passed in VkCreateInstanceInfo
         *                                  structure.
         *  @param opt_pfn_validation_proc  If not nullptr, the specified handled will be called whenever
         *                                  a call-back from any of the validation layers is received.
         *                                  Ignored otherwise.
         *  @param validation_proc_user_arg If @param opt_pfn_validation_proc is not nullptr, this argument
         *                                  will be passed to @param opt_pfn_validation_proc every time
         *                                  a debug callback is received. Ignored otherwise.
         **/
        Instance(const char*                  app_name,
                 const char*                  engine_name);

        /** Returns a raw wrapped VkInstance handle. */
        VkInstance get_instance_vk() const
        {
            return m_instance;
        }

        /** Returns a PhysicalDevice wrapper for a physical device at index @param n_device.
         *
         *  @param n_device Index of the physical device to retrieve the wrapper instance for.
         *                  This value must NOT be equal or larger than the value reported by
         *                  get_n_physical_devices().
         *
         ** @return As per description.
         **/
        Anvil::PhysicalDevice* get_physical_device(uint32_t n_device) const
        {
            return m_physical_devices[n_device];
        }

        /** Returns the total number of physical devices supported on the running platform. */
        uint32_t get_n_physical_devices() const
        {
            return static_cast<uint32_t>(m_physical_devices.size() );
        }

        /** Tells whether the specified instance extension is supported.
         *
         *  @param extension_name Name of the extension to use for the query.
         *
         *  @return true if the extension was reported as supported, false otherwise.
         **/
        bool is_instance_extension_supported(const char* extension_name) const;

    private:
        /* Private functions */
        virtual ~Instance();

        Instance& operator=(const Instance&);
        Instance           (const Instance&);

        void enumerate_instance_layers ();
        void enumerate_layer_extensions(Anvil::Layer* layer_ptr);
        void enumerate_physical_devices();
        void init                      ();

        /* Private variables */
        VkInstance m_instance;

        const char*                  m_app_name;
        const char*                  m_engine_name;

        std::vector<Anvil::Extension>       m_extensions;
        std::vector<Anvil::PhysicalDevice*> m_physical_devices;
        std::vector<Anvil::Layer>           m_supported_layers;
    };
}; /* namespace Anvil */

#endif /* WRAPPERS_INSTANCE_H */