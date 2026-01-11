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

#include "debug.h"
#include "glsl_to_spirv.h"
#include "device.h"
#include "shader_module.h"

/** Please see header for specification */
Anvil::ShaderModule::ShaderModule(Anvil::Device*             device_ptr,
                                  GLSLShaderToSPIRVGenerator& spirv_generator)
    :m_cs_entrypoint_name(nullptr),
     m_device_ptr        (device_ptr)
{
    bool              result;
    const char*       shader_spirv_blob      = spirv_generator.get_spirv_blob();
    const uint32_t    shader_spirv_blob_size = spirv_generator.get_spirv_blob_size();
    const ShaderStage shader_stage           = spirv_generator.get_shader_stage();

    anvil_assert(shader_spirv_blob      != nullptr);
    anvil_assert(shader_spirv_blob_size >  0);

    m_cs_entrypoint_name = (shader_stage == SHADER_STAGE_COMPUTE) ? "main" : nullptr;

    result = init_from_spirv_blob(shader_spirv_blob,
                                  shader_spirv_blob_size);

    anvil_assert(result);
}

/** Please see header for specification */
Anvil::ShaderModule::ShaderModule(Anvil::Device* device_ptr,
                                  const char*     spirv_blob,
                                  uint32_t        n_spirv_blob_bytes,
                                  const char*     cs_entrypoint_name)
    :m_cs_entrypoint_name(cs_entrypoint_name),
     m_device_ptr        (device_ptr)
{
    bool result = init_from_spirv_blob(spirv_blob,
                                       n_spirv_blob_bytes);

    anvil_assert(result);
}

/** Please see header for specification */
Anvil::ShaderModule::~ShaderModule()
{
}

/** Please see header for specification */
bool Anvil::ShaderModule::init_from_spirv_blob(const char* spirv_blob,
                                               uint32_t    n_spirv_blob_bytes)
{
    VkResult                 result_vk;
    VkShaderModuleCreateInfo shader_module_create_info;

    /* Set up the "shader module" create info descriptor */
    shader_module_create_info.codeSize = n_spirv_blob_bytes;
    shader_module_create_info.flags    = 0;
    shader_module_create_info.pCode    = reinterpret_cast<const uint32_t*>(spirv_blob);
    shader_module_create_info.pNext    = nullptr;
    shader_module_create_info.sType    = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;

    result_vk = vkCreateShaderModule(m_device_ptr->get_device_vk(),
                                    &shader_module_create_info,
                                     nullptr, /* pAllocator */
                                    &m_module);

    anvil_assert_vk_call_succeeded(result_vk);

    return is_vk_call_successful(result_vk);
}
