#include "Framework.h"
#include "ModelExportScene.h"

ModelExportScene::ModelExportScene()
{
    name = "IceAxe";
    string file = "Models/FBX/" + name + ".fbx";
    string clipName;

    exporter = new ModelExporter();
    exporter->ReadFile(name, file);
    exporter->ExportMaterial();
    exporter->ExportMesh();
 
    //ReadClip("Idle");
    //ReadClip("Walk");
    //ReadClip("Attack");
    //ReadClip("Roar");
    //ReadClip("Death");

    delete exporter;
}

ModelExportScene::~ModelExportScene()
{
}

void ModelExportScene::Update()
{
}

void ModelExportScene::PreRender()
{
}

void ModelExportScene::Render()
{
}

void ModelExportScene::PostRender()
{
}

void ModelExportScene::GUIRender()
{
}

void ModelExportScene::ReadClip(string clipName)
{
    //string clipName = clipName;
    string file = "Models/Animations/" + name + "/" + clipName + ".fbx";
    exporter->ReadFile(name, file);
    exporter->ExportClip(clipName);
}
