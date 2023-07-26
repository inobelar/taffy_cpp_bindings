#ifndef TAFFY_CPP_C_H
#define TAFFY_CPP_C_H

/*
    taffy_cpp C bindings

    Currently, in that header file, declared only minimal public
    taffy/taffy_cpp API. It describes the next main parts:

        Taffy       <-- Nodes tree structure
        +-- Style   <-- Node style description
        +-- Layout  <-- Node computed location & size

    and the set of types they depend on.
*/

#include <stdint.h> /* for: {u}int{16,32,64}_t */
#include <stddef.h> /* for: size_t */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* common types --------------------------------------------------------------*/

    /* Option<T> ------------------------------------------------------------ */

        /* Option<float> ---------------------------------------------------- */

        typedef struct taffy_Option_float taffy_Option_float;

        /* constructors */
        taffy_Option_float* taffy_Option_float_new_default(void);
        taffy_Option_float* taffy_Option_float_new(float* value);
        taffy_Option_float* taffy_Option_float_new_some(float value);

        /* copy constuctor */
        taffy_Option_float* taffy_Option_float_new_copy(const taffy_Option_float* other);

        /* destructor */
        void taffy_Option_float_delete(taffy_Option_float* self);

        /* getters */
        /* bool */ int taffy_Option_float_is_some(const taffy_Option_float* self);
        float taffy_Option_float_get_value(const taffy_Option_float* self);

        /* setters */
        void taffy_Option_float_set_value(taffy_Option_float* self, const float* value);

/* geometry types ------------------------------------------------------------*/

    /* Point<T> ------------------------------------------------------------- */

        /* Point<float> ----------------------------------------------------- */

        typedef struct taffy_Point_of_float taffy_Point_of_float;

        /* constructors */
        taffy_Point_of_float* taffy_Point_of_float_new_default(void);
        taffy_Point_of_float* taffy_Point_of_float_new(float x, float y);

        /* copy constructor */
        taffy_Point_of_float* taffy_Point_of_float_new_copy(const taffy_Point_of_float* other);

        /* destructor */
        void taffy_Point_of_float_delete(taffy_Point_of_float* self);

        /* getters */
        float taffy_Point_of_float_get_x(const taffy_Point_of_float* self);
        float taffy_Point_of_float_get_y(const taffy_Point_of_float* self);

        /* setters */
        void taffy_Point_of_float_set_x(taffy_Point_of_float* self, float x);
        void taffy_Point_of_float_set_y(taffy_Point_of_float* self, float y);

        /* mutators */
        float* taffy_Point_of_float_get_mut_x(taffy_Point_of_float* self);
        float* taffy_Point_of_float_get_mut_y(taffy_Point_of_float* self);

        /* specialization extras */
        taffy_Point_of_float* taffy_Point_of_float_new_ZERO(void);

    /* Size<T> -------------------------------------------------------------- */

        /* Size<float> ------------------------------------------------------ */

        typedef struct taffy_Size_of_float taffy_Size_of_float;

        /* constructors */
        taffy_Size_of_float* taffy_Size_of_float_new_default(void);
        taffy_Size_of_float* taffy_Size_of_float_new(float width, float height);

        /* copy constrcutor */
        taffy_Size_of_float* taffy_Size_of_float_new_copy(const taffy_Size_of_float* other);

        /* destructor */
        void taffy_Size_of_float_delete(taffy_Size_of_float* self);

        /* getters */
        float taffy_Size_of_float_get_width (const taffy_Size_of_float* self);
        float taffy_Size_of_float_get_height(const taffy_Size_of_float* self);

        /* setters */
        void taffy_Size_of_float_set_width (taffy_Size_of_float* self, float width );
        void taffy_Size_of_float_set_height(taffy_Size_of_float* self, float height);

        /* mutators */
        float* taffy_Size_of_float_get_mut_width (taffy_Size_of_float* self);
        float* taffy_Size_of_float_get_mut_height(taffy_Size_of_float* self);

        /* specialization extras */
        taffy_Size_of_float* taffy_Size_of_float_new_ZERO(void);

/* style types -------------------------------------------------------------- */

    /* alignment types ------------------------------------------------------ */

        /* AlignContent */
        typedef enum {
            taffy_AlignContent_Start = 0,
            taffy_AlignContent_End,
            taffy_AlignContent_FlexStart,
            taffy_AlignContent_FlexEnd,
            taffy_AlignContent_Center,
            taffy_AlignContent_Stretch,
            taffy_AlignContent_SpaceBetween,
            taffy_AlignContent_SpaceEvenly,
            taffy_AlignContent_SpaceAround
        } taffy_AlignContent;

        /* JustifyContent (same as AlignContent) */
        typedef enum {
            taffy_JustifyContent_Start = 0,
            taffy_JustifyContent_End,
            taffy_JustifyContent_FlexStart,
            taffy_JustifyContent_FlexEnd,
            taffy_JustifyContent_Center,
            taffy_JustifyContent_Stretch,
            taffy_JustifyContent_SpaceBetween,
            taffy_JustifyContent_SpaceEvenly,
            taffy_JustifyContent_SpaceAround
        } taffy_JustifyContent;

        /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -  */

        /* AlignItems */
        typedef enum {
            taffy_AlignItems_Start = 0,
            taffy_AlignItems_End,
            taffy_AlignItems_FlexStart,
            taffy_AlignItems_FlexEnd,
            taffy_AlignItems_Center,
            taffy_AlignItems_Baseline,
            taffy_AlignItems_Stretch
        } taffy_AlignItems;

        /* AlignSelf (same as AlignItems) */
        typedef enum {
            taffy_AlignSelf_Start = 0,
            taffy_AlignSelf_End,
            taffy_AlignSelf_FlexStart,
            taffy_AlignSelf_FlexEnd,
            taffy_AlignSelf_Center,
            taffy_AlignSelf_Baseline,
            taffy_AlignSelf_Stretch
        } taffy_AlignSelf;

    /* dimension types ------------------------------------------------------ */

        /* AvailableSpace --------------------------------------------------- */

        typedef struct taffy_AvailableSpace taffy_AvailableSpace;

        /* constructors */
        taffy_AvailableSpace* taffy_AvailableSpace_new_Definite(float value);
        taffy_AvailableSpace* taffy_AvailableSpace_new_MinContent(void);
        taffy_AvailableSpace* taffy_AvailableSpace_new_MaxContent(void);

        /* copy constructor */
        taffy_AvailableSpace* taffy_AvailableSpace_new_copy(const taffy_AvailableSpace* other);

        /* destructor */
        void taffy_AvailableSpace_delete(taffy_AvailableSpace* self);

        /* extras */
        taffy_AvailableSpace* taffy_AvailableSpace_new_ZERO(void);
        taffy_AvailableSpace* taffy_AvailableSpace_new_from_length(float value);
        taffy_AvailableSpace* taffy_AvailableSpace_new_from_float(float value);
        taffy_AvailableSpace* taffy_AvailableSpace_new_from_option(const taffy_Option_float* opt);

        /* LengthPercentage ------------------------------------------------- */

        typedef struct taffy_LengthPercentage taffy_LengthPercentage;

        /* constructors */
        taffy_LengthPercentage* taffy_LengthPercentage_new_Length(float value);
        taffy_LengthPercentage* taffy_LengthPercentage_new_Percent(float value);

        /* copy constructor */
        taffy_LengthPercentage* taffy_LengthPercentage_new_copy(const taffy_LengthPercentage* other);

        /* destructor */
        void taffy_LengthPercentage_delete(taffy_LengthPercentage* self);

        /* extras */
        taffy_LengthPercentage* taffy_LengthPercentage_new_ZERO(void);
        taffy_LengthPercentage* taffy_LengthPercentage_new_from_length(float value);
        taffy_LengthPercentage* taffy_LengthPercentage_new_from_percent(float value);

        /* LengthPercentageAuto --------------------------------------------- */

        typedef struct taffy_LengthPercentageAuto taffy_LengthPercentageAuto;

        /* constructors */
        taffy_LengthPercentageAuto* taffy_LengthPercentageAuto_new_Length(float value);
        taffy_LengthPercentageAuto* taffy_LengthPercentageAuto_new_Percent(float value);
        taffy_LengthPercentageAuto* taffy_LengthPercentageAuto_new_Auto(void);

        /* copy constructor */
        taffy_LengthPercentageAuto* taffy_LengthPercentageAuto_new_copy(const taffy_LengthPercentageAuto* other);

        /* destructor */
        void taffy_LengthPercentageAuto_delete(taffy_LengthPercentageAuto* self);

        /* extras */
        taffy_LengthPercentageAuto* taffy_LengthPercentageAuto_new_ZERO(void);
        taffy_LengthPercentageAuto* taffy_LengthPercentageAuto_new_AUTO(void);
        taffy_LengthPercentageAuto* taffy_LengthPercentageAuto_new_from_length(float value);
        taffy_LengthPercentageAuto* taffy_LengthPercentageAuto_new_from_percent(float value);
        taffy_LengthPercentageAuto* taffy_LengthPercentageAuto_new_from_LengthPercentage(const taffy_LengthPercentage* input);

        /* Dimension -------------------------------------------------------- */

        typedef struct taffy_Dimension taffy_Dimension;

        /* constructors */
        taffy_Dimension* taffy_Dimension_new_Length(float value);
        taffy_Dimension* taffy_Dimension_new_Percent(float value);
        taffy_Dimension* taffy_Dimension_new_Auto(void);

        /* copy constructor */
        taffy_Dimension* taffy_Dimension_new_copy(const taffy_Dimension* other);

        /* destructor */
        void taffy_Dimension_delete(taffy_Dimension* self);

        /* extras */
        taffy_Dimension* taffy_Dimension_new_ZERO(void);
        taffy_Dimension* taffy_Dimension_new_from_length(float value);
        taffy_Dimension* taffy_Dimension_new_from_percent(float value);
        taffy_Dimension* taffy_Dimension_new_from_LengthPercentage(const taffy_LengthPercentage* input);
        taffy_Dimension* taffy_Dimension_new_from_LengthPercentageAuto(const taffy_LengthPercentageAuto* input);

    /* flex types ----------------------------------------------------------- */

        /* FlexDirection ---------------------------------------------------- */

        typedef enum {
            taffy_FlexDirection_Row = 0,
            taffy_FlexDirection_Column,
            taffy_FlexDirection_RowReverse,
            taffy_FlexDirection_ColumnReverse
        } taffy_FlexDirection;

        taffy_FlexDirection taffy_FlexDirection_default(void);

        /* FlexWrap --------------------------------------------------------- */

        typedef enum {
            taffy_FlexWrap_NoWrap = 0,
            taffy_FlexWrap_Wrap,
            taffy_FlexWrap_WrapReverse
        } taffy_FlexWrap;

        taffy_FlexWrap taffy_FlexWrap_default(void);

    /* grid types ----------------------------------------------------------- */

        /* GridAutoFlow ----------------------------------------------------- */

        typedef enum {
            taffy_GridAutoFlow_Row = 0,
            taffy_GridAutoFlow_Column,
            taffy_GridAutoFlow_RowDense,
            taffy_GridAutoFlow_ColumnDense
        } taffy_GridAutoFlow;

        taffy_GridAutoFlow taffy_GridAutoFlow_default(void);

        /* GridPlacement ---------------------------------------------------- */

        typedef struct taffy_GridPlacement taffy_GridPlacement;

        /* constructors */
        taffy_GridPlacement* taffy_GridPlacement_new_default(void);
        taffy_GridPlacement* taffy_GridPlacement_new_Auto(void);
        taffy_GridPlacement* taffy_GridPlacement_new_Line(int16_t value);
        taffy_GridPlacement* taffy_GridPlacement_new_Span(uint16_t value);

        /* copy constructor */
        taffy_GridPlacement* taffy_GridPlacement_new_copy(const taffy_GridPlacement* other);

        /* destructor */
        void taffy_GridPlacement_delete(taffy_GridPlacement* self);

        /* specialization extras */
        taffy_GridPlacement* taffy_GridPlacement_new_AUTO(void);
        taffy_GridPlacement* taffy_GridPlacement_new_from_line_index(int16_t index);
        taffy_GridPlacement* taffy_GridPlacement_new_from_span(uint16_t span);

        /* GridTrackRepetition ---------------------------------------------- */

        typedef struct taffy_GridTrackRepetition taffy_GridTrackRepetition;

        /* constructors */
        taffy_GridTrackRepetition* taffy_GridTrackRepetition_new_AutoFill(void);
        taffy_GridTrackRepetition* taffy_GridTrackRepetition_new_AutoFit(void);
        taffy_GridTrackRepetition* taffy_GridTrackRepetition_new_Count(uint16_t value);

        /* copy constructor */
        taffy_GridTrackRepetition* taffy_GridTrackRepetition_new_copy(taffy_GridTrackRepetition* other);

        /* destructor */
        void taffy_GridTrackRepetition_delete(taffy_GridTrackRepetition* self);

        /* extras */
        /* null on failure */ taffy_GridTrackRepetition* taffy_GridTrackRepetition_try_from_u16(uint16_t value);
        /* null on failure */ taffy_GridTrackRepetition* taffy_GridTrackRepetition_try_from_str(const char* str);

        /* MaxTrackSizingFunction ------------------------------------------- */

        typedef struct taffy_MaxTrackSizingFunction taffy_MaxTrackSizingFunction;

        /* constructors */
        taffy_MaxTrackSizingFunction* taffy_MaxTrackSizingFunction_new_Fixed(const taffy_LengthPercentage* value);
        taffy_MaxTrackSizingFunction* taffy_MaxTrackSizingFunction_new_MinContent(void);
        taffy_MaxTrackSizingFunction* taffy_MaxTrackSizingFunction_new_MaxContent(void);
        taffy_MaxTrackSizingFunction* taffy_MaxTrackSizingFunction_new_FitContent(const taffy_LengthPercentage* value);
        taffy_MaxTrackSizingFunction* taffy_MaxTrackSizingFunction_new_Auto(void);
        taffy_MaxTrackSizingFunction* taffy_MaxTrackSizingFunction_new_Fraction(float value);

        /* copy constructor */
        taffy_MaxTrackSizingFunction* taffy_MaxTrackSizingFunction_new_copy(const taffy_MaxTrackSizingFunction* other);

        /* destructor */
        void taffy_MaxTrackSizingFunction_delete(taffy_MaxTrackSizingFunction* self);

        /* extras */
        taffy_MaxTrackSizingFunction* taffy_MaxTrackSizingFunction_new_AUTO(void);
        taffy_MaxTrackSizingFunction* taffy_MaxTrackSizingFunction_new_MIN_CONTENT(void);
        taffy_MaxTrackSizingFunction* taffy_MaxTrackSizingFunction_new_MAX_CONTENT(void);
        taffy_MaxTrackSizingFunction* taffy_MaxTrackSizingFunction_new_ZERO(void);

        taffy_MaxTrackSizingFunction* taffy_MaxTrackSizingFunction_new_fit_content(const taffy_LengthPercentage* argument);
        taffy_MaxTrackSizingFunction* taffy_MaxTrackSizingFunction_new_from_length(float value);
        taffy_MaxTrackSizingFunction* taffy_MaxTrackSizingFunction_new_from_percent(float percent);
        taffy_MaxTrackSizingFunction* taffy_MaxTrackSizingFunction_new_from_flex(float flex);

        /* MinTrackSizingFunction ------------------------------------------- */

        typedef struct taffy_MinTrackSizingFunction taffy_MinTrackSizingFunction;

        /* constructors */
        taffy_MinTrackSizingFunction* taffy_MinTrackSizingFunction_new_Fixed(const taffy_LengthPercentage* value);
        taffy_MinTrackSizingFunction* taffy_MinTrackSizingFunction_new_MinContent(void);
        taffy_MinTrackSizingFunction* taffy_MinTrackSizingFunction_new_MaxContent(void);
        taffy_MinTrackSizingFunction* taffy_MinTrackSizingFunction_new_Auto(void);

        /* copy constructor */
        taffy_MinTrackSizingFunction* taffy_MinTrackSizingFunction_new_copy(const taffy_MinTrackSizingFunction* other);

        /* destructors */
        void taffy_MinTrackSizingFunction_delete(taffy_MinTrackSizingFunction* self);

        /* extras */
        taffy_MinTrackSizingFunction* taffy_MinTrackSizingFunction_new_AUTO(void);
        taffy_MinTrackSizingFunction* taffy_MinTrackSizingFunction_new_MIN_CONTENT(void);
        taffy_MinTrackSizingFunction* taffy_MinTrackSizingFunction_new_MAX_CONTENT(void);
        taffy_MinTrackSizingFunction* taffy_MinTrackSizingFunction_new_ZERO(void);

        taffy_MinTrackSizingFunction* taffy_MinTrackSizingFunction_new_from_length(float value);
        taffy_MinTrackSizingFunction* taffy_MinTrackSizingFunction_new_from_percent(float percent);

        /* NonRepeatedTrackSizingFunction ----------------------------------- */

        typedef struct taffy_NonRepeatedTrackSizingFunction taffy_NonRepeatedTrackSizingFunction;

        /* constructors */
        taffy_NonRepeatedTrackSizingFunction* taffy_NonRepeatedTrackSizingFunction_new(const taffy_MinTrackSizingFunction* min, const taffy_MaxTrackSizingFunction* max);

        /* copy constructor */
        taffy_NonRepeatedTrackSizingFunction* taffy_NonRepeatedTrackSizingFunction_new_copy(const taffy_NonRepeatedTrackSizingFunction* other);

        /* destructor */
        void taffy_NonRepeatedTrackSizingFunction_delete(taffy_NonRepeatedTrackSizingFunction* self);

        /* getters */
        const taffy_MinTrackSizingFunction* taffy_NonRepeatedTrackSizingFunction_get_min(const taffy_NonRepeatedTrackSizingFunction* self);
        const taffy_MaxTrackSizingFunction* taffy_NonRepeatedTrackSizingFunction_get_max(const taffy_NonRepeatedTrackSizingFunction* self);

        /* setters */
        void taffy_NonRepeatedTrackSizingFunction_set_min(taffy_NonRepeatedTrackSizingFunction* self, const taffy_MinTrackSizingFunction* min);
        void taffy_NonRepeatedTrackSizingFunction_set_max(taffy_NonRepeatedTrackSizingFunction* self, const taffy_MinTrackSizingFunction* max);

        /* mutators */
        taffy_MinTrackSizingFunction* taffy_NonRepeatedTrackSizingFunction_get_mut_min(taffy_NonRepeatedTrackSizingFunction* self);
        taffy_MaxTrackSizingFunction* taffy_NonRepeatedTrackSizingFunction_get_mut_max(taffy_NonRepeatedTrackSizingFunction* self);

        /* TrackSizingFunction ---------------------------------------------- */

        typedef struct taffy_TrackSizingFunction taffy_TrackSizingFunction;

        /* constructors */
        taffy_TrackSizingFunction* taffy_TrackSizingFunction_new_Single(const taffy_NonRepeatedTrackSizingFunction* value);
        taffy_TrackSizingFunction* taffy_TrackSizingFunction_new_Repeat(
            const taffy_GridTrackRepetition* repetition,
            const taffy_NonRepeatedTrackSizingFunction* funcs, size_t funcs_count
        );

        /* copy constructor */
        taffy_TrackSizingFunction* taffy_TrackSizingFunction_new_copy(const taffy_TrackSizingFunction* other);

        /* destructor */
        void taffy_TrackSizingFunction_delete(taffy_TrackSizingFunction* self);

        /* extras */
        taffy_TrackSizingFunction* taffy_TrackSizingFunction_new_AUTO(void);
        taffy_TrackSizingFunction* taffy_TrackSizingFunction_new_MIN_CONTENT(void);
        taffy_TrackSizingFunction* taffy_TrackSizingFunction_new_MAX_CONTENT(void);
        taffy_TrackSizingFunction* taffy_TrackSizingFunction_new_ZERO(void);

        taffy_TrackSizingFunction* taffy_TrackSizingFunction_new_fit_content(const taffy_LengthPercentage* argument);
        taffy_TrackSizingFunction* taffy_TrackSizingFunction_new_from_length(float value);
        taffy_TrackSizingFunction* taffy_TrackSizingFunction_new_from_percent(float percent);
        taffy_TrackSizingFunction* taffy_TrackSizingFunction_new_from_flex(float flex);

    /* Display -------------------------------------------------------------- */

    typedef enum {
        taffy_Display_Flex = 0,
        taffy_Display_Grid,
        taffy_Display_Block,
        taffy_Display_None
    } taffy_Display;

    taffy_Display taffy_Display_default(void);

    /* Overflow ------------------------------------------------------------- */

    typedef enum {
        taffy_Overflow_Visible = 0,
        taffy_Overflow_Hidden,
        taffy_Overflow_Scroll
    } taffy_Overflow;

    taffy_Overflow taffy_Overflow_default(void);

    /* Position ------------------------------------------------------------- */

    typedef enum {
        taffy_Position_Relative = 0,
        taffy_Position_Absolute
    } taffy_Position;

    taffy_Position taffy_Position_default(void);

    /* Style ---------------------------------------------------------------- */

        /* specialized types ------------------------------------------------ */

            /* Point<Overflow> ---------------------------------------------- */

            typedef struct taffy_Point_of_Overflow taffy_Point_of_Overflow;

            /* constructors */
            taffy_Point_of_Overflow* taffy_Point_of_Overflow_new_default(void);
            taffy_Point_of_Overflow* taffy_Point_of_Overflow_new(taffy_Overflow x, taffy_Overflow y);

            /* copy constructor */
            taffy_Point_of_Overflow* taffy_Point_of_Overflow_new_copy(const taffy_Point_of_Overflow* other);

            /* destructor */
            void taffy_Point_of_Overflow_delete(taffy_Point_of_Overflow* self);

            /* getters */
            taffy_Overflow taffy_Point_of_Overflow_get_x(const taffy_Point_of_Overflow* self);
            taffy_Overflow taffy_Point_of_Overflow_get_y(const taffy_Point_of_Overflow* self);

            /* setters */
            void taffy_Point_of_Overflow_set_x(taffy_Point_of_Overflow* self, taffy_Overflow x);
            void taffy_Point_of_Overflow_set_y(taffy_Point_of_Overflow* self, taffy_Overflow y);

            /* Rect<LengthPercentage> ----------------------------------- */

            typedef struct taffy_Rect_of_LengthPercentage taffy_Rect_of_LengthPercentage;

            /* constructors */
            taffy_Rect_of_LengthPercentage* taffy_Rect_of_LengthPercentage_new(
                const taffy_LengthPercentage* left, 
                const taffy_LengthPercentage* top,
                const taffy_LengthPercentage* right,
                const taffy_LengthPercentage* bottom
            );

            /* copy constructor */
            taffy_Rect_of_LengthPercentage* taffy_Rect_of_LengthPercentage_new_copy(const taffy_Rect_of_LengthPercentage* other);

            /* destructor */
            void taffy_Rect_of_LengthPercentage_delete(taffy_Rect_of_LengthPercentage* self);

            /* getters */
            const taffy_LengthPercentage* taffy_Rect_of_LengthPercentage_get_left  (const taffy_Rect_of_LengthPercentage* self);
            const taffy_LengthPercentage* taffy_Rect_of_LengthPercentage_get_top   (const taffy_Rect_of_LengthPercentage* self);
            const taffy_LengthPercentage* taffy_Rect_of_LengthPercentage_get_right (const taffy_Rect_of_LengthPercentage* self);
            const taffy_LengthPercentage* taffy_Rect_of_LengthPercentage_get_bottom(const taffy_Rect_of_LengthPercentage* self);

            /* setters */
            void taffy_Rect_of_LengthPercentage_set_left  (taffy_Rect_of_LengthPercentage* self, const taffy_LengthPercentage* left);
            void taffy_Rect_of_LengthPercentage_set_top   (taffy_Rect_of_LengthPercentage* self, const taffy_LengthPercentage* top);
            void taffy_Rect_of_LengthPercentage_set_right (taffy_Rect_of_LengthPercentage* self, const taffy_LengthPercentage* right);
            void taffy_Rect_of_LengthPercentage_set_bottom(taffy_Rect_of_LengthPercentage* self, const taffy_LengthPercentage* bottom);

            /* mutators */
            taffy_LengthPercentage* taffy_Rect_of_LengthPercentage_get_mut_left  (taffy_Rect_of_LengthPercentage* self);
            taffy_LengthPercentage* taffy_Rect_of_LengthPercentage_get_mut_top   (taffy_Rect_of_LengthPercentage* self);
            taffy_LengthPercentage* taffy_Rect_of_LengthPercentage_get_mut_right (taffy_Rect_of_LengthPercentage* self);
            taffy_LengthPercentage* taffy_Rect_of_LengthPercentage_get_mut_bottom(taffy_Rect_of_LengthPercentage* self);

            /* Rect<LengthPercentageAuto> ----------------------------------- */

            typedef struct taffy_Rect_of_LengthPercentageAuto taffy_Rect_of_LengthPercentageAuto;

            /* constructors */
            taffy_Rect_of_LengthPercentageAuto* taffy_Rect_of_LengthPercentageAuto_new(
                const taffy_LengthPercentageAuto* left, 
                const taffy_LengthPercentageAuto* top,
                const taffy_LengthPercentageAuto* right,
                const taffy_LengthPercentageAuto* bottom
            );

            /* copy constructor */
            taffy_Rect_of_LengthPercentageAuto* taffy_Rect_of_LengthPercentageAuto_new_copy(const taffy_Rect_of_LengthPercentageAuto* other);

            /* destructor */
            void taffy_Rect_of_LengthPercentageAuto_delete(taffy_Rect_of_LengthPercentageAuto* self);

            /* getters */
            const taffy_LengthPercentageAuto* taffy_Rect_of_LengthPercentageAuto_get_left  (const taffy_Rect_of_LengthPercentageAuto* self);
            const taffy_LengthPercentageAuto* taffy_Rect_of_LengthPercentageAuto_get_top   (const taffy_Rect_of_LengthPercentageAuto* self);
            const taffy_LengthPercentageAuto* taffy_Rect_of_LengthPercentageAuto_get_right (const taffy_Rect_of_LengthPercentageAuto* self);
            const taffy_LengthPercentageAuto* taffy_Rect_of_LengthPercentageAuto_get_bottom(const taffy_Rect_of_LengthPercentageAuto* self);

            /* setters */
            void taffy_Rect_of_LengthPercentageAuto_set_left  (taffy_Rect_of_LengthPercentageAuto* self, const taffy_LengthPercentageAuto* left);
            void taffy_Rect_of_LengthPercentageAuto_set_top   (taffy_Rect_of_LengthPercentageAuto* self, const taffy_LengthPercentageAuto* top);
            void taffy_Rect_of_LengthPercentageAuto_set_right (taffy_Rect_of_LengthPercentageAuto* self, const taffy_LengthPercentageAuto* right);
            void taffy_Rect_of_LengthPercentageAuto_set_bottom(taffy_Rect_of_LengthPercentageAuto* self, const taffy_LengthPercentageAuto* bottom);

            /* mutators */
            taffy_LengthPercentageAuto* taffy_Rect_of_LengthPercentageAuto_get_mut_left  (taffy_Rect_of_LengthPercentageAuto* self);
            taffy_LengthPercentageAuto* taffy_Rect_of_LengthPercentageAuto_get_mut_top   (taffy_Rect_of_LengthPercentageAuto* self);
            taffy_LengthPercentageAuto* taffy_Rect_of_LengthPercentageAuto_get_mut_right (taffy_Rect_of_LengthPercentageAuto* self);
            taffy_LengthPercentageAuto* taffy_Rect_of_LengthPercentageAuto_get_mut_bottom(taffy_Rect_of_LengthPercentageAuto* self);

            /* Size<LengthPercentage> ---------------------------------------------- */

            typedef struct taffy_Size_of_LengthPercentage taffy_Size_of_LengthPercentage;

            /* constructors */
            taffy_Size_of_LengthPercentage* taffy_Size_of_LengthPercentage_new(const taffy_LengthPercentage* width, const taffy_LengthPercentage* height);

            /* copy constructor */
            taffy_Size_of_LengthPercentage* taffy_Size_of_LengthPercentage_new_copy(const taffy_Size_of_LengthPercentage* other);

            /* destructor */
            void taffy_Size_of_LengthPercentage_delete(taffy_Size_of_LengthPercentage* self);

            /* getters */
            const taffy_LengthPercentage* taffy_Size_of_LengthPercentage_get_width (const taffy_Size_of_LengthPercentage* self);
            const taffy_LengthPercentage* taffy_Size_of_LengthPercentage_get_height(const taffy_Size_of_LengthPercentage* self);

            /* setters */
            void taffy_Size_of_LengthPercentage_set_width (taffy_Size_of_LengthPercentage* self, const taffy_LengthPercentage* width );
            void taffy_Size_of_LengthPercentage_set_height(taffy_Size_of_LengthPercentage* self, const taffy_LengthPercentage* height);

            /* mutators */
            taffy_LengthPercentage* taffy_Size_of_LengthPercentage_get_mut_width (taffy_Size_of_LengthPercentage* self);
            taffy_LengthPercentage* taffy_Size_of_LengthPercentage_get_mut_height(taffy_Size_of_LengthPercentage* self);

            /* Size<Dimension> ---------------------------------------------- */

            typedef struct taffy_Size_of_Dimension taffy_Size_of_Dimension;

            /* constructors */
            taffy_Size_of_Dimension* taffy_Size_of_Dimension_new(const taffy_Dimension* width, const taffy_Dimension* height);

            /* copy constructor */
            taffy_Size_of_Dimension* taffy_Size_of_Dimension_new_copy(const taffy_Size_of_Dimension* other);

            /* destructor */
            void taffy_Size_of_Dimension_delete(taffy_Size_of_Dimension* self);

            /* getters */
            const taffy_Dimension* taffy_Size_of_Dimension_get_width (const taffy_Size_of_Dimension* self);
            const taffy_Dimension* taffy_Size_of_Dimension_get_height(const taffy_Size_of_Dimension* self);

            /* setters */
            void taffy_Size_of_Dimension_set_width (taffy_Size_of_Dimension* self, const taffy_Dimension* width );
            void taffy_Size_of_Dimension_set_height(taffy_Size_of_Dimension* self, const taffy_Dimension* height);

            /* mutators */
            taffy_Dimension* taffy_Size_of_Dimension_get_mut_width (taffy_Size_of_Dimension* self);
            taffy_Dimension* taffy_Size_of_Dimension_get_mut_height(taffy_Size_of_Dimension* self);

            /* Line<GridPlacement> ------------------------------------------ */

            typedef struct taffy_Line_of_GridPlacement taffy_Line_of_GridPlacement;

            /* constructors */
            taffy_Line_of_GridPlacement* taffy_Line_of_GridPlacement_new_default(void);
            taffy_Line_of_GridPlacement* taffy_Line_of_GridPlacement_new(const taffy_GridPlacement* start, const taffy_GridPlacement* end);

            /* copy constructor */
            taffy_Line_of_GridPlacement* taffy_Line_of_GridPlacement_new_copy(const taffy_Line_of_GridPlacement* other);

            /* destructor */
            void taffy_Line_of_GridPlacement_delete(taffy_Line_of_GridPlacement* self);

            /* getters */
            const taffy_GridPlacement* taffy_Line_of_GridPlacement_get_start(const taffy_Line_of_GridPlacement* self);
            const taffy_GridPlacement* taffy_Line_of_GridPlacement_get_end  (const taffy_Line_of_GridPlacement* self);

            /* setters */
            void taffy_Line_of_GridPlacement_set_start(taffy_Line_of_GridPlacement* self, const taffy_GridPlacement* start);
            void taffy_Line_of_GridPlacement_set_end  (taffy_Line_of_GridPlacement* self, const taffy_GridPlacement* end  );

            /* mutators */
            taffy_GridPlacement* taffy_Line_of_GridPlacement_get_mut_start(taffy_Line_of_GridPlacement* self);
            taffy_GridPlacement* taffy_Line_of_GridPlacement_get_mut_end  (taffy_Line_of_GridPlacement* self);

            /* Option<AlignContent> ----------------------------------------- */

            typedef struct taffy_Option_AlignContent taffy_Option_AlignContent;

            /* constructors */
            taffy_Option_AlignContent* taffy_Option_AlignContent_new_default(void);
            taffy_Option_AlignContent* taffy_Option_AlignContent_new(const taffy_AlignContent* value);
            taffy_Option_AlignContent* taffy_Option_AlignContent_new_some(taffy_AlignContent value);

            /* copy constuctor */
            taffy_Option_AlignContent* taffy_Option_AlignContent_new_copy(const taffy_Option_AlignContent* other);

            /* destructor */
            void taffy_Option_AlignContent_delete(taffy_Option_AlignContent* self);

            /* getters */
            /* bool */ int taffy_Option_AlignContent_is_some(const taffy_Option_AlignContent* self);
            taffy_AlignContent taffy_Option_AlignContent_get_value(const taffy_Option_AlignContent* self);

            /* setters */
            void taffy_Option_AlignContent_set_value(taffy_Option_AlignContent* self, const taffy_AlignContent* value);

            /* Option<JustifyContent> --------------------------------------- */

            typedef struct taffy_Option_JustifyContent taffy_Option_JustifyContent;

            /* constructors */
            taffy_Option_JustifyContent* taffy_Option_JustifyContent_new_default(void);
            taffy_Option_JustifyContent* taffy_Option_JustifyContent_new(const taffy_JustifyContent* value);
            taffy_Option_JustifyContent* taffy_Option_JustifyContent_new_some(taffy_JustifyContent value);

            /* copy constuctor */
            taffy_Option_JustifyContent* taffy_Option_JustifyContent_new_copy(const taffy_Option_JustifyContent* other);

            /* destructor */
            void taffy_Option_JustifyContent_delete(taffy_Option_JustifyContent* self);

            /* getters */
            /* bool */ int taffy_Option_JustifyContent_is_some(const taffy_Option_JustifyContent* self);
            taffy_JustifyContent taffy_Option_JustifyContent_get_value(const taffy_Option_JustifyContent* self);

            /* setters */
            void taffy_Option_JustifyContent_set_value(taffy_Option_JustifyContent* self, const taffy_JustifyContent* value);

            /* Option<AlignItems> ------------------------------------------- */

            typedef struct taffy_Option_AlignItems taffy_Option_AlignItems;

            /* constructors */
            taffy_Option_AlignItems* taffy_Option_AlignItems_new_default(void);
            taffy_Option_AlignItems* taffy_Option_AlignItems_new(const taffy_AlignItems* value);
            taffy_Option_AlignItems* taffy_Option_AlignItems_new_some(taffy_AlignItems value);

            /* copy constuctor */
            taffy_Option_AlignItems* taffy_Option_AlignItems_new_copy(const taffy_Option_AlignItems* other);

            /* destructor */
            void taffy_Option_AlignItems_delete(taffy_Option_AlignItems* self);

            /* getters */
            /* bool */ int taffy_Option_AlignItems_is_some(const taffy_Option_AlignItems* self);
            taffy_AlignItems taffy_Option_AlignItems_get_value(const taffy_Option_AlignItems* self);

            /* setters */
            void taffy_Option_AlignItems_set_value(taffy_Option_AlignItems* self, const taffy_AlignItems* value);

            /* Option<AlignSelf> -------------------------------------------- */

            typedef struct taffy_Option_AlignSelf taffy_Option_AlignSelf;

            /* constructors */
            taffy_Option_AlignSelf* taffy_Option_AlignSelf_new_default(void);
            taffy_Option_AlignSelf* taffy_Option_AlignSelf_new(const taffy_AlignSelf* value);
            taffy_Option_AlignSelf* taffy_Option_AlignSelf_new_some(taffy_AlignSelf value);

            /* copy constuctor */
            taffy_Option_AlignSelf* taffy_Option_AlignSelf_new_copy(const taffy_Option_AlignSelf* other);

            /* destructor */
            void taffy_Option_AlignSelf_delete(taffy_Option_AlignSelf* self);

            /* getters */
            /* bool */ int taffy_Option_AlignSelf_is_some(const taffy_Option_AlignSelf* self);
            taffy_AlignSelf taffy_Option_AlignSelf_get_value(const taffy_Option_AlignSelf* self);

            /* setters */
            void taffy_Option_AlignSelf_set_value(taffy_Option_AlignSelf* self, const taffy_AlignSelf* value);

            /* GridTrackVec<TrackSizingFunction> ---------------------------- */

            typedef struct {
                const taffy_TrackSizingFunction* items;
                size_t items_count;
            } taffy_GridTrackVec_of_TrackSizingFunction;

            /* GridTrackVec<NonRepeatedTrackSizingFunction> ----------------- */

            typedef struct {
                const taffy_NonRepeatedTrackSizingFunction* items;
                size_t items_count;
            } taffy_GridTrackVec_of_NonRepeatedTrackSizingFunction;

    typedef struct taffy_Style taffy_Style;

    /* constructors */
    taffy_Style* taffy_Style_new_default(void);

    /* copy construcotr */
    taffy_Style* taffy_Style_new_copy(const taffy_Style* other);

    /* destructor */
    void taffy_Style_delete(taffy_Style* self);

    /* extra */
    taffy_Style* taffy_Style_new_DEFAULT(void);

    /* getters */

        taffy_Display taffy_Style_get_display(const taffy_Style* self);

        /* Overflow properties ---------------------------------------------- */

        const taffy_Point_of_Overflow* taffy_Style_get_overflow(const taffy_Style* self);

        float taffy_Style_get_scrollbar_width(const taffy_Style* self);

        /* Position properties ---------------------------------------------- */

        taffy_Position taffy_Style_get_position(const taffy_Style* self);

        const taffy_Rect_of_LengthPercentageAuto* taffy_Style_get_inset(const taffy_Style* self);

        /* Size properties -------------------------------------------------- */

        const taffy_Size_of_Dimension* taffy_Style_get_size(const taffy_Style* self);

        const taffy_Size_of_Dimension* taffy_Style_get_min_size(const taffy_Style* self);

        const taffy_Size_of_Dimension* taffy_Style_get_max_size(const taffy_Style* self);

        const taffy_Option_float* taffy_Style_get_aspect_ratio(const taffy_Style* self);

        /* Spacing properties ----------------------------------------------- */

        const taffy_Rect_of_LengthPercentageAuto* taffy_Style_get_margin(const taffy_Style* self);

        const taffy_Rect_of_LengthPercentage* taffy_Style_get_padding(const taffy_Style* self);

        const taffy_Rect_of_LengthPercentage* taffy_Style_get_border(const taffy_Style* self);

        /* Alignment properties --------------------------------------------- */

        const taffy_Option_AlignItems* taffy_Style_get_align_items(const taffy_Style* self);

        const taffy_Option_AlignSelf* taffy_Style_get_align_self(const taffy_Style* self);

        const taffy_Option_AlignItems* taffy_Style_get_justify_items(const taffy_Style* self);

        const taffy_Option_AlignSelf* taffy_Style_get_justify_self(const taffy_Style* self);

        const taffy_Option_AlignContent* taffy_Style_get_align_content(const taffy_Style* self);

        const taffy_Option_JustifyContent* taffy_Style_get_justify_content(const taffy_Style* self);

        const taffy_Size_of_LengthPercentage* taffy_Style_get_gap(const taffy_Style* self);

        /* Flexbox properties ----------------------------------------------- */

        taffy_FlexDirection taffy_Style_get_flex_direction(const taffy_Style* self);

        taffy_FlexWrap taffy_Style_get_flex_wrap(const taffy_Style* self);

        const taffy_Dimension* taffy_Style_get_flex_basis(const taffy_Style* self);

        float taffy_Style_get_flex_grow(const taffy_Style* self);

        float taffy_Style_get_flex_shrink(const taffy_Style* self);

        /* Grid container properies ----------------------------------------- */

        taffy_GridTrackVec_of_TrackSizingFunction taffy_Style_get_grid_template_rows(const taffy_Style* self);

        taffy_GridTrackVec_of_TrackSizingFunction taffy_Style_get_grid_template_columns(const taffy_Style* self);

        taffy_GridTrackVec_of_NonRepeatedTrackSizingFunction taffy_Style_get_grid_auto_rows(const taffy_Style* self);

        taffy_GridTrackVec_of_NonRepeatedTrackSizingFunction taffy_Style_get_grid_auto_columns(const taffy_Style* self);

        taffy_GridAutoFlow taffy_Style_get_grid_auto_flow(const taffy_Style* self);

        /* Grid child properties -------------------------------------------- */

        const taffy_Line_of_GridPlacement* taffy_Style_get_grid_row(const taffy_Style* self);

        const taffy_Line_of_GridPlacement* taffy_Style_get_grid_column(const taffy_Style* self);

    /* setters */

        void taffy_Style_set_display(taffy_Style* self, taffy_Display value);

        /* Overflow properties ---------------------------------------------- */

        void taffy_Style_set_overflow(taffy_Style* self, const taffy_Point_of_Overflow* value);

        void taffy_Style_set_scrollbar_width(taffy_Style* self, float value);

        /* Position properties ---------------------------------------------- */

        void taffy_Style_set_position(taffy_Style* self, taffy_Position value);

        void taffy_Style_set_inset(taffy_Style* self, const taffy_Rect_of_LengthPercentageAuto* value);

        /* Size properties -------------------------------------------------- */

        void taffy_Style_set_size(taffy_Style* self, const taffy_Size_of_Dimension* value);

        void taffy_Style_set_min_size(taffy_Style* self, const taffy_Size_of_Dimension* value);

        void taffy_Style_set_max_size(taffy_Style* self, const taffy_Size_of_Dimension* value);

        void taffy_Style_set_aspect_ratio(taffy_Style* self, const taffy_Option_float* value);

        /* Spacing properties ----------------------------------------------- */

        void taffy_Style_set_margin(taffy_Style* self, const taffy_Rect_of_LengthPercentageAuto* value);

        void taffy_Style_set_padding(taffy_Style* self, const taffy_Rect_of_LengthPercentage* value);

        void taffy_Style_set_border(taffy_Style* self, const taffy_Rect_of_LengthPercentage* value);

        /* Alignment properties --------------------------------------------- */

        void taffy_Style_set_align_items(taffy_Style* self, const taffy_Option_AlignItems* value);

        void taffy_Style_set_align_self(taffy_Style* self, const taffy_Option_AlignSelf* value);

        void taffy_Style_set_justify_items(taffy_Style* self, const taffy_Option_AlignItems* value);

        void taffy_Style_set_justify_self(taffy_Style* self, const taffy_Option_AlignSelf* value);

        void taffy_Style_set_align_content(taffy_Style* self, const taffy_Option_AlignContent* value);

        void taffy_Style_set_justify_content(taffy_Style* self, const taffy_Option_JustifyContent* value);

        void taffy_Style_set_gap(taffy_Style* self, const taffy_Size_of_LengthPercentage* value);

        /* Flexbox properties ----------------------------------------------- */

        void taffy_Style_set_flex_direction(taffy_Style* self, taffy_FlexDirection value);

        void taffy_Style_set_flex_wrap(taffy_Style* self, taffy_FlexWrap value);

        void taffy_Style_set_flex_basis(taffy_Style* self, const taffy_Dimension* value);

        void taffy_Style_set_flex_grow(taffy_Style* self, float value);

        void taffy_Style_set_flex_shrink(taffy_Style* self, float value);

        /* Grid container properies ----------------------------------------- */

        void taffy_Style_set_grid_template_rows(taffy_Style* self, const taffy_TrackSizingFunction* funcs, size_t funcs_count);

        void taffy_Style_set_grid_template_columns(taffy_Style* self, const taffy_TrackSizingFunction* funcs, size_t funcs_count);

        void taffy_Style_set_grid_auto_rows(taffy_Style* self, const taffy_NonRepeatedTrackSizingFunction* funcs, size_t funcs_count);

        void taffy_Style_set_grid_auto_columns(taffy_Style* self, const taffy_NonRepeatedTrackSizingFunction* funcs, size_t funcs_count);

        void taffy_Style_set_grid_auto_flow(taffy_Style* self, taffy_GridAutoFlow value);

        /* Grid child properties -------------------------------------------- */

        void taffy_Style_set_grid_row(taffy_Style* self, const taffy_Line_of_GridPlacement* value);

        void taffy_Style_set_grid_column(taffy_Style* self, const taffy_Line_of_GridPlacement* value);

/* tree types --------------------------------------------------------------- */

    /* Layout --------------------------------------------------------------- */

    typedef struct taffy_Layout taffy_Layout;

    /* constructors */
    taffy_Layout* taffy_Layout_new_default(void);
    taffy_Layout* taffy_Layout_new(uint32_t order, const taffy_Size_of_float* size, const taffy_Point_of_float* location);
    taffy_Layout* taffy_Layout_new_with_order(uint32_t order);

    /* copy constructor */
    taffy_Layout* taffy_Layout_new_copy(const taffy_Layout* other);

    /* destructor */
    void taffy_Layout_delete(taffy_Layout* self);

    /* getters */
    uint32_t                    taffy_Layout_get_order   (const taffy_Layout* self);
    const taffy_Size_of_float*  taffy_Layout_get_size    (const taffy_Layout* self);
    const taffy_Point_of_float* taffy_Layout_get_location(const taffy_Layout* self);

    /* setters */
    void taffy_Layout_set_order   (taffy_Layout* self, uint32_t order);
    void taffy_Layout_set_size    (taffy_Layout* self, const taffy_Size_of_float* size);
    void taffy_Layout_set_location(taffy_Layout* self, const taffy_Point_of_float* location);

    /* mutators */
    uint32_t*             taffy_Layout_get_mut_order   (taffy_Layout* self);
    taffy_Size_of_float*  taffy_Layout_get_mut_size    (taffy_Layout* self);
    taffy_Point_of_float* taffy_Layout_get_mut_location(taffy_Layout* self);

    /* NodeId --------------------------------------------------------------- */

    typedef struct {
        uint64_t id;
    } taffy_NodeId;

    /* TaffyError ----------------------------------------------------------- */

    typedef enum {
        taffy_TaffyError_Type_Ok = 0,

        taffy_TaffyError_Type_ChildIndexOutOfBounds,
        taffy_TaffyError_Type_InvalidParentNode,
        taffy_TaffyError_Type_InvalidChildNode,
        taffy_TaffyError_Type_InvalidInputNode
    } taffy_TaffyError_Type;

    typedef struct {
        taffy_TaffyError_Type type;

        taffy_NodeId node;
        size_t child_index;
        size_t child_count;
    } taffy_TaffyError;

    /* TaffyResult<T> ------------------------------------------------------- */

        /* TaffyResult<void> ------------------------------------------------ */

        typedef struct {
            taffy_TaffyError error;
            
            /* void value; */ /* <-- Heh, no value - data type is void :) */ 
        } taffy_TaffyResult_of_void;

        /* TaffyResult<bool> ------------------------------------------------ */

        typedef struct {
            
            taffy_TaffyError error;
            /* bool */ int value;
        } taffy_TaffyResult_of_bool;

        /* TaffyResult<size_t> ---------------------------------------------- */

        typedef struct { 
            taffy_TaffyError error;
            
            size_t value;
        } taffy_TaffyResult_of_size_t;

        /* TaffyResult<NodeId> ---------------------------------------------- */

        typedef struct {
            taffy_TaffyError error;
            
            taffy_NodeId value;
        } taffy_TaffyResult_of_NodeId;

        /* TaffyResult<Vec<NodeId>> ----------------------------------------- */

        typedef struct {
            taffy_TaffyError error;

            const taffy_NodeId* items;
            size_t              items_count;
        } taffy_TaffyResult_of_Vec_of_NodeId;

        void taffy_TaffyResult_of_Vec_of_NodeId_delete(taffy_TaffyResult_of_Vec_of_NodeId self);

        /* TaffyResult<&Style> ---------------------------------------------- */

        typedef struct {
            taffy_TaffyError error;

            const taffy_Style* value;
        } taffy_TaffyResult_of_Style_const_ref;

        /* TaffyResult<&Layout> --------------------------------------------- */

        typedef struct {
            taffy_TaffyError error;

            const taffy_Layout* value;
        } taffy_TaffyResult_of_Layout_const_ref;

    /* Taffy ---------------------------------------------------------------- */

        /* specialized types ------------------------------------------------ */

            /* Size<AvailableSpace> ----------------------------------------- */

            typedef struct taffy_Size_of_AvailableSpace taffy_Size_of_AvailableSpace;

            /* constructors */
            taffy_Size_of_AvailableSpace* taffy_taffy_Size_of_AvailableSpace_new(const taffy_AvailableSpace* width, const taffy_AvailableSpace* height);

            /* copy constructor */
            taffy_Size_of_AvailableSpace* taffy_Size_of_AvailableSpace_new_copy(const taffy_Size_of_AvailableSpace* other);

            /* destructor */
            void taffy_Size_of_AvailableSpace_delete(taffy_Size_of_AvailableSpace* self);

            /* getters */
            const taffy_AvailableSpace* taffy_Size_of_AvailableSpace_get_width (const taffy_Size_of_AvailableSpace* self);
            const taffy_AvailableSpace* taffy_Size_of_AvailableSpace_get_height(const taffy_Size_of_AvailableSpace* self);

            /* setters */
            void taffy_Size_of_AvailableSpace_set_width (taffy_Size_of_AvailableSpace* self, const taffy_AvailableSpace* width );
            void taffy_Size_of_AvailableSpace_set_height(taffy_Size_of_AvailableSpace* self, const taffy_AvailableSpace* height);

            /* mutators */
            taffy_AvailableSpace* taffy_Size_of_AvailableSpace_get_mut_width (taffy_Size_of_AvailableSpace* self);
            taffy_AvailableSpace* taffy_Size_of_AvailableSpace_get_mut_height(taffy_Size_of_AvailableSpace* self);

    typedef struct taffy_Taffy taffy_Taffy;

    /* constructors */
    taffy_Taffy* taffy_Taffy_new_default(void);
    taffy_Taffy* taffy_Taffy_new_with_capacity(size_t capacity);

    /* destructor */
    void taffy_Taffy_delete(taffy_Taffy* self);

    /* config */
    void taffy_Taffy_enable_rounding (taffy_Taffy* self);
    void taffy_Taffy_disable_rounding(taffy_Taffy* self);

    /* methods */
    taffy_TaffyResult_of_NodeId taffy_Taffy_new_leaf(
        taffy_Taffy* self, 
        
        const taffy_Style* layout
    );

    /* TODO: new_leaf_with_measure(...) */

    taffy_TaffyResult_of_NodeId taffy_Taffy_new_with_children(
        taffy_Taffy* self,

        const taffy_Style* layout,
        const taffy_NodeId* childs, size_t childs_count
    );

    void taffy_Taffy_clear(taffy_Taffy* self);

    taffy_TaffyResult_of_NodeId taffy_Taffy_remove(
        taffy_Taffy* self,

        taffy_NodeId node
    );

    /* TODO: set_measure(...) */

    taffy_TaffyResult_of_void taffy_Taffy_add_child(
        taffy_Taffy* self,

        taffy_NodeId parent, taffy_NodeId child
    );

    taffy_TaffyResult_of_void taffy_Taffy_insert_child_at_index(
        taffy_Taffy* self,

        taffy_NodeId parent, size_t child_index, taffy_NodeId child
    );

    taffy_TaffyResult_of_void taffy_Taffy_set_children(
        taffy_Taffy* self,

        taffy_NodeId parent, const taffy_NodeId* childs, size_t childs_count 
    );

    taffy_TaffyResult_of_NodeId taffy_Taffy_remove_child(
        taffy_Taffy* self,

        taffy_NodeId parent, taffy_NodeId child
    );

    taffy_TaffyResult_of_NodeId taffy_Taffy_remove_child_at_index(
        taffy_Taffy* self,

        taffy_NodeId parent, size_t child_index
    );

    taffy_TaffyResult_of_NodeId replace_child_at_index(
        taffy_Taffy* self,

        taffy_NodeId parent, size_t child_index, taffy_NodeId child
    );

    taffy_TaffyResult_of_NodeId taffy_Taffy_child_at_index(
        const taffy_Taffy* self,

        taffy_NodeId parent, size_t child_index
    );

    taffy_TaffyResult_of_size_t taffy_Taffy_child_count(
        const taffy_Taffy* self,

        taffy_NodeId parent
    );

    /* ATTENTION: 
        dont forget to call 
            `taffy_TaffyResult_of_Vec_of_NodeId_delete()` 
        after use
    */
    taffy_TaffyResult_of_Vec_of_NodeId taffy_Taffy_children(
        const taffy_Taffy* self,

        taffy_NodeId parent
    );

    taffy_TaffyResult_of_void taffy_Taffy_set_style(
        taffy_Taffy* self,

        taffy_NodeId node, const taffy_Style* style
    );

    taffy_TaffyResult_of_Style_const_ref taffy_Taffy_style(
        const taffy_Taffy* self,

        taffy_NodeId node
    );

    taffy_TaffyResult_of_Layout_const_ref taffy_Taffy_layout(
        const taffy_Taffy* self,

        taffy_NodeId node
    );

    taffy_TaffyResult_of_void taffy_Taffy_mark_dirty(
        taffy_Taffy* self,

        taffy_NodeId node
    );

    taffy_TaffyResult_of_bool taffy_Taffy_dirty(
        const taffy_Taffy* self,

        taffy_NodeId node
    );

    taffy_TaffyResult_of_void taffy_Taffy_compute_layout(
        taffy_Taffy* self,

        taffy_NodeId node, const taffy_Size_of_AvailableSpace* available_space
    );

#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#endif /* TAFFY_CPP_C_H */
