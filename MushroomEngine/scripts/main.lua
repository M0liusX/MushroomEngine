print("Lua: Game Start!")

-- COLLISION SHAPES
SPHERE = 0
PLANE = 1
CUBE = 2

-- BEHAVIORS
KINEMATIC = 0
CHARACTER = 1

-- _ShowPhysics(App)

object = _LoadGameObject(App)
_SetModel(App, object, "models/uvcube.obj")
_SetCollider(App, object, SPHERE)
_SetPosition(App, object, {x= 2, y= -5, z= 2.5})
_SetScale(App, object, {x= .3, y= .3, z= .3})
_SetBehavior(App, object, CHARACTER)


cube = _LoadGameObject(App)
_SetModel(App, cube, "models/uvcube.obj")
_SetCollider(App, cube, CUBE)
_SetPosition(App, cube, {x=0, y=.5, z=2.5})
_SetScale(App, cube, {x= 5, y=1, z= 5})

for i=0,100 do
    offset = math.random(-2, 2) * math.random()
    cube = _LoadGameObject(App)
    _SetModel(App, cube, "models/uvcube.obj")
    _SetCollider(App, cube, CUBE)
    _SetPosition(App, cube, {x=offset, y=.5, z=6.5 + i * 2.5})
    _SetScale(App, cube, {x= 2, y=1, z= 2})
end

--[[
floor = _LoadGameObject(App)
_SetModel(App, floor, "models/quad.obj")
_SetCollider(App, floor, PLANE)
_SetPosition(App, floor, {x= 0, y= 0, z= 2.5})
_SetScale(App, floor, {x= 5, y= 5, z= 5})

for i=0,100 do
    cube = _LoadGameObject(App)
    _SetModel(App, cube, "models/cube.obj")
    _SetCollider(App, cube, CUBE)
    _SetPosition(App, cube, {x= -2 + .75 * i, y= -.15 + -.4     * i, z= 4})
    _SetScale(App, cube, {x= .3, y= .3, z= .3})
end

for i=1,10 do
    for j=1,10 do
        vase = _LoadGameObject(App)
        _SetModel(App, vase, "models/smooth_vase.obj")
        _SetSphereCollider(App, vase, 0.22, {x= 0, y=-.06, z= 0})
        _SetBehavior(App, vase, KINEMATIC)
        _SetPosition(App, vase, {x= i*.2 - 1, y= -2, z=  j*.2 + 1.5})
        _SetScale(App, vase, {x= .3, y= .3, z= .3})
    end
end

vase = _LoadGameObject(App)
_SetModel(App, vase, "models/smooth_vase.obj")
_SetSphereCollider(App, vase, 0.22, {x= 0, y=-.06, z= 0})
_SetBehavior(App, vase, KINEMATIC)
_SetPosition(App, vase, {x= 0, y= -2, z= 2.5})
_SetScale(App, vase, {x= .3, y= .3, z= .3})


cube = _LoadGameObject(App)
_SetModel(App, cube, "models/colored_cube.obj")
_SetSphereCollider(App, cube, 1.05, {x= 0, y=0, z= 0})
_SetBehavior(App, cube, KINEMATIC)
_SetPosition(App, cube, {x= 0, y= -2, z= 4})
_SetScale(App, cube, {x= .3, y= .3, z= .3})
]]