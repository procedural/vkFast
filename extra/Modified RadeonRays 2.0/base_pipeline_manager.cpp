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

#include "base_pipeline_manager.h"
#include "debug.h"
#include "descriptor_set_group.h"
#include "device.h"
#include "pipeline_layout.h"
#include "pipeline_layout_manager.h"
#include <algorithm>

/** Please see header for specification */
Anvil::BasePipelineManager::BasePipelineManager(Anvil::Device*        device_ptr)
    :m_device_ptr        (device_ptr),
     m_pipeline_counter  (0)
{
    anvil_assert(!use_pipeline_cache && pipeline_cache_to_reuse_ptr == nullptr ||
                 use_pipeline_cache);

    m_pipeline_layout_manager_ptr = Anvil::PipelineLayoutManager::acquire(m_device_ptr);
}

/** Please see header for specification */
Anvil::BasePipelineManager::~BasePipelineManager()
{
}

/* Please see header for specification */
bool Anvil::BasePipelineManager::add_regular_pipeline(uint32_t                           n_shader_module_stage_entrypoints,
                                                      const ShaderModuleStageEntryPoint* shader_module_stage_entrypoint_ptrs,
                                                      PipelineID*                        out_pipeline_id_ptr)
{
    PipelineID                new_pipeline_id  = 0;
    std::shared_ptr<Pipeline> new_pipeline_ptr;

    /* Create & store the new descriptor */
    new_pipeline_id  = (m_pipeline_counter++);
    new_pipeline_ptr = std::shared_ptr<Pipeline>(new Pipeline(m_device_ptr,
                                                              n_shader_module_stage_entrypoints,
                                                              shader_module_stage_entrypoint_ptrs,
                                                              false /* in_is_proxy */) );

    *out_pipeline_id_ptr         = new_pipeline_id;
    m_pipelines[new_pipeline_id] = new_pipeline_ptr;

    /* All done */
    return true;
}

/* Please see header for specification */
bool Anvil::BasePipelineManager::attach_dsg_to_pipeline(PipelineID                 pipeline_id,
                                                        Anvil::DescriptorSetGroup* dsg_ptr)
{
    std::shared_ptr<Pipeline> pipeline_ptr;
    bool                      result = false;

    if (dsg_ptr == nullptr)
    {
        anvil_assert(!(dsg_ptr == nullptr) );

        goto end;
    }

    /* Retrieve pipeline's descriptor and attach the specified DSG */
    if (m_pipelines.find(pipeline_id) == m_pipelines.end() )
    {
        anvil_assert(!(m_pipelines.find(pipeline_id) == m_pipelines.end() ));

        goto end;
    }
    else
    {
        pipeline_ptr = m_pipelines[pipeline_id];
    }

    /* Make sure the DSG has not already been attached */
    if (std::find(pipeline_ptr->descriptor_set_groups.begin(),
                  pipeline_ptr->descriptor_set_groups.end(),
                  dsg_ptr) != pipeline_ptr->descriptor_set_groups.end() )
    {
        anvil_assert(false);

        goto end;
    }

    /* If we reached this place, we can attach the DSG */
    pipeline_ptr->dirty        = true;
    pipeline_ptr->layout_dirty = true;

    pipeline_ptr->descriptor_set_groups.push_back(dsg_ptr);

    /* All done */
    result = true;
end:
    return result;
}

/* Please see header for specification */
bool Anvil::BasePipelineManager::attach_push_constant_range_to_pipeline(PipelineID         pipeline_id,
                                                                        uint32_t           offset,
                                                                        uint32_t           size,
                                                                        VkShaderStageFlags stages)
{
    std::shared_ptr<Pipeline> pipeline_ptr;
    bool                      result = false;

    /* Retrieve pipeline's descriptor and add the specified push constant range */
    if (m_pipelines.find(pipeline_id) == m_pipelines.end() )
    {
        anvil_assert(!(m_pipelines.find(pipeline_id) == m_pipelines.end()));

        goto end;
    }
    else
    {
        pipeline_ptr = m_pipelines[pipeline_id];
    }

    if (pipeline_ptr->is_proxy)
    {
        anvil_assert(!pipeline_ptr->is_proxy);

        goto end;
    }

    pipeline_ptr->dirty        = true;
    pipeline_ptr->layout_dirty = true;

    pipeline_ptr->push_constant_ranges.push_back(Anvil::PushConstantRange(offset,
                                                                           size,
                                                                           stages) );

    /* All done */
    result = true;
end:
    return result;
}

/* Please see header for specification */
bool Anvil::BasePipelineManager::delete_pipeline(PipelineID pipeline_id)
{
    bool result = false;

    auto pipeline_iterator = m_pipelines.find(pipeline_id);

    if (pipeline_iterator == m_pipelines.end() )
    {
        goto end;
    }

    m_pipelines.erase(pipeline_iterator);

    /* All done */
    result = true;
end:
    return result;
}

/* Please see header for specification */
VkPipeline Anvil::BasePipelineManager::get_pipeline(PipelineID pipeline_id)
{
    std::shared_ptr<Pipeline> pipeline_ptr;
    VkPipeline                result = VK_NULL_HANDLE;

    if (m_pipelines.find(pipeline_id) == m_pipelines.end() )
    {
        anvil_assert(!(m_pipelines.find(pipeline_id) == m_pipelines.end()) );

        goto end;
    }
    else
    {
        pipeline_ptr = m_pipelines[pipeline_id];
    }

    if (pipeline_ptr->is_proxy)
    {
        anvil_assert(!pipeline_ptr->is_proxy);

        goto end;
    }

    if (pipeline_ptr->dirty)
    {
        bake();

        anvil_assert( pipeline_ptr->baked_pipeline != VK_NULL_HANDLE);
        anvil_assert(!pipeline_ptr->dirty);
    }

    result = m_pipelines[pipeline_id]->baked_pipeline;
end:
    return result;
}

/* Please see header for specification */
Anvil::PipelineLayout* Anvil::BasePipelineManager::get_pipeline_layout(PipelineID pipeline_id)
{
    Anvil::PipelineLayout*   pipeline_layout_ptr = nullptr;
    std::shared_ptr<Pipeline> pipeline_ptr;
    Anvil::PipelineLayout*   result_ptr          = nullptr;

    if (m_pipelines.find(pipeline_id) == m_pipelines.end() )
    {
        anvil_assert(!(m_pipelines.find(pipeline_id) == m_pipelines.end()) );

        goto end;
    }
    else
    {
        pipeline_ptr = m_pipelines[pipeline_id];
    }

    if (pipeline_ptr->is_proxy)
    {
        anvil_assert(!pipeline_ptr->is_proxy);

        goto end;
    }

    if (pipeline_ptr->layout_dirty)
    {
        if (pipeline_ptr->layout_ptr != nullptr)
        {
            pipeline_ptr->layout_ptr->release();

            pipeline_ptr->layout_ptr = nullptr;
        }

        if (!m_pipeline_layout_manager_ptr->get_retained_layout(pipeline_ptr->descriptor_set_groups,
                                                                pipeline_ptr->push_constant_ranges,
                                                               &pipeline_ptr->layout_ptr) )
        {
            anvil_assert(false);

            goto end;
        }

        pipeline_ptr->layout_dirty = false;
    }

    result_ptr = pipeline_ptr->layout_ptr;

end:
    return result_ptr;
}
