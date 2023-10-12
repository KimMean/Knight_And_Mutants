#include "Framework.h"
#include "TerrainEditorScene.h"

TerrainEditorScene::TerrainEditorScene()
{
	//terrain = new Terrain(Vector2(64, 64));
	terrainEditor = new TerrainEditor();
	terrainEditor->CreateTerrain(Vector2(64, 64));
	line = new LineCollider();
	skybox = new SkyBox();
}

TerrainEditorScene::~TerrainEditorScene()
{
	delete terrainEditor;
	delete line;
	delete skybox;
}

void TerrainEditorScene::Update()
{

	terrainEditor->Update();
    //if (KEY_PRESS(MK_LBUTTON))
    //{
    //    PickingTerrain();
    //}
	//
	//terrain->gameObject->GetTransform()->UpdateWorld();
	//line->GetTransform()->Pos() = CAM->GetTransform()->Pos();
	//line->SetDirection(CAM->ScreenPointToRayDirection(mousePos));
	line->SetLine(CAM->transform->GlobalPos() + CAM->transform->Forward(),
		CAM->ScreenPointToRay(mousePos).dir);
	//line->GetTransform()->Pos() += DELTA;
	//line->GetTransform()->UpdateWorld();
}

void TerrainEditorScene::PreRender()
{
	skybox->Render();
}

void TerrainEditorScene::Render()
{
	terrainEditor->Render();
	line->Render();
}

void TerrainEditorScene::PostRender()
{
}

void TerrainEditorScene::GUIRender()
{
	terrainEditor->GUIRender();
}

// 선택된 지형 확인
//Vector3 TerrainEditorScene::PickingTerrain()
//{
//    Ray ray = CAM->ScreenPointToRay(mousePos);
//
//    Vector2 size = terrain->GetTerrainSize();
//
//    for (UINT z = 0; z < size.y; z++)
//    {
//        for (UINT x = 0; x < size.x; x++)
//        {
//            UINT index[4];
//            index[0] = (size.x+1) * z + x;
//            index[1] = (size.x+1) * z + x + 1;
//            index[2] = (size.x+1) * (z + 1) + x;
//            index[3] = (size.x+1) * (z + 1) + x + 1;
//    
//            //vector<VertexUVNormalTangent>* vertices = (Mesh<VertexType>)terrain->GetMesh()->GetVertices();
//            //Mesh<VertexType>* mesh = terrain->GetMesh();
//            vector<VertexType>& vertices = terrain->GetMesh()->GetVertices();
//
//            // 사각형을 이루는 4개의 정점을 구함
//            Vector3 p[4];
//            for (UINT i = 0; i < 4; i++)
//                p[i] = vertices[index[i]].pos;
//            
//            // 3각형 안에 포함되는지를 검사함
//            float distance = 0.0f;
//            if (Intersects(ray.pos, ray.dir, p[0], p[1], p[2], distance))
//            {
//                vertices[index[0]].pos.y += 1.0f;
//                vertices[index[2]].pos.y += 1.0f;
//                vertices[index[1]].pos.y += 1.0f;
//                return ray.pos + ray.dir * distance;
//            }
//    
//            if (Intersects(ray.pos, ray.dir, p[3], p[1], p[2], distance))
//            {
//                vertices[index[1]].pos.y += 1.0f;
//                vertices[index[2]].pos.y += 1.0f;
//                vertices[index[3]].pos.y += 1.0f;
//                return ray.pos + ray.dir * distance;
//            }
//        }
//    }
//    terrain->GetMesh()->UpdateVertex();
//}
