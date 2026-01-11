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

/** Compute pipeline manager. A class which inherits from the base pipeline object manager.
 *
 *  Apart from exposing the functionality offered by the parent class under slightly
 *  renamed, pipeline-specific function names, this wrapper implements the baking process.
 *
 **/
#ifndef WRAPPERS_COMPUTE_PIPELINE_MANAGER_H
#define WRAPPERS_COMPUTE_PIPELINE_MANAGER_H

#include "base_pipeline_manager.h"
#include "types.h"
#include <memory>

namespace Anvil
{

    typedef PipelineID ComputePipelineID;

    class ComputePipelineManager : public BasePipelineManager
    {
    public:
        /* Public functions */
       explicit ComputePipelineManager(Anvil::Device*        device_ptr);

       ~ComputePipelineManager();


       /** Registers a new pipeline object.
        *
        *  The function does not automatically bake the new pipeline. This action can either be explicitly
        *  requested by calling bake(), or by calling one of the get_*() functions.
        *
        *  @param in_disable_optimizations                If true, the pipeline will be created with the 
        *                                                 VK_PIPELINE_CREATE_DISABLE_OPTIMIZATION_BIT flag.
        *  @param in_allow_derivatives                    If true, the pipeline will be created with the
        *                                                 VK_PIPELINE_CREATE_ALLOW_DERIVATIVES_BIT flag.
        *  @param in_compute_shader_stage_entrypoint_info Compute shader stage info.
        *  @param out_pipeline_id_ptr                     Deref will be set to the ID of the result pipeline object. Must not be nullptr.
        *
        *  @return true if the function executed successfully, false otherwise.
        **/
       bool add_regular_pipeline(const ShaderModuleStageEntryPoint& in_compute_shader_stage_entrypoint_info,
                                 ComputePipelineID*                 out_compute_pipeline_id_ptr)
       {
           return BasePipelineManager::add_regular_pipeline(1, /* n_pipeline_stage_shader_info_items */
                                                           &in_compute_shader_stage_entrypoint_info,
                                                            out_compute_pipeline_id_ptr);
       }

       bool bake();

       /** Deletes an existing pipeline.
        *
        *  @param pipeline_id ID of a pipeline to delete.
        *
        *  @return true if successful, false otherwise.
        **/
       bool delete_pipeline(ComputePipelineID pipeline_id)
       {
           return BasePipelineManager::delete_pipeline(pipeline_id);
       }

       /** Retrieves a VkPipeline instance associated with the specified pipeline ID.
        *
        *  The function will bake a pipeline object (and, possibly, a pipeline layout object, too) if
        *  the specified pipeline is marked as dirty.
        *
        *  @param pipeline_id ID of the pipeline to return the raw Vulkan pipeline handle for.
        *
        *  @return VkPipeline handle or nullptr if the function failed.
        **/
       VkPipeline get_compute_pipeline(ComputePipelineID pipeline_id)
       {
           return BasePipelineManager::get_pipeline(pipeline_id);
       }

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
       Anvil::PipelineLayout* get_compute_pipeline_layout(ComputePipelineID pipeline_id)
       {
           return BasePipelineManager::get_pipeline_layout(pipeline_id);
       }
    };
}; /* Vulkan namespace */

#endif /* WRAPPERS_COMPUTE_PIPELINE_MANAGER_H */