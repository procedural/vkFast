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

#include "misc/debug.h"
#include "wrappers/instance.h"
#include "wrappers/physical_device.h"
#include <algorithm>

/** Please see header for specification */
Anvil::Instance::Instance(const char*                  app_name,
                          const char*                  engine_name)
    :m_app_name                                      (app_name),
     m_engine_name                                   (engine_name)
{
    anvil_assert(app_name    != nullptr);
    anvil_assert(engine_name != nullptr);

    init();
}

/** Please see header for specification */
Anvil::Instance::~Instance()
{
}

/** Enumerates and caches all layers supported by the Vulkan Instance. */
void Anvil::Instance::enumerate_instance_layers()
{
    VkLayerProperties* layer_props_ptr = nullptr;
    uint32_t           n_layers        = 0;
    VkResult           result;

    /* Retrieve layer data */
    result = vkEnumerateInstanceLayerProperties(&n_layers,
                                                nullptr); /* pProperties */
    anvil_assert_vk_call_succeeded(result);

    layer_props_ptr = new VkLayerProperties[n_layers + 1 /* global layer */];
    anvil_assert(layer_props_ptr != nullptr);

    result = vkEnumerateInstanceLayerProperties(&n_layers,
                                                 layer_props_ptr);

    anvil_assert_vk_call_succeeded(result);

    /* Convert raw layer props data to internal descriptors */
    for (uint32_t n_layer = 0;
                  n_layer < n_layers;
                ++n_layer)
    {
        Anvil::Layer* layer_ptr = nullptr;

        if (n_layer < n_layers)
        {
            m_supported_layers.push_back(Anvil::Layer(layer_props_ptr[n_layer]) );

            layer_ptr = &m_supported_layers[n_layer];
        }

        enumerate_layer_extensions(layer_ptr);
    }

    /* Release the raw layer data */
    delete [] layer_props_ptr;
    layer_props_ptr = nullptr;
}

/** Enumerates all available layer extensions. The enumerated extensions will be stored
 *  in the specified _vulkan_layer descriptor.
 *
 *  @param layer_ptr Layer to enumerate the extensions for. If nullptr, device extensions
 *                   will be retrieved instead.
 **/
void Anvil::Instance::enumerate_layer_extensions(Anvil::Layer* layer_ptr)
{
    VkExtensionProperties* extension_props_ptr = nullptr;
    uint32_t               n_extensions        = 0;
    VkResult               result;

    /* Check if the layer supports any extensions  at all*/
    const char* layer_name = (layer_ptr != nullptr) ? layer_ptr->name.c_str()
                                                 : nullptr;

    result = vkEnumerateInstanceExtensionProperties(layer_name,
                                                   &n_extensions,
                                                    nullptr); /* pProperties */
    anvil_assert_vk_call_succeeded(result);

    if (n_extensions > 0)
    {
        extension_props_ptr = new VkExtensionProperties[n_extensions];
        anvil_assert(extension_props_ptr != nullptr);

        result = vkEnumerateInstanceExtensionProperties(layer_name,
                                                       &n_extensions,
                                                        extension_props_ptr);

        anvil_assert_vk_call_succeeded(result);

        /* Convert raw extension props data to internal descriptors */
        for (uint32_t n_extension = 0;
                      n_extension < n_extensions;
                    ++n_extension)
        {
            layer_ptr->extensions.push_back(Anvil::Extension(extension_props_ptr[n_extension]) );
        }

        /* Release the raw layer data */
        delete [] extension_props_ptr;
        extension_props_ptr = nullptr;
    }
}

/** Enumerates and caches all available physical devices. */
void Anvil::Instance::enumerate_physical_devices()
{
    VkPhysicalDevice* devices            = nullptr;
    uint32_t          n_physical_devices = 0;
    VkResult          result;

    /* Retrieve physical device handles */
    result = vkEnumeratePhysicalDevices(m_instance,
                                       &n_physical_devices,
                                        nullptr); /* pPhysicalDevices */
    anvil_assert_vk_call_succeeded(result);

    if (n_physical_devices == 0)
    {
#ifdef _WIN32
        MessageBox(HWND_DESKTOP,
                   "No physical devices reported for the Vulkan instance",
                   "Error",
                   MB_OK | MB_ICONERROR);

        exit(1);
#else
        fprintf(stderr,"No physical devices reported for the Vulkan instance");
        fflush(stderr);
        exit(1);
#endif
    }

    devices = new VkPhysicalDevice[n_physical_devices];
    anvil_assert(devices != nullptr);

    result = vkEnumeratePhysicalDevices(m_instance,
                                       &n_physical_devices,
                                        devices);
    anvil_assert_vk_call_succeeded(result);

    /* Fill out internal physical device descriptors */
    for (unsigned int n_physical_device = 0;
                      n_physical_device < n_physical_devices;
                    ++n_physical_device)
    {
        m_physical_devices.push_back(new Anvil::PhysicalDevice(this,
                                                                n_physical_device,
                                                                devices[n_physical_device]) );
    }

    /* Clean up */
    delete [] devices;
    devices = nullptr;
}

/** Initializes the wrapper. */
void Anvil::Instance::init()
{
    VkApplicationInfo    app_info;
    VkInstanceCreateInfo create_info;
    const char**         enabled_layer_name_ptrs = nullptr;
    uint32_t             n_required_extensions   = 0;
    VkResult             result;
    const char**         required_extensions     = nullptr;

    n_required_extensions = 0;
    required_extensions   = NULL;

    /* Set up the app info descriptor **/
    app_info.apiVersion         = VK_MAKE_VERSION(1, 0, 0);
    app_info.applicationVersion = 0;
    app_info.engineVersion      = 0;
    app_info.pApplicationName   = m_app_name;
    app_info.pEngineName        = m_engine_name;
    app_info.pNext              = nullptr;
    app_info.sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO;

    /* Set up the create info descriptor */
    memset(&create_info,
           0,
           sizeof(create_info) );

    create_info.enabledExtensionCount = n_required_extensions;

    create_info.flags                   = 0;
    create_info.pApplicationInfo        = &app_info;
    create_info.pNext                   = nullptr;
    create_info.ppEnabledExtensionNames = required_extensions;
    create_info.sType                   = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;

    /* Create a new Vulkan instance */
    result = vkCreateInstance(&create_info,
                              nullptr, /* pAllocator */
                              &m_instance);
    anvil_assert_vk_call_succeeded(result);

    /* Continue initializing */
    enumerate_physical_devices();

    /* Clean up */
    if (enabled_layer_name_ptrs != nullptr)
    {
        delete [] enabled_layer_name_ptrs;

        enabled_layer_name_ptrs = nullptr;
    }
}

/** Please see header for specification */
bool Anvil::Instance::is_instance_extension_supported(const char* extension_name) const
{
    return std::find(m_extensions.begin(),
                     m_extensions.end(),
                     extension_name) != m_extensions.end();
}
