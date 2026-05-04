// NOTE(Constantine):
// g++ -g -O0 -I"../Orochi 2024" main.cpp "../Orochi 2024/build/obj/x64/Release/WMMA/*.o" -lcuda
// * Create testKernel file with CUDA code *
// cuda-gdb --args ./a.out cuda
// b main
// b testKernel
// r
// tui e
// foc n
// ref

//
// Copyright (c) 2021-2022 Advanced Micro Devices, Inc. All rights reserved.
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

#include <Orochi/Orochi.h>
#include <Test/Common.h>


int main(int argc, char** argv )
{
  oroApi api = ORO_API_HIP; // NOTE(Constantine): ORO_API_HIP needs AMD ROCm-capable GPU (RDNA 1.0+)

  int a = oroInitialize( api, 0 );
  if( a != 0 )
  {
    printf("initialization failed\n");
    return 0;
  }
  printf( ">> executing on %s\n", ( api == ORO_API_HIP )? "hip":"cuda" );

  printf(">> testing initialization\n");
  oroError e;
  e = oroInit( 0 );
  oroDevice device;
  e = oroDeviceGet( &device, 0 );
  oroCtx ctx;
  e = oroCtxCreate( &ctx, 0, device );

  printf(">> testing device props\n");
  {
    oroDeviceProp props;
    oroGetDeviceProperties( &props, device );
    printf("executing on %s (%s)\n", props.name, props.gcnArchName );
    int v;
    oroDriverGetVersion( &v );
    printf("running on driver: %d\n", v);
  }
  printf(">> testing kernel execution\n");
  {
    oroFunction function;
    {
      const char* code = "extern \"C\" __global__ void testKernel( int* __restrict__ a )\n"
      "{\n"
      "  int tid = threadIdx.x;\n"
      "  atomicAdd( a, tid );\n"
      "}\n";
      const char* funcName = "testKernel";
      orortcProgram prog;
      orortcResult e;
      e = orortcCreateProgram( &prog, code, funcName, 0, 0, 0 );
      std::vector<const char*> opts;
      //opts.push_back( "--device-debug" );       // https://stackoverflow.com/questions/54670436/using-cuda-gdb-with-nvrtc
      //opts.push_back( "--generate-line-info" ); // https://docs.nvidia.com/cuda/nvrtc/index.html
      opts.push_back( "-I ../" );

      e = orortcCompileProgram( prog, opts.size(), opts.data() );
      if( e != ORORTC_SUCCESS )
      {
        size_t logSize;
        orortcGetProgramLogSize(prog, &logSize);
        if (logSize)
        {
          std::string log(logSize, '\0');
          orortcGetProgramLog(prog, &log[0]);
          std::cout << log << '\n';
        };
      }
      size_t codeSize;
      e = orortcGetCodeSize(prog, &codeSize);

      std::vector<char> codec(codeSize);
      e = orortcGetCode(prog, codec.data());
      e = orortcDestroyProgram(&prog);
      oroModule module;
      std::vector<oroJitOption> linkOpts;
      std::vector<void *> linkOptsData;
      linkOpts.push_back( oroJitOptionGenerateDebugInfo ); linkOptsData.push_back( (void *)1 );
      oroError ee = oroModuleLoadDataEx(&module, codec.data(), linkOpts.size(), linkOpts.data(), linkOptsData.data());
      ee = oroModuleGetFunction(&function, module, funcName);
    }

    oroStream stream;
    oroStreamCreate( &stream );

    oroEvent start, stop;
    oroEventCreateWithFlags( &start, 0 );
    oroEventCreateWithFlags( &stop, 0 );
    oroEventRecord( start, stream );

    int a_host = -1;
    int* a_device = nullptr;
    oroMalloc( (oroDeviceptr*)&a_device, sizeof( int ) );
    oroMemset( (oroDeviceptr)a_device, 0, sizeof( int ) );
    const void* args[] = { &a_device };
    oroError e = oroModuleLaunchKernel( function, 1,1,1, 64,1,1, 0, stream, (void**)args, 0 );

    oroEventRecord( stop, stream );

    oroDeviceSynchronize();

    oroStreamDestroy( stream );
    oroMemcpyDtoH( &a_host, (oroDeviceptr)a_device, sizeof( int ) );
    printf("a_host (expected 2016): %d\n", a_host);
    oroFree( (oroDeviceptr)a_device );

    float milliseconds = 0.0f;
    oroEventElapsedTime( &milliseconds, start, stop );
    printf( ">> kernel - %.5f ms\n", milliseconds );
    oroEventDestroy( start );
    oroEventDestroy( stop );
  }
  printf(">> done\n");
  return 0;
}
