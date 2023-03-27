#pragma once

#include <lua.hpp>

#include <iostream>
#include <string>

namespace me {

	class MeScript {
	public:
		MeScript(void* app);
		~MeScript();

		MeScript(const MeScript&) = delete;
		MeScript& operator=(const MeScript&) = delete;

		void InitScript(const std::string& filepath);
	private:
		bool CheckLua(int r);
		lua_State* luaState;
	};

}
