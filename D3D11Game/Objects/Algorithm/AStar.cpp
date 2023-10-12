#include "Framework.h"

AStar::AStar(UINT width, UINT height)
    : width(width), height(height)
{
    heap = new Heap();
}

AStar::~AStar()
{
    for (Node* node : nodes)
        delete node;

    delete heap;
}

void AStar::Update()
{
    //if (KEY_DOWN('F'))
    //{
    //    for (Node* node : nodes)
    //    {
    //        if (node->transform->Pos().y <= 3.0f)
    //        {
    //            node->SetState(Node::OBSTACLE);
    //        }
    //    }
    //}
    //if (KEY_DOWN('F'))
    //{
    //    Ray ray = CAM->ScreenPointToRay(mousePos);
    //
    //    for (Node* node : nodes)
    //    {          
    //        if(node->IsRayCollision(ray, nullptr))
    //        {
    //            if(node->state == Node::NONE)
    //                node->SetState(Node::OBSTACLE);
    //            else if (node->state == Node::OBSTACLE)
    //                node->SetState(Node::NONE);
    //            break;
    //        }
    //    }
    //}
    //if (KEY_DOWN('K'))
    //    SaveNodeState();
    //if (KEY_DOWN('L'))
    //    LoadNodeState();
}

void AStar::Render()
{
    if (!Environment::Get()->isAStar) return;
    for (Node* node : nodes)
        node->Render();
}

void AStar::SetNode(Terrain* terrain)
{
    Vector2 size = terrain->GetTerrainSize();

    interval.x = size.x / width;
    interval.y = size.y / height;

    nodes.reserve((width + 1) * (height + 1));
    for (UINT z = 0; z <= height; z++)
    {
        for (UINT x = 0; x <= width; x++)
        {
            Vector3 pos = Vector3(x * interval.x, 0, z * interval.y);
            pos.y = terrain->GetHeight(pos);

            nodes.push_back(new Node(pos, nodes.size()));
            //nodes.back()->transform->Scale() = { interval.x, 1.0f, interval.y };
            nodes.back()->transform->Scale() = { 0.5f, 0.5f, 0.5f };
            nodes.back()->transform->UpdateWorld();

            //if (pos.y > 0.0f)
            //{
            //    nodes.back()->SetState(Node::OBSTACLE);
            //    AddObstacle(nodes.back());
            //}
        }
    }

    SetEdge();
}

int AStar::FindCloseNode(Vector3 pos)
{
    float minDist = FLT_MAX;

    int index = -1;

    for (UINT i = 0; i < nodes.size(); i++)
    {
        if (nodes[i]->state == Node::OBSTACLE)
            continue;

        float distance = Distance(pos, nodes[i]->transform->GlobalPos());

        if (minDist > distance)
        {
            minDist = distance;
            index = i;
        }
    }

    return index;
}

Vector3 AStar::FindRandomPos()
{
    float x = Random(5.0f, 60.0f);
    float z = Random(5.0f, 60.0f);

    int idx = FindCloseNode(Vector3(x, 0.0f, z));
    
    return nodes[idx]->transform->Pos();
}

int AStar::FindRandomPos(Vector3 pos, float range)
{
    vector<Node*> findNodes;

    for (Node* node : nodes)
    {
        float distance = Distance(pos, node->transform->GlobalPos());

        if (distance < range)
        {
            findNodes.push_back(node);
        }
    }

    Node* findNode = findNodes[Random(0, findNodes.size())];

    return findNode->index;
}

void AStar::GetPath(IN int start, IN int end, OUT vector<Vector3>& path)
{
    Reset();
    path.clear();

    //1. 시작노드 초기화하고 오픈노드에 추가
    float G = 0;
    float H = GetDiagonalManhattanDistance(start, end);

    nodes[start]->f = G + H;
    nodes[start]->g = G;
    nodes[start]->h = H;
    nodes[start]->via = start;
    nodes[start]->state = Node::OPEN;

    //openNodes.push_back(start);
    heap->Insert(nodes[start]);

    while (nodes[end]->state != Node::CLOSED)
    {
        //if (openNodes.empty())
        if(heap->Empty())
            return;

        //2. 오픈노드 중에서 효율이 가장 좋은 노드 찾기
        int curIndex = GetMinNode();
        //3. 찾은 노드와 연결된 노드의 정보 갱신후 오픈노드에 추가하고
        //확장이 끝난 노드는 닫기
        Extend(curIndex, end);
        nodes[curIndex]->state = Node::CLOSED;
    }

    //5. BackTracking
    int curIndex = end;

    while (curIndex != start)
    {
        nodes[curIndex]->state = Node::USING;
        path.push_back(nodes[curIndex]->transform->GlobalPos());
        curIndex = nodes[curIndex]->via;
    }

    //path.push_back(nodes[start]->pos);
}

void AStar::MakeDirectPath(IN Vector3 start, IN Vector3 end, OUT vector<Vector3>& path)
{
    int cutNodeNum = 0;
    Ray ray;
    ray.pos = start;

    for(UINT i = 0; i < path.size(); i++)
    {
        if (!IsCollisionObstacle(start, path[i]))
        {
            cutNodeNum = path.size() - i - 1;
            break;
        }
    }

    for (UINT i = 0; i < cutNodeNum; i++)
        path.pop_back();

    cutNodeNum = 0;
    ray.pos = end;

    for (UINT i = 0; i < path.size(); i++)
    {
        if (!IsCollisionObstacle(end, path[path.size() - i - 1]))
        {
            cutNodeNum = path.size() - i - 1;
            break;
        }
    }

    for (UINT i = 0; i < cutNodeNum; i++)
        path.erase(path.begin());
}

bool AStar::IsCollisionObstacle(Vector3 start, Vector3 end)
{
    Ray ray(start, (end - start).GetNormalized());
    float distance = Distance(start, end);

    Contact contact;

    for (Collider* obstacle : obstacles)
    {        
        if (obstacle->IsRayCollision(ray, &contact))
        {
            if (contact.distance < distance)
            {
                return true;
            }
        }
    }

    return false;
}

void AStar::AddObstacle(Collider* collider)
{
    obstacles.push_back(collider);
}

void AStar::Reset()
{
    for (Node* node : nodes)
    {
        if (node->state != Node::OBSTACLE)
            node->state = Node::NONE;
    }

    heap->Clear();
}

float AStar::GetDiagonalManhattanDistance(int start, int end)
{
    Vector3 startPos = nodes[start]->transform->GlobalPos();
    Vector3 endPos = nodes[end]->transform->GlobalPos();

    Vector3 temp = endPos - startPos;

    float x = abs(temp.x);
    float z = abs(temp.z);

    float maxSize = max(x, z);
    float minSize = min(x, z);

    return (maxSize - minSize) + sqrt(minSize * minSize * 2);
}

void AStar::Extend(int center, int end)
{
    for (Node::Edge* edge : nodes[center]->edges)
    {
        int index = edge->index;

        if (nodes[index]->state == Node::CLOSED)
            continue;
        if (nodes[index]->state == Node::OBSTACLE)
            continue;

        float G = nodes[center]->g + edge->cost;
        float H = GetDiagonalManhattanDistance(index, end);
        float F = G + H;

        if (nodes[index]->state == Node::OPEN)
        {
            if (F < nodes[index]->f)
            {
                nodes[index]->g = G;
                nodes[index]->f = F;
                nodes[index]->via = center;
            }
        }
        else if(nodes[index]->state == Node:: NONE)
        {
            nodes[index]->g = G;
            nodes[index]->h = H;
            nodes[index]->f = F;
            nodes[index]->via = center;
            nodes[index]->state = Node::OPEN;

            //openNodes.push_back(index);
            heap->Insert(nodes[index]);
        }
    }
}

int AStar::GetMinNode()
{
    return heap->DeleteRoot()->index;
}

void AStar::SetEdge()
{
    UINT width = this->width + 1;

    for (UINT i = 0; i < nodes.size(); i++)
    {
        if (i % width != width - 1)
        {
            nodes[i]->AddEdge(nodes[i + 1]);
            nodes[i + 1]->AddEdge(nodes[i]);
        }    

        if (i < nodes.size() - width)
        {
            nodes[i]->AddEdge(nodes[i + width]);
            nodes[i + width]->AddEdge(nodes[i]);
        }

        if (i < nodes.size() - width && i % width != width - 1)
        {
            nodes[i]->AddEdge(nodes[i + width + 1]);
            nodes[i + width + 1]->AddEdge(nodes[i]);
        }

        if (i < nodes.size() - width && i % width != 0)
        {
            nodes[i]->AddEdge(nodes[i + width - 1]);
            nodes[i + width - 1]->AddEdge(nodes[i]);
        }
    }
}

void AStar::SaveNodeState()
{
    BinaryWriter* writer = new BinaryWriter("TextData/Transforms/AStar.stat");

    for (UINT i = 0; i < nodes.size(); i++)
    {
        writer->UInt(i);
        writer->UInt(nodes[i]->state);
    }

    delete writer;
}

void AStar::LoadNodeState()
{

    BinaryReader* reader = new BinaryReader("TextData/Transforms/AStar.stat");

    if (reader->IsFailed()) return;

    for (UINT i = 0; i < nodes.size(); i++)
    {
        int idx = reader->UInt();
        nodes[idx]->state = (Node::State)reader->UInt();
    }

    delete reader;
}
