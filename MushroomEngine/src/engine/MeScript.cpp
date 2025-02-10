#include "MeScript.hpp"
#include "MeLuaWrappers.hpp"

namespace me {


MeScript::MeScript(void* app)
{
	luaState = luaL_newstate();
	luaL_openlibs(luaState);
	RegisterWrappers(luaState);
	lua_pushlightuserdata(luaState, app);
	lua_setglobal(luaState, "App");
}


MeScript::~MeScript()
{
	lua_close(luaState);
}
 

bool MeScript::CheckLua(int r)
{
	if (r != LUA_OK) {
		std::string errormsg = lua_tostring(luaState, -1);
		std::cout << errormsg << std::endl;
		return false;
	}
	return true;
}


void MeScript::InitScript(const std::string& filepath)
{
	CheckLua(luaL_dofile(luaState, filepath.c_str()));
}

}

//lua_getglobal(L, "a");
//if (lua_isnumber(L, -1))
//{
//	float a_in_cpp = (float)lua_tonumber(L, -1);
//	std::cout << "a_in_cpp = " << a_in_cpp << std::endl;
//}