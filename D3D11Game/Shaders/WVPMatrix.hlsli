// World Buffer
cbuffer WorldBuffer : register(b0)
{
    matrix world;
}

// View Buffer
cbuffer ViewBuffer : register(b1)
{
    matrix view;
    matrix invView;
}

// Projection Buffer
cbuffer ProjectionBuffer : register(b2)
{
    matrix projection;
}

