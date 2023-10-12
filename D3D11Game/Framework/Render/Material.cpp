#include "Framework.h"

Material::Material()
{
    char path[128];
    GetCurrentDirectoryA(128, path);
    projectPath = path;

	materialData = {};
	constBuffer = new ConstBuffer(&materialData, sizeof(MaterialData));

	SetShaderFile();
	SetDiffuseMap();
	SetSpecularMap();
	SetNormalMap();
}

Material::~Material()
{
	delete constBuffer;
}

void Material::GUIRender()
{
    string title = name + "_Material";

    if (ImGui::TreeNode(title.c_str()))
    {
        //char str[128];
        //strcpy_s(str, 128, editName.c_str());
        //ImGui::PushItemWidth(100);
        //ImGui::InputText("Name", str, 128);
        //ImGui::PopItemWidth();
        //editName = str;

        //ImGui::SameLine();
        //if (ImGui::Button("Edit"))
        //    name = editName;

        ImGui::ColorEdit3("Diffuse", (float*)&materialData.diffuse);
        ImGui::ColorEdit3("Specular", (float*)&materialData.specular);
        ImGui::ColorEdit3("Ambient", (float*)&materialData.ambient);
        ImGui::ColorEdit4("Emissive", (float*)&materialData.emissive);

        ImGui::SliderFloat("Shininess", &materialData.shininess, 1, 50);

        SelectMap("DM", DIFFUSE);
        ImGui::SameLine();
        UnselectMap(DIFFUSE);
        SelectMap("SM", SPECULAR);
        ImGui::SameLine();
        UnselectMap(SPECULAR);
        SelectMap("NM", NORMAL);
        ImGui::SameLine();
        UnselectMap(NORMAL);

        SaveDialog();
        LoadDialog();

        ImGui::TreePop();
    }
}

void Material::SetMaterial()
{
	diffuseMap->SetPixelShaderSlot(0);	// Texture2D diffuse
	specularMap->SetPixelShaderSlot(1);	// Texture2D specualr
	normalMap->SetPixelShaderSlot(2);	// Texture2D normal
    
	constBuffer->SetPixelShaderBuffer(1);	// 상수버퍼 할당
    
	vertexShader->SetShader();	// vertexShader 지정
	pixelShader->SetShader();	// pixelShader 지정
}

void Material::SetShaderFile(wstring shaderFile)
{
	vertexShader = Shader::AddVertexShader(shaderFile);
	pixelShader = Shader::AddPixelShader(shaderFile);
}

void Material::SetDiffuseMap(wstring textureFile)
{
	if (textureFile.length() > 0)
		diffuseMap = Texture::AddTexture(textureFile, L"DM");
	else
		diffuseMap = Texture::AddTexture(L"Textures/Color/White.png", L"DM");
}

void Material::SetSpecularMap(wstring textureFile)
{
	if (textureFile.length() > 0)
		specularMap = Texture::AddTexture(textureFile, L"SM");
	else
		specularMap = Texture::AddTexture(L"Textures/Color/White.png", L"SM");
}

void Material::SetNormalMap(wstring textureFile)
{
	if (textureFile.length() > 0)
		normalMap = Texture::AddTexture(textureFile, L"NM");
	else
		normalMap = Texture::AddTexture(L"Textures/Color/White.png", L"NM");
}

void Material::SaveMaterial(string filePath)
{
	tinyxml2::XMLDocument* document = new tinyxml2::XMLDocument();
	tinyxml2::XMLElement* material = document->NewElement("Material");

    material->SetAttribute("Name", name.c_str());
    document->InsertFirstChild(material);

    tinyxml2::XMLElement* shader = document->NewElement("Shader");
    string shaderFile;
    if (vertexShader)
        shaderFile = ToString(vertexShader->GetName());
    shader->SetAttribute("Vertex", shaderFile.c_str());
    if (pixelShader)
        shaderFile = ToString(pixelShader->GetName());
    shader->SetAttribute("Pixel", shaderFile.c_str());
    material->InsertFirstChild(shader);

    tinyxml2::XMLElement* texture = document->NewElement("Texture");
    string textureFile = ToString(diffuseMap->GetFile());
    texture->SetAttribute("Diffuse", textureFile.c_str());
    textureFile = ToString(specularMap->GetFile());
    texture->SetAttribute("Specular", textureFile.c_str());
    textureFile = ToString(normalMap->GetFile());
    texture->SetAttribute("Normal", textureFile.c_str());
    material->InsertEndChild(texture);

    tinyxml2::XMLElement* property = document->NewElement("Property");
    tinyxml2::XMLElement* diffuse = document->NewElement("Diffuse");
    
    diffuse->SetAttribute("R", materialData.diffuse.x);
    diffuse->SetAttribute("G", materialData.diffuse.y);
    diffuse->SetAttribute("B", materialData.diffuse.z);
    diffuse->SetAttribute("A", materialData.diffuse.w);
    property->InsertEndChild(diffuse);

    tinyxml2::XMLElement* specular = document->NewElement("Specular");
    specular->SetAttribute("R", materialData.specular.x);
    specular->SetAttribute("G", materialData.specular.y);
    specular->SetAttribute("B", materialData.specular.z);
    specular->SetAttribute("A", materialData.specular.w);
    property->InsertEndChild(specular);

    tinyxml2::XMLElement* ambient = document->NewElement("Ambient");
    ambient->SetAttribute("R", materialData.ambient.x);
    ambient->SetAttribute("G", materialData.ambient.y);
    ambient->SetAttribute("B", materialData.ambient.z);
    ambient->SetAttribute("A", materialData.ambient.w);
    property->InsertEndChild(ambient);

    tinyxml2::XMLElement* emissive = document->NewElement("Emissive");
    emissive->SetAttribute("R", materialData.emissive.x);
    emissive->SetAttribute("G", materialData.emissive.y);
    emissive->SetAttribute("B", materialData.emissive.z);
    emissive->SetAttribute("A", materialData.emissive.w);
    property->InsertEndChild(emissive);

    property->SetAttribute("Shininess", materialData.shininess);
    property->SetAttribute("HasNormalMap", materialData.hasNormalMap);

    material->InsertEndChild(property);

    document->SaveFile(filePath.c_str());

    delete document;
}

void Material::LoadMaterial(string filePath)
{
    this->file = filePath;

    tinyxml2::XMLDocument* document = new tinyxml2::XMLDocument();
    document->LoadFile(file.c_str());
    tinyxml2::XMLElement* material = document->FirstChildElement();
    name = material->Attribute("Name");

    tinyxml2::XMLElement* shader = material->FirstChildElement();
    wstring shaderFile = ToWString(shader->Attribute("Vertex"));
    //shaderFile = shaderFile.substr(shaderFile.find_first_of('/') + 1, shaderFile.length());
    vertexShader = Shader::AddVertexShader(shaderFile);
    shaderFile = ToWString(shader->Attribute("Pixel"));
    //shaderFile = shaderFile.substr(shaderFile.find_first_of('/') + 1, shaderFile.length());
    pixelShader = Shader::AddPixelShader(shaderFile);

    tinyxml2::XMLElement* texture = shader->NextSiblingElement();
    wstring textureFile = ToWString(texture->Attribute("Diffuse"));
    SetDiffuseMap(textureFile);
    textureFile = ToWString(texture->Attribute("Specular"));
    SetSpecularMap(textureFile);
    textureFile = ToWString(texture->Attribute("Normal"));
    SetNormalMap(textureFile);

    tinyxml2::XMLElement* property = texture->NextSiblingElement();
    tinyxml2::XMLElement* diffuse = property->FirstChildElement();
    materialData.diffuse.x = diffuse->FloatAttribute("R");
    materialData.diffuse.y = diffuse->FloatAttribute("G");
    materialData.diffuse.z = diffuse->FloatAttribute("B");
    materialData.diffuse.w = diffuse->FloatAttribute("A");

    tinyxml2::XMLElement* specular = diffuse->NextSiblingElement();
    materialData.specular.x = specular->FloatAttribute("R");
    materialData.specular.y = specular->FloatAttribute("G");
    materialData.specular.z = specular->FloatAttribute("B");
    materialData.specular.w = specular->FloatAttribute("A");

    tinyxml2::XMLElement* ambient = specular->NextSiblingElement();
    materialData.ambient.x = ambient->FloatAttribute("R");
    materialData.ambient.y = ambient->FloatAttribute("G");
    materialData.ambient.z = ambient->FloatAttribute("B");
    materialData.ambient.w = ambient->FloatAttribute("A");

    tinyxml2::XMLElement* emissive = ambient->NextSiblingElement();
    materialData.emissive.x = emissive->FloatAttribute("R");
    materialData.emissive.y = emissive->FloatAttribute("G");
    materialData.emissive.z = emissive->FloatAttribute("B");
    materialData.emissive.w = emissive->FloatAttribute("A");

    materialData.shininess = property->FloatAttribute("Shininess");
    materialData.hasNormalMap = property->IntAttribute("HasNormalMap");

    delete document;
}

void Material::SelectMap(string name, MapType type)
{
    ImGui::SetWindowFontScale(2.0f);
    ImGui::TextColored(ImVec4(1, 0.5f, 0.8f, 1), name.c_str());
    ImGui::SetWindowFontScale(1.0f);
    ImGui::SameLine();

    ImTextureID textureID = nullptr;

    switch (type)
    {
    case Material::DIFFUSE:
        textureID = diffuseMap->GetSRV();
        break;
    case Material::SPECULAR:
        textureID = specularMap->GetSRV();
        break;
    case Material::NORMAL:
        textureID = normalMap->GetSRV();
        break;
    default:
        break;
    }

    if (ImGui::ImageButton(textureID, ImVec2(50, 50)))
    {
        DIALOG->OpenDialog(this->name + name, name, ".png,.jpg,.tga", ".");
    }

    if (DIALOG->Display(this->name + name))
    {
        if (DIALOG->IsOk())
        {
            string file = DIALOG->GetFilePathName();

            char path[128];
            GetCurrentDirectoryA(128, path);

            file = file.substr(strlen(path) + 1, file.length());

            switch (type)
            {
            case Material::DIFFUSE:
                SetDiffuseMap(ToWString(file));
                break;
            case Material::SPECULAR:
                SetSpecularMap(ToWString(file));
                break;
            case Material::NORMAL:
                SetNormalMap(ToWString(file));
                break;
            default:
                break;
            }
        }

        DIALOG->Close();
    }
}

void Material::UnselectMap(MapType type)
{
    ImTextureID textureID = nullptr;

    switch (type)
    {
    case Material::DIFFUSE:
        textureID = Texture::AddTexture()->GetSRV();
        //textureID = Texture::Add(L"Textures/UI/Cancel.png", L"DMCancel")->GetSRV();
        break;
    case Material::SPECULAR:
        textureID = Texture::AddTexture()->GetSRV();
        //textureID = Texture::Add(L"Textures/UI/Cancel.png", L"SMCancel")->GetSRV();
        break;
    case Material::NORMAL:
        textureID = Texture::AddTexture()->GetSRV();
        //textureID = Texture::Add(L"Textures/UI/Cancel.png", L"NMCancel")->GetSRV();
        break;
    default:
        break;
    }

    if (ImGui::ImageButton(textureID, ImVec2(20, 20)))
    {
        switch (type)
        {
        case Material::DIFFUSE:
            SetDiffuseMap(L"");
            break;
        case Material::SPECULAR:
            SetSpecularMap(L"");
            break;
        case Material::NORMAL:
            SetNormalMap(L"");
            break;
        default:
            break;
        }
    }
}

void Material::SaveDialog()
{
    string key = "Save";

    if (ImGui::Button("Save"))
    {
        if (file.empty())
            SaveMaterial("TextData/Materials/" + name + ".mat");
        else
            SaveMaterial(file);
    }

    ImGui::SameLine();

    if (ImGui::Button("SaveAs"))
        DIALOG->OpenDialog(key.c_str(), key.c_str(), ".mat", ".");

    if (DIALOG->Display(key.c_str()))
    {
        if (DIALOG->IsOk())
        {
            string file = DIALOG->GetFilePathName();

            file = file.substr(projectPath.size() + 1, file.size());

            SaveMaterial(file);
        }

        DIALOG->Close();
    }
}

void Material::LoadDialog()
{
    string key = "Load";

    if (ImGui::Button("Load"))
    {
        DIALOG->OpenDialog(key.c_str(), key.c_str(), ".mat", ".");
    }

    if (DIALOG->Display(key.c_str()))
    {
        if (DIALOG->IsOk())
        {
            string file = DIALOG->GetFilePathName();

            file = file.substr(projectPath.size() + 1, file.size());

            LoadMaterial(file);
        }

        DIALOG->Close();
    }
}
