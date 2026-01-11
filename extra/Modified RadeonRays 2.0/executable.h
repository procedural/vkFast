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

//#define DUMP_SPIRV_BLOB

#include "function.h"

namespace Calc {

    // Class that represent Executable. In Vulkan implementation the only useful is CreateFunction that compiles shader to SPIR-V and then create Function. Each Function is decoupled from others and stored in a separate file.
    class Executable {
    public:
        Executable(Anvil::Device *in_device, bool in_use_compute_pipe)
                : m_device(in_device),
                  m_file_name_or_source_code(), m_source_mode(
                        Anvil::GLSLShaderToSPIRVGenerator::MODE_USE_SPECIFIED_SOURCE),
                  m_use_compute_pipe(in_use_compute_pipe) { }

        Executable(Anvil::Device *in_device,
                         const std::string &inFileName,
                         bool in_use_compute_pipe)
                : m_device(in_device),
                  m_file_name_or_source_code(inFileName), m_source_mode(
                        Anvil::GLSLShaderToSPIRVGenerator::MODE_LOAD_SOURCE_FROM_FILE),
                  m_use_compute_pipe(in_use_compute_pipe) { }

        Executable(Anvil::Device *in_device, char const *in_source_code,
                         size_t in_source_code_size, bool in_use_compute_pipe)
                : m_device(in_device),
                  m_file_name_or_source_code(), m_source_mode(
                        Anvil::GLSLShaderToSPIRVGenerator::MODE_USE_SPECIFIED_SOURCE),
                  m_use_compute_pipe(in_use_compute_pipe) {
            m_file_name_or_source_code = std::string(in_source_code,
                                                     in_source_code_size);
        }

        virtual ~Executable() { }

        // adds a defination to all future CreateFunction calls, replaces if
        // any existing definition of the same name
        void AddDefinition(const char *name, const char *value) {
            m_defines[name] = value;
        }
        
        void RemoveDefinition( const char *name ) {
            auto&& existing = m_defines.find(name);
            Assert(existing != m_defines.end() );
            m_defines.erase(existing);
        }

        // useful whilst developing, not currently used
        Function *CreateFunction(const void * spirv_blob, uint32_t n_spirv_blob_bytes) {

            Anvil::ShaderModule *shaderModule = new Anvil::ShaderModule(
                    m_device, (const char *)spirv_blob, n_spirv_blob_bytes, "main");

            Anvil::ShaderModuleStageEntryPoint functionEntryPoint = Anvil::ShaderModuleStageEntryPoint(
                    "main", shaderModule, Anvil::SHADER_STAGE_COMPUTE);

            return new Function(m_device, functionEntryPoint,
                                      shaderModule, m_use_compute_pipe
#if _DEBUG
                    , m_file_name_or_source_code
#endif
            );
        }

        void DeleteFunction(Function *func) {
            delete func;
        }

    private:
        Anvil::Device *m_device;
        std::string m_file_name_or_source_code;
        Anvil::GLSLShaderToSPIRVGenerator::Mode m_source_mode;
        typedef std::map<std::string, std::string> define_table;
        define_table m_defines;
        bool m_use_compute_pipe;
    };
}
