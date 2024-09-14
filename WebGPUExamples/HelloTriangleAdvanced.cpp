#include <fstream>
#include <glm/gtc/matrix_transform.hpp>

#include "SomeApp/App.h"
#include "SomeApp/BindGroup.h"
#include "SomeApp/BufferBuilder.h"
#include "SomeApp/ColorAttachmentBuilder.h"
#include "SomeApp/create.h"

int main() {
    some::App app(some::AppSettings{.title = "GLFW"});
    std::string shaderPath = std::format("{}/HelloTriangleAdvanced.wgsl",
                                         RESOURCES);
    wgpu::ShaderModule module =
            some::createShaderModule(shaderPath, app.device);

    wgpu::VertexAttribute posAttrib{wgpu::VertexFormat::Float32x3, 0, 0};
    auto vertexBuffer = some::BufferBuilder<float>()
            .data(std::vector<float>{
                200, 200, 0,
                300, 400, 0,
                600, 300, 0
            })
            .addUsage(wgpu::BufferUsage::Vertex)
            .build(app.device);

    wgpu::VertexAttribute colAttrib{wgpu::VertexFormat::Float32x3, 0, 1};
    auto colorBuffer = some::BufferBuilder<float>()
            .data(std::vector<float>{1, 0, 0, 0, 1, 0, 0, 0, 1})
            .addUsage(wgpu::BufferUsage::Vertex)
            .build(app.device);

    auto indexBuffer = some::BufferBuilder<uint32_t>()
            .addUsage(wgpu::BufferUsage::Index)
            .data(std::vector<uint32_t>{0, 1, 2})
            .build(app.device);

    std::vector layouts{
        some::createBufferLayout(sizeof(float) * 3, &posAttrib),
        some::createBufferLayout(sizeof(float) * 3, &colAttrib),
    };

    auto orthoMat = glm::ortho<float>(0, static_cast<float>(app.getWidth()),
                                      static_cast<float>(app.getHeight()), 0,
                                      -1, 1);
    auto orthoBuf = some::BufferBuilder<float>()
            .label("Orthographic Matrix Uniform")
            .addUsage(wgpu::BufferUsage::Uniform)
            .data(orthoMat)
            .build(app.device);

    auto timeBuf = some::BufferBuilder<float>()
            .label("Time uniform")
            .addUsage(wgpu::BufferUsage::Uniform)
            .data(std::vector<float>{0.0})
            .build(app.device);

    auto bindGroup = some::BindGroup()
            .label("Uniform Bind Group")
            .newEntry(0, orthoBuf, 0, orthoBuf.GetSize())
            .newEntry(1, timeBuf, 0, timeBuf.GetSize())
            .bind(app.device);

    wgpu::ColorTargetState cts = {
        .format = app.capabilities.formats[0]
    };

    wgpu::VertexState vs = {
        .module = module,
        .bufferCount = layouts.size(),
        .buffers = layouts.data(),
    };

    wgpu::FragmentState fs = {
        .module = module, .targetCount = 1, .targets = &cts
    };

    wgpu::PipelineLayoutDescriptor pipelineLayoutDesc = {
        .bindGroupLayoutCount = 1,
        .bindGroupLayouts = &bindGroup.getLayout()
    };
    auto pipelineLayout = app.device.CreatePipelineLayout(&pipelineLayoutDesc);

    wgpu::RenderPipelineDescriptor pipelineDesc = {
        .layout = pipelineLayout, .vertex = vs, .fragment = &fs,
    };
    wgpu::RenderPipeline pipeline = app.device.CreateRenderPipeline(
        &pipelineDesc);

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
        pass.SetVertexBuffer(0, vertexBuffer, 0, vertexBuffer.GetSize());
        pass.SetVertexBuffer(1, colorBuffer, 0, colorBuffer.GetSize());
        pass.SetIndexBuffer(indexBuffer, wgpu::IndexFormat::Uint32);
        pass.SetBindGroup(0, bindGroup.get(), 0, nullptr);
        pass.DrawIndexed(3, 1, 0, 0);
        pass.End();
        wgpu::CommandBuffer commands = encoder.Finish();
        app.device.GetQueue().Submit(1, &commands);
        app.surface.Present();
        app.pollEvents();

        app.device.GetQueue().WriteBuffer(timeBuf, 0,
                                          std::vector{
                                              static_cast<float>(glfwGetTime())
                                          }.data(), sizeof(float));
    }
}
