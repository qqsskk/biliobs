#pragma once
#include <vector>

//ö��ֱ����Դ
std::vector<OBSSource> OBSEnumSources();

//ö�ٳ�����
std::vector<OBSSceneItem> OBSEnumSceneItems(obs_scene_t* scene);

//ö�ٹ�����
std::vector<OBSSource> OBSEnumFilters(obs_source_t* source);

