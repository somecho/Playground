#include <chrono>
#include <fstream>
#include <SomeApp/App.h>

#include "SomeApp/ColorAttachmentBuilder.h"

int main() {
    some::App app(some::AppSettings{.title = "Hello Triangle"});

    std::stringstream stream;
    stream << std::ifstream("resources/HelloTriangleSimple.wgsl").rdbuf();
    std::string source = stream.str();

    wgpu::ShaderSourceWGSL wgsl;
    wgsl.code = source.c_str();

    const wgpu::ShaderModuleDescriptor shaderDesc{.nextInChain = &wgsl};
    const wgpu::ShaderModule shader = app.device.
            CreateShaderModule(&shaderDesc);
    wgpu::ColorTargetState cts{.format = app.capabilities.formats[0]};
    const wgpu::VertexState vs{.module = shader};
    wgpu::FragmentState fs{.module = shader, .targetCount = 1, .targets = &cts};
    const wgpu::RenderPipelineDescriptor renderDesc{
        .vertex = vs,
        .fragment = &fs
    };
    wgpu::RenderPipeline pipeline = app.device.
            CreateRenderPipeline(&renderDesc);


    while (!app.shouldClose()) {
        auto att = some::ColorAttachmentBuilder()
                .view(app.getCurrentTextureView())
                .build();

        wgpu::RenderPassDescriptor renderPassDesc{
            .colorAttachmentCount = 1,
            .colorAttachments = &att
        };
        wgpu::CommandEncoder encoder = app.device.CreateCommandEncoder();
        wgpu::RenderPassEncoder pass = encoder.BeginRenderPass(&renderPassDesc);
        pass.SetPipeline(pipeline);
        pass.Draw(3);
        pass.End();
        wgpu::CommandBuffer commands = encoder.Finish();
        app.device.GetQueue().Submit(1, &commands);
        app.surface.Present();
        app.pollEvents();
    }

    return 0;
}
