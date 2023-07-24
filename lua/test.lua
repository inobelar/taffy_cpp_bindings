local t = require "libtaffy_cpp_lua"

local inspect = require "inspect"

print(inspect(t))

local opt = t.Option_float.new(3)
local opt_copy = opt:copy()
print(opt:is_some())
print(opt_copy:is_some())
opt:set_value(nil)
opt:set_value(42)

local p = t.Point_of_float.new(10, 20)
print(p:get_x())
print(p:get_y())
p = t.Point_of_float.ZERO();
print(p:get_x() .. ' ' .. p:get_y())