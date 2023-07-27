
-- via: https://stackoverflow.com/a/74661335/
local function script_path()
    local str = debug.getinfo(2, "S").source:sub(2)
    return str:match("(.*[/\\])") or "./"
end

-- -----------------------------------------------------------------------------
-- Update modules search pathes
    -- Reference: https://stackoverflow.com/a/9145447/

-- Add 'lust' (relative) path into search pathes
package.path = script_path() .. '/third_party/lust-master/?.lua;' .. package.path

-- Add 'inspect' (relative) path into search pathes
package.path = script_path() .. '/third_party/inspect.lua-master/?.lua;' .. package.path

-- -----------------------------------------------------------------------------

local lust = require 'lust'
local describe, it, expect = lust.describe, lust.it, lust.expect

local inspect = require "inspect"


local t = require "libtaffy_cpp_lua"

-- -----------------------------------------------------------------------------

print(inspect(t))

describe('taffy_cpp lua binding', function()

    describe('Option_float', function()
        describe('Constructors', function()
            it('Default constructor', function()
                local opt = t.Option_float.new()

                expect( opt ).to.exist()
                expect( opt:is_some() ).to.be( false )
                expect( opt:get_value() ).to.be( nil )
            end)

            it('None constructor', function()
                local opt = t.Option_float.new(nil)

                expect( opt ).to.exist()
                expect( opt:is_some() ).to.be( false )
                expect( opt:get_value() ).to.be( nil )
            end)

            it('Some constructor', function()
                local opt = t.Option_float.new(42)

                expect( opt ).to.exist()
                expect( opt:is_some() ).to.be( true )
                expect( opt:get_value() ).to.be( 42 )
            end)
        end)

        describe('Copying', function()
            it('Copying none', function()
                local opt = t.Option_float.new()
                local copy = opt:copy()

                expect( copy ).to.exist()
                expect( copy:is_some() ).to.be( false )
                expect( copy:get_value() ).to.be( nil )
            end)

            it('Copying some', function()
                local opt = t.Option_float.new(42)
                local copy = opt:copy()

                expect( copy ).to.exist()
                expect( copy:is_some() ).to.be( true )
                expect( copy:get_value() ).to.be( 42 )
            end)
        end)

        it('Get/Set value', function()
           local opt = t.Option_float.new(10)

           expect( opt:get_value() ).to.be( 10 )

           opt:set_value(42)

           expect( opt:get_value() ).to.be( 42 )

           opt:set_value(nil)

           expect( opt:get_value() ).to.be( nil )
        end)
    end) -- Option_float

    describe('Point_of_float', function()
        describe('Constructors', function()
            it('common', function()
                local point = t.Point_of_float.new(10, 20)

                expect( point ).to.exist()
                expect( point:get_x() ).to.be( 10 )
                expect( point:get_y() ).to.be( 20 )
            end)

            it('table (array)', function()
                local point = t.Point_of_float.new({10, 20})

                expect( point ).to.exist()
                expect( point:get_x() ).to.be( 10 )
                expect( point:get_y() ).to.be( 20 )
            end)

            it('table (array) explicit indexes', function()
                local point1 = t.Point_of_float.new({[1] = 10, [2] = 20})

                expect( point1 ).to.exist()
                expect( point1:get_x() ).to.be( 10 )
                expect( point1:get_y() ).to.be( 20 )

                local point2 = t.Point_of_float.new({[2] = 20, [1] = 10})

                expect( point2 ).to.exist()
                expect( point2:get_x() ).to.be( 10 )
                expect( point2:get_y() ).to.be( 20 )
            end)

            it('table (dictionary)', function()
                local point1 = t.Point_of_float.new({x = 10, y = 20})

                expect( point1 ).to.exist()
                expect( point1:get_x() ).to.be( 10 )
                expect( point1:get_y() ).to.be( 20 )

                local point2 = t.Point_of_float.new({y = 20, x = 10})

                expect( point2 ).to.exist()
                expect( point2:get_x() ).to.be( 10 )
                expect( point2:get_y() ).to.be( 20 )
            end)

            it('table (without parentheses)', function()
                local point1 = t.Point_of_float.new {10, 20}

                expect( point1 ).to.exist()
                expect( point1:get_x() ).to.be( 10 )
                expect( point1:get_y() ).to.be( 20 )

                local point2 = t.Point_of_float.new {x = 10, y = 20}

                expect( point2 ).to.exist()
                expect( point2:get_x() ).to.be( 10 )
                expect( point2:get_y() ).to.be( 20 )
            end)

            it('Named constructors', function()
                local point = t.Point_of_float.ZERO()

                expect( point ).to.exist()
                expect( point:get_x() ).to.be( 0 )
                expect( point:get_y() ).to.be( 0 )
            end)
        end)

        it('Copying', function()
            local point = t.Point_of_float.new(10, 20)

            local copy = point:copy()

            expect( copy ).to.exist()
            expect( copy:get_x() ).to.be( 10 )
            expect( copy:get_y() ).to.be( 20 )
        end)

        it('Getters/Setters', function()
            local point = t.Point_of_float.new(10, 20)

            expect( point ).to.exist()
            expect( point:get_x() ).to.be( 10 )
            expect( point:get_y() ).to.be( 20 )

            point:set_x(35)
            point:set_y(42)

            expect( point:get_x() ).to.be( 35 )
            expect( point:get_y() ).to.be( 42 )
        end)

        it('Properties', function()
            local point = t.Point_of_float.new(10, 20)

            expect( point ).to.exist()
            expect( point.x ).to.be( 10 )
            expect( point.y ).to.be( 20 )

            point.x = 35
            point.y = 42

            expect( point.x ).to.be( 35 )
            expect( point.y ).to.be( 42 )
        end)
    end) -- Point_of_float

    describe('Size_of_float', function()
        describe('Constructors', function()
            it('common', function()
                local size = t.Size_of_float.new(10, 20)

                expect( size ).to.exist()
                expect( size:get_width()  ).to.be( 10 )
                expect( size:get_height() ).to.be( 20 )
            end)

            it('table (array)', function()
                local size = t.Size_of_float.new({10, 20})

                expect( size ).to.exist()
                expect( size:get_width()  ).to.be( 10 )
                expect( size:get_height() ).to.be( 20 )
            end)

            it('table (array) explicit indexes', function()
                local size1 = t.Size_of_float.new({[1] = 10, [2] = 20})

                expect( size1 ).to.exist()
                expect( size1:get_width()  ).to.be( 10 )
                expect( size1:get_height() ).to.be( 20 )

                local size2 = t.Size_of_float.new({[2] = 20, [1] = 10})

                expect( size2 ).to.exist()
                expect( size2:get_width()  ).to.be( 10 )
                expect( size2:get_height() ).to.be( 20 )
            end)

            it('table (dictionary)', function()
                local size1 = t.Size_of_float.new({width = 10, height = 20})

                expect( size1 ).to.exist()
                expect( size1:get_width()  ).to.be( 10 )
                expect( size1:get_height() ).to.be( 20 )

                local size2 = t.Size_of_float.new({height = 20, width = 10})

                expect( size2 ).to.exist()
                expect( size2:get_width()  ).to.be( 10 )
                expect( size2:get_height() ).to.be( 20 )
            end)

            it('table (without parentheses)', function()
                local size1 = t.Size_of_float.new {10, 20}

                expect( size1 ).to.exist()
                expect( size1:get_width()  ).to.be( 10 )
                expect( size1:get_height() ).to.be( 20 )

                local size2 = t.Size_of_float.new {width = 10, height = 20}

                expect( size2 ).to.exist()
                expect( size2:get_width()  ).to.be( 10 )
                expect( size2:get_height() ).to.be( 20 )
            end)

            it('Named constructors', function()
                local size = t.Size_of_float.ZERO()

                expect( size ).to.exist()
                expect( size:get_width()  ).to.be( 0 )
                expect( size:get_height() ).to.be( 0 )
            end)
        end)

        it('Copying', function()
            local size = t.Size_of_float.new(10, 20)

            local copy = size:copy()

            expect( copy ).to.exist()
            expect( copy:get_width()  ).to.be( 10 )
            expect( copy:get_height() ).to.be( 20 )
        end)

        it('Getters/Setters', function()
            local size = t.Size_of_float.new(10, 20)

            expect( size ).to.exist()
            expect( size:get_width()  ).to.be( 10 )
            expect( size:get_height() ).to.be( 20 )

            size:set_width(35)
            size:set_height(42)

            expect( size:get_width()  ).to.be( 35 )
            expect( size:get_height() ).to.be( 42 )
        end)

        it('Properties', function()
            local size = t.Size_of_float.new(10, 20)

            expect( size ).to.exist()
            expect( size.width  ).to.be( 10 )
            expect( size.height ).to.be( 20 )

            size.width  = 35
            size.height = 42

            expect( size.width  ).to.be( 35 )
            expect( size.height ).to.be( 42 )
        end)
    end) -- Size_of_float

    describe('AlignContent', function()
        it('Values', function()
            expect( t.AlignContent.Start        ).to.be(0)
            expect( t.AlignContent.End          ).to.be(1)
            expect( t.AlignContent.FlexStart    ).to.be(2)
            expect( t.AlignContent.FlexEnd      ).to.be(3)
            expect( t.AlignContent.Center       ).to.be(4)
            expect( t.AlignContent.Stretch      ).to.be(5)
            expect( t.AlignContent.SpaceBetween ).to.be(6)
            expect( t.AlignContent.SpaceEvenly  ).to.be(7)
            expect( t.AlignContent.SpaceAround  ).to.be(8)
        end)
    end) -- AlignContent

    describe('JustifyContent', function()
        it('Values', function()
            expect( t.JustifyContent.Start        ).to.be(0)
            expect( t.JustifyContent.End          ).to.be(1)
            expect( t.JustifyContent.FlexStart    ).to.be(2)
            expect( t.JustifyContent.FlexEnd      ).to.be(3)
            expect( t.JustifyContent.Center       ).to.be(4)
            expect( t.JustifyContent.Stretch      ).to.be(5)
            expect( t.JustifyContent.SpaceBetween ).to.be(6)
            expect( t.JustifyContent.SpaceEvenly  ).to.be(7)
            expect( t.JustifyContent.SpaceAround  ).to.be(8)
        end)
    end) -- JustifyContent

    describe('AlignItems', function()
        it('Values', function()
            expect( t.AlignItems.Start     ).to.be(0)
            expect( t.AlignItems.End       ).to.be(1)
            expect( t.AlignItems.FlexStart ).to.be(2)
            expect( t.AlignItems.FlexEnd   ).to.be(3)
            expect( t.AlignItems.Center    ).to.be(4)
            expect( t.AlignItems.Baseline  ).to.be(5)
            expect( t.AlignItems.Stretch   ).to.be(6)
        end)
    end) -- AlignItems

    describe('AlignSelf', function()
        it('Values', function()
            expect( t.AlignSelf.Start     ).to.be(0)
            expect( t.AlignSelf.End       ).to.be(1)
            expect( t.AlignSelf.FlexStart ).to.be(2)
            expect( t.AlignSelf.FlexEnd   ).to.be(3)
            expect( t.AlignSelf.Center    ).to.be(4)
            expect( t.AlignSelf.Baseline  ).to.be(5)
            expect( t.AlignSelf.Stretch   ).to.be(6)
        end)
    end) -- AlignSelf

end)


-- -----------------------------------------------------------------------------
-- TODO: TEMPORARLY UNCOVERED BY TESTS

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

local max_tsf = t.MaxTrackSizingFunction.Fixed( t.LengthPercentage.ZERO() )
max_tsf = t.MaxTrackSizingFunction.MinContent()
max_tsf = t.MaxTrackSizingFunction.MaxContent()
max_tsf = t.MaxTrackSizingFunction.FitContent( t.LengthPercentage.ZERO() )
max_tsf = t.MaxTrackSizingFunction.Auto()
max_tsf = t.MaxTrackSizingFunction.Fraction(42)
max_tsf = max_tsf:copy()
max_tsf = t.MaxTrackSizingFunction.AUTO()
max_tsf = t.MaxTrackSizingFunction.MIN_CONTENT()
max_tsf = t.MaxTrackSizingFunction.MAX_CONTENT()
max_tsf = t.MaxTrackSizingFunction.ZERO()
max_tsf = t.MaxTrackSizingFunction.fit_content( t.LengthPercentage.ZERO() )
max_tsf = t.MaxTrackSizingFunction.from_length(42)
max_tsf = t.MaxTrackSizingFunction.from_percent(42)
max_tsf = t.MaxTrackSizingFunction.from_flex(42)

local min_tsf = t.MinTrackSizingFunction.Fixed( t.LengthPercentage.ZERO() )
min_tsf = t.MinTrackSizingFunction.MinContent()
min_tsf = t.MinTrackSizingFunction.MaxContent()
min_tsf = t.MinTrackSizingFunction.Auto()
min_tsf = min_tsf:copy()
min_tsf = t.MinTrackSizingFunction.AUTO()
min_tsf = t.MinTrackSizingFunction.MIN_CONTENT()
min_tsf = t.MinTrackSizingFunction.MAX_CONTENT()
min_tsf = t.MinTrackSizingFunction.ZERO()
min_tsf = t.MinTrackSizingFunction.from_length(42)
min_tsf = t.MinTrackSizingFunction.from_percent(42)

local nrtsf = t.NonRepeatedTrackSizingFunction.new( t.MinTrackSizingFunction.Auto(), t.MaxTrackSizingFunction.Auto() )
nrtsf = t.NonRepeatedTrackSizingFunction.new({ t.MinTrackSizingFunction.Auto(), t.MaxTrackSizingFunction.Auto() })
nrtsf = t.NonRepeatedTrackSizingFunction.new({[1] = t.MinTrackSizingFunction.Auto(), [2] = t.MaxTrackSizingFunction.Auto() })
nrtsf = t.NonRepeatedTrackSizingFunction.new({[2] = t.MaxTrackSizingFunction.Auto(), [1] = t.MinTrackSizingFunction.Auto() })
nrtsf = t.NonRepeatedTrackSizingFunction.new { t.MinTrackSizingFunction.Auto(), t.MaxTrackSizingFunction.Auto() }
-- TODO: add other tests here

-- TODO: add taffy_TrackSizingFunction

