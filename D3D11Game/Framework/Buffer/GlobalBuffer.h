#pragma once

class MatrixBuffer : public ConstBuffer
{
public:
    MatrixBuffer() : ConstBuffer(&matrix, sizeof(Matrix))
    {
        matrix = XMMatrixIdentity();        // �⺻ ������� ����
    }

    void SetMatrix(Matrix value)
    {
        matrix = XMMatrixTranspose(value);  // ��ġ ��� ��ȯ
    }

private:
    Matrix matrix;
};


class ViewBuffer : public ConstBuffer
{
private:
    struct Data
    {
        Matrix view;        // ī�޶��� �����
        Matrix invView;     // ī�޶��� ���� ���
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