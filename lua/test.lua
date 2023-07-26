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

print('AlignContent.Start       : ' .. tostring(t.AlignContent.Start))
print('AlignContent.End         : ' .. tostring(t.AlignContent.End))
print('AlignContent.FlexStart   : ' .. tostring(t.AlignContent.FlexStart))
print('AlignContent.FlexEnd     : ' .. tostring(t.AlignContent.FlexEnd))
print('AlignContent.Center      : ' .. tostring(t.AlignContent.Center))
print('AlignContent.Stretch     : ' .. tostring(t.AlignContent.Stretch))
print('AlignContent.SpaceBetween: ' .. tostring(t.AlignContent.SpaceBetween))
print('AlignContent.SpaceEvenly : ' .. tostring(t.AlignContent.SpaceEvenly))
print('AlignContent.SpaceAround : ' .. tostring(t.AlignContent.SpaceAround))

print('JustifyContent.Start       : ' .. tostring(t.JustifyContent.Start))
print('JustifyContent.End         : ' .. tostring(t.JustifyContent.End))
print('JustifyContent.FlexStart   : ' .. tostring(t.JustifyContent.FlexStart))
print('JustifyContent.FlexEnd     : ' .. tostring(t.JustifyContent.FlexEnd))
print('JustifyContent.Center      : ' .. tostring(t.JustifyContent.Center))
print('JustifyContent.Stretch     : ' .. tostring(t.JustifyContent.Stretch))
print('JustifyContent.SpaceBetween: ' .. tostring(t.JustifyContent.SpaceBetween))
print('JustifyContent.SpaceEvenly : ' .. tostring(t.JustifyContent.SpaceEvenly))
print('JustifyContent.SpaceAround : ' .. tostring(t.JustifyContent.SpaceAround))

print('AlignItems.Start    : ' .. tostring(t.AlignItems.Start))
print('AlignItems.End      : ' .. tostring(t.AlignItems.End))
print('AlignItems.FlexStart: ' .. tostring(t.AlignItems.FlexStart))
print('AlignItems.FlexEnd  : ' .. tostring(t.AlignItems.FlexEnd))
print('AlignItems.Center   : ' .. tostring(t.AlignItems.Center))
print('AlignItems.Baseline : ' .. tostring(t.AlignItems.Baseline))
print('AlignItems.Stretch  : ' .. tostring(t.AlignItems.Stretch))

print('AlignSelf.Start    : ' .. tostring(t.AlignSelf.Start))
print('AlignSelf.End      : ' .. tostring(t.AlignSelf.End))
print('AlignSelf.FlexStart: ' .. tostring(t.AlignSelf.FlexStart))
print('AlignSelf.FlexEnd  : ' .. tostring(t.AlignSelf.FlexEnd))
print('AlignSelf.Center   : ' .. tostring(t.AlignSelf.Center))
print('AlignSelf.Baseline : ' .. tostring(t.AlignSelf.Baseline))
print('AlignSelf.Stretch  : ' .. tostring(t.AlignSelf.Stretch))

-- -----------------------------------------------------------------------------


local avail_space = t.AvailableSpace.Definite(42)
avail_space = t.AvailableSpace.MinContent(42)
avail_space = t.AvailableSpace.MaxContent(42)
avail_space = t.AvailableSpace.ZERO()
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

local lpa = t.LengthPercentageAuto.Length(42)
lpa = t.LengthPercentageAuto.Percent(42)
lpa = t.LengthPercentageAuto.Auto()
lpa = lpa:copy()
lpa = t.LengthPercentageAuto.from_length(42)
lpa = t.LengthPercentageAuto.from_percent(42)
lpa = t.LengthPercentageAuto.from( t.LengthPercentage.ZERO() )

local d = t.Dimension.Length(42)
d = t.Dimension.Percent(42)
d = t.Dimension.Auto()
d = d:copy()
d = t.Dimension.from_length(42)
d = t.Dimension.from_percent(42)
d = t.Dimension.from( t.LengthPercentage.ZERO() )
d = t.Dimension.from( t.LengthPercentageAuto.ZERO() )

print('FlexDirection.Row          : ' .. tostring(t.FlexDirection.Row))
print('FlexDirection.Column       : ' .. tostring(t.FlexDirection.Column))
print('FlexDirection.RowReverse   : ' .. tostring(t.FlexDirection.RowReverse))
print('FlexDirection.ColumnReverse: ' .. tostring(t.FlexDirection.ColumnReverse))
print('FlexDirection.Default      : ' .. tostring(t.FlexDirection.Default))

print('FlexWrap.NoWrap     : ' .. tostring(t.FlexWrap.NoWrap))
print('FlexWrap.Wrap       : ' .. tostring(t.FlexWrap.Wrap))
print('FlexWrap.WrapReverse: ' .. tostring(t.FlexWrap.WrapReverse))
print('FlexWrap.Default    : ' .. tostring(t.FlexWrap.Default))

print('GridAutoFlow.Row        : ' .. tostring(t.GridAutoFlow.Row))
print('GridAutoFlow.Column     : ' .. tostring(t.GridAutoFlow.Column))
print('GridAutoFlow.RowDense   : ' .. tostring(t.GridAutoFlow.RowDense))
print('GridAutoFlow.ColumnDense: ' .. tostring(t.GridAutoFlow.ColumnDense))
print('GridAutoFlow.Default    : ' .. tostring(t.GridAutoFlow.Default))

local gp = t.GridPlacement.new()
gp = t.GridPlacement.Auto()
gp = t.GridPlacement.Line(42)
gp = t.GridPlacement.Span(42)
gp = gp:copy()
gp = t.GridPlacement.AUTO()
gp = t.GridPlacement.from_line_index(42)
gp = t.GridPlacement.from_span(42)

local gtr = t.GridTrackRepetition.AutoFill()
gtr = t.GridTrackRepetition.AutoFit()
gtr = t.GridTrackRepetition.Count(42)
gtr = gtr:copy()
gtr = t.GridTrackRepetition.try_from(42)
gtr = t.GridTrackRepetition.try_from("auto-fill") -- TODO: check not null
gtr = t.GridTrackRepetition.try_from("auto-fit") -- TODO: check not null
gtr = t.GridTrackRepetition.try_from("sometext") -- TODO: check is null