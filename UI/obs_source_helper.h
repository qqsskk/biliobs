#pragma once
#include <vector>

//枚举直播来源
std::vector<OBSSource> OBSEnumSources();

//枚举场景项
std::vector<OBSSceneItem> OBSEnumSceneItems(obs_scene_t* scene);

//枚举过滤器
std::vector<OBSSource> OBSEnumFilters(obs_source_t* source);

