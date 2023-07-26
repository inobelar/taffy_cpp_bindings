local t = require "libtaffy_cpp_lua"

local inspect = require "inspect"

print(inspect(t))

local opt = t.Option_float.new(3)
local opt_copy = opt:copy()
print(opt:is_some())
print(opt_copy:is_some())
opt:set_value(nil)
opt:set_value(42)

-- -----------------------------------------------------------------------------

local p = t.Point_of_float.new(10, 20)
print(p:get_x())
print(p:get_y())
p = t.Point_of_float.ZERO();
print("x: " .. p:get_x() .. ", y: " .. p:get_y())

p.x = 42
p.y = 32
print("x: " .. tostring(p.x) .. ", y: " .. tostring(p.y) )

p = t.Point_of_float.new({10, 20})
print("test table-array x: " .. tostring(p.x) .. ", y: " .. tostring(p.y) )

p = t.Point_of_float.new({x = 10, y = 20})
print("test table-dict x: " .. tostring(p.x) .. ", y: " .. tostring(p.y) )

p = t.Point_of_float.new({y = 20, x = 10})
print("test table-dict x: " .. tostring(p.x) .. ", y: " .. tostring(p.y) )


p = t.Point_of_float.new {10, 20}
print("test 2 table-array x: " .. tostring(p.x) .. ", y: " .. tostring(p.y) )

p = t.Point_of_float.new {x = 10, y = 20}
print("test 2 table-dict x: " .. tostring(p.x) .. ", y: " .. tostring(p.y) )


p = t.Point_of_float.new {[1] = 10, [2] = 20}
print("test 3 table-array x: " .. tostring(p.x) .. ", y: " .. tostring(p.y) )

-- -----------------------------------------------------------------------------

local s = t.Size_of_float.new(10, 20)
print(s:get_width())
print(s:get_height())
s = t.Size_of_float.ZERO();
print("width: " .. s:get_width() .. ", height: " .. s:get_height())

s.width  = 42
s.height = 32
print("width: " .. tostring(s.width) .. ", height: " .. tostring(s.height) )

s = t.Size_of_float.new({10, 20})
print("test table-array width: " .. tostring(s.width) .. ", height: " .. tostring(s.height) )

s = t.Size_of_float.new({width = 10, height = 20})
print("test table-dict width: " .. tostring(s.width) .. ", height: " .. tostring(s.height) )

s = t.Size_of_float.new({height = 20, width = 10})
print("test table-dict width: " .. tostring(s.width) .. ", height: " .. tostring(s.height) )


s = t.Size_of_float.new {10, 20}
print("test 2 table-array width: " .. tostring(s.width) .. ", height: " .. tostring(s.height) )

s = t.Size_of_float.new {width = 10, height = 20}
print("test 2 table-dict width: " .. tostring(s.width) .. ", height: " .. tostring(s.height) )


s = t.Size_of_float.new {[1] = 10, [2] = 20}
print("test 3 table-array width: " .. tostring(s.width) .. ", height: " .. tostring(s.height) )

-- -----------------------------------------------------------------------------


local avail_space = t.AvailableSpace.Definite(42)
avail_space = t.AvailableSpace.MinContent(42)
avail_space = t.AvailableSpace.MaxContent(42)
avail_space = t.AvailableSpace.ZERO(42)
avail_space = t.AvailableSpace.from_length(42)
avail_space = t.AvailableSpace.from(42)
avail_space = t.AvailableSpace.from(t.Option_float.new())
avail_space = t.AvailableSpace.from(t.Option_float.new(nil))
avail_space = t.AvailableSpace.from(t.Option_float.new(42))

local lp = t.LengthPercentage.Length(42)
lp = t.LengthPercentage.Percent(42)
lp = lp:copy()
lp = t.LengthPercentage.from_length(42)
lp = t.LengthPercentage.from_percent(42)