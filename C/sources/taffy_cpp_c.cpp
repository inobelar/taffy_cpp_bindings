#include <taffy_cpp_c.h>

#include <taffy/tree/taffy_tree/tree/Taffy.hpp>

// -----------------------------------------------------------------------------

#include <cassert> // for: assert()

#define ASSERT_NOT_NULL(pointer) \
    assert(pointer != nullptr)

#define ASSERT_UNREACHABLE() \
    assert(false)

// -----------------------------------------------------------------------------
// Option<float>

taffy_Option_float* taffy_Option_float_new_default(void)
{
    return reinterpret_cast<taffy_Option_float*>( new taffy::Option<float>{} );
}

taffy_Option_float* taffy_Option_float_new(float* value)
{
    return reinterpret_cast<taffy_Option_float*>(
        (value == nullptr) ?
            new taffy::Option<float>{} // None
        :
            new taffy::Option<float>{*value} // Some
    );
}

taffy_Option_float* taffy_Option_float_new_some(float value)
{
    return reinterpret_cast<taffy_Option_float*>( new taffy::Option<float>{value} );
}

taffy_Option_float* taffy_Option_float_new_copy(const taffy_Option_float* other)
{
    ASSERT_NOT_NULL(other);

    const taffy::Option<float>* o = reinterpret_cast<const taffy::Option<float>*>(other);
    return reinterpret_cast<taffy_Option_float*>( new taffy::Option<float>{*o} );
}

void taffy_Option_float_delete(taffy_Option_float* self)
{
    ASSERT_NOT_NULL(self);

    delete reinterpret_cast<taffy::Option<float>*>(self);
    self = nullptr;
}

int taffy_Option_float_is_some(const taffy_Option_float* self)
{
    ASSERT_NOT_NULL(self);

    return reinterpret_cast<const taffy::Option<float>*>(self)->is_some() ? 1 : 0;
}

float taffy_Option_float_get_value(const taffy_Option_float* self)
{
    ASSERT_NOT_NULL(self);

    return reinterpret_cast<const taffy::Option<float>*>(self)->value();
}

void taffy_Option_float_set_value(taffy_Option_float* self, const float* value)
{
    ASSERT_NOT_NULL(self);

    taffy::Option<float>* opt = reinterpret_cast<taffy::Option<float>*>(self);
    *opt = (value == nullptr) ?
        taffy::Option<float>{} // None
    :
        taffy::Option<float>{*value}; // Some
}

// -----------------------------------------------------------------------------
// Point<float>

taffy_Point_of_float* taffy_Point_of_float_new_default(void)
{
    return reinterpret_cast<taffy_Point_of_float*>( new taffy::Point<float>{} );
}
taffy_Point_of_float* taffy_Point_of_float_new(float x, float y)
{
    return reinterpret_cast<taffy_Point_of_float*>( new taffy::Point<float>{x, y} );
}

taffy_Point_of_float* taffy_Point_of_float_new_copy(const taffy_Point_of_float* other)
{
    ASSERT_NOT_NULL(other);

    const taffy::Point<float>* o = reinterpret_cast<const taffy::Point<float>*>(other);
    return reinterpret_cast<taffy_Point_of_float*>( new taffy::Point<float>{*o} );
}


void taffy_Point_of_float_delete(taffy_Point_of_float* self)
{
    ASSERT_NOT_NULL(self);

    delete reinterpret_cast<taffy::Point<float>*>(self);
    self = nullptr;
}

float taffy_Point_of_float_get_x(const taffy_Point_of_float* self)
{
    ASSERT_NOT_NULL(self);

    return reinterpret_cast<const taffy::Point<float>*>(self)->x;
}

float taffy_Point_of_float_get_y(const taffy_Point_of_float* self)
{
    ASSERT_NOT_NULL(self);

    return reinterpret_cast<const taffy::Point<float>*>(self)->y;
}

void taffy_Point_of_float_set_x(taffy_Point_of_float* self, float x)
{
    ASSERT_NOT_NULL(self);

    reinterpret_cast<taffy::Point<float>*>(self)->x = x;
}

void taffy_Point_of_float_set_y(taffy_Point_of_float* self, float y)
{
    ASSERT_NOT_NULL(self);

    reinterpret_cast<taffy::Point<float>*>(self)->x = y;
}

float* taffy_Point_of_float_get_mut_x(taffy_Point_of_float* self)
{
    ASSERT_NOT_NULL(self);

    return &( reinterpret_cast<taffy::Point<float>*>(self)->x );
}

float* taffy_Point_of_float_get_mut_y(taffy_Point_of_float* self)
{
    ASSERT_NOT_NULL(self);

    return &( reinterpret_cast<taffy::Point<float>*>(self)->y );
}

taffy_Point_of_float* taffy_Point_of_float_new_ZERO(void)
{
    return reinterpret_cast<taffy_Point_of_float*>( new taffy::Point<float>{taffy::Point<float>::ZERO()} );
}

// -----------------------------------------------------------------------------
// Size<float>

taffy_Size_of_float* taffy_Size_of_float_new_default(void)
{
    return reinterpret_cast<taffy_Size_of_float*>( new taffy::Size<float>{} );
}
taffy_Size_of_float* taffy_Size_of_float_new(float width, float height)
{
    return reinterpret_cast<taffy_Size_of_float*>( new taffy::Size<float>{width, height} );
}

taffy_Size_of_float* taffy_Size_of_float_new_copy(const taffy_Size_of_float* other)
{
    ASSERT_NOT_NULL(other);

    const taffy::Size<float>* o = reinterpret_cast<const taffy::Size<float>*>(other);
    return reinterpret_cast<taffy_Size_of_float*>( new taffy::Size<float>{*o} );
}

void taffy_Size_of_float_delete(taffy_Size_of_float* self)
{
    ASSERT_NOT_NULL(self);

    delete reinterpret_cast<taffy::Size<float>*>(self);
    self = nullptr;
}

float taffy_Size_of_float_get_width(const taffy_Size_of_float* self)
{
    ASSERT_NOT_NULL(self);

    return reinterpret_cast<const taffy::Size<float>*>(self)->width;
}

float taffy_Size_of_float_get_height(const taffy_Size_of_float* self)
{
    ASSERT_NOT_NULL(self);

    return reinterpret_cast<const taffy::Size<float>*>(self)->height;   
}

void taffy_Size_of_float_set_width(taffy_Size_of_float* self, float width )
{
    ASSERT_NOT_NULL(self);

    reinterpret_cast<taffy::Size<float>*>(self)->width = width;
}

void taffy_Size_of_float_set_height(taffy_Size_of_float* self, float height)
{
    ASSERT_NOT_NULL(self);

    reinterpret_cast<taffy::Size<float>*>(self)->height = height;
}

float* taffy_Size_of_float_get_mut_width(taffy_Size_of_float* self)
{
    ASSERT_NOT_NULL(self);

    return &( reinterpret_cast<taffy::Size<float>*>(self)->width );
}

float* taffy_Size_of_float_get_mut_height(taffy_Size_of_float* self)
{
    ASSERT_NOT_NULL(self);

    return &( reinterpret_cast<taffy::Size<float>*>(self)->height );
}

taffy_Size_of_float* taffy_Size_of_float_new_ZERO(void)
{
    return reinterpret_cast<taffy_Size_of_float*>( new taffy::Size<float>{taffy::Size<float>::ZERO()} );
}

// -----------------------------------------------------------------------------
// AlignContent

static taffy::AlignContent taffy_AlignContent_to_cpp(const taffy_AlignContent v)
{
    switch(v) {
    case taffy_AlignContent_Start        : return taffy::AlignContent::Start;
    case taffy_AlignContent_End          : return taffy::AlignContent::End;
    case taffy_AlignContent_FlexStart    : return taffy::AlignContent::FlexStart;
    case taffy_AlignContent_FlexEnd      : return taffy::AlignContent::FlexEnd;
    case taffy_AlignContent_Center       : return taffy::AlignContent::Center;
    case taffy_AlignContent_Stretch      : return taffy::AlignContent::Stretch;
    case taffy_AlignContent_SpaceBetween : return taffy::AlignContent::SpaceBetween;
    case taffy_AlignContent_SpaceEvenly  : return taffy::AlignContent::SpaceEvenly;
    case taffy_AlignContent_SpaceAround  : return taffy::AlignContent::SpaceAround;
    }

    ASSERT_UNREACHABLE();
    return taffy::AlignContent::Start;
}

static taffy_AlignContent taffy_AlignContent_to_c(const taffy::AlignContent v)
{
    switch(v) {
    case taffy::AlignContent::Start        : return taffy_AlignContent_Start;
    case taffy::AlignContent::End          : return taffy_AlignContent_End;
    case taffy::AlignContent::FlexStart    : return taffy_AlignContent_FlexStart;
    case taffy::AlignContent::FlexEnd      : return taffy_AlignContent_FlexEnd;
    case taffy::AlignContent::Center       : return taffy_AlignContent_Center;
    case taffy::AlignContent::Stretch      : return taffy_AlignContent_Stretch;
    case taffy::AlignContent::SpaceBetween : return taffy_AlignContent_SpaceBetween;
    case taffy::AlignContent::SpaceEvenly  : return taffy_AlignContent_SpaceEvenly;
    case taffy::AlignContent::SpaceAround  : return taffy_AlignContent_SpaceAround;
    }

    ASSERT_UNREACHABLE();
    return taffy_AlignContent_Start;
}

// -----------------------------------------------------------------------------
// JustifyContent

static taffy::JustifyContent taffy_JustifyContent_to_cpp(const taffy_JustifyContent v)
{
    switch(v) {
    case taffy_JustifyContent_Start        : return taffy::JustifyContent::Start;
    case taffy_JustifyContent_End          : return taffy::JustifyContent::End;
    case taffy_JustifyContent_FlexStart    : return taffy::JustifyContent::FlexStart;
    case taffy_JustifyContent_FlexEnd      : return taffy::JustifyContent::FlexEnd;
    case taffy_JustifyContent_Center       : return taffy::JustifyContent::Center;
    case taffy_JustifyContent_Stretch      : return taffy::JustifyContent::Stretch;
    case taffy_JustifyContent_SpaceBetween : return taffy::JustifyContent::SpaceBetween;
    case taffy_JustifyContent_SpaceEvenly  : return taffy::JustifyContent::SpaceEvenly;
    case taffy_JustifyContent_SpaceAround  : return taffy::JustifyContent::SpaceAround;
    }

    ASSERT_UNREACHABLE();
    return taffy::JustifyContent::Start;
}

static taffy_JustifyContent taffy_JustifyContent_to_c(const taffy::JustifyContent v)
{
    switch(v) {
    case taffy::JustifyContent::Start        : return taffy_JustifyContent_Start;
    case taffy::JustifyContent::End          : return taffy_JustifyContent_End;
    case taffy::JustifyContent::FlexStart    : return taffy_JustifyContent_FlexStart;
    case taffy::JustifyContent::FlexEnd      : return taffy_JustifyContent_FlexEnd;
    case taffy::JustifyContent::Center       : return taffy_JustifyContent_Center;
    case taffy::JustifyContent::Stretch      : return taffy_JustifyContent_Stretch;
    case taffy::JustifyContent::SpaceBetween : return taffy_JustifyContent_SpaceBetween;
    case taffy::JustifyContent::SpaceEvenly  : return taffy_JustifyContent_SpaceEvenly;
    case taffy::JustifyContent::SpaceAround  : return taffy_JustifyContent_SpaceAround;
    }

    ASSERT_UNREACHABLE();
    return taffy_JustifyContent_Start;
}

// -----------------------------------------------------------------------------
// AlignItems

static taffy::AlignItems taffy_AlignItems_to_cpp(const taffy_AlignItems v)
{
    switch(v) {
    case taffy_AlignItems_Start     : return taffy::AlignItems::Start;
    case taffy_AlignItems_End       : return taffy::AlignItems::End;
    case taffy_AlignItems_FlexStart : return taffy::AlignItems::FlexStart;
    case taffy_AlignItems_FlexEnd   : return taffy::AlignItems::FlexEnd;
    case taffy_AlignItems_Center    : return taffy::AlignItems::Center;
    case taffy_AlignItems_Baseline  : return taffy::AlignItems::Baseline;
    case taffy_AlignItems_Stretch   : return taffy::AlignItems::Stretch;
    }

    ASSERT_UNREACHABLE();
    return taffy::AlignItems::Start;
}

static taffy_AlignItems taffy_AlignItems_to_c(const taffy::AlignItems v)
{
    switch(v) {
    case taffy::AlignItems::Start     : return taffy_AlignItems_Start;
    case taffy::AlignItems::End       : return taffy_AlignItems_End;
    case taffy::AlignItems::FlexStart : return taffy_AlignItems_FlexStart;
    case taffy::AlignItems::FlexEnd   : return taffy_AlignItems_FlexEnd;
    case taffy::AlignItems::Center    : return taffy_AlignItems_Center;
    case taffy::AlignItems::Baseline  : return taffy_AlignItems_Baseline;
    case taffy::AlignItems::Stretch   : return taffy_AlignItems_Stretch;
    }

    ASSERT_UNREACHABLE();
    return taffy_AlignItems_Start;
}

// -----------------------------------------------------------------------------
// AlignSelf

static taffy::AlignSelf taffy_AlignSelf_to_cpp(const taffy_AlignSelf v)
{
    switch(v) {
    case taffy_AlignSelf_Start     : return taffy::AlignSelf::Start;
    case taffy_AlignSelf_End       : return taffy::AlignSelf::End;
    case taffy_AlignSelf_FlexStart : return taffy::AlignSelf::FlexStart;
    case taffy_AlignSelf_FlexEnd   : return taffy::AlignSelf::FlexEnd;
    case taffy_AlignSelf_Center    : return taffy::AlignSelf::Center;
    case taffy_AlignSelf_Baseline  : return taffy::AlignSelf::Baseline;
    case taffy_AlignSelf_Stretch   : return taffy::AlignSelf::Stretch;
    }

    ASSERT_UNREACHABLE();
    return taffy::AlignSelf::Start;
}

static taffy_AlignSelf taffy_AlignSelf_to_c(const taffy::AlignSelf v)
{
    switch(v) {
    case taffy::AlignSelf::Start     : return taffy_AlignSelf_Start;
    case taffy::AlignSelf::End       : return taffy_AlignSelf_End;
    case taffy::AlignSelf::FlexStart : return taffy_AlignSelf_FlexStart;
    case taffy::AlignSelf::FlexEnd   : return taffy_AlignSelf_FlexEnd;
    case taffy::AlignSelf::Center    : return taffy_AlignSelf_Center;
    case taffy::AlignSelf::Baseline  : return taffy_AlignSelf_Baseline;
    case taffy::AlignSelf::Stretch   : return taffy_AlignSelf_Stretch;
    }

    ASSERT_UNREACHABLE();
    return taffy_AlignSelf_Start;
}

// -----------------------------------------------------------------------------
// AvailableSpace

taffy_AvailableSpace* taffy_AvailableSpace_new_Definite(float value)
{
    return reinterpret_cast<taffy_AvailableSpace*>( new taffy::AvailableSpace{ taffy::AvailableSpace::Definite(value) } );
}

taffy_AvailableSpace* taffy_AvailableSpace_new_MinContent(void)
{
    return reinterpret_cast<taffy_AvailableSpace*>( new taffy::AvailableSpace{ taffy::AvailableSpace::MinContent() } );
}

taffy_AvailableSpace* taffy_AvailableSpace_new_MaxContent(void)
{
    return reinterpret_cast<taffy_AvailableSpace*>( new taffy::AvailableSpace{ taffy::AvailableSpace::MaxContent() } );
}

taffy_AvailableSpace* taffy_AvailableSpace_new_copy(const taffy_AvailableSpace* other)
{
    ASSERT_NOT_NULL(other);

    const taffy::AvailableSpace* o = reinterpret_cast<const taffy::AvailableSpace*>(other);
    return reinterpret_cast<taffy_AvailableSpace*>( new taffy::AvailableSpace{*o} );
}

void taffy_AvailableSpace_delete(taffy_AvailableSpace* self)
{
    ASSERT_NOT_NULL(self);

    delete reinterpret_cast<taffy::AvailableSpace*>(self);
    self = nullptr;
}

taffy_AvailableSpace* taffy_AvailableSpace_new_ZERO(void)
{
    return reinterpret_cast<taffy_AvailableSpace*>( new taffy::AvailableSpace{ taffy::AvailableSpace::ZERO() } );
}

taffy_AvailableSpace* taffy_AvailableSpace_new_from_length(float value)
{
    return reinterpret_cast<taffy_AvailableSpace*>( new taffy::AvailableSpace{ taffy::AvailableSpace::from_length(value) } );
}

taffy_AvailableSpace* taffy_AvailableSpace_new_from_float(float value)
{
    return reinterpret_cast<taffy_AvailableSpace*>( new taffy::AvailableSpace{ taffy::AvailableSpace::from(value) } );
}

taffy_AvailableSpace* taffy_AvailableSpace_new_from_option(const taffy_Option_float* opt)
{
    ASSERT_NOT_NULL(opt);

    const taffy::Option<float>* o = reinterpret_cast<const taffy::Option<float>*>(opt);
    return reinterpret_cast<taffy_AvailableSpace*>( new taffy::AvailableSpace{ taffy::AvailableSpace::from(*o) } );
}

// -----------------------------------------------------------------------------
// LengthPercentage

taffy_LengthPercentage* taffy_LengthPercentage_new_Length(float value)
{
    return reinterpret_cast<taffy_LengthPercentage*>( new taffy::LengthPercentage{ taffy::LengthPercentage::Length(value) } );
}

taffy_LengthPercentage* taffy_LengthPercentage_new_Percent(float value)
{
    return reinterpret_cast<taffy_LengthPercentage*>( new taffy::LengthPercentage{ taffy::LengthPercentage::Percent(value) } );
}

taffy_LengthPercentage* taffy_LengthPercentage_new_copy(const taffy_LengthPercentage* other)
{
    ASSERT_NOT_NULL(other);

    const taffy::LengthPercentage* o = reinterpret_cast<const taffy::LengthPercentage*>(other);
    return reinterpret_cast<taffy_LengthPercentage*>( new taffy::LengthPercentage{*o} );
}

void taffy_LengthPercentage_delete(taffy_LengthPercentage* self)
{
    ASSERT_NOT_NULL(self);

    delete reinterpret_cast<taffy::LengthPercentage*>(self);
    self = nullptr;
}

taffy_LengthPercentage* taffy_LengthPercentage_new_ZERO(void)
{
    return reinterpret_cast<taffy_LengthPercentage*>( new taffy::LengthPercentage{ taffy::LengthPercentage::ZERO() } );
}

taffy_LengthPercentage* taffy_LengthPercentage_new_from_length(float value)
{
    return reinterpret_cast<taffy_LengthPercentage*>( new taffy::LengthPercentage{ taffy::LengthPercentage::from_length(value) } );
}

taffy_LengthPercentage* taffy_LengthPercentage_new_from_percent(float value)
{
    return reinterpret_cast<taffy_LengthPercentage*>( new taffy::LengthPercentage{ taffy::LengthPercentage::from_percent(value) } );
}

// -----------------------------------------------------------------------------
// LengthPercentageAuto

taffy_LengthPercentageAuto* taffy_LengthPercentageAuto_new_Length(float value)
{
    return reinterpret_cast<taffy_LengthPercentageAuto*>( new taffy::LengthPercentageAuto{ taffy::LengthPercentageAuto::Length(value) } );
}

taffy_LengthPercentageAuto* taffy_LengthPercentageAuto_new_Percent(float value)
{
    return reinterpret_cast<taffy_LengthPercentageAuto*>( new taffy::LengthPercentageAuto{ taffy::LengthPercentageAuto::Percent(value) } );
}

taffy_LengthPercentageAuto* taffy_LengthPercentageAuto_new_Auto(void)
{
    return reinterpret_cast<taffy_LengthPercentageAuto*>( new taffy::LengthPercentageAuto{ taffy::LengthPercentageAuto::Auto() } );
}

taffy_LengthPercentageAuto* taffy_LengthPercentageAuto_new_copy(const taffy_LengthPercentageAuto* other)
{
    ASSERT_NOT_NULL(other);

    const taffy::LengthPercentageAuto* o = reinterpret_cast<const taffy::LengthPercentageAuto*>(other);
    return reinterpret_cast<taffy_LengthPercentageAuto*>( new taffy::LengthPercentageAuto{*o} );
}

void taffy_LengthPercentageAuto_delete(taffy_LengthPercentageAuto* self)
{
    ASSERT_NOT_NULL(self);

    delete reinterpret_cast<taffy::LengthPercentageAuto*>(self);
    self = nullptr;
}

taffy_LengthPercentageAuto* taffy_LengthPercentageAuto_new_ZERO(void)
{
    return reinterpret_cast<taffy_LengthPercentageAuto*>( new taffy::LengthPercentageAuto{ taffy::LengthPercentageAuto::ZERO() } );
}

taffy_LengthPercentageAuto* taffy_LengthPercentageAuto_new_AUTO(void)
{
    return reinterpret_cast<taffy_LengthPercentageAuto*>( new taffy::LengthPercentageAuto{ taffy::LengthPercentageAuto::AUTO() } );
}

taffy_LengthPercentageAuto* taffy_LengthPercentageAuto_new_from_length(float value)
{
    return reinterpret_cast<taffy_LengthPercentageAuto*>( new taffy::LengthPercentageAuto{ taffy::LengthPercentageAuto::from_length(value) } );
}

taffy_LengthPercentageAuto* taffy_LengthPercentageAuto_new_from_percent(float value)
{
    return reinterpret_cast<taffy_LengthPercentageAuto*>( new taffy::LengthPercentageAuto{ taffy::LengthPercentageAuto::from_percent(value) } );
}

taffy_LengthPercentageAuto* taffy_LengthPercentageAuto_new_from_LengthPercentage(const taffy_LengthPercentage* input)
{
    ASSERT_NOT_NULL(input);

    const taffy::LengthPercentage* _input = reinterpret_cast<const taffy::LengthPercentage*>(input);
    return reinterpret_cast<taffy_LengthPercentageAuto*>( new taffy::LengthPercentageAuto{ *_input } );
}

// -----------------------------------------------------------------------------
// Dimension

taffy_Dimension* taffy_Dimension_new_Length(float value)
{
    return reinterpret_cast<taffy_Dimension*>( new taffy::Dimension{ taffy::Dimension::Length(value) } );
}

taffy_Dimension* taffy_Dimension_new_Percent(float value)
{
    return reinterpret_cast<taffy_Dimension*>( new taffy::Dimension{ taffy::Dimension::Percent(value) } );
}

taffy_Dimension* taffy_Dimension_new_Auto(void)
{
    return reinterpret_cast<taffy_Dimension*>( new taffy::Dimension{ taffy::Dimension::Auto() } );
}

taffy_Dimension* taffy_Dimension_new_copy(const taffy_Dimension* other)
{
    ASSERT_NOT_NULL(other);

    const taffy::Dimension* o = reinterpret_cast<const taffy::Dimension*>(other);
    return reinterpret_cast<taffy_Dimension*>( new taffy::Dimension{*o} );
}

void taffy_Dimension_delete(taffy_Dimension* self)
{
    ASSERT_NOT_NULL(self);

    delete reinterpret_cast<taffy::Dimension*>(self);
    self = nullptr;
}

taffy_Dimension* taffy_Dimension_new_ZERO(void)
{
    return reinterpret_cast<taffy_Dimension*>( new taffy::Dimension{ taffy::Dimension::ZERO() } );
}

taffy_Dimension* taffy_Dimension_new_from_length(float value)
{
    return reinterpret_cast<taffy_Dimension*>( new taffy::Dimension{ taffy::Dimension::from_length(value) } );
}

taffy_Dimension* taffy_Dimension_new_from_percent(float value)
{
    return reinterpret_cast<taffy_Dimension*>( new taffy::Dimension{ taffy::Dimension::from_percent(value) } );
}

taffy_Dimension* taffy_Dimension_new_from_LengthPercentage(const taffy_LengthPercentage* input)
{
    ASSERT_NOT_NULL(input);

    const taffy::LengthPercentage* _input = reinterpret_cast<const taffy::LengthPercentage*>(input);
    return reinterpret_cast<taffy_Dimension*>( new taffy::Dimension{ taffy::Dimension::from(*_input) } );
}

taffy_Dimension* taffy_Dimension_new_from_LengthPercentageAuto(const taffy_LengthPercentageAuto* input)
{
    ASSERT_NOT_NULL(input);

    const taffy::LengthPercentageAuto* _input = reinterpret_cast<const taffy::LengthPercentageAuto*>(input);
    return reinterpret_cast<taffy_Dimension*>( new taffy::Dimension{ taffy::Dimension::from(*_input) } );
}

// -----------------------------------------------------------------------------
// FlexDirection

static taffy::FlexDirection taffy_FlexDirection_to_cpp(const taffy_FlexDirection v)
{
    switch(v) {
    case taffy_FlexDirection_Row           : return taffy::FlexDirection::Row();
    case taffy_FlexDirection_Column        : return taffy::FlexDirection::Column();
    case taffy_FlexDirection_RowReverse    : return taffy::FlexDirection::RowReverse();
    case taffy_FlexDirection_ColumnReverse : return taffy::FlexDirection::ColumnReverse();
    }

    ASSERT_UNREACHABLE();
    return taffy::FlexDirection{};
}

static taffy_FlexDirection taffy_FlexDirection_to_c(const taffy::FlexDirection v)
{
    switch(v.type()) {
    case taffy::FlexDirection::Type::Row           : return taffy_FlexDirection_Row;
    case taffy::FlexDirection::Type::Column        : return taffy_FlexDirection_Column;
    case taffy::FlexDirection::Type::RowReverse    : return taffy_FlexDirection_RowReverse;
    case taffy::FlexDirection::Type::ColumnReverse : return taffy_FlexDirection_ColumnReverse;
    }

    ASSERT_UNREACHABLE();
    return taffy_FlexDirection_Row;
}

taffy_FlexDirection taffy_FlexDirection_Default(void)
{
    return taffy_FlexDirection_to_c( taffy::FlexDirection{} );
}

// -----------------------------------------------------------------------------
// FlexWrap

static taffy::FlexWrap taffy_FlexWrap_to_cpp(const taffy_FlexWrap v)
{
    switch(v) {
    case taffy_FlexWrap_NoWrap      : return taffy::FlexWrap::NoWrap();
    case taffy_FlexWrap_Wrap        : return taffy::FlexWrap::Wrap();
    case taffy_FlexWrap_WrapReverse : return taffy::FlexWrap::WrapReverse();    
    }

    ASSERT_UNREACHABLE();
    return taffy::FlexWrap{};
}

static taffy_FlexWrap taffy_FlexWrap_to_c(const taffy::FlexWrap v)
{
    switch(v.type()) {
    case taffy::FlexWrap::Type::NoWrap      : return taffy_FlexWrap_NoWrap;
    case taffy::FlexWrap::Type::Wrap        : return taffy_FlexWrap_Wrap;
    case taffy::FlexWrap::Type::WrapReverse : return taffy_FlexWrap_WrapReverse;
    }

    ASSERT_UNREACHABLE();
    return taffy_FlexWrap_NoWrap;
}

taffy_FlexWrap taffy_FlexWrap_Default(void)
{
    return taffy_FlexWrap_to_c( taffy::FlexWrap{} );
}

// -----------------------------------------------------------------------------
// GridAutoFlow

static taffy::GridAutoFlow taffy_GridAutoFlow_to_cpp(const taffy_GridAutoFlow v)
{
    switch(v) {
    case taffy_GridAutoFlow_Row         : return taffy::GridAutoFlow::Row();
    case taffy_GridAutoFlow_Column      : return taffy::GridAutoFlow::Column();
    case taffy_GridAutoFlow_RowDense    : return taffy::GridAutoFlow::RowDense();
    case taffy_GridAutoFlow_ColumnDense : return taffy::GridAutoFlow::ColumnDense();
    }

    ASSERT_UNREACHABLE();
    return taffy::GridAutoFlow{};
}

static taffy_GridAutoFlow taffy_GridAutoFlow_to_c(const taffy::GridAutoFlow v)
{
    switch(v.type()) {
    case taffy::GridAutoFlow::Type::Row         : return taffy_GridAutoFlow_Row;
    case taffy::GridAutoFlow::Type::Column      : return taffy_GridAutoFlow_Column;
    case taffy::GridAutoFlow::Type::RowDense    : return taffy_GridAutoFlow_RowDense;
    case taffy::GridAutoFlow::Type::ColumnDense : return taffy_GridAutoFlow_ColumnDense;
    }

    ASSERT_UNREACHABLE();
    return taffy_GridAutoFlow_Row;
}

taffy_GridAutoFlow taffy_GridAutoFlow_Default(void)
{
    return taffy_GridAutoFlow_to_c( taffy::GridAutoFlow{} );
}

// -----------------------------------------------------------------------------
// GridPlacement

taffy_GridPlacement* taffy_GridPlacement_new_Default(void)
{
    return reinterpret_cast<taffy_GridPlacement*>( new taffy::GridPlacement{} );
}

taffy_GridPlacement* taffy_GridPlacement_new_Auto(void)
{
    return reinterpret_cast<taffy_GridPlacement*>( new taffy::GridPlacement{ taffy::GridPlacement::Auto() } );
}

taffy_GridPlacement* taffy_GridPlacement_new_Line(int16_t value)
{
    return reinterpret_cast<taffy_GridPlacement*>( new taffy::GridPlacement{ taffy::GridPlacement::Line( taffy::GridLine{value} ) } );
}

taffy_GridPlacement* taffy_GridPlacement_new_Span(uint16_t value)
{
    return reinterpret_cast<taffy_GridPlacement*>( new taffy::GridPlacement{ taffy::GridPlacement::Span( value ) } );
}

taffy_GridPlacement* taffy_GridPlacement_new_copy(const taffy_GridPlacement* other)
{
    ASSERT_NOT_NULL(other);

    const taffy::GridPlacement* o = reinterpret_cast<const taffy::GridPlacement*>(other);
    return reinterpret_cast<taffy_GridPlacement*>( new taffy::GridPlacement{*o} );
}

void taffy_GridPlacement_delete(taffy_GridPlacement* self)
{
    ASSERT_NOT_NULL(self);

    delete reinterpret_cast<taffy::GridPlacement*>(self);
    self = nullptr;
}

taffy_GridPlacement* taffy_GridPlacement_new_AUTO(void)
{
    return reinterpret_cast<taffy_GridPlacement*>( new taffy::GridPlacement{ taffy::GridPlacement::AUTO() } );
}

taffy_GridPlacement* taffy_GridPlacement_new_from_line_index(int16_t index)
{
    return reinterpret_cast<taffy_GridPlacement*>( new taffy::GridPlacement{ taffy::GridPlacement::from_line_index(index) } );
}

taffy_GridPlacement* taffy_GridPlacement_new_from_span(uint16_t span)
{
    return reinterpret_cast<taffy_GridPlacement*>( new taffy::GridPlacement{ taffy::GridPlacement::from_span(span) } );
}

// -----------------------------------------------------------------------------
// GridTrackRepetition

taffy_GridTrackRepetition* taffy_GridTrackRepetition_new_AutoFill(void)
{
    return reinterpret_cast<taffy_GridTrackRepetition*>( new taffy::GridTrackRepetition{ taffy::GridTrackRepetition::AutoFill() } );
}

taffy_GridTrackRepetition* taffy_GridTrackRepetition_new_AutoFit(void)
{
    return reinterpret_cast<taffy_GridTrackRepetition*>( new taffy::GridTrackRepetition{ taffy::GridTrackRepetition::AutoFit() } );
}

taffy_GridTrackRepetition* taffy_GridTrackRepetition_new_Count(uint16_t value)
{
    return reinterpret_cast<taffy_GridTrackRepetition*>( new taffy::GridTrackRepetition{ taffy::GridTrackRepetition::Count(value) } );
}

taffy_GridTrackRepetition* taffy_GridTrackRepetition_new_copy(taffy_GridTrackRepetition* other)
{
    ASSERT_NOT_NULL(other);

    const taffy::GridTrackRepetition* o = reinterpret_cast<const taffy::GridTrackRepetition*>(other);
    return reinterpret_cast<taffy_GridTrackRepetition*>( new taffy::GridTrackRepetition{*o} );
}

void taffy_GridTrackRepetition_delete(taffy_GridTrackRepetition* self)
{
    ASSERT_NOT_NULL(self);

    delete reinterpret_cast<taffy::GridTrackRepetition*>(self);
    self = nullptr;
}

taffy_GridTrackRepetition* taffy_GridTrackRepetition_try_from_u16(uint16_t value)
{
    const auto result = taffy::GridTrackRepetition::try_from(value);
    if(result.is_ok()) {
        return reinterpret_cast<taffy_GridTrackRepetition*>( new taffy::GridTrackRepetition{ result.value() });
    } else {
        return nullptr;
    }
}

taffy_GridTrackRepetition* taffy_GridTrackRepetition_try_from_str(const char* str)
{
    const auto result = taffy::GridTrackRepetition::try_from(str);
    if(result.is_ok()) {
        return reinterpret_cast<taffy_GridTrackRepetition*>( new taffy::GridTrackRepetition{ result.value() });
    } else {
        return nullptr;
    }
}

// -----------------------------------------------------------------------------
// MaxTrackSizingFunction

taffy_MaxTrackSizingFunction* taffy_MaxTrackSizingFunction_new_Fixed(const taffy_LengthPercentage* value)
{
    ASSERT_NOT_NULL(value);

    const taffy::LengthPercentage* v = reinterpret_cast<const taffy::LengthPercentage*>(value);
    return reinterpret_cast<taffy_MaxTrackSizingFunction*>( new taffy::MaxTrackSizingFunction{ taffy::MaxTrackSizingFunction::Fixed(*v) } );
}

taffy_MaxTrackSizingFunction* taffy_MaxTrackSizingFunction_new_MinContent(void)
{
    return reinterpret_cast<taffy_MaxTrackSizingFunction*>( new taffy::MaxTrackSizingFunction{ taffy::MaxTrackSizingFunction::MinContent() } );
}

taffy_MaxTrackSizingFunction* taffy_MaxTrackSizingFunction_new_MaxContent(void)
{
    return reinterpret_cast<taffy_MaxTrackSizingFunction*>( new taffy::MaxTrackSizingFunction{ taffy::MaxTrackSizingFunction::MaxContent() } );
}

taffy_MaxTrackSizingFunction* taffy_MaxTrackSizingFunction_new_FitContent(const taffy_LengthPercentage* value)
{
    ASSERT_NOT_NULL(value);

    const taffy::LengthPercentage* v = reinterpret_cast<const taffy::LengthPercentage*>(value);
    return reinterpret_cast<taffy_MaxTrackSizingFunction*>( new taffy::MaxTrackSizingFunction{ taffy::MaxTrackSizingFunction::FitContent(*v) } );
}

taffy_MaxTrackSizingFunction* taffy_MaxTrackSizingFunction_new_Auto(void)
{
    return reinterpret_cast<taffy_MaxTrackSizingFunction*>( new taffy::MaxTrackSizingFunction{ taffy::MaxTrackSizingFunction::Auto() } );
}

taffy_MaxTrackSizingFunction* taffy_MaxTrackSizingFunction_new_Fraction(float value)
{
    return reinterpret_cast<taffy_MaxTrackSizingFunction*>( new taffy::MaxTrackSizingFunction{ taffy::MaxTrackSizingFunction::Fraction(value) } );
}

taffy_MaxTrackSizingFunction* taffy_MaxTrackSizingFunction_new_copy(const taffy_MaxTrackSizingFunction* other)
{
    ASSERT_NOT_NULL(other);

    const taffy::MaxTrackSizingFunction* o = reinterpret_cast<const taffy::MaxTrackSizingFunction*>(other);
    return reinterpret_cast<taffy_MaxTrackSizingFunction*>( new taffy::MaxTrackSizingFunction{*o} );
}

void taffy_MaxTrackSizingFunction_delete(taffy_MaxTrackSizingFunction* self)
{
    ASSERT_NOT_NULL(self);

    delete reinterpret_cast<taffy::MaxTrackSizingFunction*>(self);
    self = nullptr;
}

taffy_MaxTrackSizingFunction* taffy_MaxTrackSizingFunction_new_AUTO(void)
{
    return reinterpret_cast<taffy_MaxTrackSizingFunction*>( new taffy::MaxTrackSizingFunction{ taffy::MaxTrackSizingFunction::AUTO() } );
}

taffy_MaxTrackSizingFunction* taffy_MaxTrackSizingFunction_new_MIN_CONTENT(void)
{
    return reinterpret_cast<taffy_MaxTrackSizingFunction*>( new taffy::MaxTrackSizingFunction{ taffy::MaxTrackSizingFunction::MIN_CONTENT() } );
}

taffy_MaxTrackSizingFunction* taffy_MaxTrackSizingFunction_new_MAX_CONTENT(void)
{
    return reinterpret_cast<taffy_MaxTrackSizingFunction*>( new taffy::MaxTrackSizingFunction{ taffy::MaxTrackSizingFunction::MAX_CONTENT() } );
}

taffy_MaxTrackSizingFunction* taffy_MaxTrackSizingFunction_new_ZERO(void)
{
    return reinterpret_cast<taffy_MaxTrackSizingFunction*>( new taffy::MaxTrackSizingFunction{ taffy::MaxTrackSizingFunction::ZERO() } );
}

taffy_MaxTrackSizingFunction* taffy_MaxTrackSizingFunction_new_fit_content(const taffy_LengthPercentage* argument)
{
    const taffy::LengthPercentage* arg = reinterpret_cast<const taffy::LengthPercentage*>(argument);
    return reinterpret_cast<taffy_MaxTrackSizingFunction*>( new taffy::MaxTrackSizingFunction{ taffy::MaxTrackSizingFunction::fit_content(*arg) } );
}

taffy_MaxTrackSizingFunction* taffy_MaxTrackSizingFunction_new_from_length(float value)
{
    return reinterpret_cast<taffy_MaxTrackSizingFunction*>( new taffy::MaxTrackSizingFunction{ taffy::MaxTrackSizingFunction::from_length(value) } );
}

taffy_MaxTrackSizingFunction* taffy_MaxTrackSizingFunction_new_from_percent(float percent)
{
    return reinterpret_cast<taffy_MaxTrackSizingFunction*>( new taffy::MaxTrackSizingFunction{ taffy::MaxTrackSizingFunction::from_percent(percent) } );
}

taffy_MaxTrackSizingFunction* taffy_MaxTrackSizingFunction_new_from_flex(float flex)
{
    return reinterpret_cast<taffy_MaxTrackSizingFunction*>( new taffy::MaxTrackSizingFunction{ taffy::MaxTrackSizingFunction::from_flex(flex) } );
}

// -----------------------------------------------------------------------------
// MinTrackSizingFunction

taffy_MinTrackSizingFunction* taffy_MinTrackSizingFunction_new_Fixed(const taffy_LengthPercentage* value)
{
    ASSERT_NOT_NULL(value);

    const taffy::LengthPercentage* v = reinterpret_cast<const taffy::LengthPercentage*>(value);
    return reinterpret_cast<taffy_MinTrackSizingFunction*>( new taffy::MinTrackSizingFunction{ taffy::MinTrackSizingFunction::Fixed(*v) } );
}

taffy_MinTrackSizingFunction* taffy_MinTrackSizingFunction_new_MinContent(void)
{
    return reinterpret_cast<taffy_MinTrackSizingFunction*>( new taffy::MinTrackSizingFunction{ taffy::MinTrackSizingFunction::MinContent() } );
}

taffy_MinTrackSizingFunction* taffy_MinTrackSizingFunction_new_MaxContent(void)
{
    return reinterpret_cast<taffy_MinTrackSizingFunction*>( new taffy::MinTrackSizingFunction{ taffy::MinTrackSizingFunction::MaxContent() } );
}

taffy_MinTrackSizingFunction* taffy_MinTrackSizingFunction_new_Auto(void)
{
    return reinterpret_cast<taffy_MinTrackSizingFunction*>( new taffy::MinTrackSizingFunction{ taffy::MinTrackSizingFunction::Auto() } );
}

taffy_MinTrackSizingFunction* taffy_MinTrackSizingFunction_new_copy(const taffy_MinTrackSizingFunction* other)
{
    ASSERT_NOT_NULL(other);

    const taffy::MinTrackSizingFunction* o = reinterpret_cast<const taffy::MinTrackSizingFunction*>(other);
    return reinterpret_cast<taffy_MinTrackSizingFunction*>( new taffy::MinTrackSizingFunction{*o} );
}

void taffy_MinTrackSizingFunction_delete(taffy_MinTrackSizingFunction* self)
{
    ASSERT_NOT_NULL(self);

    delete reinterpret_cast<taffy::MinTrackSizingFunction*>(self);
    self = nullptr;
}

taffy_MinTrackSizingFunction* taffy_MinTrackSizingFunction_new_AUTO(void)
{
    return reinterpret_cast<taffy_MinTrackSizingFunction*>( new taffy::MinTrackSizingFunction{ taffy::MinTrackSizingFunction::AUTO() } );
}

taffy_MinTrackSizingFunction* taffy_MinTrackSizingFunction_new_MIN_CONTENT(void)
{
    return reinterpret_cast<taffy_MinTrackSizingFunction*>( new taffy::MinTrackSizingFunction{ taffy::MinTrackSizingFunction::MIN_CONTENT() } );
}

taffy_MinTrackSizingFunction* taffy_MinTrackSizingFunction_new_MAX_CONTENT(void)
{
    return reinterpret_cast<taffy_MinTrackSizingFunction*>( new taffy::MinTrackSizingFunction{ taffy::MinTrackSizingFunction::MAX_CONTENT() } );
}

taffy_MinTrackSizingFunction* taffy_MinTrackSizingFunction_new_ZERO(void)
{
    return reinterpret_cast<taffy_MinTrackSizingFunction*>( new taffy::MinTrackSizingFunction{ taffy::MinTrackSizingFunction::ZERO() } );
}

taffy_MinTrackSizingFunction* taffy_MinTrackSizingFunction_new_from_length(float value)
{
    return reinterpret_cast<taffy_MinTrackSizingFunction*>( new taffy::MinTrackSizingFunction{ taffy::MinTrackSizingFunction::from_length(value) } );
}

taffy_MinTrackSizingFunction* taffy_MinTrackSizingFunction_new_from_percent(float percent)
{
    return reinterpret_cast<taffy_MinTrackSizingFunction*>( new taffy::MinTrackSizingFunction{ taffy::MinTrackSizingFunction::from_percent(percent) } );
}

// -----------------------------------------------------------------------------
// NonRepeatedTrackSizingFunction

taffy_NonRepeatedTrackSizingFunction* taffy_NonRepeatedTrackSizingFunction_new(const taffy_MinTrackSizingFunction* min, const taffy_MaxTrackSizingFunction* max)
{
    ASSERT_NOT_NULL(min);
    ASSERT_NOT_NULL(max);

    const taffy::MinTrackSizingFunction* _min = reinterpret_cast<const taffy::MinTrackSizingFunction*>(min);
    const taffy::MaxTrackSizingFunction* _max = reinterpret_cast<const taffy::MaxTrackSizingFunction*>(max);
    return reinterpret_cast<taffy_NonRepeatedTrackSizingFunction*>( new taffy::NonRepeatedTrackSizingFunction{*_min, *_max} );
}

taffy_NonRepeatedTrackSizingFunction* taffy_NonRepeatedTrackSizingFunction_new_copy(const taffy_NonRepeatedTrackSizingFunction* other)
{
    ASSERT_NOT_NULL(other);

    const taffy::NonRepeatedTrackSizingFunction* o = reinterpret_cast<const taffy::NonRepeatedTrackSizingFunction*>(other);
    return reinterpret_cast<taffy_NonRepeatedTrackSizingFunction*>( new taffy::NonRepeatedTrackSizingFunction{*o} );
}

void taffy_NonRepeatedTrackSizingFunction_delete(taffy_NonRepeatedTrackSizingFunction* self)
{
    ASSERT_NOT_NULL(self);

    delete reinterpret_cast<taffy::NonRepeatedTrackSizingFunction*>(self);
    self = nullptr;
}

const taffy_MinTrackSizingFunction* taffy_NonRepeatedTrackSizingFunction_get_min(const taffy_NonRepeatedTrackSizingFunction* self)
{
    ASSERT_NOT_NULL(self);

    return reinterpret_cast<const taffy_MinTrackSizingFunction*>( &( reinterpret_cast<const taffy::NonRepeatedTrackSizingFunction*>(self)->min ) );
}

const taffy_MaxTrackSizingFunction* taffy_NonRepeatedTrackSizingFunction_get_max(const taffy_NonRepeatedTrackSizingFunction* self)
{
    ASSERT_NOT_NULL(self);

    return reinterpret_cast<const taffy_MaxTrackSizingFunction*>( &( reinterpret_cast<const taffy::NonRepeatedTrackSizingFunction*>(self)->max ) );
}

void taffy_NonRepeatedTrackSizingFunction_set_min(taffy_NonRepeatedTrackSizingFunction* self, const taffy_MinTrackSizingFunction* min)
{
    ASSERT_NOT_NULL(self);
    ASSERT_NOT_NULL(min);

    const taffy::MinTrackSizingFunction* _min = reinterpret_cast<const taffy::MinTrackSizingFunction*>(min);
    reinterpret_cast<taffy::NonRepeatedTrackSizingFunction*>(self)->min = *_min;
}

void taffy_NonRepeatedTrackSizingFunction_set_max(taffy_NonRepeatedTrackSizingFunction* self, const taffy_MinTrackSizingFunction* max)
{
    ASSERT_NOT_NULL(self);
    ASSERT_NOT_NULL(max);

    const taffy::MaxTrackSizingFunction* _max = reinterpret_cast<const taffy::MaxTrackSizingFunction*>(max);
    reinterpret_cast<taffy::NonRepeatedTrackSizingFunction*>(self)->max = *_max;
}

taffy_MinTrackSizingFunction* taffy_NonRepeatedTrackSizingFunction_get_mut_min(taffy_NonRepeatedTrackSizingFunction* self)
{
    ASSERT_NOT_NULL(self);

    return reinterpret_cast<taffy_MinTrackSizingFunction*>( &( reinterpret_cast<taffy::NonRepeatedTrackSizingFunction*>(self)->min ) );
}

taffy_MaxTrackSizingFunction* taffy_NonRepeatedTrackSizingFunction_get_mut_max(taffy_NonRepeatedTrackSizingFunction* self)
{
    ASSERT_NOT_NULL(self);

    return reinterpret_cast<taffy_MaxTrackSizingFunction*>( &( reinterpret_cast<taffy::NonRepeatedTrackSizingFunction*>(self)->max ) );
}

// -----------------------------------------------------------------------------
// TrackSizingFunction

taffy_TrackSizingFunction* taffy_TrackSizingFunction_new_Single(const taffy_NonRepeatedTrackSizingFunction* value)
{
    ASSERT_NOT_NULL(value);

    const taffy::NonRepeatedTrackSizingFunction* v = reinterpret_cast<const taffy::NonRepeatedTrackSizingFunction*>(value);
    return reinterpret_cast<taffy_TrackSizingFunction*>( new taffy::TrackSizingFunction{ taffy::TrackSizingFunction::Single(*v) } );
}

taffy_TrackSizingFunction* taffy_TrackSizingFunction_new_Repeat(
    const taffy_GridTrackRepetition* repetition,
    const taffy_NonRepeatedTrackSizingFunction* funcs, size_t funcs_count
)
{
    ASSERT_NOT_NULL(repetition);

    if(funcs_count > 0)
    {
        ASSERT_NOT_NULL(funcs);
    }

    
    const taffy::GridTrackRepetition* _repetition = reinterpret_cast<const taffy::GridTrackRepetition*>(repetition);
    const taffy::NonRepeatedTrackSizingFunction* _funcs = reinterpret_cast<const taffy::NonRepeatedTrackSizingFunction*>(funcs);

    return reinterpret_cast<taffy_TrackSizingFunction*>( 
        new taffy::TrackSizingFunction { 
            taffy::TrackSizingFunction::Repeat(
                *_repetition, 
                taffy::GridTrackVec<taffy::NonRepeatedTrackSizingFunction>{_funcs, _funcs + funcs_count} // NOTE: Entire vector copy here :/
            )
        } 
    );
}

taffy_TrackSizingFunction* taffy_TrackSizingFunction_new_copy(const taffy_TrackSizingFunction* other)
{
    ASSERT_NOT_NULL(other);

    const taffy::TrackSizingFunction* o = reinterpret_cast<const taffy::TrackSizingFunction*>(other);
    return reinterpret_cast<taffy_TrackSizingFunction*>( new taffy::TrackSizingFunction{*o} );
}

void taffy_TrackSizingFunction_delete(taffy_TrackSizingFunction* self)
{
    ASSERT_NOT_NULL(self);

    delete reinterpret_cast<taffy::TrackSizingFunction*>(self);
    self = nullptr;
}

taffy_TrackSizingFunction* taffy_TrackSizingFunction_new_AUTO(void)
{
    return reinterpret_cast<taffy_TrackSizingFunction*>( new taffy::TrackSizingFunction{ taffy::TrackSizingFunction::AUTO() } );
}

taffy_TrackSizingFunction* taffy_TrackSizingFunction_new_MIN_CONTENT(void)
{
    return reinterpret_cast<taffy_TrackSizingFunction*>( new taffy::TrackSizingFunction{ taffy::TrackSizingFunction::MIN_CONTENT() } );
}

taffy_TrackSizingFunction* taffy_TrackSizingFunction_new_MAX_CONTENT(void)
{
    return reinterpret_cast<taffy_TrackSizingFunction*>( new taffy::TrackSizingFunction{ taffy::TrackSizingFunction::MAX_CONTENT() } );
}

taffy_TrackSizingFunction* taffy_TrackSizingFunction_new_ZERO(void)
{
    return reinterpret_cast<taffy_TrackSizingFunction*>( new taffy::TrackSizingFunction{ taffy::TrackSizingFunction::ZERO() } );
}

taffy_TrackSizingFunction* taffy_TrackSizingFunction_new_fit_content(const taffy_LengthPercentage* argument)
{
    ASSERT_NOT_NULL(argument);

    const taffy::LengthPercentage* arg = reinterpret_cast<const taffy::LengthPercentage*>(argument);
    return reinterpret_cast<taffy_TrackSizingFunction*>( new taffy::TrackSizingFunction{ taffy::TrackSizingFunction::fit_content(*arg) } );
}

taffy_TrackSizingFunction* taffy_TrackSizingFunction_new_from_length(float value)
{
    return reinterpret_cast<taffy_TrackSizingFunction*>( new taffy::TrackSizingFunction{ taffy::TrackSizingFunction::from_length(value) } );
}

taffy_TrackSizingFunction* taffy_TrackSizingFunction_new_from_percent(float percent)
{
    return reinterpret_cast<taffy_TrackSizingFunction*>( new taffy::TrackSizingFunction{ taffy::TrackSizingFunction::from_percent(percent) } );
}

taffy_TrackSizingFunction* taffy_TrackSizingFunction_new_from_flex(float flex)
{
    return reinterpret_cast<taffy_TrackSizingFunction*>( new taffy::TrackSizingFunction{ taffy::TrackSizingFunction::from_flex(flex) } );
}

// -----------------------------------------------------------------------------
// Display

static taffy::Display taffy_Display_to_cpp(const taffy_Display v)
{
    switch(v) {
    case taffy_Display_Flex  : return taffy::Display::Flex();
    case taffy_Display_Grid  : return taffy::Display::Grid();
    case taffy_Display_Block : return taffy::Display::Block();
    case taffy_Display_None  : return taffy::Display::None();
    }

    ASSERT_UNREACHABLE();
    return taffy::Display{};
}

static taffy_Display taffy_Display_to_c(const taffy::Display v)
{
    switch(v.type()) {
    case taffy::Display::Type::Flex  : return taffy_Display_Flex;
    case taffy::Display::Type::Grid  : return taffy_Display_Grid;
    case taffy::Display::Type::Block : return taffy_Display_Block;
    case taffy::Display::Type::None  : return taffy_Display_None;
    }

    ASSERT_UNREACHABLE();
    return taffy_Display_Flex;
}

taffy_Display taffy_Display_Default(void)
{
    return taffy_Display_to_c( taffy::Display{} );
}

// -----------------------------------------------------------------------------
// Overflow

static taffy::Overflow taffy_Overflow_to_cpp(const taffy_Overflow v)
{
    switch(v) {
    case taffy_Overflow_Visible  : return taffy::Overflow::Visible();
    case taffy_Overflow_Hidden   : return taffy::Overflow::Hidden();
    case taffy_Overflow_Scroll   : return taffy::Overflow::Scroll();
    }

    ASSERT_UNREACHABLE();
    return taffy::Overflow{};
}

static taffy_Overflow taffy_Overflow_to_c(const taffy::Overflow v)
{
    switch(v.type()) {
    case taffy::Overflow::Type::Visible  : return taffy_Overflow_Visible;
    case taffy::Overflow::Type::Hidden   : return taffy_Overflow_Hidden;
    case taffy::Overflow::Type::Scroll   : return taffy_Overflow_Scroll;
    }

    ASSERT_UNREACHABLE();
    return taffy_Overflow_Visible;
}

taffy_Overflow taffy_Overflow_Default(void)
{
    return taffy_Overflow_to_c( taffy::Overflow{} );
}

// -----------------------------------------------------------------------------
// Position

static taffy::Position taffy_Position_to_cpp(const taffy_Position v)
{
    switch(v) {
    case taffy_Position_Relative : return taffy::Position::Relative();
    case taffy_Position_Absolute : return taffy::Position::Absolute();
    }

    ASSERT_UNREACHABLE();
    return taffy::Position{};
}

static taffy_Position taffy_Position_to_c(const taffy::Position v)
{
    switch(v.type()) {
    case taffy::Position::Type::Relative : return taffy_Position_Relative;
    case taffy::Position::Type::Absolute : return taffy_Position_Absolute;
    }

    ASSERT_UNREACHABLE();
    return taffy_Position_Relative;
}

taffy_Position taffy_Position_Default(void)
{
    return taffy_Position_to_c( taffy::Position{} );
}

// -----------------------------------------------------------------------------
// Point<Overflow>

taffy_Point_of_Overflow* taffy_Point_of_Overflow_new_default(void)
{
    return reinterpret_cast<taffy_Point_of_Overflow*>( new taffy::Point<taffy::Overflow>{} );
}

taffy_Point_of_Overflow* taffy_Point_of_Overflow_new(taffy_Overflow x, taffy_Overflow y)
{
    return reinterpret_cast<taffy_Point_of_Overflow*>( 
        new taffy::Point<taffy::Overflow>{
            taffy_Overflow_to_cpp(x), 
            taffy_Overflow_to_cpp(y)
        }
    );
}

taffy_Point_of_Overflow* taffy_Point_of_Overflow_new_copy(const taffy_Point_of_Overflow* other)
{
    ASSERT_NOT_NULL(other);

    const taffy::Point<taffy::Overflow>* o = reinterpret_cast<const taffy::Point<taffy::Overflow>*>(other);
    return reinterpret_cast<taffy_Point_of_Overflow*>( new taffy::Point<taffy::Overflow>{*o} );
}

void taffy_Point_of_Overflow_delete(taffy_Point_of_Overflow* self)
{
    ASSERT_NOT_NULL(self);

    delete reinterpret_cast<taffy::Point<taffy::Overflow>*>(self);
    self = nullptr;
}

taffy_Overflow taffy_Point_of_Overflow_get_x(const taffy_Point_of_Overflow* self)
{
    ASSERT_NOT_NULL(self);

    return taffy_Overflow_to_c( reinterpret_cast<const taffy::Point<taffy::Overflow>*>(self)->x );
}

taffy_Overflow taffy_Point_of_Overflow_get_y(const taffy_Point_of_Overflow* self)
{
    ASSERT_NOT_NULL(self);

    return taffy_Overflow_to_c( reinterpret_cast<const taffy::Point<taffy::Overflow>*>(self)->y );
}

void taffy_Point_of_Overflow_set_x(taffy_Point_of_Overflow* self, taffy_Overflow x)
{
    ASSERT_NOT_NULL(self);

    reinterpret_cast<taffy::Point<taffy::Overflow>*>(self)->x = taffy_Overflow_to_cpp(x);
}

void taffy_Point_of_Overflow_set_y(taffy_Point_of_Overflow* self, taffy_Overflow y)
{
    ASSERT_NOT_NULL(self);

    reinterpret_cast<taffy::Point<taffy::Overflow>*>(self)->y = taffy_Overflow_to_cpp(y);
}

// -----------------------------------------------------------------------------
// Rect<LengthPercentage>

taffy_Rect_of_LengthPercentage* taffy_Rect_of_LengthPercentage_new(
    const taffy_LengthPercentage* left, 
    const taffy_LengthPercentage* top,
    const taffy_LengthPercentage* right,
    const taffy_LengthPercentage* bottom
)
{
    ASSERT_NOT_NULL(left);
    ASSERT_NOT_NULL(top);
    ASSERT_NOT_NULL(right);
    ASSERT_NOT_NULL(botom);

    const taffy::LengthPercentage* _left   = reinterpret_cast<const taffy::LengthPercentage*>(left);
    const taffy::LengthPercentage* _top    = reinterpret_cast<const taffy::LengthPercentage*>(top);
    const taffy::LengthPercentage* _right  = reinterpret_cast<const taffy::LengthPercentage*>(right);
    const taffy::LengthPercentage* _bottom = reinterpret_cast<const taffy::LengthPercentage*>(bottom);

    return reinterpret_cast<taffy_Rect_of_LengthPercentage*>(
        new taffy::Rect<taffy::LengthPercentage>{ *_left, *_top, *_right, *_bottom }
    );
}

taffy_Rect_of_LengthPercentage* taffy_Rect_of_LengthPercentage_new_copy(const taffy_Rect_of_LengthPercentage* other)
{
    ASSERT_NOT_NULL(other);

    const taffy::Rect<taffy::LengthPercentage>* o = reinterpret_cast<const taffy::Rect<taffy::LengthPercentage>*>(other);
    return reinterpret_cast<taffy_Rect_of_LengthPercentage*>( new taffy::Rect<taffy::LengthPercentage>{*o} );
}

void taffy_Rect_of_LengthPercentage_delete(taffy_Rect_of_LengthPercentage* self)
{
    ASSERT_NOT_NULL(self);

    delete reinterpret_cast<taffy::Rect<taffy::LengthPercentage>*>(self);
    self = nullptr;
}

const taffy_LengthPercentage* taffy_Rect_of_LengthPercentage_get_left(const taffy_Rect_of_LengthPercentage* self)
{
    ASSERT_NOT_NULL(self);

    return reinterpret_cast<const taffy_LengthPercentage*>( &( reinterpret_cast<const taffy::Rect<taffy::LengthPercentage>*>(self)->left ) );
}

const taffy_LengthPercentage* taffy_Rect_of_LengthPercentage_get_top(const taffy_Rect_of_LengthPercentage* self)
{
    ASSERT_NOT_NULL(self);

    return reinterpret_cast<const taffy_LengthPercentage*>( &( reinterpret_cast<const taffy::Rect<taffy::LengthPercentage>*>(self)->top ) );
}

const taffy_LengthPercentage* taffy_Rect_of_LengthPercentage_get_right(const taffy_Rect_of_LengthPercentage* self)
{
    ASSERT_NOT_NULL(self);

    return reinterpret_cast<const taffy_LengthPercentage*>( &( reinterpret_cast<const taffy::Rect<taffy::LengthPercentage>*>(self)->right ) );
}

const taffy_LengthPercentage* taffy_Rect_of_LengthPercentage_get_bottom(const taffy_Rect_of_LengthPercentage* self)
{
    ASSERT_NOT_NULL(self);

    return reinterpret_cast<const taffy_LengthPercentage*>( &( reinterpret_cast<const taffy::Rect<taffy::LengthPercentage>*>(self)->bottom ) );
}

void taffy_Rect_of_LengthPercentage_set_left(taffy_Rect_of_LengthPercentage* self, const taffy_LengthPercentage* left)
{
    ASSERT_NOT_NULL(self);
    ASSERT_NOT_NULL(left);

    const taffy::LengthPercentage* _left = reinterpret_cast<const taffy::LengthPercentage*>(left);
    reinterpret_cast<taffy::Rect<taffy::LengthPercentage>*>(self)->left = *_left;
}

void taffy_Rect_of_LengthPercentage_set_top(taffy_Rect_of_LengthPercentage* self, const taffy_LengthPercentage* top)
{
    ASSERT_NOT_NULL(self);
    ASSERT_NOT_NULL(top);

    const taffy::LengthPercentage* _top = reinterpret_cast<const taffy::LengthPercentage*>(top);
    reinterpret_cast<taffy::Rect<taffy::LengthPercentage>*>(self)->top = *_top;
}

void taffy_Rect_of_LengthPercentage_set_right(taffy_Rect_of_LengthPercentage* self, const taffy_LengthPercentage* right)
{
    ASSERT_NOT_NULL(self);
    ASSERT_NOT_NULL(right);

    const taffy::LengthPercentage* _right = reinterpret_cast<const taffy::LengthPercentage*>(right);
    reinterpret_cast<taffy::Rect<taffy::LengthPercentage>*>(self)->right = *_right;
}

void taffy_Rect_of_LengthPercentage_set_bottom(taffy_Rect_of_LengthPercentage* self, const taffy_LengthPercentage* bottom)
{
    ASSERT_NOT_NULL(self);
    ASSERT_NOT_NULL(bottom);

    const taffy::LengthPercentage* _bottom = reinterpret_cast<const taffy::LengthPercentage*>(bottom);
    reinterpret_cast<taffy::Rect<taffy::LengthPercentage>*>(self)->bottom = *_bottom;
}

taffy_LengthPercentage* taffy_Rect_of_LengthPercentage_get_mut_left(taffy_Rect_of_LengthPercentage* self)
{
    ASSERT_NOT_NULL(self);

    return reinterpret_cast<taffy_LengthPercentage*>( &( reinterpret_cast<taffy::Rect<taffy::LengthPercentage>*>(self)->left ) );
}

taffy_LengthPercentage* taffy_Rect_of_LengthPercentage_get_mut_top(taffy_Rect_of_LengthPercentage* self)
{
    ASSERT_NOT_NULL(self);

    return reinterpret_cast<taffy_LengthPercentage*>( &( reinterpret_cast<taffy::Rect<taffy::LengthPercentage>*>(self)->top ) );
}

taffy_LengthPercentage* taffy_Rect_of_LengthPercentage_get_mut_right(taffy_Rect_of_LengthPercentage* self)
{
    ASSERT_NOT_NULL(self);

    return reinterpret_cast<taffy_LengthPercentage*>( &( reinterpret_cast<taffy::Rect<taffy::LengthPercentage>*>(self)->right ) );
}

taffy_LengthPercentage* taffy_Rect_of_LengthPercentage_get_mut_bottom(taffy_Rect_of_LengthPercentage* self)
{
    ASSERT_NOT_NULL(self);

    return reinterpret_cast<taffy_LengthPercentage*>( &( reinterpret_cast<taffy::Rect<taffy::LengthPercentage>*>(self)->bottom ) );
}

// -----------------------------------------------------------------------------
// Rect<LengthPercentageAuto>

taffy_Rect_of_LengthPercentageAuto* taffy_Rect_of_LengthPercentageAuto_new(
    const taffy_LengthPercentageAuto* left, 
    const taffy_LengthPercentageAuto* top,
    const taffy_LengthPercentageAuto* right,
    const taffy_LengthPercentageAuto* bottom
)
{
    ASSERT_NOT_NULL(left);
    ASSERT_NOT_NULL(top);
    ASSERT_NOT_NULL(right);
    ASSERT_NOT_NULL(botom);

    const taffy::LengthPercentageAuto* _left   = reinterpret_cast<const taffy::LengthPercentageAuto*>(left);
    const taffy::LengthPercentageAuto* _top    = reinterpret_cast<const taffy::LengthPercentageAuto*>(top);
    const taffy::LengthPercentageAuto* _right  = reinterpret_cast<const taffy::LengthPercentageAuto*>(right);
    const taffy::LengthPercentageAuto* _bottom = reinterpret_cast<const taffy::LengthPercentageAuto*>(bottom);

    return reinterpret_cast<taffy_Rect_of_LengthPercentageAuto*>(
        new taffy::Rect<taffy::LengthPercentageAuto>{ *_left, *_top, *_right, *_bottom }
    );
}

taffy_Rect_of_LengthPercentageAuto* taffy_Rect_of_LengthPercentageAuto_new_copy(const taffy_Rect_of_LengthPercentageAuto* other)
{
    ASSERT_NOT_NULL(other);

    const taffy::Rect<taffy::LengthPercentageAuto>* o = reinterpret_cast<const taffy::Rect<taffy::LengthPercentageAuto>*>(other);
    return reinterpret_cast<taffy_Rect_of_LengthPercentageAuto*>( new taffy::Rect<taffy::LengthPercentageAuto>{*o} );
}

void taffy_Rect_of_LengthPercentageAuto_delete(taffy_Rect_of_LengthPercentageAuto* self)
{
    ASSERT_NOT_NULL(self);

    delete reinterpret_cast<taffy::Rect<taffy::LengthPercentageAuto>*>(self);
    self = nullptr;
}

const taffy_LengthPercentageAuto* taffy_Rect_of_LengthPercentageAuto_get_left(const taffy_Rect_of_LengthPercentageAuto* self)
{
    ASSERT_NOT_NULL(self);

    return reinterpret_cast<const taffy_LengthPercentageAuto*>( &( reinterpret_cast<const taffy::Rect<taffy::LengthPercentageAuto>*>(self)->left ) );
}

const taffy_LengthPercentageAuto* taffy_Rect_of_LengthPercentageAuto_get_top(const taffy_Rect_of_LengthPercentageAuto* self)
{
    ASSERT_NOT_NULL(self);

    return reinterpret_cast<const taffy_LengthPercentageAuto*>( &( reinterpret_cast<const taffy::Rect<taffy::LengthPercentageAuto>*>(self)->top ) );
}

const taffy_LengthPercentageAuto* taffy_Rect_of_LengthPercentageAuto_get_right(const taffy_Rect_of_LengthPercentageAuto* self)
{
    ASSERT_NOT_NULL(self);

    return reinterpret_cast<const taffy_LengthPercentageAuto*>( &( reinterpret_cast<const taffy::Rect<taffy::LengthPercentageAuto>*>(self)->right ) );
}

const taffy_LengthPercentageAuto* taffy_Rect_of_LengthPercentageAuto_get_bottom(const taffy_Rect_of_LengthPercentageAuto* self)
{
    ASSERT_NOT_NULL(self);

    return reinterpret_cast<const taffy_LengthPercentageAuto*>( &( reinterpret_cast<const taffy::Rect<taffy::LengthPercentageAuto>*>(self)->bottom ) );
}

void taffy_Rect_of_LengthPercentageAuto_set_left(taffy_Rect_of_LengthPercentageAuto* self, const taffy_LengthPercentageAuto* left)
{
    ASSERT_NOT_NULL(self);
    ASSERT_NOT_NULL(left);

    const taffy::LengthPercentageAuto* _left = reinterpret_cast<const taffy::LengthPercentageAuto*>(left);
    reinterpret_cast<taffy::Rect<taffy::LengthPercentageAuto>*>(self)->left = *_left;
}

void taffy_Rect_of_LengthPercentageAuto_set_top(taffy_Rect_of_LengthPercentageAuto* self, const taffy_LengthPercentageAuto* top)
{
    ASSERT_NOT_NULL(self);
    ASSERT_NOT_NULL(top);

    const taffy::LengthPercentageAuto* _top = reinterpret_cast<const taffy::LengthPercentageAuto*>(top);
    reinterpret_cast<taffy::Rect<taffy::LengthPercentageAuto>*>(self)->top = *_top;
}

void taffy_Rect_of_LengthPercentageAuto_set_right(taffy_Rect_of_LengthPercentageAuto* self, const taffy_LengthPercentageAuto* right)
{
    ASSERT_NOT_NULL(self);
    ASSERT_NOT_NULL(right);

    const taffy::LengthPercentageAuto* _right = reinterpret_cast<const taffy::LengthPercentageAuto*>(right);
    reinterpret_cast<taffy::Rect<taffy::LengthPercentageAuto>*>(self)->right = *_right;
}

void taffy_Rect_of_LengthPercentageAuto_set_bottom(taffy_Rect_of_LengthPercentageAuto* self, const taffy_LengthPercentageAuto* bottom)
{
    ASSERT_NOT_NULL(self);
    ASSERT_NOT_NULL(bottom);

    const taffy::LengthPercentageAuto* _bottom = reinterpret_cast<const taffy::LengthPercentageAuto*>(bottom);
    reinterpret_cast<taffy::Rect<taffy::LengthPercentageAuto>*>(self)->bottom = *_bottom;
}

taffy_LengthPercentageAuto* taffy_Rect_of_LengthPercentageAuto_get_mut_left(taffy_Rect_of_LengthPercentageAuto* self)
{
    ASSERT_NOT_NULL(self);

    return reinterpret_cast<taffy_LengthPercentageAuto*>( &( reinterpret_cast<taffy::Rect<taffy::LengthPercentageAuto>*>(self)->left ) );
}

taffy_LengthPercentageAuto* taffy_Rect_of_LengthPercentageAuto_get_mut_top(taffy_Rect_of_LengthPercentageAuto* self)
{
    ASSERT_NOT_NULL(self);

    return reinterpret_cast<taffy_LengthPercentageAuto*>( &( reinterpret_cast<taffy::Rect<taffy::LengthPercentageAuto>*>(self)->top ) );
}

taffy_LengthPercentageAuto* taffy_Rect_of_LengthPercentageAuto_get_mut_right(taffy_Rect_of_LengthPercentageAuto* self)
{
    ASSERT_NOT_NULL(self);

    return reinterpret_cast<taffy_LengthPercentageAuto*>( &( reinterpret_cast<taffy::Rect<taffy::LengthPercentageAuto>*>(self)->right ) );
}

taffy_LengthPercentageAuto* taffy_Rect_of_LengthPercentageAuto_get_mut_bottom(taffy_Rect_of_LengthPercentageAuto* self)
{
    ASSERT_NOT_NULL(self);

    return reinterpret_cast<taffy_LengthPercentageAuto*>( &( reinterpret_cast<taffy::Rect<taffy::LengthPercentageAuto>*>(self)->bottom ) );
}

// -----------------------------------------------------------------------------
// Size<LengthPercentage>

taffy_Size_of_LengthPercentage* taffy_Size_of_LengthPercentage_new(const taffy_LengthPercentage* width, const taffy_LengthPercentage* height)
{
    ASSERT_NOT_NULL(width);
    ASSERT_NOT_NULL(height);

    const taffy::LengthPercentage* _width  = reinterpret_cast<const taffy::LengthPercentage*>(width);
    const taffy::LengthPercentage* _height = reinterpret_cast<const taffy::LengthPercentage*>(height);

    return reinterpret_cast<taffy_Size_of_LengthPercentage*>(
        new taffy::Size<taffy::LengthPercentage>{ *_width, *_height }
    );
}

taffy_Size_of_LengthPercentage* taffy_Size_of_LengthPercentage_new_copy(const taffy_Size_of_LengthPercentage* other)
{
    ASSERT_NOT_NULL(other);

    const taffy::Size<taffy::LengthPercentage>* o = reinterpret_cast<const taffy::Size<taffy::LengthPercentage>*>(other);
    return reinterpret_cast<taffy_Size_of_LengthPercentage*>( new taffy::Size<taffy::LengthPercentage>{*o} );
}

void taffy_Size_of_LengthPercentage_delete(taffy_Size_of_LengthPercentage* self)
{
    ASSERT_NOT_NULL(self);

    delete reinterpret_cast<taffy::Size<taffy::LengthPercentage>*>(self);
    self = nullptr;
}

const taffy_LengthPercentage* taffy_Size_of_LengthPercentage_get_width (const taffy_Size_of_LengthPercentage* self)
{
    ASSERT_NOT_NULL(self);

    return reinterpret_cast<const taffy_LengthPercentage*>( &( reinterpret_cast<const taffy::Size<taffy::LengthPercentage>*>(self)->width ) );
}

const taffy_LengthPercentage* taffy_Size_of_LengthPercentage_get_height(const taffy_Size_of_LengthPercentage* self)
{
    ASSERT_NOT_NULL(self);

    return reinterpret_cast<const taffy_LengthPercentage*>( &( reinterpret_cast<const taffy::Size<taffy::LengthPercentage>*>(self)->height ) );
}

void taffy_Size_of_LengthPercentage_set_width(taffy_Size_of_LengthPercentage* self, const taffy_LengthPercentage* width)
{
    ASSERT_NOT_NULL(self);
    ASSERT_NOT_NULL(width);

    const taffy::LengthPercentage* _width = reinterpret_cast<const taffy::LengthPercentage*>(width);
    reinterpret_cast<taffy::Size<taffy::LengthPercentage>*>(self)->width = *_width;
}

void taffy_Size_of_LengthPercentage_set_height(taffy_Size_of_LengthPercentage* self, const taffy_LengthPercentage* height)
{
    ASSERT_NOT_NULL(self);
    ASSERT_NOT_NULL(height);

    const taffy::LengthPercentage* _height = reinterpret_cast<const taffy::LengthPercentage*>(height);
    reinterpret_cast<taffy::Size<taffy::LengthPercentage>*>(self)->height = *_height;
}

taffy_LengthPercentage* taffy_Size_of_LengthPercentage_get_mut_width(taffy_Size_of_LengthPercentage* self)
{
    ASSERT_NOT_NULL(self);

    return reinterpret_cast<taffy_LengthPercentage*>( &( reinterpret_cast<taffy::Size<taffy::LengthPercentage>*>(self)->width ) );
}

taffy_LengthPercentage* taffy_Size_of_LengthPercentage_get_mut_height(taffy_Size_of_LengthPercentage* self)
{
    ASSERT_NOT_NULL(self);

    return reinterpret_cast<taffy_LengthPercentage*>( &( reinterpret_cast<taffy::Size<taffy::LengthPercentage>*>(self)->height ) );
}

// -----------------------------------------------------------------------------
// Size<Dimension>

taffy_Size_of_Dimension* taffy_Size_of_Dimension_new(const taffy_Dimension* width, const taffy_Dimension* height)
{
    ASSERT_NOT_NULL(width);
    ASSERT_NOT_NULL(height);

    const taffy::Dimension* _width  = reinterpret_cast<const taffy::Dimension*>(width);
    const taffy::Dimension* _height = reinterpret_cast<const taffy::Dimension*>(height);

    return reinterpret_cast<taffy_Size_of_Dimension*>(
        new taffy::Size<taffy::Dimension>{ *_width, *_height }
    );
}

taffy_Size_of_Dimension* taffy_Size_of_Dimension_new_copy(const taffy_Size_of_Dimension* other)
{
    ASSERT_NOT_NULL(other);

    const taffy::Size<taffy::Dimension>* o = reinterpret_cast<const taffy::Size<taffy::Dimension>*>(other);
    return reinterpret_cast<taffy_Size_of_Dimension*>( new taffy::Size<taffy::Dimension>{*o} );
}

void taffy_Size_of_Dimension_delete(taffy_Size_of_Dimension* self)
{
    ASSERT_NOT_NULL(self);

    delete reinterpret_cast<taffy::Size<taffy::Dimension>*>(self);
    self = nullptr;
}

const taffy_Dimension* taffy_Size_of_Dimension_get_width (const taffy_Size_of_Dimension* self)
{
    ASSERT_NOT_NULL(self);

    return reinterpret_cast<const taffy_Dimension*>( &( reinterpret_cast<const taffy::Size<taffy::Dimension>*>(self)->width ) );
}

const taffy_Dimension* taffy_Size_of_Dimension_get_height(const taffy_Size_of_Dimension* self)
{
    ASSERT_NOT_NULL(self);

    return reinterpret_cast<const taffy_Dimension*>( &( reinterpret_cast<const taffy::Size<taffy::Dimension>*>(self)->height ) );
}

void taffy_Size_of_Dimension_set_width(taffy_Size_of_Dimension* self, const taffy_Dimension* width)
{
    ASSERT_NOT_NULL(self);
    ASSERT_NOT_NULL(width);

    const taffy::Dimension* _width = reinterpret_cast<const taffy::Dimension*>(width);
    reinterpret_cast<taffy::Size<taffy::Dimension>*>(self)->width = *_width;
}

void taffy_Size_of_Dimension_set_height(taffy_Size_of_Dimension* self, const taffy_Dimension* height)
{
    ASSERT_NOT_NULL(self);
    ASSERT_NOT_NULL(height);

    const taffy::Dimension* _height = reinterpret_cast<const taffy::Dimension*>(height);
    reinterpret_cast<taffy::Size<taffy::Dimension>*>(self)->height = *_height;
}

taffy_Dimension* taffy_Size_of_Dimension_get_mut_width(taffy_Size_of_Dimension* self)
{
    ASSERT_NOT_NULL(self);

    return reinterpret_cast<taffy_Dimension*>( &( reinterpret_cast<taffy::Size<taffy::Dimension>*>(self)->width ) );
}

taffy_Dimension* taffy_Size_of_Dimension_get_mut_height(taffy_Size_of_Dimension* self)
{
    ASSERT_NOT_NULL(self);

    return reinterpret_cast<taffy_Dimension*>( &( reinterpret_cast<taffy::Size<taffy::Dimension>*>(self)->height ) );
}

// -----------------------------------------------------------------------------
// Line<GridPlacement>

taffy_Line_of_GridPlacement* taffy_Line_of_GridPlacement_new_default(void)
{
    return reinterpret_cast<taffy_Line_of_GridPlacement*>(
        new taffy::Line<taffy::GridPlacement>{}
    );
}

taffy_Line_of_GridPlacement* taffy_Line_of_GridPlacement_new(const taffy_GridPlacement* start, const taffy_GridPlacement* end)
{
    ASSERT_NOT_NULL(start);
    ASSERT_NOT_NULL(end);

    const taffy::GridPlacement* _start = reinterpret_cast<const taffy::GridPlacement*>(start);
    const taffy::GridPlacement* _end   = reinterpret_cast<const taffy::GridPlacement*>(end);

    return reinterpret_cast<taffy_Line_of_GridPlacement*>(
        new taffy::Line<taffy::GridPlacement>{ *_start, *_end }
    );
}

taffy_Line_of_GridPlacement* taffy_Line_of_GridPlacement_new_copy(const taffy_Line_of_GridPlacement* other)
{
    ASSERT_NOT_NULL(other);

    const taffy::Line<taffy::GridPlacement>* o = reinterpret_cast<const taffy::Line<taffy::GridPlacement>*>(other);
    return reinterpret_cast<taffy_Line_of_GridPlacement*>( new taffy::Line<taffy::GridPlacement>{*o} );
}

void taffy_Line_of_GridPlacement_delete(taffy_Line_of_GridPlacement* self)
{
    ASSERT_NOT_NULL(self);

    delete reinterpret_cast<taffy::Line<taffy::GridPlacement>*>(self);
    self = nullptr;
}

const taffy_GridPlacement* taffy_Line_of_GridPlacement_get_start(const taffy_Line_of_GridPlacement* self)
{
    ASSERT_NOT_NULL(self);

    return reinterpret_cast<const taffy_GridPlacement*>( &( reinterpret_cast<const taffy::Line<taffy::GridPlacement>*>(self)->start ) );
}

const taffy_GridPlacement* taffy_Line_of_GridPlacement_get_end(const taffy_Line_of_GridPlacement* self)
{
    ASSERT_NOT_NULL(self);

    return reinterpret_cast<const taffy_GridPlacement*>( &( reinterpret_cast<const taffy::Line<taffy::GridPlacement>*>(self)->end ) );
}

void taffy_Line_of_GridPlacement_set_start(taffy_Line_of_GridPlacement* self, const taffy_GridPlacement* start)
{
    ASSERT_NOT_NULL(self);
    ASSERT_NOT_NULL(start);

    const taffy::GridPlacement* _start = reinterpret_cast<const taffy::GridPlacement*>(start);
    reinterpret_cast<taffy::Line<taffy::GridPlacement>*>(self)->start = *_start;
}

void taffy_Line_of_GridPlacement_set_end(taffy_Line_of_GridPlacement* self, const taffy_GridPlacement* end)
{
    ASSERT_NOT_NULL(self);
    ASSERT_NOT_NULL(end);

    const taffy::GridPlacement* _end = reinterpret_cast<const taffy::GridPlacement*>(end);
    reinterpret_cast<taffy::Line<taffy::GridPlacement>*>(self)->end = *_end;
}

taffy_GridPlacement* taffy_Line_of_GridPlacement_get_mut_start(taffy_Line_of_GridPlacement* self)
{
    ASSERT_NOT_NULL(self);

    return reinterpret_cast<taffy_GridPlacement*>( &( reinterpret_cast<taffy::Line<taffy::GridPlacement>*>(self)->start ) );
}

taffy_GridPlacement* taffy_Line_of_GridPlacement_get_mut_end(taffy_Line_of_GridPlacement* self)
{
    ASSERT_NOT_NULL(self);

    return reinterpret_cast<taffy_GridPlacement*>( &( reinterpret_cast<taffy::Line<taffy::GridPlacement>*>(self)->end ) );
}

// -----------------------------------------------------------------------------
// Option<AlignContent>

taffy_Option_AlignContent* taffy_Option_AlignContent_new_default(void)
{
    return reinterpret_cast<taffy_Option_AlignContent*>( new taffy::Option<taffy::AlignContent>{} );
}

taffy_Option_AlignContent* taffy_Option_AlignContent_new(const taffy_AlignContent* value)
{
    return reinterpret_cast<taffy_Option_AlignContent*>(
        (value == nullptr) ?
            new taffy::Option<taffy::AlignContent>{} // None
        :
            new taffy::Option<taffy::AlignContent>{ taffy_AlignContent_to_cpp(*value) } // Some
    );
}

taffy_Option_AlignContent* taffy_Option_AlignContent_new_some(taffy_AlignContent value)
{
    return reinterpret_cast<taffy_Option_AlignContent*>(
        new taffy::Option<taffy::AlignContent>{ taffy_AlignContent_to_cpp(value) }
    );
}

taffy_Option_AlignContent* taffy_Option_AlignContent_new_copy(const taffy_Option_AlignContent* other)
{
    ASSERT_NOT_NULL(other);

    const taffy::Option<taffy::AlignContent>* o = reinterpret_cast<const taffy::Option<taffy::AlignContent>*>(other);
    return reinterpret_cast<taffy_Option_AlignContent*>( new taffy::Option<taffy::AlignContent>{*o} );
}

void taffy_Option_AlignContent_delete(taffy_Option_AlignContent* self)
{
    ASSERT_NOT_NULL(self);

    delete reinterpret_cast<taffy::Option<taffy::AlignContent>*>(self);
    self = nullptr;
}

int taffy_Option_AlignContent_is_some(const taffy_Option_AlignContent* self)
{
    ASSERT_NOT_NULL(self);

    return reinterpret_cast<const taffy::Option<taffy::AlignContent>*>(self)->is_some() ? 1 : 0;
}

taffy_AlignContent taffy_Option_AlignContent_get_value(const taffy_Option_AlignContent* self)
{
    ASSERT_NOT_NULL(self);

    return taffy_AlignContent_to_c( reinterpret_cast<const taffy::Option<taffy::AlignContent>*>(self)->value() );
}

void taffy_Option_AlignContent_set_value(taffy_Option_AlignContent* self, const taffy_AlignContent* value)
{
    ASSERT_NOT_NULL(self);

    taffy::Option<taffy::AlignContent>* opt = reinterpret_cast<taffy::Option<taffy::AlignContent>*>(self);
    *opt = (value == nullptr) ?
        taffy::Option<taffy::AlignContent>{} // None
    :
        taffy::Option<taffy::AlignContent>{ taffy_AlignContent_to_cpp(*value) }; // Some
}

// -----------------------------------------------------------------------------
// Option<JustifyContent>

taffy_Option_JustifyContent* taffy_Option_JustifyContent_new_default(void)
{
    return reinterpret_cast<taffy_Option_JustifyContent*>( new taffy::Option<taffy::JustifyContent>{} );
}

taffy_Option_JustifyContent* taffy_Option_JustifyContent_new(const taffy_JustifyContent* value)
{
    return reinterpret_cast<taffy_Option_JustifyContent*>(
        (value == nullptr) ?
            new taffy::Option<taffy::JustifyContent>{} // None
        :
            new taffy::Option<taffy::JustifyContent>{ taffy_JustifyContent_to_cpp(*value) } // Some
    );
}

taffy_Option_JustifyContent* taffy_Option_JustifyContent_new_some(taffy_JustifyContent value)
{
    return reinterpret_cast<taffy_Option_JustifyContent*>(
        new taffy::Option<taffy::JustifyContent>{ taffy_JustifyContent_to_cpp(value) }
    );
}

taffy_Option_JustifyContent* taffy_Option_JustifyContent_new_copy(const taffy_Option_JustifyContent* other)
{
    ASSERT_NOT_NULL(other);

    const taffy::Option<taffy::JustifyContent>* o = reinterpret_cast<const taffy::Option<taffy::JustifyContent>*>(other);
    return reinterpret_cast<taffy_Option_JustifyContent*>( new taffy::Option<taffy::JustifyContent>{*o} );
}

void taffy_Option_JustifyContent_delete(taffy_Option_JustifyContent* self)
{
    ASSERT_NOT_NULL(self);

    delete reinterpret_cast<taffy::Option<taffy::JustifyContent>*>(self);
    self = nullptr;
}

int taffy_Option_JustifyContent_is_some(const taffy_Option_JustifyContent* self)
{
    ASSERT_NOT_NULL(self);

    return reinterpret_cast<const taffy::Option<taffy::JustifyContent>*>(self)->is_some() ? 1 : 0;
}

taffy_JustifyContent taffy_Option_JustifyContent_get_value(const taffy_Option_JustifyContent* self)
{
    ASSERT_NOT_NULL(self);

    return taffy_JustifyContent_to_c( reinterpret_cast<const taffy::Option<taffy::JustifyContent>*>(self)->value() );
}

void taffy_Option_JustifyContent_set_value(taffy_Option_JustifyContent* self, const taffy_JustifyContent* value)
{
    ASSERT_NOT_NULL(self);

    taffy::Option<taffy::JustifyContent>* opt = reinterpret_cast<taffy::Option<taffy::JustifyContent>*>(self);
    *opt = (value == nullptr) ?
        taffy::Option<taffy::JustifyContent>{} // None
    :
        taffy::Option<taffy::JustifyContent>{ taffy_JustifyContent_to_cpp(*value) }; // Some
}

// -----------------------------------------------------------------------------
// Option<AlignItems>

taffy_Option_AlignItems* taffy_Option_AlignItems_new_default(void)
{
    return reinterpret_cast<taffy_Option_AlignItems*>( new taffy::Option<taffy::AlignItems>{} );
}

taffy_Option_AlignItems* taffy_Option_AlignItems_new(const taffy_AlignItems* value)
{
    return reinterpret_cast<taffy_Option_AlignItems*>(
        (value == nullptr) ?
            new taffy::Option<taffy::AlignItems>{} // None
        :
            new taffy::Option<taffy::AlignItems>{ taffy_AlignItems_to_cpp(*value) } // Some
    );
}

taffy_Option_AlignItems* taffy_Option_AlignItems_new_some(taffy_AlignItems value)
{
    return reinterpret_cast<taffy_Option_AlignItems*>(
        new taffy::Option<taffy::AlignItems>{ taffy_AlignItems_to_cpp(value) }
    );
}

taffy_Option_AlignItems* taffy_Option_AlignItems_new_copy(const taffy_Option_AlignItems* other)
{
    ASSERT_NOT_NULL(other);

    const taffy::Option<taffy::AlignItems>* o = reinterpret_cast<const taffy::Option<taffy::AlignItems>*>(other);
    return reinterpret_cast<taffy_Option_AlignItems*>( new taffy::Option<taffy::AlignItems>{*o} );
}

void taffy_Option_AlignItems_delete(taffy_Option_AlignItems* self)
{
    ASSERT_NOT_NULL(self);

    delete reinterpret_cast<taffy::Option<taffy::AlignItems>*>(self);
    self = nullptr;
}

int taffy_Option_AlignItems_is_some(const taffy_Option_AlignItems* self)
{
    ASSERT_NOT_NULL(self);

    return reinterpret_cast<const taffy::Option<taffy::AlignItems>*>(self)->is_some() ? 1 : 0;
}

taffy_AlignItems taffy_Option_AlignItems_get_value(const taffy_Option_AlignItems* self)
{
    ASSERT_NOT_NULL(self);

    return taffy_AlignItems_to_c( reinterpret_cast<const taffy::Option<taffy::AlignItems>*>(self)->value() );
}

void taffy_Option_AlignItems_set_value(taffy_Option_AlignItems* self, const taffy_AlignItems* value)
{
    ASSERT_NOT_NULL(self);

    taffy::Option<taffy::AlignItems>* opt = reinterpret_cast<taffy::Option<taffy::AlignItems>*>(self);
    *opt = (value == nullptr) ?
        taffy::Option<taffy::AlignItems>{} // None
    :
        taffy::Option<taffy::AlignItems>{ taffy_AlignItems_to_cpp(*value) }; // Some
}

// -----------------------------------------------------------------------------
// Option<AlignSelf>

taffy_Option_AlignSelf* taffy_Option_AlignSelf_new_default(void)
{
    return reinterpret_cast<taffy_Option_AlignSelf*>( new taffy::Option<taffy::AlignSelf>{} );
}

taffy_Option_AlignSelf* taffy_Option_AlignSelf_new(const taffy_AlignSelf* value)
{
    return reinterpret_cast<taffy_Option_AlignSelf*>(
        (value == nullptr) ?
            new taffy::Option<taffy::AlignSelf>{} // None
        :
            new taffy::Option<taffy::AlignSelf>{ taffy_AlignSelf_to_cpp(*value) } // Some
    );
}

taffy_Option_AlignSelf* taffy_Option_AlignSelf_new_some(taffy_AlignSelf value)
{
    return reinterpret_cast<taffy_Option_AlignSelf*>(
        new taffy::Option<taffy::AlignSelf>{ taffy_AlignSelf_to_cpp(value) }
    );
}

taffy_Option_AlignSelf* taffy_Option_AlignSelf_new_copy(const taffy_Option_AlignSelf* other)
{
    ASSERT_NOT_NULL(other);

    const taffy::Option<taffy::AlignSelf>* o = reinterpret_cast<const taffy::Option<taffy::AlignSelf>*>(other);
    return reinterpret_cast<taffy_Option_AlignSelf*>( new taffy::Option<taffy::AlignSelf>{*o} );
}

void taffy_Option_AlignSelf_delete(taffy_Option_AlignSelf* self)
{
    ASSERT_NOT_NULL(self);

    delete reinterpret_cast<taffy::Option<taffy::AlignSelf>*>(self);
    self = nullptr;
}

int taffy_Option_AlignSelf_is_some(const taffy_Option_AlignSelf* self)
{
    ASSERT_NOT_NULL(self);

    return reinterpret_cast<const taffy::Option<taffy::AlignSelf>*>(self)->is_some() ? 1 : 0;
}

taffy_AlignSelf taffy_Option_AlignSelf_get_value(const taffy_Option_AlignSelf* self)
{
    ASSERT_NOT_NULL(self);

    return taffy_AlignSelf_to_c( reinterpret_cast<const taffy::Option<taffy::AlignSelf>*>(self)->value() );
}

void taffy_Option_AlignSelf_set_value(taffy_Option_AlignSelf* self, const taffy_AlignSelf* value)
{
    ASSERT_NOT_NULL(self);

    taffy::Option<taffy::AlignSelf>* opt = reinterpret_cast<taffy::Option<taffy::AlignSelf>*>(self);
    *opt = (value == nullptr) ?
        taffy::Option<taffy::AlignSelf>{} // None
    :
        taffy::Option<taffy::AlignSelf>{ taffy_AlignSelf_to_cpp(*value) }; // Some
}

// -----------------------------------------------------------------------------
// Style

taffy_Style* taffy_Style_new_default(void)
{
    return reinterpret_cast<taffy_Style*>( new taffy::Style{} );
}

taffy_Style* taffy_Style_new_copy(const taffy_Style* other)
{
    ASSERT_NOT_NULL(self);

    const taffy::Style* o = reinterpret_cast<const taffy::Style*>(other);
    return reinterpret_cast<taffy_Style*>( new taffy::Style{*o} );
}

void taffy_Style_delete(taffy_Style* self)
{
    ASSERT_NOT_NULL(self);

    delete reinterpret_cast<taffy::Style*>(self);
    self = nullptr;
}

taffy_Style* taffy_Style_new_DEFAULT(void)
{
    return reinterpret_cast<taffy_Style*>( new taffy::Style{ taffy::Style::DEFAULT() } );
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Style :: getters

taffy_Display taffy_Style_get_display(const taffy_Style* self)
{
    ASSERT_NOT_NULL(self);

    return taffy_Display_to_c( reinterpret_cast<const taffy::Style*>(self)->display );
}

const taffy_Point_of_Overflow* taffy_Style_get_overflow(const taffy_Style* self)
{
    ASSERT_NOT_NULL(self);

    return reinterpret_cast<const taffy_Point_of_Overflow*>( &( reinterpret_cast<const taffy::Style*>(self)->overflow ) );
}

float taffy_Style_get_scrollbar_width(const taffy_Style* self)
{
    ASSERT_NOT_NULL(self);

    return reinterpret_cast<const taffy::Style*>(self)->scrollbar_width;
}

taffy_Position taffy_Style_get_position(const taffy_Style* self)
{
    ASSERT_NOT_NULL(self);

    return taffy_Position_to_c( reinterpret_cast<const taffy::Style*>(self)->position );
}

const taffy_Rect_of_LengthPercentageAuto* taffy_Style_get_inset(const taffy_Style* self)
{
    ASSERT_NOT_NULL(self);

    return reinterpret_cast<const taffy_Rect_of_LengthPercentageAuto*>( &( reinterpret_cast<const taffy::Style*>(self)->inset ) );
}

const taffy_Size_of_Dimension* taffy_Style_get_size(const taffy_Style* self)
{
    ASSERT_NOT_NULL(self);

    return reinterpret_cast<const taffy_Size_of_Dimension*>( &( reinterpret_cast<const taffy::Style*>(self)->size ) );
}

const taffy_Size_of_Dimension* taffy_Style_get_min_size(const taffy_Style* self)
{
    ASSERT_NOT_NULL(self);

    return reinterpret_cast<const taffy_Size_of_Dimension*>( &( reinterpret_cast<const taffy::Style*>(self)->min_size ) );
}

const taffy_Size_of_Dimension* taffy_Style_get_max_size(const taffy_Style* self)
{
    ASSERT_NOT_NULL(self);

    return reinterpret_cast<const taffy_Size_of_Dimension*>( &( reinterpret_cast<const taffy::Style*>(self)->max_size ) );
}

const taffy_Option_float* taffy_Style_get_aspect_ratio(const taffy_Style* self)
{
    ASSERT_NOT_NULL(self);

    return reinterpret_cast<const taffy_Option_float*>( &( reinterpret_cast<const taffy::Style*>(self)->aspect_ratio ) );
}

const taffy_Rect_of_LengthPercentageAuto* taffy_Style_get_margin(const taffy_Style* self)
{
    ASSERT_NOT_NULL(self);

    return reinterpret_cast<const taffy_Rect_of_LengthPercentageAuto*>( &( reinterpret_cast<const taffy::Style*>(self)->margin ) );
}

const taffy_Rect_of_LengthPercentage* taffy_Style_get_padding(const taffy_Style* self)
{
    ASSERT_NOT_NULL(self);

    return reinterpret_cast<const taffy_Rect_of_LengthPercentage*>( &(reinterpret_cast<const taffy::Style*>(self)->padding ) );
}

const taffy_Rect_of_LengthPercentage* taffy_Style_get_border(const taffy_Style* self)
{
    ASSERT_NOT_NULL(self);

    return reinterpret_cast<const taffy_Rect_of_LengthPercentage*>( &(reinterpret_cast<const taffy::Style*>(self)->border ) );
}

const taffy_Option_AlignItems* taffy_Style_get_align_items(const taffy_Style* self)
{
    ASSERT_NOT_NULL(self);

    return reinterpret_cast<const taffy_Option_AlignItems*>( &( reinterpret_cast<const taffy::Style*>(self)->align_items ) );
}

const taffy_Option_AlignSelf* taffy_Style_get_align_self(const taffy_Style* self)
{
    ASSERT_NOT_NULL(self);

    return reinterpret_cast<const taffy_Option_AlignSelf*>( &( reinterpret_cast<const taffy::Style*>(self)->align_self ) );
}

const taffy_Option_AlignItems* taffy_Style_get_justify_items(const taffy_Style* self)
{
    ASSERT_NOT_NULL(self);

    return reinterpret_cast<const taffy_Option_AlignItems*>( &( reinterpret_cast<const taffy::Style*>(self)->justify_items ) );
}

const taffy_Option_AlignSelf* taffy_Style_get_justify_self(const taffy_Style* self)
{
    ASSERT_NOT_NULL(self);

    return reinterpret_cast<const taffy_Option_AlignSelf*>( &( reinterpret_cast<const taffy::Style*>(self)->justify_self ) );
}

const taffy_Option_AlignContent* taffy_Style_get_align_content(const taffy_Style* self)
{
    ASSERT_NOT_NULL(self);

    return reinterpret_cast<const taffy_Option_AlignContent*>( &( reinterpret_cast<const taffy::Style*>(self)->align_content ) );
}

const taffy_Option_JustifyContent* taffy_Style_get_justify_content(const taffy_Style* self)
{
    ASSERT_NOT_NULL(self);

    return reinterpret_cast<const taffy_Option_JustifyContent*>( &( reinterpret_cast<const taffy::Style*>(self)->justify_content ) );
}

const taffy_Size_of_LengthPercentage* taffy_Style_get_gap(const taffy_Style* self)
{
    ASSERT_NOT_NULL(self);

    return reinterpret_cast<const taffy_Size_of_LengthPercentage*>( &( reinterpret_cast<const taffy::Style*>(self)->gap ) );
}

taffy_FlexDirection taffy_Style_get_flex_direction(const taffy_Style* self)
{
    ASSERT_NOT_NULL(self);

    return taffy_FlexDirection_to_c( reinterpret_cast<const taffy::Style*>(self)->flex_direction );
}

taffy_FlexWrap taffy_Style_get_flex_wrap(const taffy_Style* self)
{
    ASSERT_NOT_NULL(self);

    return taffy_FlexWrap_to_c( reinterpret_cast<const taffy::Style*>(self)->flex_wrap );
}

const taffy_Dimension* taffy_Style_get_flex_basis(const taffy_Style* self)
{
    ASSERT_NOT_NULL(self);

    return reinterpret_cast<const taffy_Dimension*>( &( reinterpret_cast<const taffy::Style*>(self)->flex_basis ) );
}

float taffy_Style_get_flex_grow(const taffy_Style* self)
{
    ASSERT_NOT_NULL(self);

    return reinterpret_cast<const taffy::Style*>(self)->flex_grow;
}

float taffy_Style_get_flex_shrink(const taffy_Style* self)
{
    ASSERT_NOT_NULL(self);

    return reinterpret_cast<const taffy::Style*>(self)->flex_shrink;
}

const taffy_GridTrackVec_of_TrackSizingFunction taffy_Style_get_grid_template_rows(const taffy_Style* self)
{
    ASSERT_NOT_NULL(self);

    const taffy::GridTrackVec<taffy::TrackSizingFunction>& vec = reinterpret_cast<const taffy::Style*>(self)->grid_template_rows;

    taffy_GridTrackVec_of_TrackSizingFunction result;
    result.items       = reinterpret_cast<const taffy_TrackSizingFunction*>( vec.data() );
    result.items_count = vec.size();
    return result;
}

const taffy_GridTrackVec_of_TrackSizingFunction taffy_Style_get_grid_template_columns(const taffy_Style* self)
{
    ASSERT_NOT_NULL(self);

    const taffy::GridTrackVec<taffy::TrackSizingFunction>& vec = reinterpret_cast<const taffy::Style*>(self)->grid_template_columns;

    taffy_GridTrackVec_of_TrackSizingFunction result;
    result.items       = reinterpret_cast<const taffy_TrackSizingFunction*>( vec.data() );
    result.items_count = vec.size();
    return result;
}

const taffy_GridTrackVec_of_NonRepeatedTrackSizingFunction taffy_Style_get_grid_auto_rows(const taffy_Style* self)
{
    ASSERT_NOT_NULL(self);

    const taffy::GridTrackVec<taffy::NonRepeatedTrackSizingFunction>& vec = reinterpret_cast<const taffy::Style*>(self)->grid_auto_rows;

    taffy_GridTrackVec_of_NonRepeatedTrackSizingFunction result;
    result.items       = reinterpret_cast<const taffy_NonRepeatedTrackSizingFunction*>( vec.data() );
    result.items_count = vec.size();
    return result;
}

const taffy_GridTrackVec_of_NonRepeatedTrackSizingFunction taffy_Style_get_grid_auto_columns(const taffy_Style* self)
{
    ASSERT_NOT_NULL(self);

    const taffy::GridTrackVec<taffy::NonRepeatedTrackSizingFunction>& vec = reinterpret_cast<const taffy::Style*>(self)->grid_auto_columns;

    taffy_GridTrackVec_of_NonRepeatedTrackSizingFunction result;
    result.items       = reinterpret_cast<const taffy_NonRepeatedTrackSizingFunction*>( vec.data() );
    result.items_count = vec.size();
    return result;
}

taffy_GridAutoFlow taffy_Style_get_grid_auto_flow(const taffy_Style* self)
{
    ASSERT_NOT_NULL(self);

    return taffy_GridAutoFlow_to_c( reinterpret_cast<const taffy::Style*>(self)->grid_auto_flow );
}

const taffy_Line_of_GridPlacement* taffy_Style_get_grid_row(const taffy_Style* self)
{
    ASSERT_NOT_NULL(self);
    
    return reinterpret_cast<const taffy_Line_of_GridPlacement*>( &( reinterpret_cast<const taffy::Style*>(self)->grid_row ) );
}

const taffy_Line_of_GridPlacement* taffy_Style_get_grid_column(const taffy_Style* self)
{
    ASSERT_NOT_NULL(self);
    
    return reinterpret_cast<const taffy_Line_of_GridPlacement*>( &( reinterpret_cast<const taffy::Style*>(self)->grid_column ) );
}

// -----------------------------------------------------------------------------
// Style :: setters

void taffy_Style_set_display(taffy_Style* self, taffy_Display value)
{
    ASSERT_NOT_NULL(self);

    reinterpret_cast<taffy::Style*>(self)->display = taffy_Display_to_cpp(value);
}

void taffy_Style_set_overflow(taffy_Style* self, const taffy_Point_of_Overflow* value)
{
    ASSERT_NOT_NULL(self);
    ASSERT_NOT_NULL(value);

    const taffy::Point<taffy::Overflow>* v = reinterpret_cast<const taffy::Point<taffy::Overflow>*>(value);
    reinterpret_cast<taffy::Style*>(self)->overflow = *v;
}

void taffy_Style_set_scrollbar_width(taffy_Style* self, float value)
{
    ASSERT_NOT_NULL(self);

    reinterpret_cast<taffy::Style*>(self)->scrollbar_width = value; 
}

void taffy_Style_set_position(taffy_Style* self, taffy_Position value)
{
    ASSERT_NOT_NULL(self);

    reinterpret_cast<taffy::Style*>(self)->position = taffy_Position_to_cpp(value);
}

void taffy_Style_set_inset(taffy_Style* self, const taffy_Rect_of_LengthPercentageAuto* value)
{
    ASSERT_NOT_NULL(self);
    ASSERT_NOT_NULL(value);

    const taffy::Rect<taffy::LengthPercentageAuto>* v = reinterpret_cast<const taffy::Rect<taffy::LengthPercentageAuto>*>(value);
    reinterpret_cast<taffy::Style*>(self)->inset = *v;
}

void taffy_Style_set_size(taffy_Style* self, const taffy_Size_of_Dimension* value)
{
    ASSERT_NOT_NULL(self);
    ASSERT_NOT_NULL(value);

    const taffy::Size<taffy::Dimension>* v = reinterpret_cast<const taffy::Size<taffy::Dimension>*>(value);
    reinterpret_cast<taffy::Style*>(self)->size = *v;
}

void taffy_Style_set_min_size(taffy_Style* self, const taffy_Size_of_Dimension* value)
{
    ASSERT_NOT_NULL(self);
    ASSERT_NOT_NULL(value);

    const taffy::Size<taffy::Dimension>* v = reinterpret_cast<const taffy::Size<taffy::Dimension>*>(value);
    reinterpret_cast<taffy::Style*>(self)->min_size = *v;
}

void taffy_Style_set_max_size(taffy_Style* self, const taffy_Size_of_Dimension* value)
{
    ASSERT_NOT_NULL(self);
    ASSERT_NOT_NULL(value);

    const taffy::Size<taffy::Dimension>* v = reinterpret_cast<const taffy::Size<taffy::Dimension>*>(value);
    reinterpret_cast<taffy::Style*>(self)->max_size = *v;
}

void taffy_Style_set_aspect_ratio(taffy_Style* self, const taffy_Option_float* value)
{
    ASSERT_NOT_NULL(self);
    ASSERT_NOT_NULL(value);

    const taffy::Option<float>* v = reinterpret_cast<const taffy::Option<float>*>(value);
    reinterpret_cast<taffy::Style*>(self)->aspect_ratio = *v;
}

void taffy_Style_set_margin(taffy_Style* self, const taffy_Rect_of_LengthPercentageAuto* value)
{
    ASSERT_NOT_NULL(self);
    ASSERT_NOT_NULL(value);

    const taffy::Rect<taffy::LengthPercentageAuto>* v = reinterpret_cast<const taffy::Rect<taffy::LengthPercentageAuto>*>(value);
    reinterpret_cast<taffy::Style*>(self)->margin = *v;
}

void taffy_Style_set_padding(taffy_Style* self, const taffy_Rect_of_LengthPercentage* value)
{
    ASSERT_NOT_NULL(self);
    ASSERT_NOT_NULL(value);

    const taffy::Rect<taffy::LengthPercentage>* v = reinterpret_cast<const taffy::Rect<taffy::LengthPercentage>*>(value);
    reinterpret_cast<taffy::Style*>(self)->padding = *v;
}

void taffy_Style_set_border(taffy_Style* self, const taffy_Rect_of_LengthPercentage* value)
{
    ASSERT_NOT_NULL(self);
    ASSERT_NOT_NULL(value);

    const taffy::Rect<taffy::LengthPercentage>* v = reinterpret_cast<const taffy::Rect<taffy::LengthPercentage>*>(value);
    reinterpret_cast<taffy::Style*>(self)->border = *v;
}

void taffy_Style_set_align_items(taffy_Style* self, const taffy_Option_AlignItems* value)
{
    ASSERT_NOT_NULL(self);
    ASSERT_NOT_NULL(value);

    const taffy::Option<taffy::AlignItems>* v = reinterpret_cast<const taffy::Option<taffy::AlignItems>*>(value);
    reinterpret_cast<taffy::Style*>(self)->align_items = *v;
}

void taffy_Style_set_align_self(taffy_Style* self, const taffy_Option_AlignSelf* value)
{
    ASSERT_NOT_NULL(self);
    ASSERT_NOT_NULL(value);

    const taffy::Option<taffy::AlignSelf>* v = reinterpret_cast<const taffy::Option<taffy::AlignSelf>*>(value);
    reinterpret_cast<taffy::Style*>(self)->align_self = *v;
}

void taffy_Style_set_justify_items(taffy_Style* self, const taffy_Option_AlignItems* value)
{
    ASSERT_NOT_NULL(self);
    ASSERT_NOT_NULL(value);

    const taffy::Option<taffy::AlignItems>* v = reinterpret_cast<const taffy::Option<taffy::AlignItems>*>(value);
    reinterpret_cast<taffy::Style*>(self)->justify_items = *v;
}

void taffy_Style_set_justify_self(taffy_Style* self, const taffy_Option_AlignSelf* value)
{
    ASSERT_NOT_NULL(self);
    ASSERT_NOT_NULL(value);

    const taffy::Option<taffy::AlignSelf>* v = reinterpret_cast<const taffy::Option<taffy::AlignSelf>*>(value);
    reinterpret_cast<taffy::Style*>(self)->justify_self = *v;
}

void taffy_Style_set_align_content(taffy_Style* self, const taffy_Option_AlignContent* value)
{
    ASSERT_NOT_NULL(self);
    ASSERT_NOT_NULL(value);

    const taffy::Option<taffy::AlignContent>* v = reinterpret_cast<const taffy::Option<taffy::AlignContent>*>(value);
    reinterpret_cast<taffy::Style*>(self)->align_content = *v;
}

void taffy_Style_set_justify_content(taffy_Style* self, const taffy_Option_JustifyContent* value)
{
    ASSERT_NOT_NULL(self);
    ASSERT_NOT_NULL(value);

    const taffy::Option<taffy::JustifyContent>* v = reinterpret_cast<const taffy::Option<taffy::JustifyContent>*>(value);
    reinterpret_cast<taffy::Style*>(self)->justify_content = *v;
}

void taffy_Style_set_gap(taffy_Style* self, const taffy_Size_of_LengthPercentage* value)
{
    ASSERT_NOT_NULL(self);
    ASSERT_NOT_NULL(value);

    const taffy::Size<taffy::LengthPercentage>* v = reinterpret_cast<const taffy::Size<taffy::LengthPercentage>*>(value);
    reinterpret_cast<taffy::Style*>(self)->gap = *v;
}

void taffy_Style_set_flex_direction(taffy_Style* self, taffy_FlexDirection value)
{
    ASSERT_NOT_NULL(self);

    reinterpret_cast<taffy::Style*>(self)->flex_direction = taffy_FlexDirection_to_cpp(value);
}

void taffy_Style_set_flex_wrap(taffy_Style* self, taffy_FlexWrap value)
{
    ASSERT_NOT_NULL(self);

    reinterpret_cast<taffy::Style*>(self)->flex_wrap = taffy_FlexWrap_to_cpp(value);
}

void taffy_Style_set_flex_basis(taffy_Style* self, const taffy_Dimension* value)
{
    ASSERT_NOT_NULL(self);
    ASSERT_NOT_NULL(value);

    const taffy::Dimension* v = reinterpret_cast<const taffy::Dimension*>(value);
    reinterpret_cast<taffy::Style*>(self)->flex_basis = *v;
}

void taffy_Style_set_flex_grow(taffy_Style* self, float value)
{
    ASSERT_NOT_NULL(self);

    reinterpret_cast<taffy::Style*>(self)->flex_grow = value;
}

void taffy_Style_set_flex_shrink(taffy_Style* self, float value)
{
    ASSERT_NOT_NULL(self);

    reinterpret_cast<taffy::Style*>(self)->flex_shrink = value;
}

void taffy_Style_set_grid_template_rows(taffy_Style* self, const taffy_TrackSizingFunction* funcs, size_t funcs_count)
{
    ASSERT_NOT_NULL(self);
    if(funcs_count > 0) {
        ASSERT_NOT_NULL(funcs);
    }

    const taffy::TrackSizingFunction* _funcs = reinterpret_cast<const taffy::TrackSizingFunction*>(funcs);
    reinterpret_cast<taffy::Style*>(self)->grid_template_rows =
        taffy::GridTrackVec<taffy::TrackSizingFunction>{_funcs, _funcs + funcs_count}; // NOTE: Entire vector copy :/
}

void taffy_Style_set_grid_template_columns(taffy_Style* self, const taffy_TrackSizingFunction* funcs, size_t funcs_count)
{
    ASSERT_NOT_NULL(self);
    if(funcs_count > 0) {
        ASSERT_NOT_NULL(funcs);
    }

    const taffy::TrackSizingFunction* _funcs = reinterpret_cast<const taffy::TrackSizingFunction*>(funcs);
    reinterpret_cast<taffy::Style*>(self)->grid_template_columns =
        taffy::GridTrackVec<taffy::TrackSizingFunction>{_funcs, _funcs + funcs_count}; // NOTE: Entire vector copy :/
}

void taffy_Style_set_grid_auto_rows(taffy_Style* self, const taffy_NonRepeatedTrackSizingFunction* funcs, size_t funcs_count)
{
    ASSERT_NOT_NULL(self);
    if(funcs_count > 0) {
        ASSERT_NOT_NULL(funcs);
    }

    const taffy::NonRepeatedTrackSizingFunction* _funcs = reinterpret_cast<const taffy::NonRepeatedTrackSizingFunction*>(funcs);
    reinterpret_cast<taffy::Style*>(self)->grid_auto_rows =
        taffy::GridTrackVec<taffy::NonRepeatedTrackSizingFunction>{_funcs, _funcs + funcs_count}; // NOTE: Entire vector copy :/
}

void taffy_Style_set_grid_auto_columns(taffy_Style* self, const taffy_NonRepeatedTrackSizingFunction* funcs, size_t funcs_count)
{
    ASSERT_NOT_NULL(self);
    if(funcs_count > 0) {
        ASSERT_NOT_NULL(funcs);
    }

    const taffy::NonRepeatedTrackSizingFunction* _funcs = reinterpret_cast<const taffy::NonRepeatedTrackSizingFunction*>(funcs);
    reinterpret_cast<taffy::Style*>(self)->grid_auto_columns =
        taffy::GridTrackVec<taffy::NonRepeatedTrackSizingFunction>{_funcs, _funcs + funcs_count}; // NOTE: Entire vector copy :/
}

void taffy_Style_set_grid_auto_flow(taffy_Style* self, taffy_GridAutoFlow value)
{
    ASSERT_NOT_NULL(self);

    reinterpret_cast<taffy::Style*>(self)->grid_auto_flow = taffy_GridAutoFlow_to_cpp(value);
}

void taffy_Style_set_grid_row(taffy_Style* self, const taffy_Line_of_GridPlacement* value)
{
    ASSERT_NOT_NULL(self);
    ASSERT_NOT_NULL(value);

    const taffy::Line<taffy::GridPlacement>* v = reinterpret_cast<const taffy::Line<taffy::GridPlacement>*>(value);
    reinterpret_cast<taffy::Style*>(self)->grid_row = *v;
}

void taffy_Style_set_grid_column(taffy_Style* self, const taffy_Line_of_GridPlacement* value)
{
    ASSERT_NOT_NULL(self);
    ASSERT_NOT_NULL(value);

    const taffy::Line<taffy::GridPlacement>* v = reinterpret_cast<const taffy::Line<taffy::GridPlacement>*>(value);
    reinterpret_cast<taffy::Style*>(self)->grid_column = *v;
}

// -----------------------------------------------------------------------------
// Layout

taffy_Layout* taffy_Layout_new_default(void)
{
    return reinterpret_cast<taffy_Layout*>( new taffy::Layout{} );
}

taffy_Layout* taffy_Layout_new(uint32_t order, const taffy_Size_of_float* size, const taffy_Point_of_float* location)
{
    ASSERT_NOT_NULL(size);
    ASSERT_NOT_NULL(location);

    const taffy::Size<float>* _size = reinterpret_cast<const taffy::Size<float>*>(size);
    const taffy::Point<float>* _location = reinterpret_cast<const taffy::Point<float>*>(location);

    return reinterpret_cast<taffy_Layout*>(
        new taffy::Layout{ order, *_size, *_location }
    );
}

taffy_Layout* taffy_Layout_new_with_order(uint32_t order)
{
    return reinterpret_cast<taffy_Layout*>(
        new taffy::Layout{ taffy::Layout::with_order(order) }
    );
}

taffy_Layout* taffy_Layout_new_copy(const taffy_Layout* other)
{
    ASSERT_NOT_NULL(other);

    const taffy::Layout* o = reinterpret_cast<const taffy::Layout*>(other);
    return reinterpret_cast<taffy_Layout*>( new taffy::Layout{*o} );
}

void taffy_Layout_delete(taffy_Layout* self)
{
    ASSERT_NOT_NULL(self);

    delete reinterpret_cast<taffy::Layout*>(self);
    self = nullptr;
}

uint32_t taffy_Layout_get_order(const taffy_Layout* self)
{
    ASSERT_NOT_NULL(self);

    return reinterpret_cast<const taffy::Layout*>(self)->order;
}

const taffy_Size_of_float* taffy_Layout_get_size(const taffy_Layout* self)
{
    ASSERT_NOT_NULL(self);

    return reinterpret_cast<const taffy_Size_of_float*>( &( reinterpret_cast<const taffy::Layout*>(self)->size ) );
}

const taffy_Point_of_float* taffy_Layout_get_location(const taffy_Layout* self)
{
    ASSERT_NOT_NULL(self);

    return reinterpret_cast<const taffy_Point_of_float*>( &( reinterpret_cast<const taffy::Layout*>(self)->location ) );
}

void taffy_Layout_set_order(taffy_Layout* self, uint32_t order)
{
    ASSERT_NOT_NULL(self);

    reinterpret_cast<taffy::Layout*>(self)->order = order;
}

void taffy_Layout_set_size(taffy_Layout* self, const taffy_Size_of_float* size)
{
    ASSERT_NOT_NULL(self);
    ASSERT_NOT_NULL(size);

    const taffy::Size<float>* _size = reinterpret_cast<const taffy::Size<float>*>(size);
    reinterpret_cast<taffy::Layout*>(self)->size = *_size;
}

void taffy_Layout_set_location(taffy_Layout* self, const taffy_Point_of_float* location)
{
    ASSERT_NOT_NULL(self);
    ASSERT_NOT_NULL(size);

    const taffy::Point<float>* _location = reinterpret_cast<const taffy::Point<float>*>(location);
    reinterpret_cast<taffy::Layout*>(self)->location = *_location;
}

uint32_t* taffy_Layout_get_mut_order(taffy_Layout* self)
{
    ASSERT_NOT_NULL(self);

    return &( reinterpret_cast<taffy::Layout*>(self)->order );
}

taffy_Size_of_float* taffy_Layout_get_mut_size(taffy_Layout* self)
{
    ASSERT_NOT_NULL(self);

    return reinterpret_cast<taffy_Size_of_float*>( &( reinterpret_cast<taffy::Layout*>(self)->size ) );
}

taffy_Point_of_float* taffy_Layout_get_mut_location(taffy_Layout* self)
{
    ASSERT_NOT_NULL(self);

    return reinterpret_cast<taffy_Point_of_float*>( &( reinterpret_cast<taffy::Layout*>(self)->location ) );
}

// -----------------------------------------------------------------------------
// Taffy

taffy_TaffyError_Type taffy_TaffyError_Type_from_cpp(const taffy::TaffyError::Type t)
{
    switch(t) {
    case taffy::TaffyError::Type::ChildIndexOutOfBounds : return taffy_TaffyError_Type_ChildIndexOutOfBounds;
    case taffy::TaffyError::Type::InvalidParentNode     : return taffy_TaffyError_Type_InvalidParentNode;
    case taffy::TaffyError::Type::InvalidChildNode      : return taffy_TaffyError_Type_InvalidChildNode;
    case taffy::TaffyError::Type::InvalidInputNode      : return taffy_TaffyError_Type_InvalidInputNode;
    }

    ASSERT_UNREACHABLE();
}

taffy_TaffyResult_of_void taffy_TaffyResult_of_void_from_cpp(const taffy::TaffyResult<void>& result)
{
    taffy_TaffyResult_of_void ret;

    if(result.is_ok())
    {
        ret.error.type = taffy_TaffyError_Type_Ok;

        ret.error.node.id     = 0;
        ret.error.child_index = 0;
        ret.error.child_count = 0;
    }
    else
    {
        ret.error.type = taffy_TaffyError_Type_from_cpp(result.error().type());

        ret.error.node.id     = static_cast<uint64_t>( result.error().node() );
        ret.error.child_index = result.error().child_index();
        ret.error.child_count = result.error().child_count();
    }

    return ret;
}

taffy_TaffyResult_of_bool taffy_TaffyResult_of_bool_from_cpp(const taffy::TaffyResult<bool>& result)
{
    taffy_TaffyResult_of_bool ret;

    if(result.is_ok())
    {
        ret.error.type = taffy_TaffyError_Type_Ok;

        ret.error.node.id     = 0;
        ret.error.child_index = 0;
        ret.error.child_count = 0;

        ret.value = result.value() ? 1 : 0;
    }
    else
    {
        ret.error.type = taffy_TaffyError_Type_from_cpp(result.error().type());

        ret.error.node.id     = static_cast<uint64_t>( result.error().node() );
        ret.error.child_index = result.error().child_index();
        ret.error.child_count = result.error().child_count();
        
        ret.value = 0;
    }

    return ret;
}

taffy_TaffyResult_of_size_t taffy_TaffyResult_of_size_t_from_cpp(const taffy::TaffyResult<size_t>& result)
{
    taffy_TaffyResult_of_size_t ret;

    if(result.is_ok())
    {
        ret.error.type = taffy_TaffyError_Type_Ok;

        ret.error.node.id     = 0;
        ret.error.child_index = 0;
        ret.error.child_count = 0;
        
        ret.value = result.value();
    }
    else
    {
        ret.error.type = taffy_TaffyError_Type_from_cpp(result.error().type());

        ret.error.node.id     = static_cast<uint64_t>( result.error().node() );
        ret.error.child_index = result.error().child_index();
        ret.error.child_count = result.error().child_count();
        
        ret.value = 0;
    }

    return ret;
}

taffy_TaffyResult_of_NodeId taffy_TaffyResult_of_NodeId_from_cpp(const taffy::TaffyResult<taffy::NodeId>& result)
{
    taffy_TaffyResult_of_NodeId ret;

    if(result.is_ok())
    {
        ret.error.type = taffy_TaffyError_Type_Ok;

        ret.error.node.id     = 0;
        ret.error.child_index = 0;
        ret.error.child_count = 0;
        
        ret.value.id = static_cast<uint64_t>( result.value() );
    }
    else
    {
        ret.error.type = taffy_TaffyError_Type_from_cpp(result.error().type());

        ret.error.node.id     = static_cast<uint64_t>( result.error().node() );
        ret.error.child_index = result.error().child_index();
        ret.error.child_count = result.error().child_count();
        
        ret.value.id = 0;
    }

    return ret;
}

taffy_TaffyResult_of_Vec_of_NodeId taffy_TaffyResult_of_Vec_of_NodeId_from_cpp(const taffy::TaffyResult<taffy::Vec<taffy::NodeId>>& result)
{
    taffy_TaffyResult_of_Vec_of_NodeId ret;

    if(result.is_ok())
    {
        ret.error.type = taffy_TaffyError_Type_Ok;

        ret.error.node.id     = 0;
        ret.error.child_index = 0;
        ret.error.child_count = 0;
        
        const size_t items_count = result.value().size();
        if(items_count > 0)
        {
            // Copy items
            taffy_NodeId* items = new taffy_NodeId[items_count];
            for(size_t i = 0; i < items_count; ++i) {
                items[i].id = static_cast<uint64_t>( result.value()[i] );
            }

            ret.items       = items;
            ret.items_count = items_count;
        }
    }
    else
    {
        ret.error.type = taffy_TaffyError_Type_from_cpp(result.error().type());

        ret.error.node.id     = static_cast<uint64_t>( result.error().node() );
        ret.error.child_index = result.error().child_index();
        ret.error.child_count = result.error().child_count();
        
        ret.items       = nullptr;
        ret.items_count = 0;
    }

    return ret;
}

void taffy_TaffyResult_of_Vec_of_NodeId_delete(taffy_TaffyResult_of_Vec_of_NodeId self)
{
    if(self.items != nullptr)
    {
        delete [] reinterpret_cast<const taffy::NodeId*>(self.items);
        self.items = nullptr;
    }

    self.items_count = 0;
}

taffy_TaffyResult_of_Style_const_ref taffy_TaffyResult_of_Style_const_ref_from_cpp(const taffy::TaffyResult<std::reference_wrapper<taffy::Style const>>& result)
{
    taffy_TaffyResult_of_Style_const_ref ret;

    if(result.is_ok())
    {
        ret.error.type = taffy_TaffyError_Type_Ok;

        ret.error.node.id     = 0;
        ret.error.child_index = 0;
        ret.error.child_count = 0;
        
        ret.value = reinterpret_cast<const taffy_Style*>( &( result.value().get() ) );
    }
    else
    {
        ret.error.type = taffy_TaffyError_Type_from_cpp(result.error().type());

        ret.error.node.id     = static_cast<uint64_t>( result.error().node() );
        ret.error.child_index = result.error().child_index();
        ret.error.child_count = result.error().child_count();
        
        ret.value = nullptr;
    }

    return ret;
}

taffy_TaffyResult_of_Layout_const_ref taffy_TaffyResult_of_Layout_const_ref_from_cpp(const taffy::TaffyResult<std::reference_wrapper<taffy::Layout const>>& result)
{
    taffy_TaffyResult_of_Layout_const_ref ret;

    if(result.is_ok())
    {
        ret.error.type = taffy_TaffyError_Type_Ok;

        ret.error.node.id     = 0;
        ret.error.child_index = 0;
        ret.error.child_count = 0;
        
        ret.value = reinterpret_cast<const taffy_Layout*>( &( result.value().get() ) );
    }
    else
    {
        ret.error.type = taffy_TaffyError_Type_from_cpp(result.error().type());

        ret.error.node.id     = static_cast<uint64_t>( result.error().node() );
        ret.error.child_index = result.error().child_index();
        ret.error.child_count = result.error().child_count();
        
        ret.value = nullptr;
    }

    return ret;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

taffy_Taffy* taffy_Taffy_new_default(void)
{
    return reinterpret_cast<taffy_Taffy*>(
        new taffy::Taffy{}
    );
}
taffy_Taffy* taffy_Taffy_new_with_capacity(size_t capacity)
{
    return reinterpret_cast<taffy_Taffy*>(
        new taffy::Taffy{ taffy::Taffy::with_capacity(capacity) }
    );
}

void taffy_Taffy_delete(taffy_Taffy* self)
{
    ASSERT_NOT_NULL(self);

    delete reinterpret_cast<taffy::Taffy*>(self);
    self = nullptr;
}

void taffy_Taffy_enable_rounding(taffy_Taffy* self)
{
    ASSERT_NOT_NULL(self);

    reinterpret_cast<taffy::Taffy*>(self)->enable_rounding();
}

void taffy_Taffy_disable_rounding(taffy_Taffy* self)
{
    ASSERT_NOT_NULL(self);

    reinterpret_cast<taffy::Taffy*>(self)->disable_rounding();
}

taffy_TaffyResult_of_NodeId taffy_Taffy_new_leaf(
    taffy_Taffy* self, 
    
    const taffy_Style* layout
)
{
    ASSERT_NOT_NULL(self);
    ASSERT_NOT_NULL(layout);

    const taffy::Style* _layout = reinterpret_cast<const taffy::Style*>(layout);
    const auto result = reinterpret_cast<taffy::Taffy*>(self)->new_leaf(*_layout);

    return taffy_TaffyResult_of_NodeId_from_cpp(result);
}

/* TODO: new_leaf_with_measure(...) */

taffy_TaffyResult_of_NodeId taffy_Taffy_new_with_children(
    taffy_Taffy* self,

    const taffy_Style* layout,
    const taffy_NodeId* childs, size_t childs_count
)
{
    ASSERT_NOT_NULL(self);
    ASSERT_NOT_NULL(layout);
    if(childs_count > 0)
    {
        ASSERT_NOT_NULL(childs);
    }

    const taffy::Style* _layout = reinterpret_cast<const taffy::Style*>(layout);
    const taffy::NodeId* _childs = reinterpret_cast<const taffy::NodeId*>(childs);

    const auto result = reinterpret_cast<taffy::Taffy*>(self)->new_with_children(
        *_layout, 
        taffy::Vec<taffy::NodeId>{_childs, _childs + childs_count} // NOTE: Entire vector copy here :/
    );

    return taffy_TaffyResult_of_NodeId_from_cpp(result);
}

void taffy_Taffy_clear(taffy_Taffy* self)
{
    ASSERT_NOT_NULL(self);

    reinterpret_cast<taffy::Taffy*>(self)->clear();
}

taffy_TaffyResult_of_NodeId taffy_Taffy_remove(
    taffy_Taffy* self,

    taffy_NodeId node
)
{
    ASSERT_NOT_NULL(self);

    const auto result = reinterpret_cast<taffy::Taffy*>(self)->remove(
        taffy::NodeId{node.id}
    );

    return taffy_TaffyResult_of_NodeId_from_cpp(result);
}

/* TODO: set_measure(...) */

taffy_TaffyResult_of_void taffy_Taffy_add_child(
    taffy_Taffy* self,

    taffy_NodeId parent, taffy_NodeId child
)
{
    ASSERT_NOT_NULL(self);

    const auto result = reinterpret_cast<taffy::Taffy*>(self)->add_child(
        taffy::NodeId{parent.id}, taffy::NodeId{child.id}
    );

    return taffy_TaffyResult_of_void_from_cpp(result);
}

taffy_TaffyResult_of_void taffy_Taffy_insert_child_at_index(
    taffy_Taffy* self,

    taffy_NodeId parent, size_t child_index, taffy_NodeId child
)
{
    ASSERT_NOT_NULL(self);

    const auto result = reinterpret_cast<taffy::Taffy*>(self)->insert_child_at_index(
        taffy::NodeId{parent.id}, child_index, taffy::NodeId{child.id}
    );

    return taffy_TaffyResult_of_void_from_cpp(result);
}

taffy_TaffyResult_of_void taffy_Taffy_set_children(
    taffy_Taffy* self,

    taffy_NodeId parent, const taffy_NodeId* childs, size_t childs_count 
)
{
    ASSERT_NOT_NULL(self);
    if(childs_count > 0) {
        ASSERT_NOT_NULL(childs);
    }

    const taffy::NodeId* _childs = reinterpret_cast<const taffy::NodeId*>(childs);

    const auto result = reinterpret_cast<taffy::Taffy*>(self)->set_children(
        taffy::NodeId{parent.id},
        taffy::Vec<taffy::NodeId>{_childs, _childs + childs_count} // NOTE: Entire vector copy here :/
    );

    return taffy_TaffyResult_of_void_from_cpp(result);
}

taffy_TaffyResult_of_NodeId taffy_Taffy_remove_child(
    taffy_Taffy* self,

    taffy_NodeId parent, taffy_NodeId child
)
{
    ASSERT_NOT_NULL(self);

    const auto result = reinterpret_cast<taffy::Taffy*>(self)->remove_child(
        taffy::NodeId{parent.id}, taffy::NodeId{child.id}
    );

    return taffy_TaffyResult_of_NodeId_from_cpp(result);
}

taffy_TaffyResult_of_NodeId taffy_Taffy_remove_child_at_index(
    taffy_Taffy* self,

    taffy_NodeId parent, size_t child_index
)
{
    ASSERT_NOT_NULL(self);

    const auto result = reinterpret_cast<taffy::Taffy*>(self)->remove_child_at_index(
        taffy::NodeId{parent.id}, child_index
    );

    return taffy_TaffyResult_of_NodeId_from_cpp(result);
}

taffy_TaffyResult_of_NodeId replace_child_at_index(
    taffy_Taffy* self,

    taffy_NodeId parent, size_t child_index, taffy_NodeId child
)
{
    ASSERT_NOT_NULL(self);

    const auto result = reinterpret_cast<taffy::Taffy*>(self)->replace_child_at_index(
        taffy::NodeId{parent.id}, child_index, taffy::NodeId{child.id}
    );

    return taffy_TaffyResult_of_NodeId_from_cpp(result);
}

taffy_TaffyResult_of_NodeId taffy_Taffy_child_at_index(
    const taffy_Taffy* self,

    taffy_NodeId parent, size_t child_index
)
{
    ASSERT_NOT_NULL(self);

    const auto result = reinterpret_cast<const taffy::Taffy*>(self)->child_at_index(
        taffy::NodeId{parent.id}, child_index
    );

    return taffy_TaffyResult_of_NodeId_from_cpp(result);
}

taffy_TaffyResult_of_size_t taffy_Taffy_child_count(
    const taffy_Taffy* self,

    taffy_NodeId parent
)
{
    ASSERT_NOT_NULL(self);

    const auto result = reinterpret_cast<const taffy::Taffy*>(self)->child_count(
        taffy::NodeId{parent.id}
    );

    return taffy_TaffyResult_of_size_t_from_cpp(result);
}

taffy_TaffyResult_of_Vec_of_NodeId taffy_Taffy_children(
    const taffy_Taffy* self,

    taffy_NodeId parent
)
{
    ASSERT_NOT_NULL(self);

    const auto result = reinterpret_cast<const taffy::Taffy*>(self)->Children(
        taffy::NodeId{parent.id}
    );

    return taffy_TaffyResult_of_Vec_of_NodeId_from_cpp(result);
}

taffy_TaffyResult_of_void taffy_Taffy_set_style(
    taffy_Taffy* self,

    taffy_NodeId node, const taffy_Style* style
)
{
    ASSERT_NOT_NULL(self);
    ASSERT_NOT_NULL(style);

    const taffy::Style* _style = reinterpret_cast<const taffy::Style*>(style);

    const auto result = reinterpret_cast<taffy::Taffy*>(self)->set_style(
        taffy::NodeId{node.id}, *_style
    );

    return taffy_TaffyResult_of_void_from_cpp(result);
}

taffy_TaffyResult_of_Style_const_ref taffy_Taffy_style(
    const taffy_Taffy* self,

    taffy_NodeId node
)
{
    ASSERT_NOT_NULL(self);
    
    const auto result = reinterpret_cast<const taffy::Taffy*>(self)->style(
        taffy::NodeId{node.id}
    );

    return taffy_TaffyResult_of_Style_const_ref_from_cpp(result);
}

taffy_TaffyResult_of_Layout_const_ref taffy_Taffy_layout(
    const taffy_Taffy* self,

    taffy_NodeId node
)
{
    ASSERT_NOT_NULL(self);
    
    const auto result = reinterpret_cast<const taffy::Taffy*>(self)->layout(
        taffy::NodeId{node.id}
    );

    return taffy_TaffyResult_of_Layout_const_ref_from_cpp(result);
}

taffy_TaffyResult_of_void taffy_Taffy_mark_dirty(
    taffy_Taffy* self,

    taffy_NodeId node
)
{
    ASSERT_NOT_NULL(self);
    
    const auto result = reinterpret_cast<taffy::Taffy*>(self)->mark_dirty(
        taffy::NodeId{node.id}
    );

    return taffy_TaffyResult_of_void_from_cpp(result);
}

taffy_TaffyResult_of_bool taffy_Taffy_dirty(
    const taffy_Taffy* self,

    taffy_NodeId node
)
{
    ASSERT_NOT_NULL(self);
    
    const auto result = reinterpret_cast<const taffy::Taffy*>(self)->dirty(
        taffy::NodeId{node.id}
    );

    return taffy_TaffyResult_of_bool_from_cpp(result);
}