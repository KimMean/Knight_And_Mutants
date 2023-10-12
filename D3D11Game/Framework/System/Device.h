#pragma once

class Device : public Singleton<Device>
{
private :
	friend class Singleton;

	Device();
	~Device();

public :
	ID3D11Device* GetDevice() { return device; }
	ID3D11DeviceContext* GetDeviceContext() { return deviceContext; }
	IDXGISwapChain* GetSwapChain() { return swapChain; }

public:
	void SetViewport();
	void Clear();

	void Present();

private :
	void CreateDeviceAndSwapChain();
	void CreateBackBufferAndDepthStencillBuffer();


private :
	ID3D11Device* device;
	ID3D11DeviceContext* deviceContext;

	IDXGISwapChain* swapChain;
	ID3D11RenderTargetView* renderTargetView;
	ID3D11DepthStencilView* depthStencilView;
};