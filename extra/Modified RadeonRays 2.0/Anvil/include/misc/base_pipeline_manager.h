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

/** Base pipeline object manager. An abstract class which:
 *
 *  - simplifies the process of creating pipeline & derivative pipeline objects.
 *  - relies on PipelineLayoutManager to automatically re-use pipeline layout objects
 *    if the same layout is used for more than one pipeline object.
 *  - tracks life-time of baked Vulkan pipeline objects.
 *  - optionally defers the process of baking these objects until they're needed.
 *
 *  Any number of push constant ranges, as well as specialization constants can be assigned
 *  to the created pipeline objects.
 *
 *  This class is inherited by compute & graphics pipeline managers to provide common
 *  implementation for shared areas of functionality.
 **/
#ifndef MISC_BASE_PIPELINE_MANAGER_H
#define MISC_BASE_PIPELINE_MANAGER_H

#include "misc/debug.h"
#include "misc/types.h"
#include <memory>
#include <vector>

namespace Anvil
{

    class BasePipelineManager
    {
    public:
       /* Public type definitions */

       typedef uint32_t ShaderIndex;

       /* Public functions */

       /** Constructor. Initializes base layer of a pipeline manager.
        *
        *  @param device_ptr                  Device to use.
        *  @param use_pipeline_cache          true if a pipeline cache should be used to spawn new pipeline objects.
        *                                     What pipeline cache ends up being used depends on @param pipeline_cache_to_reuse_ptr -
        *                                     if a nullptr object is passed via this argument, a new pipeline cache instance will
        *                                     be created, and later released by the destructor. If a non-nullptr object is passed,
        *                                     it will be used instead.
        *  @param pipeline_cache_to_reuse_ptr Please see above.
        **/
       explicit BasePipelineManager(Anvil::Device*        device_ptr);

       /** Destructor. Releases internally managed objects. */
       virtual ~BasePipelineManager();

       /** Appends a new push constant range description to a pipeline object. The actual data needs to be specified
        *  when creating a command buffer.
        *
        *  The function marks the specified pipeline as dirty, meaning it will be re-baked at the next get_() call time.
        *
        *  @param pipeline_id ID of the pipeline to append the push constant range to. Must not describe a proxy
        *                     pipeline.
        *  @param offset      Start offset, at which the push constant data will start.
        *  @param size        Size of the push constant data.
        *  @param stages      Pipeline stages the push constant data should be accessible to.
        *
        *  @return true if successful, false otherwise.
        **/
       bool attach_push_constant_range_to_pipeline(PipelineID         pipeline_id,
                                                   uint32_t           offset,
                                                   uint32_t           size,
                                                   VkShaderStageFlags stages);

       /** Appends all descriptor sets, in order as described by the specified DescriptorSetGroup instance, to the
        *  list of descriptor sets which have been already defined for the specified pipeline object.
        *
        *  This function marks the pipeline as dirty, meaning it will be re-baked at the next get_*() call.
        *
        *  @param pipeline_id ID of the compute pipeline to attach the descriptor sets defined by the group to.
        *  @param dsg_ptr     Descriptor set group instance, whose descriptor sets should be appended.
        *
        *  @return true if successful, false otherwise.
        **/
       bool attach_dsg_to_pipeline(PipelineID                 pipeline_id,
                                   Anvil::DescriptorSetGroup* dsg_ptr);

       virtual bool bake() = 0;

    protected:
       /* Protected type declarations */

       /** Internal pipeline object descriptor */
       typedef struct Pipeline
       {
           Anvil::Device*            device_ptr;

           DescriptorSetGroups                      descriptor_set_groups;
           PushConstantRanges                       push_constant_ranges;

           std::vector<ShaderModuleStageEntryPoint> shader_stages;

           bool            layout_dirty;
           PipelineLayout* layout_ptr;

           VkPipeline      baked_pipeline;
           bool            dirty;
           bool            is_bakeable;
           bool            is_proxy;

           /** Stores the specified shader modules and associates an empty specialization constant map for each
            *  shader.
            *
            *  @param in_n_shader_module_stage_entrypoints   Number of shader module stage entrypoint descriptors available under
            *                                                @param in_shader_module_stage_entrypoint_ptrs. Must be >= 1.
            *  @param in_shader_module_stage_entrypoint_ptrs Array of shader module stage entrypoint descriptors. Must hold
            *                                                @param in_n_shader_module_stage_entrypoints elements. Must not be nullptr.
            **/
           void init_shader_modules(uint32_t                           in_n_shader_module_stage_entrypoints,
                                    const ShaderModuleStageEntryPoint* in_shader_module_stage_entrypoint_ptrs)
           {
               shader_stages.reserve(in_n_shader_module_stage_entrypoints);

               for (uint32_t n_shader_module_stage_entrypoint = 0;
                             n_shader_module_stage_entrypoint < in_n_shader_module_stage_entrypoints;
                           ++n_shader_module_stage_entrypoint)
               {
                   shader_stages.push_back(in_shader_module_stage_entrypoint_ptrs[n_shader_module_stage_entrypoint]);
               }
           }

           /** Constructor. Should be used to initialize a derivative pipeline object from an existing
            *  pipeline object managed by the pipeline manager.
            *
            *  @param in_device_ptr                          Device to use.
            *  @param in_disable_optimizations               If true, the pipeline will be created with the 
            *                                                VK_PIPELINE_CREATE_DISABLE_OPTIMIZATION_BIT flag.
            *  @param in_allow_derivatives                   If true, the pipeline will be created with the
            *                                                VK_PIPELINE_CREATE_ALLOW_DERIVATIVES_BIT flag.
            *  @param in_base_pipeline_ptr                   Pointer to the internal Pipeline descriptor. Must not be nullptr.
            *                                                Must not refer to a proxy pipeline.
            *  @param in_n_shader_module_stage_entrypoints   Number of shader module stage entrypoint descriptors available under
            *                                                @param in_shader_module_stage_entrypoint_ptrs. Must be >= 1.
            *  @param in_shader_module_stage_entrypoint_ptrs Array of shader module stage entrypoint descriptors. Must hold
            *                                                @param in_n_shader_module_stage_entrypoints elements. Must not be nullptr.
            **/
           Pipeline(Anvil::Device*                     in_device_ptr,
                    uint32_t                           in_n_shader_module_stage_entrypoints,
                    const ShaderModuleStageEntryPoint* in_shader_module_stage_entrypoint_ptrs)
           {
               baked_pipeline        = VK_NULL_HANDLE;
               device_ptr            = in_device_ptr;
               dirty                 = true;
               is_bakeable           = true;
               is_proxy              = false;
               layout_dirty          = true;
               layout_ptr            = nullptr;

               init_shader_modules(in_n_shader_module_stage_entrypoints,
                                   in_shader_module_stage_entrypoint_ptrs);

               anvil_assert(!base_pipeline_ptr->is_proxy);
           }

           /** Constructor. Should be used to initialize a regular pipeline object.
            *
            *  @param in_device_ptr                          Device to use.
            *  @param in_disable_optimizations               If true, the pipeline will be created with the 
            *                                                VK_PIPELINE_CREATE_DISABLE_OPTIMIZATION_BIT flag.
            *  @param in_allow_derivatives                   If true, the pipeline will be created with the
            *                                                VK_PIPELINE_CREATE_ALLOW_DERIVATIVES_BIT flag.
            *  @param in_n_shader_module_stage_entrypoints   Number of shader module stage entrypoint descriptors available under
            *                                                @param in_shader_module_stage_entrypoint_ptrs. Must be >= 1.
            *  @param in_shader_module_stage_entrypoint_ptrs Array of shader module stage entrypoint descriptors. Must hold
            *                                                @param in_n_shader_module_stage_entrypoints elements. Must not be nullptr.
            *  @param in_is_proxy                            true if the created pipeline is a proxy pipeline; false otherwise.
            * 
            **/
           Pipeline(Anvil::Device*                     in_device_ptr,
                    uint32_t                           in_n_shader_module_stage_entrypoints,
                    const ShaderModuleStageEntryPoint* in_shader_module_stage_entrypoint_ptrs,
                    bool                               in_is_proxy)
           {
               baked_pipeline        = VK_NULL_HANDLE;
               device_ptr            = in_device_ptr;
               dirty                 = true;
               is_bakeable           = true;
               is_proxy              = in_is_proxy;
               layout_dirty          = true;
               layout_ptr            = nullptr;

               init_shader_modules(in_n_shader_module_stage_entrypoints,
                                   in_shader_module_stage_entrypoint_ptrs);

               anvil_assert(!is_proxy                                             ||
                             is_proxy && in_n_shader_module_stage_entrypoints == 0);
           }

           ~Pipeline()
           {
           }

       private:
           Pipeline();

       } Pipeline;

       typedef std::map<PipelineID, std::shared_ptr<Pipeline> > Pipelines;

       /* Protected functions */

       /** Registers a new pipeline object.
        *
        *  The function does not automatically bake the new pipeline. This action can either be explicitly
        *  requested by calling bake(), or by calling one of the get_*() functions.
        *
        *  @param disable_optimizations               If true, the pipeline will be created with the 
        *                                             VK_PIPELINE_CREATE_DISABLE_OPTIMIZATION_BIT flag.
        *  @param allow_derivatives                   If true, the pipeline will be created with the
        *                                             VK_PIPELINE_CREATE_ALLOW_DERIVATIVES_BIT flag.
        *  @param n_shader_module_stage_entrypoints   Number of shader module stage entrypoint descriptors available under
        *                                             @param in_shader_module_stage_entrypoint_ptrs. Must be >= 1.
        *  @param shader_module_stage_entrypoint_ptrs Array of shader module stage entrypoint descriptors. Must hold
        *                                             @param in_n_shader_module_stage_entrypoints elements. Must not be nullptr.
        *  @param out_pipeline_id_ptr                 Deref will be set to the ID of the result pipeline object. Must not be nullptr.
        *
        *  @return true if the function executed successfully, false otherwise.
        **/
       bool add_regular_pipeline(uint32_t                           n_shader_module_stage_entrypoints,
                                 const ShaderModuleStageEntryPoint* shader_module_stage_entrypoint_ptrs,
                                 PipelineID*                        out_pipeline_id_ptr);

       /** Deletes an existing pipeline.
        *
        *  @param pipeline_id ID of a pipeline to delete.
        *
        *  @return true if successful, false otherwise.
        **/
       bool delete_pipeline(PipelineID pipeline_id);

       /** Retrieves a VkPipeline instance associated with the specified pipeline ID.
        *
        *  The function will bake a pipeline object (and, possibly, a pipeline layout object, too) if
        *  the specified pipeline is marked as dirty.
        *
        *  @param pipeline_id ID of the pipeline to return the raw Vulkan pipeline handle for. Must not
        *                     describe a proxy pipeline.
        *
        *  @return VkPipeline handle or nullptr if the function failed.
        **/
       VkPipeline get_pipeline(PipelineID pipeline_id);

       /** Retrieves a PipelineLayout instance associated with the specified pipeline ID.
        *
        *  The function will bake a pipeline object (and, possibly, a pipeline layout object, too) if
        *  the specified pipeline is marked as dirty.
        *
        *  @param pipeline_id ID of the pipeline to return the wrapper instance for.
        *                     Must not describe a proxy pipeline.
        *
        *  @return Ptr to a PipelineLayout instance or nullptr if the function failed.
        **/
       Anvil::PipelineLayout* get_pipeline_layout(PipelineID pipeline_id);

       /* Protected members */
       Anvil::Device* m_device_ptr;
       uint32_t       m_pipeline_counter;
       Pipelines      m_pipelines;

       PipelineLayoutManager* m_pipeline_layout_manager_ptr;

private:
       /* Private functions */
       BasePipelineManager& operator=(const BasePipelineManager&);
       BasePipelineManager           (const BasePipelineManager&);

       void release_pipeline_vulkan_objects(std::shared_ptr<Pipeline> pipeline_ptr);
    };
}; /* Vulkan namespace */

#endif /* MISC_BASE_PIPELINE_MANAGER_H */