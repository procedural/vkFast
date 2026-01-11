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

#include "buffer.h"
#include "descriptor_pool.h"
#include "descriptor_set.h"
#include "descriptor_set_layout.h"
#include "device.h"

/** Please see header for specification */
Anvil::DescriptorSet::BindingItem& Anvil::DescriptorSet::BindingItem::operator=(const BufferBindingElement& element)
{
    buffer_ptr      = element.buffer_ptr;
    dirty           = true;
    size            = element.size;
    start_offset    = element.start_offset;

    return *this;
}

/** Please see header for specification */
Anvil::DescriptorSet::BindingItem::~BindingItem()
{
}

/** Please see header for specification */
Anvil::DescriptorSet::BufferBindingElement::BufferBindingElement(Anvil::Buffer* in_buffer_ptr)
{
    anvil_assert(in_buffer_ptr != nullptr);

    buffer_ptr   = in_buffer_ptr;
    size         = -1;
    start_offset = -1;
}

/** Please see header for specification */
Anvil::DescriptorSet::BufferBindingElement::BufferBindingElement(Anvil::Buffer* in_buffer_ptr,
                                                                 VkDeviceSize   in_start_offset,
                                                                 VkDeviceSize   in_size)
{
    anvil_assert(in_buffer_ptr != nullptr);

    if (in_size != VK_WHOLE_SIZE)
    {
        anvil_assert(in_start_offset + in_size <= in_buffer_ptr->get_size() );
    }

    buffer_ptr   = in_buffer_ptr;
    size         = in_size;
    start_offset = in_start_offset;
}

/** Please see header for specification */
Anvil::DescriptorSet::BufferBindingElement::~BufferBindingElement()
{
}

/** Please see header for specification */
Anvil::DescriptorSet::BufferBindingElement::BufferBindingElement(const BufferBindingElement& in)
{
    buffer_ptr = in.buffer_ptr;
}

/** Please see header for specification */
Anvil::DescriptorSet::DescriptorSet(Anvil::Device*              device_ptr,
                                    Anvil::DescriptorPool*      parent_pool_ptr,
                                    Anvil::DescriptorSetLayout* layout_ptr,
                                    VkDescriptorSet             descriptor_set)
    :m_descriptor_set            (descriptor_set),
     m_device_ptr                (device_ptr),
     m_dirty                     (true),
     m_layout_ptr                (layout_ptr),
     m_parent_pool_ptr           (parent_pool_ptr),
     m_unusable                  (false)
{
    alloc_bindings();
}

/** Please see header for specification */
Anvil::DescriptorSet::~DescriptorSet()
{
}

/** Takes the descriptor set layout and:
 *
 *  1) resizes m_caches_ds_write_items_vk to hold as many bindings as the layout describes
 *  2) resizes each m_bindings item, described by the layout, to hold as many descriptor
 *     items, as described by the layout object.
 **/
void Anvil::DescriptorSet::alloc_bindings()
{
    const uint32_t n_bindings = m_layout_ptr->get_n_bindings();

    m_cached_ds_write_items_vk.resize(n_bindings);

    for (uint32_t n_binding = 0;
                  n_binding < n_bindings;
                ++n_binding)
    {
        uint32_t array_size = 0;
        uint32_t binding_index;

        m_layout_ptr->get_binding_properties(n_binding,
                                            &binding_index,
                                             nullptr, /* opt_out_descriptor_type_ptr */
                                            &array_size,
                                             nullptr,  /* opt_out_stage_flags_ptr                */
                                             nullptr); /* opt_out_immutable_samplers_enabled_ptr */

        if (m_bindings[binding_index].size() != array_size)
        {
            m_bindings[binding_index] = BindingItems();

            m_bindings[binding_index].resize(array_size);
        }
    }
}

/* Please see header for specification */
bool Anvil::DescriptorSet::bake()
{
    bool result = false;

    anvil_assert(!m_unusable);

    if (m_dirty)
    {
        uint32_t       cached_ds_buffer_info_items_array_offset       = (uint32_t) 0;
        uint32_t       cached_ds_image_info_items_array_offset        = (uint32_t) 0;
        uint32_t       cached_ds_texel_buffer_info_items_array_offset = (uint32_t) 0;
        const uint32_t n_bindings                                     = (uint32_t) m_bindings.size();
        uint32_t       n_max_ds_info_items_to_cache                   = 0;

        m_cached_ds_info_buffer_info_items_vk.clear();
        m_cached_ds_write_items_vk.clear();

        for (auto& binding : m_bindings)
        {
            const uint32_t n_current_binding_items = static_cast<uint32_t>(binding.second.size() );

            n_max_ds_info_items_to_cache += n_current_binding_items;
        }

        m_cached_ds_info_buffer_info_items_vk.reserve(n_max_ds_info_items_to_cache);

        for (uint32_t n_binding = 0;
                      n_binding < n_bindings;
                    ++n_binding)
        {
            uint32_t              current_binding_index;
            VkDescriptorType      descriptor_type;
            bool                  immutable_samplers_enabled                    = false;
            const uint32_t        start_ds_buffer_info_items_array_offset       = cached_ds_buffer_info_items_array_offset;
            const uint32_t        start_ds_image_info_items_array_offset        = cached_ds_image_info_items_array_offset;
            const uint32_t        start_ds_texel_buffer_info_items_array_offset = cached_ds_texel_buffer_info_items_array_offset;
            VkWriteDescriptorSet  write_ds_vk;

            if (!m_layout_ptr->get_binding_properties(n_binding,
                                                     &current_binding_index,
                                                     &descriptor_type,
                                                      nullptr, /* opt_out_descriptor_array_size_ptr */
                                                      nullptr, /* opt_out_stage_flags_ptr           */
                                                     &immutable_samplers_enabled) )
            {
                anvil_assert(false);
            }

            /* For each array item, initialize a descriptor info item.. */
            BindingItems&  current_binding_items   = m_bindings[current_binding_index];
            const uint32_t n_current_binding_items = static_cast<uint32_t>(current_binding_items.size() );

            for (uint32_t n_current_binding_item = 0;
                          n_current_binding_item < n_current_binding_items;
                        ++n_current_binding_item)
            {
                BindingItem& current_binding_item = current_binding_items[n_current_binding_item];

                if (!current_binding_item.dirty)
                {
                    continue;
                }

                if (current_binding_item.buffer_ptr != nullptr)
                {
                    VkDescriptorBufferInfo buffer_info;

                    buffer_info.buffer = current_binding_item.buffer_ptr->get_buffer();

                    if (current_binding_item.start_offset != -1)
                    {
                        buffer_info.offset = current_binding_item.start_offset;
                        buffer_info.range  = current_binding_item.size;
                    }
                    else
                    {
                        buffer_info.offset = current_binding_item.buffer_ptr->get_start_offset();
                        buffer_info.range  = current_binding_item.buffer_ptr->get_size        ();
                    }

                    m_cached_ds_info_buffer_info_items_vk.push_back(buffer_info);

                    ++cached_ds_buffer_info_items_array_offset;
                }
                else
                {
                    anvil_assert(false);

                    goto end;
                }
            }

            /* We can finally fill the write descriptor */
            if (current_binding_items.size() > 0)
            {
                write_ds_vk.descriptorCount  = (uint32_t) current_binding_items.size();
                write_ds_vk.descriptorType   = descriptor_type;
                write_ds_vk.dstArrayElement  = 0;
                write_ds_vk.dstBinding       = current_binding_index;
                write_ds_vk.dstSet           = m_descriptor_set;
                write_ds_vk.pBufferInfo      = (start_ds_buffer_info_items_array_offset != cached_ds_buffer_info_items_array_offset)             ? &m_cached_ds_info_buffer_info_items_vk[start_ds_buffer_info_items_array_offset]
                                                                                                                                                 : nullptr;
                write_ds_vk.pImageInfo       = NULL;
                write_ds_vk.pNext            = nullptr;
                write_ds_vk.pTexelBufferView = NULL;
                write_ds_vk.sType            = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;

                m_cached_ds_write_items_vk.push_back(write_ds_vk);
            }
        }

        /* Issue the Vulkan call */
        if (m_cached_ds_write_items_vk.size() > 0)
        {
            vkUpdateDescriptorSets(m_device_ptr->get_device_vk(),
                                   (uint32_t) m_cached_ds_write_items_vk.size(),
                                  &m_cached_ds_write_items_vk[0],
                                   0,        /* copyCount         */
                                   nullptr); /* pDescriptorCopies */
        }

        m_dirty = false;
    }

    result = true;

end:
    return result;
}

/** Entry-point called back whenever a new binding is added to the parent layout.
 *
 *  This will resize a number of internally managed vectors.
 *
 *  @param layout_raw_ptr Ignored.
 *  @param ds_raw_ptr     Pointer to a DescriptorSet instance which uses the modified layout as the parent.
 *                        Never nullptr.
 *
 **/
void Anvil::DescriptorSet::on_binding_added_to_layout(void* layout_raw_ptr,
                                                      void* ds_raw_ptr)
{
    Anvil::DescriptorSet* ds_ptr = static_cast<Anvil::DescriptorSet*>(ds_raw_ptr);

    ds_ptr->alloc_bindings();
}

/** Called back whenever parent descriptor pool is reset.
 *
 *  Resets m_descriptor_set back to VK_NULL_HANDLE and marks the descriptor set as unusable.
 *
 *  @param pool_raw_ptr Ignored.
 *  @param ds_raw_ptr   Affected DescriptorSet instance. Never nullptr.
 *
 **/
void Anvil::DescriptorSet::on_parent_pool_reset(void* pool_raw_ptr,
                                                void* ds_raw_ptr)
{
    Anvil::DescriptorSet* ds_ptr = static_cast<Anvil::DescriptorSet*>(ds_raw_ptr);

    /* This descriptor set instance is no longer usable.
     *
     * To restore functionality, a new Vulkan DS handle should be assigned to this instance
     * by calling set_new_vk_handle() */
    ds_ptr->m_descriptor_set = VK_NULL_HANDLE;
    ds_ptr->m_unusable       = true;
}

/** Please see header for specification */
void Anvil::DescriptorSet::set_new_vk_handle(VkDescriptorSet ds)
{
    anvil_assert(m_unusable);
    anvil_assert(ds != VK_NULL_HANDLE);

    m_descriptor_set = ds;
    m_dirty          = true;
    m_unusable       = false;
}