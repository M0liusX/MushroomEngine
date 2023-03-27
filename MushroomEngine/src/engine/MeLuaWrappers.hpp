#pragma once

#include "../MeAppBase.hpp"  // ???

#include <lua.hpp>
#include <iostream>

static int wrap_showPhysics(lua_State* L)
{
   std::cout << "C++: Show Physics" << std::endl;
   if (lua_gettop(L) != 1) return -1;
   me::MeAppBase* app = static_cast<me::MeAppBase*>(lua_touserdata(L, 1));
   app->setShowPhysics();
   return 0;
}

static int wrap_loadGameObject(lua_State* L)
{
   //std::cout << "C++: load game object" << std::endl;
   if (lua_gettop(L) != 1) return -1;
   me::MeAppBase* app = static_cast<me::MeAppBase*>(lua_touserdata(L, 1));
   int objectId = app->loadGameObject();
   lua_pushinteger(L, objectId);
   return 1;
}

static int wrap_setModel(lua_State* L)
{
   //std::cout << "C++: set model" << std::endl;
   if (lua_gettop(L) != 3) return -1;
   me::MeAppBase* app = static_cast<me::MeAppBase*>(lua_touserdata(L, 1));
   int objectId = lua_tointeger(L, 2);
   const char* model = lua_tostring(L, 3);
   app->setModel(objectId, model);
   return 0;
}


static int wrap_setSphereCollider(lua_State* L)
{
   //std::cout << "C++: set model" << std::endl;
   if (lua_gettop(L) != 4) return -1;
   me::MeAppBase* app = static_cast<me::MeAppBase*>(lua_touserdata(L, 1));
   int objectId = lua_tointeger(L, 2);
   float radius = lua_tonumber(L, 3);

   glm::vec3 offset;

   lua_pushstring(L, "x");
   lua_gettable(L, 4);
   offset.x = lua_tonumber(L, 5);
   lua_pop(L, 1);

   lua_pushstring(L, "y");
   lua_gettable(L, 4);
   offset.y = lua_tonumber(L, 5);
   lua_pop(L, 1);

   lua_pushstring(L, "z");
   lua_gettable(L, 4);
   offset.z = lua_tonumber(L, 5);
   lua_pop(L, 1);
   app->setSphereCollider(objectId, radius, offset);
   return 0;
}

static int wrap_setCollider(lua_State* L)
{
   //std::cout << "C++: set model" << std::endl;
   if (lua_gettop(L) != 3) return -1;
   me::MeAppBase* app = static_cast<me::MeAppBase*>(lua_touserdata(L, 1));
   int objectId = lua_tointeger(L, 2);
   int colliderId = lua_tointeger(L, 3);
   app->setCollider(objectId, colliderId);
   return 0;
}


static int wrap_setBehavior(lua_State* L)
{
   //std::cout << "C++: set model" << std::endl;
   if (lua_gettop(L) != 3) return -1;
   me::MeAppBase* app = static_cast<me::MeAppBase*>(lua_touserdata(L, 1));
   int objectId = lua_tointeger(L, 2);
   int behaviorId = lua_tointeger(L, 3);
   app->setBehavior(objectId, behaviorId);
   return 0;
}


static int wrap_setPosition(lua_State* L)
{
   //std::cout << "C++: set position" << std::endl;
   if (lua_gettop(L) != 3) return -1;
   me::MeAppBase* app = static_cast<me::MeAppBase*>(lua_touserdata(L, 1));
   int objectId = lua_tointeger(L, 2);
   if (!lua_istable(L, 3)) {
      return -1;
   }
   glm::vec3 position;
   
   lua_pushstring(L, "x");
   lua_gettable(L, 3);
   position.x = lua_tonumber(L, 4);
   lua_pop(L, 1);

   lua_pushstring(L, "y");
   lua_gettable(L, 3);
   position.y = lua_tonumber(L, 4);
   lua_pop(L, 1);

   lua_pushstring(L, "z");
   lua_gettable(L, 3);
   position.z = lua_tonumber(L, 4);
   lua_pop(L, 1);

   app->setPosition(objectId, position);
   return 0;
}


static int wrap_setScale(lua_State* L)
{
   //std::cout << "C++: set scale" << std::endl;
   if (lua_gettop(L) != 3) return -1;
   me::MeAppBase* app = static_cast<me::MeAppBase*>(lua_touserdata(L, 1));
   int objectId = lua_tointeger(L, 2);
   if (!lua_istable(L, 3)) {
      return -1;
   }
   glm::vec3 scale;

   lua_pushstring(L, "x");
   lua_gettable(L, 3);
   scale.x = lua_tonumber(L, 4);
   lua_pop(L, 1);

   lua_pushstring(L, "y");
   lua_gettable(L, 3);
   scale.y = lua_tonumber(L, 4);
   lua_pop(L, 1);

   lua_pushstring(L, "z");
   lua_gettable(L, 3);
   scale.z = lua_tonumber(L, 4);
   lua_pop(L, 1);

   app->setScale(objectId, scale);
   return 0;
}

static void RegisterWrappers(lua_State* L) {
   lua_register(L, "_ShowPhysics", wrap_showPhysics);
   lua_register(L, "_LoadGameObject", wrap_loadGameObject);
   lua_register(L, "_SetModel", wrap_setModel);
   lua_register(L, "_SetSphereCollider", wrap_setSphereCollider);
   lua_register(L, "_SetCollider", wrap_setCollider);
   lua_register(L, "_SetBehavior", wrap_setBehavior);
   lua_register(L, "_SetPosition", wrap_setPosition);
   lua_register(L, "_SetScale", wrap_setScale);
}
