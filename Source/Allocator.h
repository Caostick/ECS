#pragma once

#ifdef _DEBUG
#define ECSNew new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define ECSDelete delete
#else
#define ECSNew new
#define ECSDelete delete
#endif