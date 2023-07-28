
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
            expect( t.FlexDirection.Row           ).to.be( 0 )
            expect( t.FlexDirection.Column        ).to.be( 1 )
            expect( t.FlexDirection.RowReverse    ).to.be( 2 )
            expect( t.FlexDirection.ColumnReverse ).to.be( 3 )

            expect( t.FlexDirection.Default       ).to.be( 0 )
        end)
    end) -- FlexDirection

    describe('FlexWrap', function()
        it('Values', function()
            expect( t.FlexWrap.NoWrap      ).to.be( 0 )
            expect( t.FlexWrap.Wrap        ).to.be( 1 )
            expect( t.FlexWrap.WrapReverse ).to.be( 2 )

            expect( t.FlexWrap.Default     ).to.be( 0 )
        end)
    end) -- FlexWrap

    describe('GridAutoFlow', function()
        it('Values', function()
            expect( t.GridAutoFlow.Row         ).to.be( 0 )
            expect( t.GridAutoFlow.Column      ).to.be( 1 )
            expect( t.GridAutoFlow.RowDense    ).to.be( 2 )
            expect( t.GridAutoFlow.ColumnDense ).to.be( 3 )

            expect( t.GridAutoFlow.Default     ).to.be( 0 )
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
                -- local point = t.Point_of_float.new(10, 20)

                -- expect( point ).to.exist()
                -- expect( point:get_x() ).to.be( 10 )
                -- expect( point:get_y() ).to.be( 20 )
            end)

            it('table (array)', function()
                -- local point = t.Point_of_float.new({10, 20})

                -- expect( point ).to.exist()
                -- expect( point:get_x() ).to.be( 10 )
                -- expect( point:get_y() ).to.be( 20 )
            end)

            it('table (array) explicit indexes', function()
                -- local point1 = t.Point_of_float.new({[1] = 10, [2] = 20})

                -- expect( point1 ).to.exist()
                -- expect( point1:get_x() ).to.be( 10 )
                -- expect( point1:get_y() ).to.be( 20 )

                -- local point2 = t.Point_of_float.new({[2] = 20, [1] = 10})

                -- expect( point2 ).to.exist()
                -- expect( point2:get_x() ).to.be( 10 )
                -- expect( point2:get_y() ).to.be( 20 )
            end)

            it('table (dictionary)', function()
                -- local point1 = t.Point_of_float.new({x = 10, y = 20})

                -- expect( point1 ).to.exist()
                -- expect( point1:get_x() ).to.be( 10 )
                -- expect( point1:get_y() ).to.be( 20 )

                -- local point2 = t.Point_of_float.new({y = 20, x = 10})

                -- expect( point2 ).to.exist()
                -- expect( point2:get_x() ).to.be( 10 )
                -- expect( point2:get_y() ).to.be( 20 )
            end)

            it('table (without parentheses)', function()
                -- local point1 = t.Point_of_float.new {10, 20}

                -- expect( point1 ).to.exist()
                -- expect( point1:get_x() ).to.be( 10 )
                -- expect( point1:get_y() ).to.be( 20 )

                -- local point2 = t.Point_of_float.new {x = 10, y = 20}

                -- expect( point2 ).to.exist()
                -- expect( point2:get_x() ).to.be( 10 )
                -- expect( point2:get_y() ).to.be( 20 )
            end)
        end)

        it('Copying', function()

        end)

        it('Getters/Setters', function()
            -- local point = t.Point_of_float.new(10, 20)

            -- expect( point ).to.exist()
            -- expect( point:get_x() ).to.be( 10 )
            -- expect( point:get_y() ).to.be( 20 )

            -- point:set_x(35)
            -- point:set_y(42)

            -- expect( point:get_x() ).to.be( 35 )
            -- expect( point:get_y() ).to.be( 42 )
        end)

        it('Properties', function()
            -- local point = t.Point_of_float.new(10, 20)

            -- expect( point ).to.exist()
            -- expect( point.x ).to.be( 10 )
            -- expect( point.y ).to.be( 20 )

            -- point.x = 35
            -- point.y = 42

            -- expect( point.x ).to.be( 35 )
            -- expect( point.y ).to.be( 42 )
        end)
    end) -- NonRepeatedTrackSizingFunction

end)


-- -----------------------------------------------------------------------------
-- TODO: TEMPORARLY UNCOVERED BY TESTS

local nrtsf = t.NonRepeatedTrackSizingFunction.new( t.MinTrackSizingFunction.Auto(), t.MaxTrackSizingFunction.Auto() )
nrtsf = t.NonRepeatedTrackSizingFunction.new({ t.MinTrackSizingFunction.Auto(), t.MaxTrackSizingFunction.Auto() })
nrtsf = t.NonRepeatedTrackSizingFunction.new({[1] = t.MinTrackSizingFunction.Auto(), [2] = t.MaxTrackSizingFunction.Auto() })
nrtsf = t.NonRepeatedTrackSizingFunction.new({[2] = t.MaxTrackSizingFunction.Auto(), [1] = t.MinTrackSizingFunction.Auto() })
nrtsf = t.NonRepeatedTrackSizingFunction.new { t.MinTrackSizingFunction.Auto(), t.MaxTrackSizingFunction.Auto() }
-- TODO: add other tests here

-- TODO: add taffy_TrackSizingFunction

