struct VertexInput {
    @location(0) pos: vec3f,
    @location(1) col: vec3f
}

struct VertexOutput {
    @builtin(position) pos: vec4f,
    @location(0) col: vec4f
}

@group(0) @binding(0) var<uniform> orthoMat: mat4x4<f32>;
@group(0) @binding(1) var<uniform> time: f32;

@vertex
fn vertex_main(in: VertexInput) -> VertexOutput{
    var out: VertexOutput;
    out.pos =  orthoMat * vec4f(in.pos,1);
    out.col = vec4f(in.col.r * (cos(time) * 0.5 + 0.5),
                    in.col.g * (sin(time * 1.3) * 0.5 + 0.5),
                    in.col.b * (sin(time) * 0.5 + 0.5),
                    1);
    return out;
}

@fragment
fn fragment_main(@location(0) col: vec4f) -> @location(0) vec4f {
    return col;
}