#pragma once

class MatrixBuffer : public ConstBuffer
{
public:
    MatrixBuffer() : ConstBuffer(&matrix, sizeof(Matrix))
    {
        matrix = XMMatrixIdentity();        // 기본 단위행렬 생성
    }

    void SetMatrix(Matrix value)
    {
        matrix = XMMatrixTranspose(value);  // 전치 행렬 반환
    }

private:
    Matrix matrix;
};


class ViewBuffer : public ConstBuffer
{
private:
    struct Data
    {
        Matrix view;        // 카메라의 역행렬
        Matrix invView;     // 카메라의 월드 행렬
    };

public:
    ViewBuffer() : ConstBuffer(&data, sizeof(Data))
    {
        data.view = XMMatrixIdentity();
        data.invView = XMMatrixIdentity();
    }

    void SetMatrix(Matrix view, Matrix invView)
    {
        data.view = XMMatrixTranspose(view);
        data.invView = XMMatrixTranspose(invView);
    }

private:
    Data data;
};


class FloatValueBuffer : public ConstBuffer
{
public:
    FloatValueBuffer() : ConstBuffer(values, sizeof(float) * 4)
    {
    }

    float* Get() { return values; }

private:
    float values[4] = {};
};