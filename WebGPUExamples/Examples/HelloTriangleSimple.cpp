#include <chrono>
#include <SomeApp/App.h>

#include "SomeApp/ColorAttachmentBuilder.h"
#include "SomeApp/create.h"

int main() {
    some::App app(some::AppSettings{.title = "Hello Triangle"});

    std::string shaderPath = std::format("{}/HelloTriangleSimple.wgsl",
                                         RESOURCES);
    wgpu::ShaderModule shader =
            some::createShaderModule(shaderPath, app.device);

    wgpu::ColorTargetState cts{.format = app.capabilities.formats[0]};
    wgpu::VertexState vs{.module = shader};
    wgpu::FragmentState fs{.module = shader, .targetCount = 1, .targets = &cts};
    wgpu::RenderPipelineDescriptor renderDesc{
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
