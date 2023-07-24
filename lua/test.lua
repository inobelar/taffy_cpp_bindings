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