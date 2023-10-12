#include "Framework.h"

DirectLight::DirectLight()
{
	constBuffer = new ConstBuffer(&light, sizeof(DirectionalLight));
}

DirectLight::~DirectLight()
{
	delete constBuffer;
}

void DirectLight::GUIRender()
{
	if (ImGui::TreeNode("DirectLight"))
	{
		ImGui::ColorEdit3("Color", (float*)&light.color, ImGuiColorEditFlags_PickerHueWheel);
		ImGui::DragFloat3("Pos", (float*)&light.pos);
		ImGui::SliderFloat3("Dir", (float*)&light.direction, -1, 1);
		ImGui::SliderFloat("Range", &light.range, 1, 1000);
		ImGui::TreePop();
	}
}

// Direct Light ¼Â
void DirectLight::SetPixelShaderBuffer(UINT slot)
{
	constBuffer->SetPixelShaderBuffer(slot);
}
