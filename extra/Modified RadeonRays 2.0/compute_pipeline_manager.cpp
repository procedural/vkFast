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
#include "compute_pipeline_manager.h"
#include "device.h"
#include "pipeline_layout.h"
#include "shader_module.h"
#include <algorithm>


/** Constructor. Initializes the compute pipeline manager.
 *
 *  @param device_ptr              Device to use.
 *  @param use_pipeline_cache      true if a VkPipelineCache instance should be used to spawn new pipeline objects.
 *                                 What pipeline cache ends up being used depends on @param pipeline_cache_to_reuse -
 *                                 if a nullptr object is passed via this argument, a new pipeline cache instance will
 *                                 be created, and later released by the destructor. If a non-nullptr object is passed,
 *                                 it will be used instead. In the latter scenario, it is the caller's responsibility
 *                                 to release the cache when no longer needed!
 *  @param pipeline_cache_to_reuse Please see above.
 **/
Anvil::ComputePipelineManager::ComputePipelineManager(Anvil::Device*        device_ptr)
    :BasePipelineManager(device_ptr)
{
}


/* Stub destructor */
Anvil::ComputePipelineManager::~ComputePipelineManager()
{
    m_pipelines.clear();
}

/** Re-creates Vulkan compute pipeline objects for all added non-proxy pipelines marked
 *  as dirty. A new compute pipeline layout object may, but does not have to, be also
 *  created implicitly by calling this function.
 *
 *  @return true if the function was successful, false otherwise.
 **/
bool Anvil::ComputePipelineManager::bake()
{
    std::vector<VkComputePipelineCreateInfo> pipeline_create_info_items_vk;
    bool                                     result = false;
    std::vector<VkPipeline>                  result_pipeline_items_vk;
    VkResult                                 result_vk;

    typedef struct _bake_item
    {
        VkComputePipelineCreateInfo create_info;
        std::shared_ptr<Pipeline>   pipeline_ptr;

        _bake_item(const VkComputePipelineCreateInfo& in_create_info,
                   std::shared_ptr<Pipeline>          in_pipeline_ptr)
        {
            create_info  = in_create_info;
            pipeline_ptr = in_pipeline_ptr;
        }

        bool operator==(std::shared_ptr<Pipeline> in_pipeline_ptr)
        {
            return pipeline_ptr == in_pipeline_ptr;
        }
    } _bake_item;

    std::map<VkPipelineLayout, std::vector<_bake_item> > layout_to_bake_item_map;

    /* Iterate over all compute pipelines and identify the ones marked as dirty. Only these
     * need to be re-created */
    for (auto pipeline_iterator  = m_pipelines.begin();
              pipeline_iterator != m_pipelines.end();
            ++pipeline_iterator)
    {
        std::shared_ptr<Pipeline>   current_pipeline_ptr  = pipeline_iterator->second;
        VkComputePipelineCreateInfo pipeline_create_info;

        if (!current_pipeline_ptr->dirty     ||
             current_pipeline_ptr->is_proxy)
        {
            continue;
        }

        if (current_pipeline_ptr->baked_pipeline != VK_NULL_HANDLE)
        {
            vkDestroyPipeline(m_device_ptr->get_device_vk(),
                              current_pipeline_ptr->baked_pipeline,
                              nullptr /* pAllocator */);

            current_pipeline_ptr->baked_pipeline = VK_NULL_HANDLE;
        }

        if (current_pipeline_ptr->layout_dirty)
        {
            if (current_pipeline_ptr->layout_ptr != nullptr)
            {
                current_pipeline_ptr->layout_ptr->release();
            }

            current_pipeline_ptr->layout_ptr = get_pipeline_layout(pipeline_iterator->first);
        }

        /* No base pipeline requested */
        pipeline_create_info.basePipelineHandle = VK_NULL_HANDLE;
        pipeline_create_info.basePipelineIndex  = -1; /* unused */

        anvil_assert(!current_pipeline_ptr->layout_dirty);

        pipeline_create_info.flags                     = 0;
        pipeline_create_info.layout                    = current_pipeline_ptr->layout_ptr->get_pipeline_layout();
        pipeline_create_info.pNext                     = VK_NULL_HANDLE;
        pipeline_create_info.stage.flags               = 0;
        pipeline_create_info.stage.pName               = current_pipeline_ptr->shader_stages[0].name;
        pipeline_create_info.stage.pNext               = nullptr;
        pipeline_create_info.stage.pSpecializationInfo = NULL;
        pipeline_create_info.stage.stage               = VK_SHADER_STAGE_COMPUTE_BIT;
        pipeline_create_info.stage.sType               = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        pipeline_create_info.sType                     = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
        pipeline_create_info.stage.module              = current_pipeline_ptr->shader_stages[0].shader_module_ptr->get_module();
        pipeline_create_info.flags                     = 0;

        layout_to_bake_item_map[pipeline_create_info.layout].push_back(_bake_item(pipeline_create_info,
                                                                                  current_pipeline_ptr) );
    }

    /* We can finally bake the pipeline objects. */
    for (auto map_iterator  = layout_to_bake_item_map.begin();
              map_iterator != layout_to_bake_item_map.end();
            ++map_iterator)
    {
        uint32_t n_current_item = 0;

        pipeline_create_info_items_vk.clear();

        for (auto item_iterator  = map_iterator->second.begin();
                  item_iterator != map_iterator->second.end();
                ++item_iterator)
        {
            const _bake_item& current_bake_item = *item_iterator;

            pipeline_create_info_items_vk.push_back(current_bake_item.create_info);
        }

        result_pipeline_items_vk.resize(pipeline_create_info_items_vk.size() );

        result_vk = vkCreateComputePipelines(m_device_ptr->get_device_vk(),
                                             NULL,
                                             (uint32_t) pipeline_create_info_items_vk.size(),
                                            &pipeline_create_info_items_vk[0],
                                             nullptr, /* pAllocator */
                                            &result_pipeline_items_vk[0]);

        if (!is_vk_call_successful(result_vk))
        {
            anvil_assert_vk_call_succeeded(result_vk);

            goto end;
        }

        for (auto item_iterator  = map_iterator->second.begin();
                  item_iterator != map_iterator->second.end();
                ++item_iterator, ++n_current_item)
        {
            const _bake_item& current_bake_item = *item_iterator;

            anvil_assert(result_pipeline_items_vk[n_current_item] != VK_NULL_HANDLE);

            current_bake_item.pipeline_ptr->baked_pipeline  = result_pipeline_items_vk[n_current_item];
            current_bake_item.pipeline_ptr->dirty           = false;
        }
    }

    /* All done */
    result = true;
end:
    return result;
}
