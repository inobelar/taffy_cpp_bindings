
-- via: https://stackoverflow.com/a/74661335/
local function script_path()
    local str = debug.getinfo(2, "S").source:sub(2)
    return str:match("(.*[/\\])") or "./"
end

-- -----------------------------------------------------------------------------
-- Update modules search pathes
    -- Reference: https://stackoverflow.com/a/9145447/

-- Add 'lust' (relative) path into search pathes
package.path = (script_path() .. '/third_party/lust-master/?.lua;') .. package.path

-- Add 'inspect' (relative) path into search pathes
package.path = (script_path() .. '/third_party/inspect.lua-master/?.lua;') .. package.path

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

        describe('Operators', function()
            it('Comparison', function()
                local opts_equal1 = t.Option_float.new(42) == t.Option_float.new(42)
                local opts_equal2 = t.Option_float.new(nil) == t.Option_float.new(nil)
                local opts_equal3 = t.Option_float.new() == t.Option_float.new(nil)

                local opts_not_equal1 = t.Option_float.new(42) ~= t.Option_float.new(10)
                local opts_not_equal2 = t.Option_float.new(42) ~= t.Option_float.new(nil)

                expect( opts_equal1 ).to.be( true )
                expect( opts_equal2 ).to.be( true )
                expect( opts_equal3 ).to.be( true )

                expect( opts_not_equal1 ).to.be( true )
                expect( opts_not_equal2 ).to.be( true )
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

        describe('Operators', function()
            it('Comparison', function()
                local point = t.Point_of_float.new(10, 20)

                local points_equal     = point == t.Point_of_float.new(10, 20)
                local points_not_equal = point ~= t.Point_of_float.new(20, 10)

                expect( points_equal     ).to.be( true )
                expect( points_not_equal ).to.be( true )
            end)

            describe('Arithmetic', function()
                it('Addition', function()
                    local point = t.Point_of_float.new(10, 20)

                    local other = t.Point_of_float.new(2, 3)

                    local result = point + other

                    expect( result:get_x() ).to.be( 12 )
                    expect( result:get_y() ).to.be( 23 )
                end)
            end)
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

        describe('Operators', function()
            it('Comparison', function()
                local size = t.Size_of_float.new(10, 20)

                local sizes_equal     = size == t.Size_of_float.new(10, 20)
                local sizes_not_equal = size ~= t.Size_of_float.new(20, 10)

                expect( sizes_equal     ).to.be( true )
                expect( sizes_not_equal ).to.be( true )
            end)

            describe('Arithmetic', function()
                it('Addition', function()
                    local size = t.Size_of_float.new(10, 20)

                    local other = t.Size_of_float.new(2, 3)

                    local result = size + other

                    expect( result:get_width()  ).to.be( 12 )
                    expect( result:get_height() ).to.be( 23 )
                end)

                it('Substraction', function()
                    local size = t.Size_of_float.new(10, 20)

                    local other = t.Size_of_float.new(2, 3)

                    local result = size - other

                    expect( result:get_width()  ).to.be( 8 )
                    expect( result:get_height() ).to.be( 17 )
                end)
            end)
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
            expect( t.AlignContent.Start        ).to.exist()
            expect( t.AlignContent.End          ).to.exist()
            expect( t.AlignContent.FlexStart    ).to.exist()
            expect( t.AlignContent.FlexEnd      ).to.exist()
            expect( t.AlignContent.Center       ).to.exist()
            expect( t.AlignContent.Stretch      ).to.exist()
            expect( t.AlignContent.SpaceBetween ).to.exist()
            expect( t.AlignContent.SpaceEvenly  ).to.exist()
            expect( t.AlignContent.SpaceAround  ).to.exist()

            expect( t.AlignContent.Start        :to_int() ).to.be( 0 )
            expect( t.AlignContent.End          :to_int() ).to.be( 1 )
            expect( t.AlignContent.FlexStart    :to_int() ).to.be( 2 )
            expect( t.AlignContent.FlexEnd      :to_int() ).to.be( 3 )
            expect( t.AlignContent.Center       :to_int() ).to.be( 4 )
            expect( t.AlignContent.Stretch      :to_int() ).to.be( 5 )
            expect( t.AlignContent.SpaceBetween :to_int() ).to.be( 6 )
            expect( t.AlignContent.SpaceEvenly  :to_int() ).to.be( 7 )
            expect( t.AlignContent.SpaceAround  :to_int() ).to.be( 8 )

            expect( t.AlignContent.from_int(0) == t.AlignContent.Start        ).to.be( true )
            expect( t.AlignContent.from_int(1) == t.AlignContent.End          ).to.be( true )
            expect( t.AlignContent.from_int(2) == t.AlignContent.FlexStart    ).to.be( true )
            expect( t.AlignContent.from_int(3) == t.AlignContent.FlexEnd      ).to.be( true )
            expect( t.AlignContent.from_int(4) == t.AlignContent.Center       ).to.be( true )
            expect( t.AlignContent.from_int(5) == t.AlignContent.Stretch      ).to.be( true )
            expect( t.AlignContent.from_int(6) == t.AlignContent.SpaceBetween ).to.be( true )
            expect( t.AlignContent.from_int(7) == t.AlignContent.SpaceEvenly  ).to.be( true )
            expect( t.AlignContent.from_int(8) == t.AlignContent.SpaceAround  ).to.be( true )
        end)

        it('Strictness check', function()
            expect( t.AlignContent.Wrong ).to.be(nil)

            expect( function() t.AlignContent.Wrong = 42; return true end ).to.fail()

            expect( function() t.AlignContent.Start = 42; return true end ).to.fail()

            expect( t.AlignContent.Start.x ).to.be(nil)

            expect( function() t.AlignContent.Start.x = 42; return true end ).to.fail()
        end)
    end) -- AlignContent

    describe('JustifyContent', function()
        it('Values', function()
            expect( t.JustifyContent.Start        ).to.exist()
            expect( t.JustifyContent.End          ).to.exist()
            expect( t.JustifyContent.FlexStart    ).to.exist()
            expect( t.JustifyContent.FlexEnd      ).to.exist()
            expect( t.JustifyContent.Center       ).to.exist()
            expect( t.JustifyContent.Stretch      ).to.exist()
            expect( t.JustifyContent.SpaceBetween ).to.exist()
            expect( t.JustifyContent.SpaceEvenly  ).to.exist()
            expect( t.JustifyContent.SpaceAround  ).to.exist()

            expect( t.JustifyContent.Start        :to_int() ).to.be( 0 )
            expect( t.JustifyContent.End          :to_int() ).to.be( 1 )
            expect( t.JustifyContent.FlexStart    :to_int() ).to.be( 2 )
            expect( t.JustifyContent.FlexEnd      :to_int() ).to.be( 3 )
            expect( t.JustifyContent.Center       :to_int() ).to.be( 4 )
            expect( t.JustifyContent.Stretch      :to_int() ).to.be( 5 )
            expect( t.JustifyContent.SpaceBetween :to_int() ).to.be( 6 )
            expect( t.JustifyContent.SpaceEvenly  :to_int() ).to.be( 7 )
            expect( t.JustifyContent.SpaceAround  :to_int() ).to.be( 8 )

            expect( t.JustifyContent.from_int(0) == t.JustifyContent.Start        ).to.be( true )
            expect( t.JustifyContent.from_int(1) == t.JustifyContent.End          ).to.be( true )
            expect( t.JustifyContent.from_int(2) == t.JustifyContent.FlexStart    ).to.be( true )
            expect( t.JustifyContent.from_int(3) == t.JustifyContent.FlexEnd      ).to.be( true )
            expect( t.JustifyContent.from_int(4) == t.JustifyContent.Center       ).to.be( true )
            expect( t.JustifyContent.from_int(5) == t.JustifyContent.Stretch      ).to.be( true )
            expect( t.JustifyContent.from_int(6) == t.JustifyContent.SpaceBetween ).to.be( true )
            expect( t.JustifyContent.from_int(7) == t.JustifyContent.SpaceEvenly  ).to.be( true )
            expect( t.JustifyContent.from_int(8) == t.JustifyContent.SpaceAround  ).to.be( true )
        end)

        it('Strictness check', function()
            expect( t.JustifyContent.Wrong ).to.be(nil)

            expect( function() t.JustifyContent.Wrong = 42; return true end ).to.fail()

            expect( function() t.JustifyContent.Start = 42; return true end ).to.fail()

            expect( t.JustifyContent.Start.x ).to.be(nil)

            expect( function() t.JustifyContent.Start.x = 42; return true end ).to.fail()

            -- Additional check - JustifyContent is not the same as AlighContent (even if similar to it)
            expect( function() return t.AlignContent.Start == t.JustifyContent.Start end ).to.fail()
        end)
    end) -- JustifyContent

    describe('AlignItems', function()
        it('Values', function()
            expect( t.AlignItems.Start     ).to.exist()
            expect( t.AlignItems.End       ).to.exist()
            expect( t.AlignItems.FlexStart ).to.exist()
            expect( t.AlignItems.FlexEnd   ).to.exist()
            expect( t.AlignItems.Center    ).to.exist()
            expect( t.AlignItems.Baseline  ).to.exist()
            expect( t.AlignItems.Stretch   ).to.exist()

            expect( t.AlignItems.Start     :to_int() ).to.be( 0 )
            expect( t.AlignItems.End       :to_int() ).to.be( 1 )
            expect( t.AlignItems.FlexStart :to_int() ).to.be( 2 )
            expect( t.AlignItems.FlexEnd   :to_int() ).to.be( 3 )
            expect( t.AlignItems.Center    :to_int() ).to.be( 4 )
            expect( t.AlignItems.Baseline  :to_int() ).to.be( 5 )
            expect( t.AlignItems.Stretch   :to_int() ).to.be( 6 )

            expect( t.AlignItems.from_int(0) == t.AlignItems.Start     ).to.be( true )
            expect( t.AlignItems.from_int(1) == t.AlignItems.End       ).to.be( true )
            expect( t.AlignItems.from_int(2) == t.AlignItems.FlexStart ).to.be( true )
            expect( t.AlignItems.from_int(3) == t.AlignItems.FlexEnd   ).to.be( true )
            expect( t.AlignItems.from_int(4) == t.AlignItems.Center    ).to.be( true )
            expect( t.AlignItems.from_int(5) == t.AlignItems.Baseline  ).to.be( true )
            expect( t.AlignItems.from_int(6) == t.AlignItems.Stretch   ).to.be( true )
        end)

        it('Strictness check', function()
            expect( t.AlignItems.Wrong ).to.be(nil)

            expect( function() t.AlignItems.Wrong = 42; return true end ).to.fail()

            expect( function() t.AlignItems.Start = 42; return true end ).to.fail()

            expect( t.AlignItems.Start.x ).to.be(nil)

            expect( function() t.AlignItems.Start.x = 42; return true end ).to.fail()
        end)
    end) -- AlignItems

    describe('AlignSelf', function()
        it('Values', function()
            expect( t.AlignSelf.Start     ).to.exist()
            expect( t.AlignSelf.End       ).to.exist()
            expect( t.AlignSelf.FlexStart ).to.exist()
            expect( t.AlignSelf.FlexEnd   ).to.exist()
            expect( t.AlignSelf.Center    ).to.exist()
            expect( t.AlignSelf.Baseline  ).to.exist()
            expect( t.AlignSelf.Stretch   ).to.exist()

            expect( t.AlignSelf.Start     :to_int() ).to.be( 0 )
            expect( t.AlignSelf.End       :to_int() ).to.be( 1 )
            expect( t.AlignSelf.FlexStart :to_int() ).to.be( 2 )
            expect( t.AlignSelf.FlexEnd   :to_int() ).to.be( 3 )
            expect( t.AlignSelf.Center    :to_int() ).to.be( 4 )
            expect( t.AlignSelf.Baseline  :to_int() ).to.be( 5 )
            expect( t.AlignSelf.Stretch   :to_int() ).to.be( 6 )

            expect( t.AlignSelf.from_int(0) == t.AlignSelf.Start     ).to.be( true )
            expect( t.AlignSelf.from_int(1) == t.AlignSelf.End       ).to.be( true )
            expect( t.AlignSelf.from_int(2) == t.AlignSelf.FlexStart ).to.be( true )
            expect( t.AlignSelf.from_int(3) == t.AlignSelf.FlexEnd   ).to.be( true )
            expect( t.AlignSelf.from_int(4) == t.AlignSelf.Center    ).to.be( true )
            expect( t.AlignSelf.from_int(5) == t.AlignSelf.Baseline  ).to.be( true )
            expect( t.AlignSelf.from_int(6) == t.AlignSelf.Stretch   ).to.be( true )
        end)

        it('Strictness check', function()
            expect( t.AlignSelf.Wrong ).to.be(nil)

            expect( function() t.AlignSelf.Wrong = 42; return true end ).to.fail()

            expect( function() t.AlignSelf.Start = 42; return true end ).to.fail()

            expect( t.AlignSelf.Start.x ).to.be(nil)

            expect( function() t.AlignSelf.Start.x = 42; return true end ).to.fail()

            -- Additional check - AlignSelf is not the same as AlignItems (even if similar to it)
            expect( function() return t.AlignSelf.Start == t.AlignItems.Start end ).to.fail()
        end)
    end) -- AlignSelf

    describe('AvailableSpace', function()
        describe('Constructors', function()
            it('Definite', function()
                local as = t.AvailableSpace.Definite(42)

                expect( as ).to.exist()
                expect( as:get_value() ).to.be(42)
            end)

            it('MinContent', function()
                local as = t.AvailableSpace.MinContent()

                expect( as ).to.exist()
                expect( as:get_value() ).to.be(nil)
            end)

            it('MaxContent', function()
                local as = t.AvailableSpace.MaxContent()

                expect( as ).to.exist()
                expect( as:get_value() ).to.be(nil)
            end)

            it('Named constructors', function()
                local as1 = t.AvailableSpace.ZERO()

                expect( as1 ).to.exist()
                expect( as1:is_Definite() ).to.be(true)
                expect( as1:get_value() ).to.be(0)

                local as2 = t.AvailableSpace.MIN_CONTENT()

                expect( as2 ).to.exist()
                expect( as2:is_MinContent() ).to.be(true)
                expect( as2:get_value() ).to.be(nil)

                local as3 = t.AvailableSpace.MAX_CONTENT()

                expect( as3 ).to.exist()
                expect( as3:is_MaxContent() ).to.be(true)
                expect( as3:get_value() ).to.be(nil)

                local as4 = t.AvailableSpace.from_length(42)

                expect( as4 ).to.exist()
                expect( as4:is_Definite() ).to.be(true)
                expect( as4:get_value() ).to.be(42)

                local as5 = t.AvailableSpace.from(42)

                expect( as5 ).to.exist()
                expect( as5:is_Definite() ).to.be(true)
                expect( as5:get_value() ).to.be(42)

                local as6 = t.AvailableSpace.from( t.Option_float.new(42) )

                expect( as6 ).to.exist()
                expect( as6:is_Definite() ).to.be(true)
                expect( as6:get_value() ).to.be(42)

                local as7 = t.AvailableSpace.from( t.Option_float.new(nil) )

                expect( as7 ).to.exist()
                expect( as7:is_MaxContent() ).to.be(true)
                expect( as7:get_value() ).to.be(nil)
            end)
        end)

        it('Copying', function()
            local as = t.AvailableSpace.Definite(42)

            local copy = as:copy()

            expect( copy ).to.exist()
            expect( copy:is_Definite() ).to.be(true)
            expect( copy:get_value() ).to.be(42)
        end)

        describe('Operators', function()
            it('Comparison', function()
                local as_equal1 = t.AvailableSpace.Definite(42) == t.AvailableSpace.Definite(42)
                local as_equal2 = t.AvailableSpace.MinContent() == t.AvailableSpace.MinContent()
                local as_equal3 = t.AvailableSpace.MaxContent() == t.AvailableSpace.MaxContent()

                local as_not_equal1 = t.AvailableSpace.Definite(10) ~= t.AvailableSpace.Definite(20)
                local as_not_equal2 = t.AvailableSpace.Definite(42) ~= t.AvailableSpace.MinContent()
                local as_not_equal3 = t.AvailableSpace.Definite(42) ~= t.AvailableSpace.MaxContent()
                local as_not_equal4 = t.AvailableSpace.MinContent() ~= t.AvailableSpace.MaxContent()

                expect( as_equal1     ).to.be( true )
                expect( as_equal2     ).to.be( true )
                expect( as_equal3     ).to.be( true )

                expect( as_not_equal1 ).to.be( true )
                expect( as_not_equal2 ).to.be( true )
                expect( as_not_equal3 ).to.be( true )
                expect( as_not_equal4 ).to.be( true )
            end)
        end)

        it('Type checking', function()
            local as1 = t.AvailableSpace.Definite(42)

            expect( as1:is_Definite()   ).to.be( true )
            expect( as1:is_MinContent() ).to.be( false )
            expect( as1:is_MaxContent() ).to.be( false )

            local as2 = t.AvailableSpace.MinContent()

            expect( as2:is_Definite()   ).to.be( false )
            expect( as2:is_MinContent() ).to.be( true )
            expect( as2:is_MaxContent() ).to.be( false )

            local as3 = t.AvailableSpace.MaxContent()

            expect( as3:is_Definite()   ).to.be( false )
            expect( as3:is_MinContent() ).to.be( false )
            expect( as3:is_MaxContent() ).to.be( true )
        end)
    end) -- AvailableSpace

    describe('LengthPercentage', function()
        describe('Constructors', function()
            it('Length', function()
                local lp = t.LengthPercentage.Length(42)

                expect( lp ).to.exist()
                expect( lp:get_value() ).to.be(42)
            end)

            it('Percent', function()
                local lp = t.LengthPercentage.Percent(42)

                expect( lp ).to.exist()
                expect( lp:get_value() ).to.be(42)
            end)

            it('Named constructors', function()
                local lp1 = t.LengthPercentage.from_length(42)

                expect( lp1 ).to.exist()
                expect( lp1:is_Length() ).to.be(true)
                expect( lp1:get_value() ).to.be(42)

                local lp2 = t.LengthPercentage.from_percent(42)

                expect( lp2 ).to.exist()
                expect( lp2:is_Percent() ).to.be(true)
                expect( lp2:get_value() ).to.be(42)
            end)
        end)

        it('Copying', function()
            local lp = t.LengthPercentage.Length(42)

            local copy = lp:copy()

            expect( copy ).to.exist()
        end)

        describe('Operators', function()
            it('Comparison', function()
                local lp_equal1 = t.LengthPercentage.Length(42) == t.LengthPercentage.Length(42)
                local lp_equal2 = t.LengthPercentage.Percent(42) == t.LengthPercentage.Percent(42)

                local lp_not_equal1 = t.LengthPercentage.Length(10) ~= t.LengthPercentage.Length(20)
                local lp_not_equal2 = t.LengthPercentage.Percent(10) ~= t.LengthPercentage.Percent(20)
                local lp_not_equal3 = t.LengthPercentage.Length(10) ~= t.LengthPercentage.Percent(10)

                expect( lp_equal1     ).to.be( true )
                expect( lp_equal2     ).to.be( true )

                expect( lp_not_equal1 ).to.be( true )
                expect( lp_not_equal2 ).to.be( true )
                expect( lp_not_equal3 ).to.be( true )
            end)
        end)

        it('Type checking', function()
            local lp1 = t.LengthPercentage.Length(42)

            expect( lp1:is_Length()  ).to.be(true)
            expect( lp1:is_Percent() ).to.be(false)

            local lp2 = t.LengthPercentage.Percent(42)

            expect( lp2:is_Length()  ).to.be(false)
            expect( lp2:is_Percent() ).to.be(true)
        end)
    end) -- LengthPercentage

    describe('LengthPercentageAuto', function()
        describe('Constructors', function()
            it('Length', function()
                local lpa = t.LengthPercentageAuto.Length(42)

                expect( lpa ).to.exist()
                expect( lpa:get_value() ).to.be(42)
            end)

            it('Percent', function()
                local lpa = t.LengthPercentageAuto.Percent(42)

                expect( lpa ).to.exist()
                expect( lpa:get_value() ).to.be(42)
            end)

            it('Auto', function()
                local lpa = t.LengthPercentageAuto.Auto()

                expect( lpa ).to.exist()
                expect( lpa:get_value() ).to.be(nil)
            end)

            it('Named constructors', function()
                local lpa1 = t.LengthPercentageAuto.from_length(42)

                expect( lpa1 ).to.exist()
                expect( lpa1:is_Length() ).to.be(true)
                expect( lpa1:get_value() ).to.be(42)

                local lpa2 = t.LengthPercentageAuto.from_percent(42)

                expect( lpa2 ).to.exist()
                expect( lpa2:is_Percent() ).to.be(true)
                expect( lpa2:get_value() ).to.be(42)

                local lpa3 = t.LengthPercentageAuto.from( t.LengthPercentage.Length(42) )

                expect( lpa3 ).to.exist()
                expect( lpa3:is_Length() ).to.be(true)
                expect( lpa3:get_value() ).to.be(42)

                local lpa4 = t.LengthPercentageAuto.from( t.LengthPercentage.Percent(42) )

                expect( lpa4 ).to.exist()
                expect( lpa4:is_Percent() ).to.be(true)
                expect( lpa4:get_value() ).to.be(42)
            end)
        end)

        it('Copying', function()
            local lpa = t.LengthPercentageAuto.Length(42)

            local copy = lpa:copy()

            expect( copy ).to.exist()
            expect( copy:is_Length() ).to.be(true)
            expect( copy:get_value() ).to.be(42)
        end)

        describe('Operators', function()
            it('Comparison', function()
                local lpa_equal1 = t.LengthPercentageAuto.Length(42) == t.LengthPercentageAuto.Length(42)
                local lpa_equal2 = t.LengthPercentageAuto.Percent(42) == t.LengthPercentageAuto.Percent(42)
                local lpa_equal3 = t.LengthPercentageAuto.Auto() == t.LengthPercentageAuto.Auto()

                local lpa_not_equal1 = t.LengthPercentageAuto.Length(10) ~= t.LengthPercentageAuto.Length(20)
                local lpa_not_equal2 = t.LengthPercentageAuto.Percent(10) ~= t.LengthPercentageAuto.Percent(20)
                local lpa_not_equal3 = t.LengthPercentageAuto.Length(10) ~= t.LengthPercentageAuto.Percent(20)
                local lpa_not_equal4 = t.LengthPercentageAuto.Auto() ~= t.LengthPercentageAuto.Length(10)
                local lpa_not_equal5 = t.LengthPercentageAuto.Auto() ~= t.LengthPercentageAuto.Percent(10)

                expect( lpa_equal1     ).to.be( true )
                expect( lpa_equal2     ).to.be( true )
                expect( lpa_equal3     ).to.be( true )

                expect( lpa_not_equal1 ).to.be( true )
                expect( lpa_not_equal2 ).to.be( true )
                expect( lpa_not_equal3 ).to.be( true )
                expect( lpa_not_equal4 ).to.be( true )
                expect( lpa_not_equal5 ).to.be( true )
            end)
        end)

        it('Type checking', function()
            local lpa1 = t.LengthPercentageAuto.Length(42)

            expect( lpa1:is_Length()  ).to.be(true)
            expect( lpa1:is_Percent() ).to.be(false)
            expect( lpa1:is_Auto()    ).to.be(false)

            local lpa2 = t.LengthPercentageAuto.Percent(42)

            expect( lpa2:is_Length()  ).to.be(false)
            expect( lpa2:is_Percent() ).to.be(true)
            expect( lpa2:is_Auto()    ).to.be(false)

            local lpa3 = t.LengthPercentageAuto.Auto()

            expect( lpa3:is_Length()  ).to.be(false)
            expect( lpa3:is_Percent() ).to.be(false)
            expect( lpa3:is_Auto()    ).to.be(true)
        end)
    end) -- LengthPercentageAuto

    describe('Dimension', function()
        describe('Constructors', function()
            it('Length', function()
                local dimension = t.Dimension.Length(42)

                expect( dimension ).to.exist()
                expect( dimension:get_value() ).to.be(42)
            end)

            it('Percent', function()
                local dimension = t.Dimension.Percent(42)

                expect( dimension ).to.exist()
                expect( dimension:get_value() ).to.be(42)
            end)

            it('Auto', function()
                local dimension = t.Dimension.Auto()

                expect( dimension ).to.exist()
                expect( dimension:get_value() ).to.be(nil)
            end)

            it('Named constructors', function()
                local d1 = t.Dimension.from_length(42)

                expect( d1 ).to.exist()
                expect( d1:is_Length() ).to.be(true)
                expect( d1:get_value() ).to.be(42)

                local d2 = t.Dimension.from_percent(42)

                expect( d2 ).to.exist()
                expect( d2:is_Percent() ).to.be(true)
                expect( d2:get_value() ).to.be(42)

                local d3 = t.Dimension.from( t.LengthPercentage.Length(42) )

                expect( d3 ).to.exist()
                expect( d3:is_Length() ).to.be(true)
                expect( d3:get_value() ).to.be(42)

                local d4 = t.Dimension.from( t.LengthPercentage.Percent(42) )

                expect( d4 ).to.exist()
                expect( d4:is_Percent() ).to.be(true)
                expect( d4:get_value() ).to.be(42)

                local d5 = t.Dimension.from( t.LengthPercentageAuto.Length(42) )

                expect( d5 ).to.exist()
                expect( d5:is_Length() ).to.be(true)
                expect( d5:get_value() ).to.be(42)

                local d6 = t.Dimension.from( t.LengthPercentageAuto.Percent(42) )

                expect( d6 ).to.exist()
                expect( d6:is_Percent() ).to.be(true)
                expect( d6:get_value() ).to.be(42)

                local d7 = t.Dimension.from( t.LengthPercentageAuto.Auto(42) )

                expect( d7 ).to.exist()
                expect( d7:is_Auto() ).to.be(true)
                expect( d7:get_value() ).to.be(nil)
            end)
        end)

        it('Copying', function()
            local dimension = t.Dimension.Length(42)

            local copy = dimension:copy()

            expect( copy ).to.exist()
            expect( copy:is_Length() ).to.be(true)
            expect( copy:get_value() ).to.be(42)
        end)

        describe('Operators', function()
            it('Comparison', function()
                local d_equal1 = t.Dimension.Length(42) == t.Dimension.Length(42)
                local d_equal2 = t.Dimension.Percent(42) == t.Dimension.Percent(42)
                local d_equal3 = t.Dimension.Auto() == t.Dimension.Auto()

                local d_not_equal1 = t.Dimension.Length(10) ~= t.Dimension.Length(20)
                local d_not_equal2 = t.Dimension.Percent(10) ~= t.Dimension.Percent(20)
                local d_not_equal3 = t.Dimension.Length(10) ~= t.Dimension.Percent(20)
                local d_not_equal4 = t.Dimension.Auto() ~= t.Dimension.Length(10)
                local d_not_equal5 = t.Dimension.Auto() ~= t.Dimension.Percent(10)

                expect( d_equal1     ).to.be( true )
                expect( d_equal2     ).to.be( true )
                expect( d_equal3     ).to.be( true )

                expect( d_not_equal1 ).to.be( true )
                expect( d_not_equal2 ).to.be( true )
                expect( d_not_equal3 ).to.be( true )
                expect( d_not_equal4 ).to.be( true )
                expect( d_not_equal5 ).to.be( true )
            end)
        end)

        it('Type checking', function()
            local d1 = t.Dimension.Length(42)

            expect( d1:is_Length()  ).to.be(true)
            expect( d1:is_Percent() ).to.be(false)
            expect( d1:is_Auto()    ).to.be(false)

            local d2 = t.Dimension.Percent(42)

            expect( d2:is_Length()  ).to.be(false)
            expect( d2:is_Percent() ).to.be(true)
            expect( d2:is_Auto()    ).to.be(false)

            local d3 = t.Dimension.Auto()

            expect( d3:is_Length()  ).to.be(false)
            expect( d3:is_Percent() ).to.be(false)
            expect( d3:is_Auto()    ).to.be(true)
        end)
    end) -- Dimension

    describe('FlexDirection', function()
        it('Values', function()
            expect( t.FlexDirection.Row           ).to.exist()
            expect( t.FlexDirection.Column        ).to.exist()
            expect( t.FlexDirection.RowReverse    ).to.exist()
            expect( t.FlexDirection.ColumnReverse ).to.exist()
            expect( t.FlexDirection.Default       ).to.exist()

            expect( t.FlexDirection.Row           :to_int() ).to.be( 0 )
            expect( t.FlexDirection.Column        :to_int() ).to.be( 1 )
            expect( t.FlexDirection.RowReverse    :to_int() ).to.be( 2 )
            expect( t.FlexDirection.ColumnReverse :to_int() ).to.be( 3 )
            expect( t.FlexDirection.Default       :to_int() ).to.be( 0 )

            expect( t.FlexDirection.from_int(0) == t.FlexDirection.Row           ).to.be( true )
            expect( t.FlexDirection.from_int(1) == t.FlexDirection.Column        ).to.be( true )
            expect( t.FlexDirection.from_int(2) == t.FlexDirection.RowReverse    ).to.be( true )
            expect( t.FlexDirection.from_int(3) == t.FlexDirection.ColumnReverse ).to.be( true )
            expect( t.FlexDirection.from_int(0) == t.FlexDirection.Default       ).to.be( true )
        end)

        it('Strictness check', function()
            expect( t.FlexDirection.Wrong ).to.be(nil)

            expect( function() t.FlexDirection.Wrong = 42; return true end ).to.fail()

            expect( function() t.FlexDirection.Row = 42; return true end ).to.fail()

            expect( t.FlexDirection.Row.x ).to.be(nil)

            expect( function() t.FlexDirection.Row.x = 42; return true end ).to.fail()
        end)
    end) -- FlexDirection

    describe('FlexWrap', function()
        it('Values', function()
            expect( t.FlexWrap.NoWrap      ).to.exist()
            expect( t.FlexWrap.Wrap        ).to.exist()
            expect( t.FlexWrap.WrapReverse ).to.exist()
            expect( t.FlexWrap.Default     ).to.exist()

            expect( t.FlexWrap.NoWrap      :to_int() ).to.be( 0 )
            expect( t.FlexWrap.Wrap        :to_int() ).to.be( 1 )
            expect( t.FlexWrap.WrapReverse :to_int() ).to.be( 2 )
            expect( t.FlexWrap.Default     :to_int() ).to.be( 0 )

            expect( t.FlexWrap.from_int(0) == t.FlexWrap.NoWrap      ).to.be( true )
            expect( t.FlexWrap.from_int(1) == t.FlexWrap.Wrap        ).to.be( true )
            expect( t.FlexWrap.from_int(2) == t.FlexWrap.WrapReverse ).to.be( true )
            expect( t.FlexWrap.from_int(0) == t.FlexWrap.Default     ).to.be( true )
        end)

        it('Strictness check', function()
            expect( t.FlexWrap.Wrong ).to.be(nil)

            expect( function() t.FlexWrap.Wrong = 42; return true end ).to.fail()

            expect( function() t.FlexWrap.NoWrap = 42; return true end ).to.fail()

            expect( t.FlexWrap.NoWrap.x ).to.be(nil)

            expect( function() t.FlexWrap.NoWrap.x = 42; return true end ).to.fail()
        end)
    end) -- FlexWrap

    describe('GridAutoFlow', function()
        it('Values', function()
            expect( t.GridAutoFlow.Row         ).to.exist()
            expect( t.GridAutoFlow.Column      ).to.exist()
            expect( t.GridAutoFlow.RowDense    ).to.exist()
            expect( t.GridAutoFlow.ColumnDense ).to.exist()
            expect( t.GridAutoFlow.Default     ).to.exist()

            expect( t.GridAutoFlow.Row         :to_int() ).to.be( 0 )
            expect( t.GridAutoFlow.Column      :to_int() ).to.be( 1 )
            expect( t.GridAutoFlow.RowDense    :to_int() ).to.be( 2 )
            expect( t.GridAutoFlow.ColumnDense :to_int() ).to.be( 3 )
            expect( t.GridAutoFlow.Default     :to_int() ).to.be( 0 )

            expect( t.GridAutoFlow.from_int(0) == t.GridAutoFlow.Row         ).to.be( true )
            expect( t.GridAutoFlow.from_int(1) == t.GridAutoFlow.Column      ).to.be( true )
            expect( t.GridAutoFlow.from_int(2) == t.GridAutoFlow.RowDense    ).to.be( true )
            expect( t.GridAutoFlow.from_int(3) == t.GridAutoFlow.ColumnDense ).to.be( true )
            expect( t.GridAutoFlow.from_int(0) == t.GridAutoFlow.Default     ).to.be( true )
        end)

        it('Strictness check', function()
            expect( t.GridAutoFlow.Wrong ).to.be(nil)

            expect( function() t.GridAutoFlow.Wrong = 42; return true end ).to.fail()

            expect( function() t.GridAutoFlow.Row = 42; return true end ).to.fail()

            expect( t.GridAutoFlow.Row.x ).to.be(nil)

            expect( function() t.GridAutoFlow.Row.x = 42; return true end ).to.fail()
        end)
    end) -- GridAutoFlow

    describe('GridPlacement', function()
        describe('Constructors', function()
            it('default', function()
                local gp = t.GridPlacement.new()

                expect( gp ).to.exist()
                expect( gp:get_line() ).to.be(nil)
                expect( gp:get_span() ).to.be(nil)
            end)

            it('Auto', function()
                local gp = t.GridPlacement.Auto()

                expect( gp ).to.exist()
                expect( gp:get_line() ).to.be(nil)
                expect( gp:get_span() ).to.be(nil)
            end)

            it('Line', function()
                local gp = t.GridPlacement.Line(42)

                expect( gp ).to.exist()
                expect( gp:get_line() ).to.be(42)
                expect( gp:get_span() ).to.be(nil)
            end)

            it('Span', function()
                local gp = t.GridPlacement.Span(42)

                expect( gp ).to.exist()
                expect( gp:get_line() ).to.be(nil)
                expect( gp:get_span() ).to.be(42)
            end)

            it('Named constructors', function()
                local gp1 = t.GridPlacement.AUTO()

                expect( gp1 ).to.exist()
                expect( gp1:is_Auto() ).to.be(true)

                local gp2 = t.GridPlacement.from_line_index(42)

                expect( gp2 ).to.exist()
                expect( gp2:is_Line() ).to.be(true)

                local gp3 = t.GridPlacement.from_span(42)

                expect( gp3 ).to.exist()
                expect( gp3:is_Span() ).to.be(true)
            end)
        end)

        it('Copying', function()
            local gp = t.GridPlacement.Span(42)

            local copy = gp:copy()

            expect( copy ).to.exist()
            expect( copy:is_Span() ).to.be(true)
            expect( copy:get_span() ).to.be(42)
        end)

        describe('Operators', function()
            it('Comparison', function()
                local gp_equal1 = t.GridPlacement.Auto() == t.GridPlacement.Auto()
                local gp_equal2 = t.GridPlacement.Line(42) == t.GridPlacement.Line(42)
                local gp_equal3 = t.GridPlacement.Span(42) == t.GridPlacement.Span(42)

                local gp_not_equal1 = t.GridPlacement.Line(10) ~= t.GridPlacement.Line(20)
                local gp_not_equal2 = t.GridPlacement.Span(10) ~= t.GridPlacement.Span(20)
                local gp_not_equal3 = t.GridPlacement.Line(10) ~= t.GridPlacement.Span(20)
                local gp_not_equal4 = t.GridPlacement.Auto() ~= t.GridPlacement.Line(10)
                local gp_not_equal5 = t.GridPlacement.Auto() ~= t.GridPlacement.Span(10)

                expect( gp_equal1     ).to.be( true )
                expect( gp_equal2     ).to.be( true )
                expect( gp_equal3     ).to.be( true )

                expect( gp_not_equal1 ).to.be( true )
                expect( gp_not_equal2 ).to.be( true )
                expect( gp_not_equal3 ).to.be( true )
                expect( gp_not_equal4 ).to.be( true )
                expect( gp_not_equal5 ).to.be( true )
            end)
        end)

        it('Type checking', function()
            local gp1 = t.GridPlacement.new()

            expect( gp1 ).to.exist()
            expect( gp1:is_Auto() ).to.be(true)
            expect( gp1:is_Line() ).to.be(false)
            expect( gp1:is_Span() ).to.be(false)

            local gp2 = t.GridPlacement.Auto()

            expect( gp2 ).to.exist()
            expect( gp2:is_Auto() ).to.be(true)
            expect( gp2:is_Line() ).to.be(false)
            expect( gp2:is_Span() ).to.be(false)

            local gp3 = t.GridPlacement.Line(42)

            expect( gp3 ).to.exist()
            expect( gp3:is_Auto() ).to.be(false)
            expect( gp3:is_Line() ).to.be(true)
            expect( gp3:is_Span() ).to.be(false)

            local gp4 = t.GridPlacement.Span(42)

            expect( gp4 ).to.exist()
            expect( gp4:is_Auto() ).to.be(false)
            expect( gp4:is_Line() ).to.be(false)
            expect( gp4:is_Span() ).to.be(true)
        end)
    end) -- GridPlacement

    describe('GridTrackRepetition', function()
        describe('Constructors', function()
            it('AutoFill', function()
                local gtr = t.GridTrackRepetition.AutoFill()

                expect( gtr ).to.exist()
                expect( gtr:get_value() ).to.be( nil )
            end)

            it('AutoFit', function()
                local gtr = t.GridTrackRepetition.AutoFit()

                expect( gtr ).to.exist()
                expect( gtr:get_value() ).to.be( nil )
            end)

            it('Count', function()
                local gtr = t.GridTrackRepetition.Count(42)

                expect( gtr ).to.exist()
                expect( gtr: get_value() ).to.be( 42 )
            end)

            it('Named constructors', function()
                local gtr1 = t.GridTrackRepetition.try_from(42)

                expect( gtr1 ).to.exist()
                expect( gtr1:is_Count() ).to.be( true )
                expect( gtr1:get_value() ).to.be( 42 )

                local gtr2 = t.GridTrackRepetition.try_from('auto-fill')

                expect( gtr2 ).to.exist()
                expect( gtr2:is_AutoFill() ).to.be( true )
                expect( gtr2:get_value() ).to.be( nil )

                local gtr3 = t.GridTrackRepetition.try_from('auto-fit')

                expect( gtr3 ).to.exist()
                expect( gtr3:is_AutoFit() ).to.be( true )
                expect( gtr3:get_value() ).to.be( nil )

                local gtr4 = t.GridTrackRepetition.try_from('sometext')

                expect( gtr4 ).to.be(nil)
            end)
        end)

        it('Copying', function()
            local gtr = t.GridTrackRepetition.Count(42)

            local copy = gtr:copy()

            expect( copy ).to.exist()
            expect( copy:is_Count() ).to.be( true )
            expect( copy:get_value() ).to.be( 42 )
        end)

        describe('Operators', function()
            it('Comparison', function()
                local gtr_equal1 = t.GridTrackRepetition.AutoFill() == t.GridTrackRepetition.AutoFill()
                local gtr_equal2 = t.GridTrackRepetition.AutoFit() == t.GridTrackRepetition.AutoFit()
                local gtr_equal3 = t.GridTrackRepetition.Count(42) == t.GridTrackRepetition.Count(42)

                local gtr_not_equal1 = t.GridTrackRepetition.AutoFill() ~= t.GridTrackRepetition.AutoFit()
                local gtr_not_equal2 = t.GridTrackRepetition.AutoFill()  ~= t.GridTrackRepetition.Count(42)
                local gtr_not_equal3 = t.GridTrackRepetition.AutoFit() ~= t.GridTrackRepetition.Count(42)
                local gtr_not_equal4 = t.GridTrackRepetition.Count(10) ~= t.GridTrackRepetition.Count(20)

                expect( gtr_equal1     ).to.be( true )
                expect( gtr_equal2     ).to.be( true )
                expect( gtr_equal3     ).to.be( true )

                expect( gtr_not_equal1 ).to.be( true )
                expect( gtr_not_equal2 ).to.be( true )
                expect( gtr_not_equal3 ).to.be( true )
                expect( gtr_not_equal4 ).to.be( true )
            end)
        end)

        it('Type checking', function()
            local gtr1 = t.GridTrackRepetition.AutoFill()

            expect( gtr1:is_AutoFill() ).to.be( true )
            expect( gtr1:is_AutoFit()  ).to.be( false )
            expect( gtr1:is_Count()    ).to.be( false )

            local gtr2 = t.GridTrackRepetition.AutoFit()

            expect( gtr2:is_AutoFill() ).to.be( false )
            expect( gtr2:is_AutoFit()  ).to.be( true )
            expect( gtr2:is_Count()    ).to.be( false )

            local gtr3 = t.GridTrackRepetition.Count(42)

            expect( gtr3:is_AutoFill() ).to.be( false )
            expect( gtr3:is_AutoFit()  ).to.be( false )
            expect( gtr3:is_Count()    ).to.be( true )
        end)
    end) -- GridTrackRepetition

    describe('MaxTrackSizingFunction', function()
        describe('Constructors', function()
            it('Fixed', function()
                local max_tsf = t.MaxTrackSizingFunction.Fixed( t.LengthPercentage.ZERO() )

                expect( max_tsf ).to.exist()
                expect( max_tsf:get_length_percentage() ).to.be( t.LengthPercentage.ZERO() )
                expect( max_tsf:get_fraction() ).to.be( nil )
            end)

            it('MinContent', function()
                local max_tsf = t.MaxTrackSizingFunction.MinContent()

                expect( max_tsf ).to.exist()
                expect( max_tsf:get_length_percentage() ).to.be( nil )
                expect( max_tsf:get_fraction() ).to.be( nil )
            end)

            it('MaxContent', function()
                local max_tsf = t.MaxTrackSizingFunction.MaxContent()

                expect( max_tsf ).to.exist()
                expect( max_tsf:get_length_percentage() ).to.be( nil )
                expect( max_tsf:get_fraction() ).to.be( nil )
            end)

            it('FitContent', function()
                local max_tsf = t.MaxTrackSizingFunction.FitContent( t.LengthPercentage.ZERO() )

                expect( max_tsf ).to.exist()
                expect( max_tsf:get_length_percentage() ).to.be( t.LengthPercentage.ZERO() )
                expect( max_tsf:get_fraction() ).to.be( nil )
            end)

            it('Auto', function()
                local max_tsf = t.MaxTrackSizingFunction.Auto()

                expect( max_tsf ).to.exist()
                expect( max_tsf:get_length_percentage() ).to.be( nil )
                expect( max_tsf:get_fraction() ).to.be( nil )
            end)

            it('Fraction', function()
                local max_tsf = t.MaxTrackSizingFunction.Fraction(42)

                expect( max_tsf ).to.exist()
                expect( max_tsf:get_length_percentage() ).to.be( nil )
                expect( max_tsf:get_fraction() ).to.be( 42 )
            end)

            it('Named constructors', function()
                local max_tsf1 = t.MaxTrackSizingFunction.AUTO()

                expect( max_tsf1 ).to.exist()
                expect( max_tsf1:is_Auto() ).to.be( true )

                local max_tsf2 = t.MaxTrackSizingFunction.MIN_CONTENT()

                expect( max_tsf2 ).to.exist()
                expect( max_tsf2:is_MinContent() ).to.be( true )

                local max_tsf3 = t.MaxTrackSizingFunction.MAX_CONTENT()

                expect( max_tsf3 ).to.exist()
                expect( max_tsf3:is_MaxContent() ).to.be( true )

                local max_tsf4 = t.MaxTrackSizingFunction.ZERO()

                expect( max_tsf4 ).to.exist()
                expect( max_tsf4:is_Fixed() ).to.be( true )

                local max_tsf5 = t.MaxTrackSizingFunction.fit_content( t.LengthPercentage.ZERO() )

                expect( max_tsf5 ).to.exist()
                expect( max_tsf5:is_FitContent() ).to.be( true )

                local max_tsf6 = t.MaxTrackSizingFunction.from_length(42)

                expect( max_tsf6 ).to.exist()
                expect( max_tsf6:is_Fixed() ).to.be( true )

                local max_tsf7 = t.MaxTrackSizingFunction.from_percent(42)

                expect( max_tsf7 ).to.exist()
                expect( max_tsf7:is_Fixed() ).to.be( true )

                local max_tsf8 = t.MaxTrackSizingFunction.from_flex(42)

                expect( max_tsf8 ).to.exist()
                expect( max_tsf8:is_Fraction() ).to.be( true )
            end)
        end)

        it('Copying', function()
            local max_tsf = t.MaxTrackSizingFunction.Fraction(42)

            local copy = max_tsf:copy()

            expect( copy ).to.exist()
            expect( copy:is_Fraction() ).to.be( true )
            expect( copy:get_fraction() ).to.be( 42 )
        end)

        describe('Operators', function()
            it('Comparison', function()
                local equal1 = t.MaxTrackSizingFunction.Fixed( t.LengthPercentage.ZERO() ) == t.MaxTrackSizingFunction.Fixed( t.LengthPercentage.ZERO() )
                local equal2 = t.MaxTrackSizingFunction.MinContent() == t.MaxTrackSizingFunction.MinContent()
                local equal3 = t.MaxTrackSizingFunction.MaxContent() == t.MaxTrackSizingFunction.MaxContent()
                local equal4 = t.MaxTrackSizingFunction.FitContent( t.LengthPercentage.ZERO() ) == t.MaxTrackSizingFunction.FitContent( t.LengthPercentage.ZERO() )
                local equal5 = t.MaxTrackSizingFunction.Auto() == t.MaxTrackSizingFunction.Auto()
                local equal6 = t.MaxTrackSizingFunction.Fraction(42) == t.MaxTrackSizingFunction.Fraction(42)

                local not_equal1 = t.MaxTrackSizingFunction.Fixed( t.LengthPercentage.Length(10) ) ~= t.MaxTrackSizingFunction.Fixed( t.LengthPercentage.Length(20) )
                local not_equal2 = t.MaxTrackSizingFunction.Fixed( t.LengthPercentage.Length(10) ) ~= t.MaxTrackSizingFunction.Fixed( t.LengthPercentage.Percent(10) )
                local not_equal3 = t.MaxTrackSizingFunction.Fraction(10) ~= t.MaxTrackSizingFunction.Fraction(20)
                -- ... TODO: add all posible non-equal checks here (its too boring for now) ...

                expect( equal1 ).to.be( true )
                expect( equal2 ).to.be( true )
                expect( equal3 ).to.be( true )
                expect( equal4 ).to.be( true )
                expect( equal5 ).to.be( true )
                expect( equal6 ).to.be( true )

                expect( not_equal1 ).to.be( true )
                expect( not_equal2 ).to.be( true )
                expect( not_equal3 ).to.be( true )
            end)
        end)

        describe('Type checking', function()
            local max_tsf1 = t.MaxTrackSizingFunction.Fixed( t.LengthPercentage.ZERO() )

            expect( max_tsf1:is_Fixed()      ).to.be( true )
            expect( max_tsf1:is_MinContent() ).to.be( false )
            expect( max_tsf1:is_MaxContent() ).to.be( false )
            expect( max_tsf1:is_FitContent() ).to.be( false )
            expect( max_tsf1:is_Auto()       ).to.be( false )
            expect( max_tsf1:is_Fraction()   ).to.be( false )

            local max_tsf2 = t.MaxTrackSizingFunction.MinContent()

            expect( max_tsf2:is_Fixed()      ).to.be( false )
            expect( max_tsf2:is_MinContent() ).to.be( true )
            expect( max_tsf2:is_MaxContent() ).to.be( false )
            expect( max_tsf2:is_FitContent() ).to.be( false )
            expect( max_tsf2:is_Auto()       ).to.be( false )
            expect( max_tsf2:is_Fraction()   ).to.be( false )

            local max_tsf3 = t.MaxTrackSizingFunction.MaxContent()

            expect( max_tsf3:is_Fixed()      ).to.be( false )
            expect( max_tsf3:is_MinContent() ).to.be( false )
            expect( max_tsf3:is_MaxContent() ).to.be( true )
            expect( max_tsf3:is_FitContent() ).to.be( false )
            expect( max_tsf3:is_Auto()       ).to.be( false )
            expect( max_tsf3:is_Fraction()   ).to.be( false )

            local max_tsf4 = t.MaxTrackSizingFunction.FitContent( t.LengthPercentage.ZERO() )

            expect( max_tsf4:is_Fixed()      ).to.be( false )
            expect( max_tsf4:is_MinContent() ).to.be( false )
            expect( max_tsf4:is_MaxContent() ).to.be( false )
            expect( max_tsf4:is_FitContent() ).to.be( true )
            expect( max_tsf4:is_Auto()       ).to.be( false )
            expect( max_tsf4:is_Fraction()   ).to.be( false )

            local max_tsf5 = t.MaxTrackSizingFunction.Auto()

            expect( max_tsf5:is_Fixed()      ).to.be( false )
            expect( max_tsf5:is_MinContent() ).to.be( false )
            expect( max_tsf5:is_MaxContent() ).to.be( false )
            expect( max_tsf5:is_FitContent() ).to.be( false )
            expect( max_tsf5:is_Auto()       ).to.be( true )
            expect( max_tsf5:is_Fraction()   ).to.be( false )

            local max_tsf6 = t.MaxTrackSizingFunction.Fraction(42)

            expect( max_tsf6:is_Fixed()      ).to.be( false )
            expect( max_tsf6:is_MinContent() ).to.be( false )
            expect( max_tsf6:is_MaxContent() ).to.be( false )
            expect( max_tsf6:is_FitContent() ).to.be( false )
            expect( max_tsf6:is_Auto()       ).to.be( false )
            expect( max_tsf6:is_Fraction()   ).to.be( true )
        end)
    end) -- MaxTrackSizingFunction

    describe('MinTrackSizingFunction', function()
        describe('Constructors', function()
            it('Fixed', function()
                local min_tsf = t.MinTrackSizingFunction.Fixed( t.LengthPercentage.ZERO() )

                expect( min_tsf ).to.exist()
                expect( min_tsf:get_value() ).to.be( t.LengthPercentage.ZERO() )
            end)

            it('MinContent', function()
                local min_tsf = t.MinTrackSizingFunction.MinContent()

                expect( min_tsf ).to.exist()
                expect( min_tsf:get_value() ).to.be( nil )
            end)

            it('MaxContent', function()
                local min_tsf = t.MinTrackSizingFunction.MaxContent()

                expect( min_tsf ).to.exist()
                expect( min_tsf:get_value() ).to.be( nil )
            end)

            it('Auto', function()
                local min_tsf = t.MinTrackSizingFunction.Auto()

                expect( min_tsf ).to.exist()
                expect( min_tsf:get_value() ).to.be( nil )
            end)

            it('Named constructors', function()
                local min_tsf1 = t.MinTrackSizingFunction.AUTO()

                expect( min_tsf1 ).to.exist()
                expect( min_tsf1:is_Auto() ).to.be( true )

                local min_tsf2 = t.MinTrackSizingFunction.MIN_CONTENT()

                expect( min_tsf2 ).to.exist()
                expect( min_tsf2:is_MinContent() ).to.be( true )

                local min_tsf3 = t.MinTrackSizingFunction.MAX_CONTENT()

                expect( min_tsf3 ).to.exist()
                expect( min_tsf3:is_MaxContent() ).to.be( true )

                local min_tsf4 = t.MinTrackSizingFunction.ZERO()

                expect( min_tsf4 ).to.exist()
                expect( min_tsf4:is_Fixed() ).to.be( true )

                local min_tsf5 = t.MinTrackSizingFunction.from_length(42)

                expect( min_tsf5 ).to.exist()
                expect( min_tsf5:is_Fixed() ).to.be( true )

                local min_tsf6 = t.MinTrackSizingFunction.from_percent(42)

                expect( min_tsf6 ).to.exist()
                expect( min_tsf6:is_Fixed() ).to.be( true )
            end)
        end)

        it('Copying', function()
            local min_tsf = t.MinTrackSizingFunction.Fixed( t.LengthPercentage.Length(42) )

            local copy = min_tsf:copy()

            expect( copy ).to.exist()
            expect( copy:is_Fixed() ).to.be( true )
            expect( copy:get_value() ).to.be( t.LengthPercentage.Length(42) )
        end)

        describe('Operators', function()
            it('Comparison', function()
                local equal1 = t.MinTrackSizingFunction.Fixed( t.LengthPercentage.ZERO() ) == t.MinTrackSizingFunction.Fixed( t.LengthPercentage.ZERO() )
                local equal2 = t.MinTrackSizingFunction.MinContent() == t.MinTrackSizingFunction.MinContent()
                local equal3 = t.MinTrackSizingFunction.MaxContent() == t.MinTrackSizingFunction.MaxContent()
                local equal4 = t.MinTrackSizingFunction.Auto() == t.MinTrackSizingFunction.Auto()

                local not_equal1 = t.MinTrackSizingFunction.Fixed( t.LengthPercentage.Length(10) ) ~= t.MinTrackSizingFunction.Fixed( t.LengthPercentage.Length(20) )
                local not_equal2 = t.MinTrackSizingFunction.Fixed( t.LengthPercentage.Percent(10) ) ~= t.MinTrackSizingFunction.Fixed( t.LengthPercentage.Percent(20) )
                local not_equal3 = t.MinTrackSizingFunction.Fixed( t.LengthPercentage.Length(10) ) ~= t.MinTrackSizingFunction.Fixed( t.LengthPercentage.Percent(10) )
                local not_equal4 = t.MinTrackSizingFunction.MinContent() ~= t.MinTrackSizingFunction.MaxContent()
                local not_equal5 = t.MinTrackSizingFunction.MinContent() ~= t.MinTrackSizingFunction.Auto()
                local not_equal6 = t.MinTrackSizingFunction.MaxContent() ~= t.MinTrackSizingFunction.Auto()
                local not_equal7 = t.MinTrackSizingFunction.MinContent() ~= t.MinTrackSizingFunction.Fixed( t.LengthPercentage.ZERO() )
                local not_equal8 = t.MinTrackSizingFunction.MaxContent() ~= t.MinTrackSizingFunction.Fixed( t.LengthPercentage.ZERO() )
                local not_equal9 = t.MinTrackSizingFunction.Auto() ~= t.MinTrackSizingFunction.Fixed( t.LengthPercentage.ZERO() )

                expect( equal1 ).to.be( true )
                expect( equal2 ).to.be( true )
                expect( equal3 ).to.be( true )
                expect( equal4 ).to.be( true )

                expect( not_equal1 ).to.be( true )
                expect( not_equal2 ).to.be( true )
                expect( not_equal3 ).to.be( true )
                expect( not_equal4 ).to.be( true )
                expect( not_equal5 ).to.be( true )
                expect( not_equal6 ).to.be( true )
                expect( not_equal7 ).to.be( true )
                expect( not_equal8 ).to.be( true )
                expect( not_equal9 ).to.be( true )
            end)
        end)

        it('Type checking', function()
            local min_tsf1 = t.MinTrackSizingFunction.Fixed( t.LengthPercentage.ZERO() )

            expect( min_tsf1:is_Fixed()      ).to.be( true )
            expect( min_tsf1:is_MinContent() ).to.be( false )
            expect( min_tsf1:is_MaxContent() ).to.be( false )
            expect( min_tsf1:is_Auto()       ).to.be( false )

            local min_tsf2 = t.MinTrackSizingFunction.MinContent()

            expect( min_tsf2:is_Fixed()      ).to.be( false )
            expect( min_tsf2:is_MinContent() ).to.be( true )
            expect( min_tsf2:is_MaxContent() ).to.be( false )
            expect( min_tsf2:is_Auto()       ).to.be( false )

            local min_tsf3 = t.MinTrackSizingFunction.MaxContent()

            expect( min_tsf3:is_Fixed()      ).to.be( false )
            expect( min_tsf3:is_MinContent() ).to.be( false )
            expect( min_tsf3:is_MaxContent() ).to.be( true )
            expect( min_tsf3:is_Auto()       ).to.be( false )

            local min_tsf4 = t.MinTrackSizingFunction.Auto()

            expect( min_tsf4:is_Fixed()      ).to.be( false )
            expect( min_tsf4:is_MinContent() ).to.be( false )
            expect( min_tsf4:is_MaxContent() ).to.be( false )
            expect( min_tsf4:is_Auto()       ).to.be( true )
        end)
    end) -- MinTrackSizingFunction

    describe('NonRepeatedTrackSizingFunction', function()
        describe('Constructors', function()
            it('common', function()
                local nrtsf = t.NonRepeatedTrackSizingFunction.new( t.MinTrackSizingFunction.Auto(), t.MaxTrackSizingFunction.Auto() )

                expect( nrtsf ).to.exist()
                expect( nrtsf:get_min() ).to.be( t.MinTrackSizingFunction.Auto() )
                expect( nrtsf:get_max() ).to.be( t.MaxTrackSizingFunction.Auto() )
            end)

            it('table (array)', function()
                local nrtsf = t.NonRepeatedTrackSizingFunction.new({t.MinTrackSizingFunction.Auto(), t.MaxTrackSizingFunction.Auto()})

                expect( nrtsf ).to.exist()
                expect( nrtsf:get_min() ).to.be( t.MinTrackSizingFunction.Auto() )
                expect( nrtsf:get_max() ).to.be( t.MaxTrackSizingFunction.Auto() )
            end)

            it('table (array) explicit indexes', function()
                local nrtsf1 = t.NonRepeatedTrackSizingFunction.new({[1] = t.MinTrackSizingFunction.Auto(), [2] = t.MaxTrackSizingFunction.Auto()})

                expect( nrtsf1 ).to.exist()
                expect( nrtsf1:get_min() ).to.be( t.MinTrackSizingFunction.Auto() )
                expect( nrtsf1:get_max() ).to.be( t.MaxTrackSizingFunction.Auto() )

                local nrtsf2 = t.NonRepeatedTrackSizingFunction.new({[2] = t.MaxTrackSizingFunction.Auto(), [1] = t.MinTrackSizingFunction.Auto()})

                expect( nrtsf2 ).to.exist()
                expect( nrtsf2:get_min() ).to.be( t.MinTrackSizingFunction.Auto() )
                expect( nrtsf2:get_max() ).to.be( t.MaxTrackSizingFunction.Auto() )
            end)

            it('table (dictionary)', function()
                local nrtsf1 = t.NonRepeatedTrackSizingFunction.new({min = t.MinTrackSizingFunction.Auto(), max = t.MaxTrackSizingFunction.Auto()})

                expect( nrtsf1 ).to.exist()
                expect( nrtsf1:get_min() ).to.be( t.MinTrackSizingFunction.Auto() )
                expect( nrtsf1:get_max() ).to.be( t.MaxTrackSizingFunction.Auto() )

                local nrtsf2 = t.NonRepeatedTrackSizingFunction.new({max = t.MaxTrackSizingFunction.Auto(), min = t.MinTrackSizingFunction.Auto()})

                expect( nrtsf2 ).to.exist()
                expect( nrtsf2:get_min() ).to.be( t.MinTrackSizingFunction.Auto() )
                expect( nrtsf2:get_max() ).to.be( t.MaxTrackSizingFunction.Auto() )
            end)

            it('table (without parentheses)', function()
                local nrtsf1 = t.NonRepeatedTrackSizingFunction.new { t.MinTrackSizingFunction.Auto(), t.MaxTrackSizingFunction.Auto() }

                expect( nrtsf1 ).to.exist()
                expect( nrtsf1:get_min() ).to.be( t.MinTrackSizingFunction.Auto() )
                expect( nrtsf1:get_max() ).to.be( t.MaxTrackSizingFunction.Auto() )

                local nrtsf2 = t.NonRepeatedTrackSizingFunction.new { min = t.MinTrackSizingFunction.Auto(), max = t.MaxTrackSizingFunction.Auto() }

                expect( nrtsf2 ).to.exist()
                expect( nrtsf2:get_min() ).to.be( t.MinTrackSizingFunction.Auto() )
                expect( nrtsf2:get_max() ).to.be( t.MaxTrackSizingFunction.Auto() )
            end)

            it('Named constructors', function()
                local tsf1 = t.NonRepeatedTrackSizingFunction.AUTO()

                expect( tsf1:get_min() ).to.be( t.MinTrackSizingFunction.AUTO() )
                expect( tsf1:get_max() ).to.be( t.MaxTrackSizingFunction.AUTO() )

                local tsf2 = t.NonRepeatedTrackSizingFunction.MIN_CONTENT()

                expect( tsf2:get_min() ).to.be( t.MinTrackSizingFunction.MIN_CONTENT() )
                expect( tsf2:get_max() ).to.be( t.MaxTrackSizingFunction.MIN_CONTENT() )

                local tsf3 = t.NonRepeatedTrackSizingFunction.MAX_CONTENT()

                expect( tsf3:get_min() ).to.be( t.MinTrackSizingFunction.MAX_CONTENT() )
                expect( tsf3:get_max() ).to.be( t.MaxTrackSizingFunction.MAX_CONTENT() )

                local tsf4 = t.NonRepeatedTrackSizingFunction.fit_content( t.LengthPercentage.Length(42) )

                expect( tsf4:get_min() ).to.be( t.MinTrackSizingFunction.AUTO() )
                expect( tsf4:get_max() ).to.be( t.MaxTrackSizingFunction.FitContent(t.LengthPercentage.Length(42)) )

                local tsf5 = t.NonRepeatedTrackSizingFunction.ZERO()

                expect( tsf5:get_min() ).to.be( t.MinTrackSizingFunction.ZERO() )
                expect( tsf5:get_max() ).to.be( t.MaxTrackSizingFunction.ZERO() )

                local tsf6 = t.NonRepeatedTrackSizingFunction.from_length(42)

                expect( tsf6:get_min() ).to.be( t.MinTrackSizingFunction.from_length(42) )
                expect( tsf6:get_max() ).to.be( t.MaxTrackSizingFunction.from_length(42) )

                local tsf7 = t.NonRepeatedTrackSizingFunction.from_percent(42)

                expect( tsf7:get_min() ).to.be( t.MinTrackSizingFunction.from_percent(42) )
                expect( tsf7:get_max() ).to.be( t.MaxTrackSizingFunction.from_percent(42) )

                local tsf8 = t.NonRepeatedTrackSizingFunction.from_flex(42)

                expect( tsf8:get_min() ).to.be( t.MinTrackSizingFunction.AUTO() )
                expect( tsf8:get_max() ).to.be( t.MaxTrackSizingFunction.from_flex(42) )
            end)
        end)

        it('Copying', function()
            local nrtsf =
                t.NonRepeatedTrackSizingFunction.new(
                    t.MinTrackSizingFunction.Fixed( t.LengthPercentage.Length(10) ),
                    t.MaxTrackSizingFunction.Fixed( t.LengthPercentage.Percent(20) )
                )

            local copy = nrtsf:copy()

            expect( copy ).to.exist()
            expect( copy:get_min() ).to.be( t.MinTrackSizingFunction.Fixed( t.LengthPercentage.Length(10)  ) )
            expect( copy:get_max() ).to.be( t.MaxTrackSizingFunction.Fixed( t.LengthPercentage.Percent(20) ) )
        end)

        describe('Operators', function()
            it('Comparison', function()
                local nrtsf_LP1 =
                    t.NonRepeatedTrackSizingFunction.new(
                        t.MinTrackSizingFunction.Fixed( t.LengthPercentage.Length(10) ),
                        t.MaxTrackSizingFunction.Fixed( t.LengthPercentage.Percent(20) )
                    )

                local nrtsf_LP2 =
                    t.NonRepeatedTrackSizingFunction.new(
                        t.MinTrackSizingFunction.Fixed( t.LengthPercentage.Length(20) ),
                        t.MaxTrackSizingFunction.Fixed( t.LengthPercentage.Percent(10) )
                    )

                local nrtsf_PL1 =
                    t.NonRepeatedTrackSizingFunction.new(
                        t.MinTrackSizingFunction.Fixed( t.LengthPercentage.Percent(10) ),
                        t.MaxTrackSizingFunction.Fixed( t.LengthPercentage.Length(20) )
                    )

                local nrtsf_PL2 =
                    t.NonRepeatedTrackSizingFunction.new(
                        t.MinTrackSizingFunction.Fixed( t.LengthPercentage.Percent(20) ),
                        t.MaxTrackSizingFunction.Fixed( t.LengthPercentage.Length(10) )
                    )

                expect( nrtsf_LP1 == nrtsf_LP1:copy() ).to.be( true )
                expect( nrtsf_LP2 == nrtsf_LP2:copy() ).to.be( true )

                expect( nrtsf_LP1 ~= nrtsf_LP2 ).to.be( true )
                expect( nrtsf_LP1 ~= nrtsf_LP2 ).to.be( true )

                expect( nrtsf_PL1 == nrtsf_PL1:copy() ).to.be( true )
                expect( nrtsf_PL2 == nrtsf_PL2:copy() ).to.be( true )

                expect( nrtsf_PL1 ~= nrtsf_PL2 ).to.be( true )
                expect( nrtsf_PL1 ~= nrtsf_PL2 ).to.be( true )

                expect( nrtsf_LP1 ~= nrtsf_PL1 ).to.be( true )
                expect( nrtsf_LP2 ~= nrtsf_PL2 ).to.be( true )
            end)
        end)

        it('Getters/Setters', function()
            local nrtsf =
                t.NonRepeatedTrackSizingFunction.new(
                    t.MinTrackSizingFunction.Fixed( t.LengthPercentage.Length(10) ),
                    t.MaxTrackSizingFunction.Fixed( t.LengthPercentage.Percent(20) )
                )

            expect( nrtsf ).to.exist()
            expect( nrtsf:get_min() ).to.be( t.MinTrackSizingFunction.Fixed( t.LengthPercentage.Length(10)  ) )
            expect( nrtsf:get_max() ).to.be( t.MaxTrackSizingFunction.Fixed( t.LengthPercentage.Percent(20) ) )

            nrtsf:set_min( t.MinTrackSizingFunction.Fixed( t.LengthPercentage.Percent(35) ) )
            nrtsf:set_max( t.MaxTrackSizingFunction.Fixed( t.LengthPercentage.Length(42)  ) )

            expect( nrtsf:get_min() ).to.be( t.MinTrackSizingFunction.Fixed( t.LengthPercentage.Percent(35) ) )
            expect( nrtsf:get_max() ).to.be( t.MaxTrackSizingFunction.Fixed( t.LengthPercentage.Length(42)  ) )
        end)

        it('Properties', function()
            local nrtsf =
                t.NonRepeatedTrackSizingFunction.new(
                    t.MinTrackSizingFunction.Fixed( t.LengthPercentage.Length(10) ),
                    t.MaxTrackSizingFunction.Fixed( t.LengthPercentage.Percent(20) )
                )

            expect( nrtsf ).to.exist()
            expect( nrtsf.min ).to.be( t.MinTrackSizingFunction.Fixed( t.LengthPercentage.Length(10)  ) )
            expect( nrtsf.max ).to.be( t.MaxTrackSizingFunction.Fixed( t.LengthPercentage.Percent(20) ) )

            nrtsf.min = t.MinTrackSizingFunction.Fixed( t.LengthPercentage.Percent(35) )
            nrtsf.max = t.MaxTrackSizingFunction.Fixed( t.LengthPercentage.Length(42)  )

            expect( nrtsf.min ).to.be( t.MinTrackSizingFunction.Fixed( t.LengthPercentage.Percent(35) ) )
            expect( nrtsf.max ).to.be( t.MaxTrackSizingFunction.Fixed( t.LengthPercentage.Length(42)  ) )
        end)
    end) -- NonRepeatedTrackSizingFunction

    describe('TrackSizingFunction', function()
        describe('Constructors', function()
            it('Single', function()
                local tsf = t.TrackSizingFunction.Single(
                    t.NonRepeatedTrackSizingFunction.new(
                        t.MinTrackSizingFunction.MinContent(),
                        t.MaxTrackSizingFunction.MaxContent()
                    )
                )

                expect( tsf ).to.exist()
                expect( tsf:get_single_func() ).to.be(
                    t.NonRepeatedTrackSizingFunction.new(
                        t.MinTrackSizingFunction.MinContent(),
                        t.MaxTrackSizingFunction.MaxContent()
                    )
                )
            end)

            it('Repeat', function()
                local tsf = t.TrackSizingFunction.Repeat(
                    t.GridTrackRepetition.AutoFill(),
                    {
                        t.NonRepeatedTrackSizingFunction.new(
                            t.MinTrackSizingFunction.MinContent(),
                            t.MaxTrackSizingFunction.MaxContent()
                        ),
                        t.NonRepeatedTrackSizingFunction.new(
                            t.MinTrackSizingFunction.ZERO(),
                            t.MaxTrackSizingFunction.Fraction(42)
                        ),
                        t.NonRepeatedTrackSizingFunction.new(
                            t.MinTrackSizingFunction.from_length(42),
                            t.MaxTrackSizingFunction.from_percent(42)
                        )
                    }
                )

                expect( tsf ).to.exist()
                expect( tsf:get_repetition() ).to.be(
                    t.GridTrackRepetition.AutoFill()
                )

                -- NOTE: `.to.equal()` instead of `.to.be()` to not compare
                -- table pointers, but only compare tables content
                expect( tsf:get_repeat_funcs() ).to.equal(
                    {
                        t.NonRepeatedTrackSizingFunction.new(
                            t.MinTrackSizingFunction.MinContent(),
                            t.MaxTrackSizingFunction.MaxContent()
                        ),
                        t.NonRepeatedTrackSizingFunction.new(
                            t.MinTrackSizingFunction.ZERO(),
                            t.MaxTrackSizingFunction.Fraction(42)
                        ),
                        t.NonRepeatedTrackSizingFunction.new(
                            t.MinTrackSizingFunction.from_length(42),
                            t.MaxTrackSizingFunction.from_percent(42)
                        )
                    }
                )
            end)

            it('Named constructors', function()
                local tsf1 = t.TrackSizingFunction.AUTO()

                expect( tsf1:is_Single() ).to.be( true )
                expect( tsf1:get_single_func() ).to.be( t.NonRepeatedTrackSizingFunction.AUTO() )

                local tsf2 = t.TrackSizingFunction.MIN_CONTENT()

                expect( tsf2:is_Single() ).to.be( true )
                expect( tsf2:get_single_func() ).to.be( t.NonRepeatedTrackSizingFunction.MIN_CONTENT() )

                local tsf3 = t.TrackSizingFunction.MAX_CONTENT()

                expect( tsf3:is_Single() ).to.be( true )
                expect( tsf3:get_single_func() ).to.be( t.NonRepeatedTrackSizingFunction.MAX_CONTENT() )

                local tsf4 = t.TrackSizingFunction.ZERO()

                expect( tsf4:is_Single() ).to.be( true )
                expect( tsf4:get_single_func() ).to.be( t.NonRepeatedTrackSizingFunction.ZERO() )

                local tsf5 = t.TrackSizingFunction.fit_content( t.LengthPercentage.Length(42) )

                expect( tsf5:is_Single() ).to.be( true )
                expect( tsf5:get_single_func() ).to.be( t.NonRepeatedTrackSizingFunction.fit_content(t.LengthPercentage.Length(42)) )

                local tsf6 = t.TrackSizingFunction.from_length(42)

                expect( tsf6:is_Single() ).to.be( true )
                expect( tsf6:get_single_func() ).to.be( t.NonRepeatedTrackSizingFunction.from_length(42) )

                local tsf7 = t.TrackSizingFunction.from_percent(42)

                expect( tsf7:is_Single() ).to.be( true )
                expect( tsf7:get_single_func() ).to.be( t.NonRepeatedTrackSizingFunction.from_percent(42) )

                local tsf8 = t.TrackSizingFunction.from_flex(42)

                expect( tsf8:is_Single() ).to.be( true )
                expect( tsf8:get_single_func() ).to.be( t.NonRepeatedTrackSizingFunction.from_flex(42) )
            end)
        end)

        it('Copying', function()
            local tsf = t.TrackSizingFunction.Repeat(
                t.GridTrackRepetition.AutoFill(),
                {
                    t.NonRepeatedTrackSizingFunction.new(
                        t.MinTrackSizingFunction.MinContent(),
                        t.MaxTrackSizingFunction.MaxContent()
                    ),
                    t.NonRepeatedTrackSizingFunction.new(
                        t.MinTrackSizingFunction.ZERO(),
                        t.MaxTrackSizingFunction.Fraction(42)
                    ),
                    t.NonRepeatedTrackSizingFunction.new(
                        t.MinTrackSizingFunction.from_length(42),
                        t.MaxTrackSizingFunction.from_percent(42)
                    )
                }
            )

            local copy = tsf:copy()

            expect( copy ).to.exist()
            expect( copy:get_repetition() ).to.be(
                t.GridTrackRepetition.AutoFill()
            )

            -- NOTE: `.to.equal()` instead of `.to.be()` to not compare
            -- table pointers, but only compare tables content
            expect( copy:get_repeat_funcs() ).to.equal(
                {
                    t.NonRepeatedTrackSizingFunction.new(
                        t.MinTrackSizingFunction.MinContent(),
                        t.MaxTrackSizingFunction.MaxContent()
                    ),
                    t.NonRepeatedTrackSizingFunction.new(
                        t.MinTrackSizingFunction.ZERO(),
                        t.MaxTrackSizingFunction.Fraction(42)
                    ),
                    t.NonRepeatedTrackSizingFunction.new(
                        t.MinTrackSizingFunction.from_length(42),
                        t.MaxTrackSizingFunction.from_percent(42)
                    )
                }
            )
        end)

        describe('Operators', function()
            it('Comparison', function()
                local tsf1 = t.TrackSizingFunction.Single(
                    t.NonRepeatedTrackSizingFunction.new(
                        t.MinTrackSizingFunction.MinContent(),
                        t.MaxTrackSizingFunction.MaxContent()
                    )
                )

                local tsf2 = t.TrackSizingFunction.Repeat(
                    t.GridTrackRepetition.AutoFill(),
                    {
                        t.NonRepeatedTrackSizingFunction.new(
                            t.MinTrackSizingFunction.MinContent(),
                            t.MaxTrackSizingFunction.MaxContent()
                        )
                    }
                )

                local equal1 = tsf1 == t.TrackSizingFunction.Single(
                    t.NonRepeatedTrackSizingFunction.new(
                        t.MinTrackSizingFunction.MinContent(),
                        t.MaxTrackSizingFunction.MaxContent()
                    )
                )

                local equal2 = tsf2 == t.TrackSizingFunction.Repeat(
                    t.GridTrackRepetition.AutoFill(),
                    {
                        t.NonRepeatedTrackSizingFunction.new(
                            t.MinTrackSizingFunction.MinContent(),
                            t.MaxTrackSizingFunction.MaxContent()
                        )
                    }
                )

                local not_equal1 = tsf1 ~= t.TrackSizingFunction.Single(
                    t.NonRepeatedTrackSizingFunction.new(
                        t.MinTrackSizingFunction.MaxContent(),
                        t.MaxTrackSizingFunction.MinContent()
                    )
                )

                local not_equal2 = tsf2 ~= t.TrackSizingFunction.Repeat(
                    t.GridTrackRepetition.AutoFit(),
                    {
                        t.NonRepeatedTrackSizingFunction.new(
                            t.MinTrackSizingFunction.MinContent(),
                            t.MaxTrackSizingFunction.MaxContent()
                        )
                    }
                )

                local not_equal3 = tsf2 ~= t.TrackSizingFunction.Repeat(
                    t.GridTrackRepetition.AutoFill(),
                    {
                        t.NonRepeatedTrackSizingFunction.new(
                            t.MinTrackSizingFunction.MaxContent(),
                            t.MaxTrackSizingFunction.MinContent()
                        )
                    }
                )

                expect( equal1 ).to.be( true )
                expect( equal2 ).to.be( true )

                expect( not_equal1 ).to.be( true )
                expect( not_equal2 ).to.be( true )
                expect( not_equal3 ).to.be( true )
            end)
        end)

        it('Type checking', function()
            local tsf1 = t.TrackSizingFunction.Single(
                t.NonRepeatedTrackSizingFunction.new(
                    t.MinTrackSizingFunction.MinContent(),
                    t.MaxTrackSizingFunction.MaxContent()
                )
            )

            expect( tsf1:is_Single() ).to.be( true )
            expect( tsf1:is_Repeat() ).to.be( false )

            local tsf2 = t.TrackSizingFunction.Repeat(
                t.GridTrackRepetition.AutoFill(),
                {
                    t.NonRepeatedTrackSizingFunction.new(
                        t.MinTrackSizingFunction.MinContent(),
                        t.MaxTrackSizingFunction.MaxContent()
                    )
                }
            )

            expect( tsf2:is_Single() ).to.be( false )
            expect( tsf2:is_Repeat() ).to.be( true )
        end)
    end) -- TrackSizingFunction

    describe('Display', function()
        it('Values', function()
            expect( t.Display.Flex    ).to.exist()
            expect( t.Display.Grid    ).to.exist()
            expect( t.Display.Block   ).to.exist()
            expect( t.Display.None    ).to.exist()
            expect( t.Display.Default ).to.exist()

            expect( t.Display.Flex    :to_int() ).to.be( 0 )
            expect( t.Display.Grid    :to_int() ).to.be( 1 )
            expect( t.Display.Block   :to_int() ).to.be( 2 )
            expect( t.Display.None    :to_int() ).to.be( 3 )
            expect( t.Display.Default :to_int() ).to.be( 0 )

            expect( t.Display.from_int(0) == t.Display.Flex    ).to.be( true )
            expect( t.Display.from_int(1) == t.Display.Grid    ).to.be( true )
            expect( t.Display.from_int(2) == t.Display.Block   ).to.be( true )
            expect( t.Display.from_int(3) == t.Display.None    ).to.be( true )
            expect( t.Display.from_int(0) == t.Display.Default ).to.be( true )
        end)

        it('Strictness check', function()
            expect( t.Display.Wrong ).to.be(nil)

            expect( function() t.Display.Wrong = 42; return true end ).to.fail()

            expect( function() t.Display.Flex = 42; return true end ).to.fail()

            expect( t.Display.Flex.x ).to.be(nil)

            expect( function() t.Display.Flex.x = 42; return true end ).to.fail()
        end)
    end) -- Display

    describe('Overflow', function()
        it('Values', function()
            expect( t.Overflow.Visible ).to.exist()
            expect( t.Overflow.Hidden  ).to.exist()
            expect( t.Overflow.Scroll  ).to.exist()
            expect( t.Overflow.Default ).to.exist()

            expect( t.Overflow.Visible :to_int() ).to.be( 0 )
            expect( t.Overflow.Hidden  :to_int() ).to.be( 1 )
            expect( t.Overflow.Scroll  :to_int() ).to.be( 2 )
            expect( t.Overflow.Default :to_int() ).to.be( 0 )

            expect( t.Overflow.from_int(0) == t.Overflow.Visible ).to.be( true )
            expect( t.Overflow.from_int(1) == t.Overflow.Hidden  ).to.be( true )
            expect( t.Overflow.from_int(2) == t.Overflow.Scroll  ).to.be( true )
            expect( t.Overflow.from_int(0) == t.Overflow.Default ).to.be( true )
        end)

        it('Strictness check', function()
            expect( t.Overflow.Wrong ).to.be(nil)

            expect( function() t.Overflow.Wrong = 42; return true end ).to.fail()

            expect( function() t.Overflow.Visible = 42; return true end ).to.fail()

            expect( t.Overflow.Visible.x ).to.be(nil)

            expect( function() t.Overflow.Visible.x = 42; return true end ).to.fail()
        end)
    end) -- Overflow

    describe('Position', function()
        it('Values', function()
            expect( t.Position.Relative ).to.exist()
            expect( t.Position.Absolute ).to.exist()
            expect( t.Position.Default  ).to.exist()

            expect( t.Position.Relative :to_int() ).to.be( 0 )
            expect( t.Position.Absolute :to_int() ).to.be( 1 )
            expect( t.Position.Default  :to_int() ).to.be( 0 )

            expect( t.Position.from_int(0) == t.Position.Relative ).to.be( true )
            expect( t.Position.from_int(1) == t.Position.Absolute ).to.be( true )
            expect( t.Position.from_int(0) == t.Position.Default  ).to.be( true )
        end)

        it('Strictness check', function()
            expect( t.Position.Wrong ).to.be(nil)

            expect( function() t.Position.Wrong = 42; return true end ).to.fail()

            expect( function() t.Position.Relative = 42; return true end ).to.fail()

            expect( t.Position.Relative.x ).to.be(nil)

            expect( function() t.Position.Relative.x = 42; return true end ).to.fail()
        end)
    end) -- Position

    describe('Point_of_Overflow', function()
        describe('Constructors', function()
            it('common', function()
                local point = t.Point_of_Overflow.new(t.Overflow.Visible, t.Overflow.Hidden)

                expect( point ).to.exist()
                expect( point:get_x() ).to.be( t.Overflow.Visible )
                expect( point:get_y() ).to.be( t.Overflow.Hidden )
            end)

            it('table (array)', function()
                local point = t.Point_of_Overflow.new({t.Overflow.Visible, t.Overflow.Hidden})

                expect( point ).to.exist()
                expect( point:get_x() ).to.be( t.Overflow.Visible )
                expect( point:get_y() ).to.be( t.Overflow.Hidden )
            end)

            it('table (array) explicit indexes', function()
                local point1 = t.Point_of_Overflow.new({[1] = t.Overflow.Visible, [2] = t.Overflow.Hidden})

                expect( point1 ).to.exist()
                expect( point1:get_x() ).to.be( t.Overflow.Visible )
                expect( point1:get_y() ).to.be( t.Overflow.Hidden )

                local point2 = t.Point_of_Overflow.new({[2] = t.Overflow.Hidden, [1] = t.Overflow.Visible})

                expect( point2 ).to.exist()
                expect( point2:get_x() ).to.be( t.Overflow.Visible )
                expect( point2:get_y() ).to.be( t.Overflow.Hidden )
            end)

            it('table (dictionary)', function()
                local point1 = t.Point_of_Overflow.new({x = t.Overflow.Visible, y = t.Overflow.Hidden})

                expect( point1 ).to.exist()
                expect( point1:get_x() ).to.be( t.Overflow.Visible )
                expect( point1:get_y() ).to.be( t.Overflow.Hidden )

                local point2 = t.Point_of_Overflow.new({y = t.Overflow.Hidden, x = t.Overflow.Visible})

                expect( point2 ).to.exist()
                expect( point2:get_x() ).to.be( t.Overflow.Visible )
                expect( point2:get_y() ).to.be( t.Overflow.Hidden )
            end)

            it('table (without parentheses)', function()
                local point1 = t.Point_of_Overflow.new {t.Overflow.Visible, t.Overflow.Hidden}

                expect( point1 ).to.exist()
                expect( point1:get_x() ).to.be( t.Overflow.Visible )
                expect( point1:get_y() ).to.be( t.Overflow.Hidden )

                local point2 = t.Point_of_Overflow.new {x = t.Overflow.Visible, y = t.Overflow.Hidden}

                expect( point2 ).to.exist()
                expect( point2:get_x() ).to.be( t.Overflow.Visible )
                expect( point2:get_y() ).to.be( t.Overflow.Hidden )
            end)
        end)

        it('Copying', function()
            local point = t.Point_of_Overflow.new(t.Overflow.Visible, t.Overflow.Hidden)

            local copy = point:copy()

            expect( copy ).to.exist()
            expect( copy:get_x() ).to.be( t.Overflow.Visible )
            expect( copy:get_y() ).to.be( t.Overflow.Hidden )
        end)

        describe('Operators', function()
            it('Comparison', function()
                local point = t.Point_of_Overflow.new(t.Overflow.Visible, t.Overflow.Hidden)

                local points_equal     = point == t.Point_of_Overflow.new(t.Overflow.Visible, t.Overflow.Hidden)
                local points_not_equal = point ~= t.Point_of_Overflow.new(t.Overflow.Hidden, t.Overflow.Visible)

                expect( points_equal     ).to.be( true )
                expect( points_not_equal ).to.be( true )
            end)
        end)

        it('Getters/Setters', function()
            local point = t.Point_of_Overflow.new(t.Overflow.Visible, t.Overflow.Hidden)

            expect( point ).to.exist()
            expect( point:get_x() ).to.be( t.Overflow.Visible )
            expect( point:get_y() ).to.be( t.Overflow.Hidden )

            point:set_x(t.Overflow.Hidden)
            point:set_y(t.Overflow.Scroll)

            expect( point:get_x() ).to.be( t.Overflow.Hidden )
            expect( point:get_y() ).to.be( t.Overflow.Scroll )
        end)

        it('Properties', function()
            local point = t.Point_of_Overflow.new(t.Overflow.Visible, t.Overflow.Hidden)

            expect( point ).to.exist()
            expect( point.x ).to.be( t.Overflow.Visible )
            expect( point.y ).to.be( t.Overflow.Hidden )

            point.x = t.Overflow.Hidden
            point.y = t.Overflow.Scroll

            expect( point.x ).to.be( t.Overflow.Hidden )
            expect( point.y ).to.be( t.Overflow.Scroll )
        end)
    end) -- Point_of_Overflow

    describe('Rect_of_LengthPercentage', function()
        describe('Constructors', function()
            it('common', function()
                local rect = t.Rect_of_LengthPercentage.new(
                    t.LengthPercentage.Length (10),
                    t.LengthPercentage.Percent(20),
                    t.LengthPercentage.Length (30),
                    t.LengthPercentage.Percent(40)
                )

                expect( rect ).to.exist()
                expect( rect:get_left()   ).to.be( t.LengthPercentage.Length (10) )
                expect( rect:get_right()  ).to.be( t.LengthPercentage.Percent(20) )
                expect( rect:get_top()    ).to.be( t.LengthPercentage.Length (30) )
                expect( rect:get_bottom() ).to.be( t.LengthPercentage.Percent(40) )
            end)

            it('table (array)', function()
                local rect = t.Rect_of_LengthPercentage.new({
                    t.LengthPercentage.Length (10),
                    t.LengthPercentage.Percent(20),
                    t.LengthPercentage.Length (30),
                    t.LengthPercentage.Percent(40)
                })

                expect( rect ).to.exist()
                expect( rect:get_left()   ).to.be( t.LengthPercentage.Length (10) )
                expect( rect:get_right()  ).to.be( t.LengthPercentage.Percent(20) )
                expect( rect:get_top()    ).to.be( t.LengthPercentage.Length (30) )
                expect( rect:get_bottom() ).to.be( t.LengthPercentage.Percent(40) )
            end)

            it('table (array) explicit indexes', function()
                local rect1 = t.Rect_of_LengthPercentage.new({
                    [1] = t.LengthPercentage.Length (10),
                    [2] = t.LengthPercentage.Percent(20),
                    [3] = t.LengthPercentage.Length (30),
                    [4] = t.LengthPercentage.Percent(40)
                })

                expect( rect1 ).to.exist()
                expect( rect1:get_left()   ).to.be( t.LengthPercentage.Length (10) )
                expect( rect1:get_right()  ).to.be( t.LengthPercentage.Percent(20) )
                expect( rect1:get_top()    ).to.be( t.LengthPercentage.Length (30) )
                expect( rect1:get_bottom() ).to.be( t.LengthPercentage.Percent(40) )

                local rect2 = t.Rect_of_LengthPercentage.new({
                    [2] = t.LengthPercentage.Percent(20),
                    [1] = t.LengthPercentage.Length (10),
                    [4] = t.LengthPercentage.Percent(40),
                    [3] = t.LengthPercentage.Length (30)
                })

                expect( rect2 ).to.exist()
                expect( rect2:get_left()   ).to.be( t.LengthPercentage.Length (10) )
                expect( rect2:get_right()  ).to.be( t.LengthPercentage.Percent(20) )
                expect( rect2:get_top()    ).to.be( t.LengthPercentage.Length (30) )
                expect( rect2:get_bottom() ).to.be( t.LengthPercentage.Percent(40) )
            end)

            it('table (dictionary)', function()
                local rect1 = t.Rect_of_LengthPercentage.new({
                    left   = t.LengthPercentage.Length (10),
                    right  = t.LengthPercentage.Percent(20),
                    top    = t.LengthPercentage.Length (30),
                    bottom = t.LengthPercentage.Percent(40)
                })

                expect( rect1 ).to.exist()
                expect( rect1:get_left()   ).to.be( t.LengthPercentage.Length (10) )
                expect( rect1:get_right()  ).to.be( t.LengthPercentage.Percent(20) )
                expect( rect1:get_top()    ).to.be( t.LengthPercentage.Length (30) )
                expect( rect1:get_bottom() ).to.be( t.LengthPercentage.Percent(40) )

                local rect2 = t.Rect_of_LengthPercentage.new({
                    right  = t.LengthPercentage.Percent(20),
                    left   = t.LengthPercentage.Length (10),
                    bottom = t.LengthPercentage.Percent(40),
                    top    = t.LengthPercentage.Length (30)
                })

                expect( rect2 ).to.exist()
                expect( rect2:get_left()   ).to.be( t.LengthPercentage.Length (10) )
                expect( rect2:get_right()  ).to.be( t.LengthPercentage.Percent(20) )
                expect( rect2:get_top()    ).to.be( t.LengthPercentage.Length (30) )
                expect( rect2:get_bottom() ).to.be( t.LengthPercentage.Percent(40) )
            end)

            it('table (without parentheses)', function()
                local rect1 = t.Rect_of_LengthPercentage.new {
                    left   = t.LengthPercentage.Length (10),
                    right  = t.LengthPercentage.Percent(20),
                    top    = t.LengthPercentage.Length (30),
                    bottom = t.LengthPercentage.Percent(40)
                }

                expect( rect1 ).to.exist()
                expect( rect1:get_left()   ).to.be( t.LengthPercentage.Length (10) )
                expect( rect1:get_right()  ).to.be( t.LengthPercentage.Percent(20) )
                expect( rect1:get_top()    ).to.be( t.LengthPercentage.Length (30) )
                expect( rect1:get_bottom() ).to.be( t.LengthPercentage.Percent(40) )

                local rect2 = t.Rect_of_LengthPercentage.new {
                    left   = t.LengthPercentage.Length (10),
                    right  = t.LengthPercentage.Percent(20),
                    top    = t.LengthPercentage.Length (30),
                    bottom = t.LengthPercentage.Percent(40)
                }

                expect( rect2 ).to.exist()
                expect( rect2:get_left()   ).to.be( t.LengthPercentage.Length (10) )
                expect( rect2:get_right()  ).to.be( t.LengthPercentage.Percent(20) )
                expect( rect2:get_top()    ).to.be( t.LengthPercentage.Length (30) )
                expect( rect2:get_bottom() ).to.be( t.LengthPercentage.Percent(40) )
            end)
        end)

        it('Copying', function()
            local rect = t.Rect_of_LengthPercentage.new(
                t.LengthPercentage.Length (10),
                t.LengthPercentage.Percent(20),
                t.LengthPercentage.Length (30),
                t.LengthPercentage.Percent(40)
            )

            local copy = rect:copy()

            expect( copy ).to.exist()
            expect( copy:get_left()   ).to.be( t.LengthPercentage.Length (10) )
            expect( copy:get_right()  ).to.be( t.LengthPercentage.Percent(20) )
            expect( copy:get_top()    ).to.be( t.LengthPercentage.Length (30) )
            expect( copy:get_bottom() ).to.be( t.LengthPercentage.Percent(40) )
        end)

        describe('Operators', function()
            it('Comparison', function()
                local rect = t.Rect_of_LengthPercentage.new(
                    t.LengthPercentage.Length (10),
                    t.LengthPercentage.Percent(20),
                    t.LengthPercentage.Length (30),
                    t.LengthPercentage.Percent(40)
                )

                local rects_equal     = rect == t.Rect_of_LengthPercentage.new(
                    t.LengthPercentage.Length (10),
                    t.LengthPercentage.Percent(20),
                    t.LengthPercentage.Length (30),
                    t.LengthPercentage.Percent(40)
                )

                local rects_not_equal = rect ~= t.Rect_of_LengthPercentage.new(
                    t.LengthPercentage.Percent(11),
                    t.LengthPercentage.Length (22),
                    t.LengthPercentage.Percent(33),
                    t.LengthPercentage.Length (44)
                )

                expect( rects_equal     ).to.be( true )
                expect( rects_not_equal ).to.be( true )
            end)
        end)

        it('Getters/Setters', function()
            local rect = t.Rect_of_LengthPercentage.new(
                t.LengthPercentage.Length (10),
                t.LengthPercentage.Percent(20),
                t.LengthPercentage.Length (30),
                t.LengthPercentage.Percent(40)
            )

            expect( rect ).to.exist()
            expect( rect:get_left()   ).to.be( t.LengthPercentage.Length (10) )
            expect( rect:get_right()  ).to.be( t.LengthPercentage.Percent(20) )
            expect( rect:get_top()    ).to.be( t.LengthPercentage.Length (30) )
            expect( rect:get_bottom() ).to.be( t.LengthPercentage.Percent(40) )

            rect:set_left  ( t.LengthPercentage.Percent(11) )
            rect:set_right ( t.LengthPercentage.Length (22) )
            rect:set_top   ( t.LengthPercentage.Percent(33) )
            rect:set_bottom( t.LengthPercentage.Length (44) )

            expect( rect:get_left()   ).to.be( t.LengthPercentage.Percent(11) )
            expect( rect:get_right()  ).to.be( t.LengthPercentage.Length (22) )
            expect( rect:get_top()    ).to.be( t.LengthPercentage.Percent(33) )
            expect( rect:get_bottom() ).to.be( t.LengthPercentage.Length (44) )
        end)

        it('Properties', function()
            local rect = t.Rect_of_LengthPercentage.new(
                t.LengthPercentage.Length (10),
                t.LengthPercentage.Percent(20),
                t.LengthPercentage.Length (30),
                t.LengthPercentage.Percent(40)
            )

            expect( rect ).to.exist()
            expect( rect.left   ).to.be( t.LengthPercentage.Length (10) )
            expect( rect.right  ).to.be( t.LengthPercentage.Percent(20) )
            expect( rect.top    ).to.be( t.LengthPercentage.Length (30) )
            expect( rect.bottom ).to.be( t.LengthPercentage.Percent(40) )

            rect.left   = t.LengthPercentage.Percent(11)
            rect.right  = t.LengthPercentage.Length (22)
            rect.top    = t.LengthPercentage.Percent(33)
            rect.bottom = t.LengthPercentage.Length (44)

            expect( rect.left   ).to.be( t.LengthPercentage.Percent(11) )
            expect( rect.right  ).to.be( t.LengthPercentage.Length (22) )
            expect( rect.top    ).to.be( t.LengthPercentage.Percent(33) )
            expect( rect.bottom ).to.be( t.LengthPercentage.Length (44) )
        end)
    end) -- Rect_of_LengthPercentage

    describe('Rect_of_LengthPercentageAuto', function()
        describe('Constructors', function()
            it('common', function()
                local rect = t.Rect_of_LengthPercentageAuto.new(
                    t.LengthPercentageAuto.Length (10),
                    t.LengthPercentageAuto.Percent(20),
                    t.LengthPercentageAuto.Length (30),
                    t.LengthPercentageAuto.Percent(40)
                )

                expect( rect ).to.exist()
                expect( rect:get_left()   ).to.be( t.LengthPercentageAuto.Length (10) )
                expect( rect:get_right()  ).to.be( t.LengthPercentageAuto.Percent(20) )
                expect( rect:get_top()    ).to.be( t.LengthPercentageAuto.Length (30) )
                expect( rect:get_bottom() ).to.be( t.LengthPercentageAuto.Percent(40) )
            end)

            it('table (array)', function()
                local rect = t.Rect_of_LengthPercentageAuto.new({
                    t.LengthPercentageAuto.Length (10),
                    t.LengthPercentageAuto.Percent(20),
                    t.LengthPercentageAuto.Length (30),
                    t.LengthPercentageAuto.Percent(40)
                })

                expect( rect ).to.exist()
                expect( rect:get_left()   ).to.be( t.LengthPercentageAuto.Length (10) )
                expect( rect:get_right()  ).to.be( t.LengthPercentageAuto.Percent(20) )
                expect( rect:get_top()    ).to.be( t.LengthPercentageAuto.Length (30) )
                expect( rect:get_bottom() ).to.be( t.LengthPercentageAuto.Percent(40) )
            end)

            it('table (array) explicit indexes', function()
                local rect1 = t.Rect_of_LengthPercentageAuto.new({
                    [1] = t.LengthPercentageAuto.Length (10),
                    [2] = t.LengthPercentageAuto.Percent(20),
                    [3] = t.LengthPercentageAuto.Length (30),
                    [4] = t.LengthPercentageAuto.Percent(40)
                })

                expect( rect1 ).to.exist()
                expect( rect1:get_left()   ).to.be( t.LengthPercentageAuto.Length (10) )
                expect( rect1:get_right()  ).to.be( t.LengthPercentageAuto.Percent(20) )
                expect( rect1:get_top()    ).to.be( t.LengthPercentageAuto.Length (30) )
                expect( rect1:get_bottom() ).to.be( t.LengthPercentageAuto.Percent(40) )

                local rect2 = t.Rect_of_LengthPercentageAuto.new({
                    [2] = t.LengthPercentageAuto.Percent(20),
                    [1] = t.LengthPercentageAuto.Length (10),
                    [4] = t.LengthPercentageAuto.Percent(40),
                    [3] = t.LengthPercentageAuto.Length (30)
                })

                expect( rect2 ).to.exist()
                expect( rect2:get_left()   ).to.be( t.LengthPercentageAuto.Length (10) )
                expect( rect2:get_right()  ).to.be( t.LengthPercentageAuto.Percent(20) )
                expect( rect2:get_top()    ).to.be( t.LengthPercentageAuto.Length (30) )
                expect( rect2:get_bottom() ).to.be( t.LengthPercentageAuto.Percent(40) )
            end)

            it('table (dictionary)', function()
                local rect1 = t.Rect_of_LengthPercentageAuto.new({
                    left   = t.LengthPercentageAuto.Length (10),
                    right  = t.LengthPercentageAuto.Percent(20),
                    top    = t.LengthPercentageAuto.Length (30),
                    bottom = t.LengthPercentageAuto.Percent(40)
                })

                expect( rect1 ).to.exist()
                expect( rect1:get_left()   ).to.be( t.LengthPercentageAuto.Length (10) )
                expect( rect1:get_right()  ).to.be( t.LengthPercentageAuto.Percent(20) )
                expect( rect1:get_top()    ).to.be( t.LengthPercentageAuto.Length (30) )
                expect( rect1:get_bottom() ).to.be( t.LengthPercentageAuto.Percent(40) )

                local rect2 = t.Rect_of_LengthPercentageAuto.new({
                    right  = t.LengthPercentageAuto.Percent(20),
                    left   = t.LengthPercentageAuto.Length (10),
                    bottom = t.LengthPercentageAuto.Percent(40),
                    top    = t.LengthPercentageAuto.Length (30)
                })

                expect( rect2 ).to.exist()
                expect( rect2:get_left()   ).to.be( t.LengthPercentageAuto.Length (10) )
                expect( rect2:get_right()  ).to.be( t.LengthPercentageAuto.Percent(20) )
                expect( rect2:get_top()    ).to.be( t.LengthPercentageAuto.Length (30) )
                expect( rect2:get_bottom() ).to.be( t.LengthPercentageAuto.Percent(40) )
            end)

            it('table (without parentheses)', function()
                local rect1 = t.Rect_of_LengthPercentageAuto.new {
                    left   = t.LengthPercentageAuto.Length (10),
                    right  = t.LengthPercentageAuto.Percent(20),
                    top    = t.LengthPercentageAuto.Length (30),
                    bottom = t.LengthPercentageAuto.Percent(40)
                }

                expect( rect1 ).to.exist()
                expect( rect1:get_left()   ).to.be( t.LengthPercentageAuto.Length (10) )
                expect( rect1:get_right()  ).to.be( t.LengthPercentageAuto.Percent(20) )
                expect( rect1:get_top()    ).to.be( t.LengthPercentageAuto.Length (30) )
                expect( rect1:get_bottom() ).to.be( t.LengthPercentageAuto.Percent(40) )

                local rect2 = t.Rect_of_LengthPercentageAuto.new {
                    left   = t.LengthPercentageAuto.Length (10),
                    right  = t.LengthPercentageAuto.Percent(20),
                    top    = t.LengthPercentageAuto.Length (30),
                    bottom = t.LengthPercentageAuto.Percent(40)
                }

                expect( rect2 ).to.exist()
                expect( rect2:get_left()   ).to.be( t.LengthPercentageAuto.Length (10) )
                expect( rect2:get_right()  ).to.be( t.LengthPercentageAuto.Percent(20) )
                expect( rect2:get_top()    ).to.be( t.LengthPercentageAuto.Length (30) )
                expect( rect2:get_bottom() ).to.be( t.LengthPercentageAuto.Percent(40) )
            end)
        end)

        it('Copying', function()
            local rect = t.Rect_of_LengthPercentageAuto.new(
                t.LengthPercentageAuto.Length (10),
                t.LengthPercentageAuto.Percent(20),
                t.LengthPercentageAuto.Length (30),
                t.LengthPercentageAuto.Percent(40)
            )

            local copy = rect:copy()

            expect( copy ).to.exist()
            expect( copy:get_left()   ).to.be( t.LengthPercentageAuto.Length (10) )
            expect( copy:get_right()  ).to.be( t.LengthPercentageAuto.Percent(20) )
            expect( copy:get_top()    ).to.be( t.LengthPercentageAuto.Length (30) )
            expect( copy:get_bottom() ).to.be( t.LengthPercentageAuto.Percent(40) )
        end)

        describe('Operators', function()
            it('Comparison', function()
                local rect = t.Rect_of_LengthPercentageAuto.new(
                    t.LengthPercentageAuto.Length (10),
                    t.LengthPercentageAuto.Percent(20),
                    t.LengthPercentageAuto.Length (30),
                    t.LengthPercentageAuto.Percent(40)
                )

                local rects_equal     = rect == t.Rect_of_LengthPercentageAuto.new(
                    t.LengthPercentageAuto.Length (10),
                    t.LengthPercentageAuto.Percent(20),
                    t.LengthPercentageAuto.Length (30),
                    t.LengthPercentageAuto.Percent(40)
                )

                local rects_not_equal = rect ~= t.Rect_of_LengthPercentageAuto.new(
                    t.LengthPercentageAuto.Percent(11),
                    t.LengthPercentageAuto.Length (22),
                    t.LengthPercentageAuto.Percent(33),
                    t.LengthPercentageAuto.Length (44)
                )

                expect( rects_equal     ).to.be( true )
                expect( rects_not_equal ).to.be( true )
            end)
        end)

        it('Getters/Setters', function()
            local rect = t.Rect_of_LengthPercentageAuto.new(
                t.LengthPercentageAuto.Length (10),
                t.LengthPercentageAuto.Percent(20),
                t.LengthPercentageAuto.Length (30),
                t.LengthPercentageAuto.Percent(40)
            )

            expect( rect ).to.exist()
            expect( rect:get_left()   ).to.be( t.LengthPercentageAuto.Length (10) )
            expect( rect:get_right()  ).to.be( t.LengthPercentageAuto.Percent(20) )
            expect( rect:get_top()    ).to.be( t.LengthPercentageAuto.Length (30) )
            expect( rect:get_bottom() ).to.be( t.LengthPercentageAuto.Percent(40) )

            rect:set_left  ( t.LengthPercentageAuto.Percent(11) )
            rect:set_right ( t.LengthPercentageAuto.Length (22) )
            rect:set_top   ( t.LengthPercentageAuto.Percent(33) )
            rect:set_bottom( t.LengthPercentageAuto.Length (44) )

            expect( rect:get_left()   ).to.be( t.LengthPercentageAuto.Percent(11) )
            expect( rect:get_right()  ).to.be( t.LengthPercentageAuto.Length (22) )
            expect( rect:get_top()    ).to.be( t.LengthPercentageAuto.Percent(33) )
            expect( rect:get_bottom() ).to.be( t.LengthPercentageAuto.Length (44) )
        end)

        it('Properties', function()
            local rect = t.Rect_of_LengthPercentageAuto.new(
                t.LengthPercentageAuto.Length (10),
                t.LengthPercentageAuto.Percent(20),
                t.LengthPercentageAuto.Length (30),
                t.LengthPercentageAuto.Percent(40)
            )

            expect( rect ).to.exist()
            expect( rect.left   ).to.be( t.LengthPercentageAuto.Length (10) )
            expect( rect.right  ).to.be( t.LengthPercentageAuto.Percent(20) )
            expect( rect.top    ).to.be( t.LengthPercentageAuto.Length (30) )
            expect( rect.bottom ).to.be( t.LengthPercentageAuto.Percent(40) )

            rect.left   = t.LengthPercentageAuto.Percent(11)
            rect.right  = t.LengthPercentageAuto.Length (22)
            rect.top    = t.LengthPercentageAuto.Percent(33)
            rect.bottom = t.LengthPercentageAuto.Length (44)

            expect( rect.left   ).to.be( t.LengthPercentageAuto.Percent(11) )
            expect( rect.right  ).to.be( t.LengthPercentageAuto.Length (22) )
            expect( rect.top    ).to.be( t.LengthPercentageAuto.Percent(33) )
            expect( rect.bottom ).to.be( t.LengthPercentageAuto.Length (44) )
        end)
    end) -- Rect_of_LengthPercentageAuto

    describe('Size_of_LengthPercentage', function()
        describe('Constructors', function()
            it('common', function()
                local size = t.Size_of_LengthPercentage.new(
                    t.LengthPercentage.Length (10),
                    t.LengthPercentage.Percent(20)
                )

                expect( size ).to.exist()
                expect( size:get_width()  ).to.be( t.LengthPercentage.Length (10) )
                expect( size:get_height() ).to.be( t.LengthPercentage.Percent(20) )
            end)

            it('table (array)', function()
                local size = t.Size_of_LengthPercentage.new({
                    t.LengthPercentage.Length (10),
                    t.LengthPercentage.Percent(20)
                })

                expect( size ).to.exist()
                expect( size:get_width()  ).to.be( t.LengthPercentage.Length (10) )
                expect( size:get_height() ).to.be( t.LengthPercentage.Percent(20) )
            end)

            it('table (array) explicit indexes', function()
                local size1 = t.Size_of_LengthPercentage.new({
                    [1] = t.LengthPercentage.Length (10),
                    [2] = t.LengthPercentage.Percent(20)
                })

                expect( size1 ).to.exist()
                expect( size1:get_width()  ).to.be( t.LengthPercentage.Length (10) )
                expect( size1:get_height() ).to.be( t.LengthPercentage.Percent(20) )

                local size2 = t.Size_of_LengthPercentage.new({
                    [2] = t.LengthPercentage.Percent(20),
                    [1] = t.LengthPercentage.Length (10)
                })

                expect( size2 ).to.exist()
                expect( size2:get_width()  ).to.be( t.LengthPercentage.Length (10) )
                expect( size2:get_height() ).to.be( t.LengthPercentage.Percent(20) )
            end)

            it('table (dictionary)', function()
                local size1 = t.Size_of_LengthPercentage.new({
                    width  = t.LengthPercentage.Length (10),
                    height = t.LengthPercentage.Percent(20)
                })

                expect( size1 ).to.exist()
                expect( size1:get_width()  ).to.be( t.LengthPercentage.Length (10) )
                expect( size1:get_height() ).to.be( t.LengthPercentage.Percent(20) )

                local size2 = t.Size_of_LengthPercentage.new({
                    height = t.LengthPercentage.Percent(20),
                    width  = t.LengthPercentage.Length (10)
                })

                expect( size2 ).to.exist()
                expect( size2:get_width()  ).to.be( t.LengthPercentage.Length (10) )
                expect( size2:get_height() ).to.be( t.LengthPercentage.Percent(20) )
            end)

            it('table (without parentheses)', function()
                local size1 = t.Size_of_LengthPercentage.new {
                    t.LengthPercentage.Length (10),
                    t.LengthPercentage.Percent(20)
                }

                expect( size1 ).to.exist()
                expect( size1:get_width()  ).to.be( t.LengthPercentage.Length (10) )
                expect( size1:get_height() ).to.be( t.LengthPercentage.Percent(20) )

                local size2 = t.Size_of_LengthPercentage.new {
                    width  = t.LengthPercentage.Length (10),
                    height = t.LengthPercentage.Percent(20)
                }

                expect( size2 ).to.exist()
                expect( size2:get_width()  ).to.be( t.LengthPercentage.Length (10) )
                expect( size2:get_height() ).to.be( t.LengthPercentage.Percent(20) )
            end)
        end)

        it('Copying', function()
            local size = t.Size_of_LengthPercentage.new(
                t.LengthPercentage.Length (10),
                t.LengthPercentage.Percent(20)
            )

            local copy = size:copy()

            expect( copy ).to.exist()
            expect( copy:get_width()  ).to.be( t.LengthPercentage.Length (10) )
            expect( copy:get_height() ).to.be( t.LengthPercentage.Percent(20) )
        end)

        describe('Operators', function()
            it('Comparison', function()
                local size = t.Size_of_LengthPercentage.new(
                    t.LengthPercentage.Length (10),
                    t.LengthPercentage.Percent(20)
                )

                local sizes_equal     = size == t.Size_of_LengthPercentage.new(
                    t.LengthPercentage.Length (10),
                    t.LengthPercentage.Percent(20)
                )
                local sizes_not_equal = size ~= t.Size_of_LengthPercentage.new(
                    t.LengthPercentage.Percent (11),
                    t.LengthPercentage.Length  (22)
                )

                expect( sizes_equal     ).to.be( true )
                expect( sizes_not_equal ).to.be( true )
            end)
        end)

        it('Getters/Setters', function()
            local size = t.Size_of_LengthPercentage.new(
                t.LengthPercentage.Length (10),
                t.LengthPercentage.Percent(20)
            )

            expect( size ).to.exist()
            expect( size:get_width()  ).to.be( t.LengthPercentage.Length (10) )
            expect( size:get_height() ).to.be( t.LengthPercentage.Percent(20) )

            size:set_width ( t.LengthPercentage.Percent(11) )
            size:set_height( t.LengthPercentage.Length (22) )

            expect( size:get_width()  ).to.be( t.LengthPercentage.Percent(11) )
            expect( size:get_height() ).to.be( t.LengthPercentage.Length (22) )
        end)

        it('Properties', function()
            local size = t.Size_of_LengthPercentage.new(
                t.LengthPercentage.Length (10),
                t.LengthPercentage.Percent(20)
            )

            expect( size ).to.exist()
            expect( size.width  ).to.be( t.LengthPercentage.Length (10) )
            expect( size.height ).to.be( t.LengthPercentage.Percent(20) )

            size.width  = t.LengthPercentage.Percent(11)
            size.height = t.LengthPercentage.Length (22)

            expect( size.width  ).to.be( t.LengthPercentage.Percent(11) )
            expect( size.height ).to.be( t.LengthPercentage.Length (22) )
        end)
    end) -- Size_of_LengthPercentage

    describe('Size_of_Dimension', function()
        describe('Constructors', function()
            it('common', function()
                local size = t.Size_of_Dimension.new(
                    t.Dimension.Length (10),
                    t.Dimension.Percent(20)
                )

                expect( size ).to.exist()
                expect( size:get_width()  ).to.be( t.Dimension.Length (10) )
                expect( size:get_height() ).to.be( t.Dimension.Percent(20) )
            end)

            it('table (array)', function()
                local size = t.Size_of_Dimension.new({
                    t.Dimension.Length (10),
                    t.Dimension.Percent(20)
                })

                expect( size ).to.exist()
                expect( size:get_width()  ).to.be( t.Dimension.Length (10) )
                expect( size:get_height() ).to.be( t.Dimension.Percent(20) )
            end)

            it('table (array) explicit indexes', function()
                local size1 = t.Size_of_Dimension.new({
                    [1] = t.Dimension.Length (10),
                    [2] = t.Dimension.Percent(20)
                })

                expect( size1 ).to.exist()
                expect( size1:get_width()  ).to.be( t.Dimension.Length (10) )
                expect( size1:get_height() ).to.be( t.Dimension.Percent(20) )

                local size2 = t.Size_of_Dimension.new({
                    [2] = t.Dimension.Percent(20),
                    [1] = t.Dimension.Length (10)
                })

                expect( size2 ).to.exist()
                expect( size2:get_width()  ).to.be( t.Dimension.Length (10) )
                expect( size2:get_height() ).to.be( t.Dimension.Percent(20) )
            end)

            it('table (dictionary)', function()
                local size1 = t.Size_of_Dimension.new({
                    width  = t.Dimension.Length (10),
                    height = t.Dimension.Percent(20)
                })

                expect( size1 ).to.exist()
                expect( size1:get_width()  ).to.be( t.Dimension.Length (10) )
                expect( size1:get_height() ).to.be( t.Dimension.Percent(20) )

                local size2 = t.Size_of_Dimension.new({
                    height = t.Dimension.Percent(20),
                    width  = t.Dimension.Length (10)
                })

                expect( size2 ).to.exist()
                expect( size2:get_width()  ).to.be( t.Dimension.Length (10) )
                expect( size2:get_height() ).to.be( t.Dimension.Percent(20) )
            end)

            it('table (without parentheses)', function()
                local size1 = t.Size_of_Dimension.new {
                    t.Dimension.Length (10),
                    t.Dimension.Percent(20)
                }

                expect( size1 ).to.exist()
                expect( size1:get_width()  ).to.be( t.Dimension.Length (10) )
                expect( size1:get_height() ).to.be( t.Dimension.Percent(20) )

                local size2 = t.Size_of_Dimension.new {
                    width  = t.Dimension.Length (10),
                    height = t.Dimension.Percent(20)
                }

                expect( size2 ).to.exist()
                expect( size2:get_width()  ).to.be( t.Dimension.Length (10) )
                expect( size2:get_height() ).to.be( t.Dimension.Percent(20) )
            end)
        end)

        it('Copying', function()
            local size = t.Size_of_Dimension.new(
                t.Dimension.Length (10),
                t.Dimension.Percent(20)
            )

            local copy = size:copy()

            expect( copy ).to.exist()
            expect( copy:get_width()  ).to.be( t.Dimension.Length (10) )
            expect( copy:get_height() ).to.be( t.Dimension.Percent(20) )
        end)

        describe('Operators', function()
            it('Comparison', function()
                local size = t.Size_of_Dimension.new(
                    t.Dimension.Length (10),
                    t.Dimension.Percent(20)
                )

                local sizes_equal     = size == t.Size_of_Dimension.new(
                    t.Dimension.Length (10),
                    t.Dimension.Percent(20)
                )
                local sizes_not_equal = size ~= t.Size_of_Dimension.new(
                    t.Dimension.Percent (11),
                    t.Dimension.Length  (22)
                )

                expect( sizes_equal     ).to.be( true )
                expect( sizes_not_equal ).to.be( true )
            end)
        end)

        it('Getters/Setters', function()
            local size = t.Size_of_Dimension.new(
                t.Dimension.Length (10),
                t.Dimension.Percent(20)
            )

            expect( size ).to.exist()
            expect( size:get_width()  ).to.be( t.Dimension.Length (10) )
            expect( size:get_height() ).to.be( t.Dimension.Percent(20) )

            size:set_width ( t.Dimension.Percent(11) )
            size:set_height( t.Dimension.Length (22) )

            expect( size:get_width()  ).to.be( t.Dimension.Percent(11) )
            expect( size:get_height() ).to.be( t.Dimension.Length (22) )
        end)

        it('Properties', function()
            local size = t.Size_of_Dimension.new(
                t.Dimension.Length (10),
                t.Dimension.Percent(20)
            )

            expect( size ).to.exist()
            expect( size.width  ).to.be( t.Dimension.Length (10) )
            expect( size.height ).to.be( t.Dimension.Percent(20) )

            size.width  = t.Dimension.Percent(11)
            size.height = t.Dimension.Length (22)

            expect( size.width  ).to.be( t.Dimension.Percent(11) )
            expect( size.height ).to.be( t.Dimension.Length (22) )
        end)
    end) -- Size_of_Dimension

    describe('Line_of_GridPlacement', function()
        describe('Constructors', function()
            it('default', function()
                local line = t.Line_of_GridPlacement.new()

                expect( line ).to.exist()
                expect( line:get_start() ).to.be( t.GridPlacement.Auto() )
                expect( line:get_end()   ).to.be( t.GridPlacement.Auto() )
            end)

            it('common', function()
                local line = t.Line_of_GridPlacement.new(
                    t.GridPlacement.Auto(),
                    t.GridPlacement.Span(42)
                )

                expect( line ).to.exist()
                expect( line:get_start()  ).to.be( t.GridPlacement.Auto()   )
                expect( line:get_end()    ).to.be( t.GridPlacement.Span(42) )
            end)

            it('table (array)', function()
                local line = t.Line_of_GridPlacement.new({
                    t.GridPlacement.Auto(),
                    t.GridPlacement.Span(42)
                })

                expect( line ).to.exist()
                expect( line:get_start()  ).to.be( t.GridPlacement.Auto()   )
                expect( line:get_end()    ).to.be( t.GridPlacement.Span(42) )
            end)

            it('table (array) explicit indexes', function()
                local line1 = t.Line_of_GridPlacement.new({
                    [1] = t.GridPlacement.Auto(),
                    [2] = t.GridPlacement.Span(42)
                })

                expect( line1 ).to.exist()
                expect( line1:get_start()  ).to.be( t.GridPlacement.Auto()   )
                expect( line1:get_end()    ).to.be( t.GridPlacement.Span(42) )

                local line2 = t.Line_of_GridPlacement.new({
                    [2] = t.GridPlacement.Span(42),
                    [1] = t.GridPlacement.Auto()
                })

                expect( line2 ).to.exist()
                expect( line2:get_start()  ).to.be( t.GridPlacement.Auto()   )
                expect( line2:get_end()    ).to.be( t.GridPlacement.Span(42) )
            end)

            it('table (dictionary)', function()
                local line1 = t.Line_of_GridPlacement.new({
                    Start = t.GridPlacement.Auto(),
                    End   = t.GridPlacement.Span(42)
                })

                expect( line1 ).to.exist()
                expect( line1:get_start()  ).to.be( t.GridPlacement.Auto()   )
                expect( line1:get_end()    ).to.be( t.GridPlacement.Span(42) )

                local line2 = t.Line_of_GridPlacement.new({
                    End   = t.GridPlacement.Span(42),
                    Start = t.GridPlacement.Auto()
                })

                expect( line2 ).to.exist()
                expect( line2:get_start()  ).to.be( t.GridPlacement.Auto()   )
                expect( line2:get_end()    ).to.be( t.GridPlacement.Span(42) )
            end)

            it('table (without parentheses)', function()
                local line1 = t.Line_of_GridPlacement.new {
                    t.GridPlacement.Auto(),
                    t.GridPlacement.Span(42)
                }

                expect( line1 ).to.exist()
                expect( line1:get_start()  ).to.be( t.GridPlacement.Auto()   )
                expect( line1:get_end()    ).to.be( t.GridPlacement.Span(42) )

                local line2 = t.Line_of_GridPlacement.new {
                    Start = t.GridPlacement.Auto(),
                    End   = t.GridPlacement.Span(42)
                }

                expect( line2 ).to.exist()
                expect( line2:get_start()  ).to.be( t.GridPlacement.Auto()   )
                expect( line2:get_end()    ).to.be( t.GridPlacement.Span(42) )
            end)
        end)

        it('Copying', function()
            local line = t.Line_of_GridPlacement.new(
                t.GridPlacement.Auto(),
                t.GridPlacement.Span(42)
            )

            local copy = line:copy()

            expect( copy ).to.exist()
            expect( copy:get_start()  ).to.be( t.GridPlacement.Auto()   )
            expect( copy:get_end()    ).to.be( t.GridPlacement.Span(42) )
        end)

        describe('Operators', function()
            it('Comparison', function()
                local line = t.Line_of_GridPlacement.new(
                    t.GridPlacement.Auto(),
                    t.GridPlacement.Span(42)
                )

                local lines_equal      = line == t.Line_of_GridPlacement.new(
                    t.GridPlacement.Auto(),
                    t.GridPlacement.Span(42)
                )
                local lines_not_equal1 = line ~= t.Line_of_GridPlacement.new(
                    t.GridPlacement.Auto(),
                    t.GridPlacement.Span(10)
                )
                local lines_not_equal2 = line ~= t.Line_of_GridPlacement.new(
                    t.GridPlacement.Span(42),
                    t.GridPlacement.Auto()
                )

                expect( lines_equal     ).to.be( true )
                expect( lines_not_equal1 ).to.be( true )
                expect( lines_not_equal2 ).to.be( true )
            end)
        end)

        it('Getters/Setters', function()
            local line = t.Line_of_GridPlacement.new(
                t.GridPlacement.Auto(),
                t.GridPlacement.Span(42)
            )

            expect( line ).to.exist()
            expect( line:get_start()  ).to.be( t.GridPlacement.Auto()   )
            expect( line:get_end()    ).to.be( t.GridPlacement.Span(42) )

            line:set_start( t.GridPlacement.Span(10) )
            line:set_end  ( t.GridPlacement.Auto()   )

            expect( line:get_start()  ).to.be( t.GridPlacement.Span(10) )
            expect( line:get_end()    ).to.be( t.GridPlacement.Auto()   )
        end)

        it('Properties', function()
            local line = t.Line_of_GridPlacement.new(
                t.GridPlacement.Auto(),
                t.GridPlacement.Span(42)
            )

            expect( line ).to.exist()
            expect( line.Start ).to.be( t.GridPlacement.Auto()   )
            expect( line.End   ).to.be( t.GridPlacement.Span(42) )

            line.Start = t.GridPlacement.Span(10)
            line.End   = t.GridPlacement.Auto()

            expect( line.Start ).to.be( t.GridPlacement.Span(10) )
            expect( line.End   ).to.be( t.GridPlacement.Auto()   )
        end)
    end) -- Line_of_GridPlacement

end)
