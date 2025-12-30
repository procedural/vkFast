/**********************************************************************
Copyright (c) 2016 Advanced Micro Devices, Inc. All rights reserved.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
********************************************************************/
#pragma once

#include <string>
#include <cstdint>

#include "wrappers/physical_device.h"

#include <vulkan/vulkan.h>
#include <vulkan/vk_sdk_platform.h>
#include <vulkan/vk_layer.h>

#include "misc/debug.h"
#include "wrappers/instance.h"

#include "common.h"
#include "device.h"

extern Anvil::Instance* m_anvil_instance;

static inline void calcInitializeInstance()
{
    if (m_anvil_instance == NULL)
    {
        m_anvil_instance = new Anvil::Instance("RadeonRays", "RadeonRays");
    }
}

static inline std::uint32_t calcGetDeviceCount()
{
    return nullptr != m_anvil_instance ? m_anvil_instance->get_n_physical_devices() : 0;
}

static inline void calcGetDeviceSpec(std::uint32_t idx, Calc::DeviceSpec& spec)
{
    if (idx < calcGetDeviceCount())
    {
        const Anvil::PhysicalDevice* device = m_anvil_instance->get_physical_device( idx );

        spec.name = device->get_device_properties().deviceName;
        spec.vendor = device->get_device_properties().deviceName;

        uint64_t localMemory = 0;
        uint64_t hostMemory = 0;

        for ( uint32_t i = 0; i < device->get_memory_properties().types.size(); ++i )
        {
            const Anvil::MemoryType& memoryType = device->get_memory_properties().types[ i ];
            if ( memoryType.flags & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT )
            {
                localMemory += memoryType.heap_ptr->size;
            }

            else if ( memoryType.flags & VK_MEMORY_PROPERTY_HOST_CACHED_BIT )
            {
                hostMemory += memoryType.heap_ptr->size;
            }
        }

        spec.global_mem_size = static_cast< std::size_t >(hostMemory);
        spec.local_mem_size = static_cast< std::size_t >(localMemory);
        spec.min_alignment = static_cast< std::uint32_t >( device->get_device_properties().limits.minMemoryMapAlignment );
        spec.max_alloc_size = static_cast< std::size_t >(hostMemory);
        spec.max_local_size = static_cast< std::size_t >(localMemory);
    }
    else
    {
        anvil_assert(!"Index is out of bounds");
    }
}

static inline Calc::Device* calcCreateDevice(std::uint32_t idx)
{
    Calc::Device* toReturn = nullptr;

    if (idx < calcGetDeviceCount())
    {
        Anvil::PhysicalDevice* physical_device = m_anvil_instance->get_physical_device( idx );

        std::vector<const char*> required_extension_names;
        std::vector<const char*> required_layer_names;

        Anvil::Device* newDevice = new Anvil::Device( physical_device, required_extension_names, required_layer_names, false, true );

        // see if we have a compute device available
        Calc::Device* toReturn = new Calc::Device( newDevice, true );
        bool initVkOk = (nullptr != toReturn) ? toReturn->InitializeVulkanResources() : false;

        if( initVkOk == false )
        {
            anvil_assert(!"No valid Vulkan device found");
        }

        return toReturn;
    }
    else
    {
        anvil_assert(!"Index is out of bounds");
    }

    return toReturn;
}

static inline Calc::Device* calcCreateDevice(Anvil::Device* device, Anvil::CommandPool* cmd_pool)
{
    // TODO graphics or compute pipe selection
    Calc::Device* toReturn = new Calc::Device(device, true);
    bool initVkOk = (nullptr != toReturn) ? toReturn->InitializeVulkanCommandBuffer(cmd_pool) : false;
    if (initVkOk == false)
    {
        delete toReturn;
        return nullptr;
    } else
    {
        return toReturn;
    }
}

static inline void calcDeleteDevice(Calc::Device* device)
{
    delete device;
}
