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
#include "wrappers/buffer.h"
#include "wrappers/command_buffer.h"
#include "wrappers/command_pool.h"
#include "wrappers/compute_pipeline_manager.h"
#include "wrappers/descriptor_set.h"
#include "wrappers/device.h"
#include "wrappers/pipeline_layout.h"


/** Please see header for specification */
Anvil::CommandBufferBase::BindDescriptorSetsCommand::BindDescriptorSetsCommand(VkPipelineBindPoint    in_pipeline_bind_point,
                                                                               Anvil::PipelineLayout* in_layout_ptr,
                                                                               uint32_t               in_first_set,
                                                                               uint32_t               in_set_count,
                                                                               Anvil::DescriptorSet** in_descriptor_set_ptrs,
                                                                               uint32_t               in_dynamic_offset_count,
                                                                               const uint32_t*        in_dynamic_offset_ptrs)
{
    first_set           = in_first_set;
    layout_ptr          = in_layout_ptr;
    pipeline_bind_point = in_pipeline_bind_point;

    for (uint32_t n_set = 0;
                  n_set < in_set_count;
                ++n_set)
    {
        descriptor_sets.push_back(in_descriptor_set_ptrs[n_set]);
    }

    for (uint32_t n_dynamic_offset = 0;
                  n_dynamic_offset < in_dynamic_offset_count;
                ++n_dynamic_offset)
    {
        dynamic_offsets.push_back(in_dynamic_offset_ptrs[n_dynamic_offset]);
    }
}

/** Please see header for specification */
Anvil::CommandBufferBase::BindDescriptorSetsCommand::~BindDescriptorSetsCommand()
{
}

/** Please see header for specification */
Anvil::CommandBufferBase::BindPipelineCommand::BindPipelineCommand(VkPipelineBindPoint in_pipeline_bind_point,
                                                                   Anvil::PipelineID   in_pipeline_id)
{
    pipeline_bind_point = in_pipeline_bind_point;
    pipeline_id         = in_pipeline_id;
}

/** Please see header for specification */
Anvil::CommandBufferBase::CopyBufferCommand::CopyBufferCommand(Anvil::Buffer*      in_src_buffer_ptr,
                                                               Anvil::Buffer*      in_dst_buffer_ptr,
                                                               uint32_t            in_region_count,
                                                               const VkBufferCopy* in_region_ptrs)
{
    dst_buffer     = in_dst_buffer_ptr->get_buffer();
    dst_buffer_ptr = in_dst_buffer_ptr;
    src_buffer     = in_src_buffer_ptr->get_buffer();
    src_buffer_ptr = in_src_buffer_ptr;

    for (uint32_t n_region = 0;
                  n_region < in_region_count;
                ++n_region)
    {
        regions.push_back(in_region_ptrs[n_region]);
    }
}

/** Please see header for specification */
Anvil::CommandBufferBase::CopyBufferCommand::~CopyBufferCommand()
{
}

/** Please see header for specification */
Anvil::CommandBufferBase::DispatchCommand::DispatchCommand(uint32_t in_x,
                                                            uint32_t in_y,
                                                            uint32_t in_z)
{
    x = in_x;
    y = in_y;
    z = in_z;
}

/** Please see header for specification */
Anvil::PipelineBarrierCommand::PipelineBarrierCommand(VkPipelineStageFlags       in_src_stage_mask,
                                                      VkPipelineStageFlags       in_dst_stage_mask,
                                                      VkBool32                   in_by_region,
                                                      uint32_t                   in_memory_barrier_count,
                                                      const MemoryBarrier* const in_memory_barrier_ptr_ptr,
                                                      uint32_t                   in_buffer_memory_barrier_count,
                                                      const BufferBarrier* const in_buffer_memory_barrier_ptr_ptr)
{
    by_region      = in_by_region;
    dst_stage_mask = static_cast<VkPipelineStageFlagBits>(in_dst_stage_mask);
    src_stage_mask = static_cast<VkPipelineStageFlagBits>(in_src_stage_mask);

    for (uint32_t n_buffer_memory_barrier = 0;
                  n_buffer_memory_barrier < in_buffer_memory_barrier_count;
                ++n_buffer_memory_barrier)
    {
        buffer_barriers.push_back(in_buffer_memory_barrier_ptr_ptr[n_buffer_memory_barrier]);
    }

    for (uint32_t n_memory_barrier = 0;
                  n_memory_barrier < in_memory_barrier_count;
                ++n_memory_barrier)
    {
        memory_barriers.push_back(in_memory_barrier_ptr_ptr[n_memory_barrier]);
    }
}

/** Please see header for specification */
Anvil::CommandBufferBase::PushConstantsCommand::PushConstantsCommand(Anvil::PipelineLayout* in_layout_ptr,
                                                                     VkShaderStageFlags     in_stage_flags,
                                                                     uint32_t               in_offset,
                                                                     uint32_t               in_size,
                                                                     const void*            in_values)
{
    layout_ptr  = in_layout_ptr;
    offset      = in_offset;
    size        = in_size;
    stage_flags = in_stage_flags;
    values      = in_values;
}

/** Please see header for specification */
Anvil::CommandBufferBase::PushConstantsCommand::~PushConstantsCommand()
{
    if (layout_ptr != nullptr)
    {
        layout_ptr->release();
    }
}

/** Constructor.
 *
 *  @param device_ptr              Device to use.
 *  @param parent_command_pool_ptr Command pool to allocate the commands from. Must not be nullptr.
 *  @param type                    Command buffer type
 **/
Anvil::CommandBufferBase::CommandBufferBase(Anvil::Device*           device_ptr,
                                            Anvil::CommandPool*      parent_command_pool_ptr,
                                            Anvil::CommandBufferType type)
    :m_command_buffer         (VK_NULL_HANDLE),
     m_device_ptr             (device_ptr),
     m_is_renderpass_active   (false),
     m_parent_command_pool_ptr(parent_command_pool_ptr),
     m_recording_in_progress  (false),
     m_type                   (type)
{
    anvil_assert(parent_command_pool_ptr != nullptr);
}

/** Destructor.
 *
 *  Releases the underlying Vulkan command buffer instance.
 *
 *  Throws an assertion failure if recording is in progress.
 **/
Anvil::CommandBufferBase::~CommandBufferBase()
{
    anvil_assert(!m_recording_in_progress);

    if (m_command_buffer          != VK_NULL_HANDLE &&
        m_parent_command_pool_ptr != nullptr)
    {
        /* Unregister the command buffer from the pool */
        m_parent_command_pool_ptr->on_command_buffer_wrapper_destroyed(this);

        /* Physically free the command buffer we own */
        vkFreeCommandBuffers(m_device_ptr->get_device_vk(),
                             m_parent_command_pool_ptr->get_command_pool(),
                             1, /* commandBufferCount */
                            &m_command_buffer);

        m_command_buffer = VK_NULL_HANDLE;
    }
}

/** Called back when the parent command pool is released. */
void Anvil::CommandBufferBase::on_parent_pool_released()
{
    m_parent_command_pool_ptr = nullptr;
}

/* Please see header for specification */
bool Anvil::CommandBufferBase::record_bind_descriptor_sets(VkPipelineBindPoint    in_pipeline_bind_point,
                                                           Anvil::PipelineLayout* in_layout_ptr,
                                                           uint32_t               in_first_set,
                                                           uint32_t               in_set_count,
                                                           Anvil::DescriptorSet** in_descriptor_set_ptrs,
                                                           uint32_t               in_dynamic_offset_count,
                                                           const uint32_t*        in_dynamic_offset_ptrs)
{
    /* Note: Command supported inside and outside the renderpass. */
    VkDescriptorSet dss_vk[16];
    bool            result = false;

    anvil_assert(in_set_count < sizeof(dss_vk) / sizeof(dss_vk[0]) );

    for (uint32_t n_set = 0;
                  n_set < in_set_count;
                ++n_set)
    {
        dss_vk[n_set] = in_descriptor_set_ptrs[n_set]->get_descriptor_set_vk();
    }

    if (!m_recording_in_progress)
    {
        anvil_assert(m_recording_in_progress);

        goto end;
    }

    vkCmdBindDescriptorSets(m_command_buffer,
                            in_pipeline_bind_point,
                            in_layout_ptr->get_pipeline_layout(),
                            in_first_set,
                            in_set_count,
                            dss_vk,
                            in_dynamic_offset_count,
                            in_dynamic_offset_ptrs);

    result = true;
end:
    return result;
}

/* Please see header for specification */
bool Anvil::CommandBufferBase::record_bind_pipeline(VkPipelineBindPoint in_pipeline_bind_point,
                                                    Anvil::PipelineID   in_pipeline_id)
{
    /* Command supported inside and outside the renderpass. */
    VkPipeline pipeline_vk = VK_NULL_HANDLE;
    bool       result      = false;

    if (!m_recording_in_progress)
    {
        anvil_assert(m_recording_in_progress);

        goto end;
    }

    anvil_assert(in_pipeline_bind_point == VK_PIPELINE_BIND_POINT_COMPUTE);
    pipeline_vk = m_device_ptr->get_compute_pipeline_manager()->get_compute_pipeline(in_pipeline_id);

    vkCmdBindPipeline(m_command_buffer,
                      in_pipeline_bind_point,
                      pipeline_vk);

    result = true;
end:
    return result;
}

/* Please see header for specification */
bool Anvil::CommandBufferBase::record_copy_buffer(Anvil::Buffer*      in_src_buffer_ptr,
                                                  Anvil::Buffer*      in_dst_buffer_ptr,
                                                  uint32_t            in_region_count,
                                                  const VkBufferCopy* in_region_ptrs)
{
    bool result = false;

    if (m_is_renderpass_active)
    {
        anvil_assert(!m_is_renderpass_active);

        goto end;
    }

    if (!m_recording_in_progress)
    {
        anvil_assert(m_recording_in_progress);

        goto end;
    }

    vkCmdCopyBuffer(m_command_buffer,
                    in_src_buffer_ptr->get_buffer(),
                    in_dst_buffer_ptr->get_buffer(),
                    in_region_count,
                    in_region_ptrs);

    result = true;
end:
    return result;
}

/* Please see header for specification */
bool Anvil::CommandBufferBase::record_dispatch(uint32_t in_x,
                                               uint32_t in_y,
                                               uint32_t in_z)
{
    bool result = false;

    if (m_is_renderpass_active)
    {
        anvil_assert(!m_is_renderpass_active);

        goto end;
    }

    if (!m_recording_in_progress)
    {
        anvil_assert(m_recording_in_progress);

        goto end;
    }

    vkCmdDispatch(m_command_buffer,
                  in_x,
                  in_y,
                  in_z);

    result = true;
end:
    return result;
}

/* Please see header for specification */
bool Anvil::CommandBufferBase::record_pipeline_barrier(VkPipelineStageFlags       in_src_stage_mask,
                                                       VkPipelineStageFlags       in_dst_stage_mask,
                                                       VkBool32                   in_by_region,
                                                       uint32_t                   in_memory_barrier_count,
                                                       const MemoryBarrier* const in_memory_barriers_ptr,
                                                       uint32_t                   in_buffer_memory_barrier_count,
                                                       const BufferBarrier* const in_buffer_memory_barriers_ptr)
{
    /* NOTE: The command can be executed both inside and outside a renderpass */
    VkBufferMemoryBarrier buffer_barriers_vk[16];
    VkImageMemoryBarrier  image_barriers_vk [16];
    VkMemoryBarrier       memory_barriers_vk[16];
    bool                  result = false;

    if (!m_recording_in_progress)
    {
        anvil_assert(m_recording_in_progress);

        goto end;
    }

    anvil_assert(sizeof(buffer_barriers_vk) / sizeof(buffer_barriers_vk[0]) >= in_buffer_memory_barrier_count &&
                sizeof(image_barriers_vk)  / sizeof(image_barriers_vk [0]) >= in_image_memory_barrier_count  &&
                sizeof(memory_barriers_vk) / sizeof(memory_barriers_vk[0]) >= in_memory_barrier_count);

    for (uint32_t n_buffer_barrier = 0;
                  n_buffer_barrier < in_buffer_memory_barrier_count;
                ++n_buffer_barrier)
    {
        buffer_barriers_vk[n_buffer_barrier] = in_buffer_memory_barriers_ptr[n_buffer_barrier].get_barrier_vk();
    }

    for (uint32_t n_memory_barrier = 0;
                  n_memory_barrier < in_memory_barrier_count;
                ++n_memory_barrier)
    {
        memory_barriers_vk[n_memory_barrier] = in_memory_barriers_ptr[n_memory_barrier].get_barrier_vk();
    }

    vkCmdPipelineBarrier(m_command_buffer,
                         in_src_stage_mask,
                         in_dst_stage_mask,
                         in_by_region,
                         in_memory_barrier_count,
                         memory_barriers_vk,
                         in_buffer_memory_barrier_count,
                         buffer_barriers_vk,
                         0,
                         NULL);

    result = true;
end:
    return result;
}

/* Please see header for specification */
bool Anvil::CommandBufferBase::record_push_constants(Anvil::PipelineLayout* in_layout_ptr,
                                                     VkShaderStageFlags     in_stage_flags,
                                                     uint32_t               in_offset,
                                                     uint32_t               in_size,
                                                     const void*            in_values)
{
    /* NOTE: The command can be executed both inside and outside a renderpass */
    bool result = false;

    if (!m_recording_in_progress)
    {
        anvil_assert(m_recording_in_progress);

        goto end;
    }

    vkCmdPushConstants(m_command_buffer,
                       in_layout_ptr->get_pipeline_layout(),
                       in_stage_flags,
                       in_offset,
                       in_size,
                       in_values);

    result = true;
end:
    return result;
}

/* Please see header for specification */
bool Anvil::CommandBufferBase::reset(bool should_release_resources)
{
    bool     result    = false;
    VkResult result_vk;

    if (m_recording_in_progress)
    {
        anvil_assert(!m_recording_in_progress);

        goto end;
    }

    result_vk = vkResetCommandBuffer(m_command_buffer,
                                     (should_release_resources) ? VK_COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT : 0);

    if (!is_vk_call_successful(result_vk) )
    {
        anvil_assert_vk_call_succeeded(result_vk);

        goto end;
    }

    result = true;
end:
    return result;
}

/* Please see header for specification */
bool Anvil::CommandBufferBase::stop_recording()
{
    bool     result = false;
    VkResult result_vk;

    if (!m_recording_in_progress)
    {
        anvil_assert(m_recording_in_progress);

        goto end;
    }

    result_vk = vkEndCommandBuffer(m_command_buffer);

    if (!is_vk_call_successful(result_vk))
    {
        anvil_assert_vk_call_succeeded(result_vk);

        goto end;
    }

    m_recording_in_progress = false;
    result                  = true;
end:
    return result;
}

/* Please see header for specification */
Anvil::PrimaryCommandBuffer::PrimaryCommandBuffer(Anvil::Device*      device_ptr,
                                                  Anvil::CommandPool* parent_command_pool_ptr)
    :CommandBufferBase(device_ptr,
                       parent_command_pool_ptr,
                       COMMAND_BUFFER_TYPE_PRIMARY)
{
    VkCommandBufferAllocateInfo alloc_info;
    VkResult                    result_vk;

    alloc_info.commandBufferCount = 1;
    alloc_info.commandPool        = parent_command_pool_ptr->get_command_pool();
    alloc_info.level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    alloc_info.pNext              = nullptr;
    alloc_info.sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;

    result_vk = vkAllocateCommandBuffers(m_device_ptr->get_device_vk(),
                                        &alloc_info,
                                        &m_command_buffer);

    anvil_assert_vk_call_succeeded(result_vk);
}

/* Please see header for specification */
bool Anvil::PrimaryCommandBuffer::start_recording(bool one_time_submit,
                                                  bool simultaneous_use_allowed)
{
    VkCommandBufferBeginInfo command_buffer_begin_info;
    bool                     result    = false;
    VkResult                 result_vk;

    if (m_recording_in_progress)
    {
        anvil_assert(!m_recording_in_progress);

        goto end;
    }

    command_buffer_begin_info.flags            = ((one_time_submit)          ? VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT  : 0) |
                                                 ((simultaneous_use_allowed) ? VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT : 0);
    command_buffer_begin_info.pNext            = nullptr;
    command_buffer_begin_info.pInheritanceInfo = nullptr;  /* Only relevant for secondary-level command buffers */
    command_buffer_begin_info.sType            = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    result_vk = vkBeginCommandBuffer(m_command_buffer,
                                    &command_buffer_begin_info);

    if (!is_vk_call_successful(result_vk) )
    {
        anvil_assert_vk_call_succeeded(result_vk);

        goto end;
    }

    m_recording_in_progress = true;
    result                  = true;

end:
    return result;
}