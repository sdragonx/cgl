/*

 agg.hpp

 sdragonx 2020-01-18 17:10:08

*/
#ifndef AGG_HPP_20200118171008
#define AGG_HPP_20200118171008

#include <cgl/public.h>

//
// header
//

#include "agg/agg_alpha_mask_u8.h"
#include "agg/agg_arc.h"
#include "agg/agg_array.h"
#include "agg/agg_arrowhead.h"
#include "agg/agg_basics.h"
#include "agg/agg_bezier_arc.h"
#include "agg/agg_bitset_iterator.h"
#include "agg/agg_blur.h"
#include "agg/agg_bounding_rect.h"
#include "agg/agg_bspline.h"
#include "agg/agg_clip_liang_barsky.h"
#include "agg/agg_color_gray.h"
#include "agg/agg_color_rgba.h"
#include "agg/agg_config.h"
#include "agg/agg_conv_adaptor_vcgen.h"
#include "agg/agg_conv_adaptor_vpgen.h"
#include "agg/agg_conv_bspline.h"
#include "agg/agg_conv_clip_polygon.h"
#include "agg/agg_conv_clip_polyline.h"
#include "agg/agg_conv_close_polygon.h"
#include "agg/agg_conv_concat.h"
#include "agg/agg_conv_contour.h"
#include "agg/agg_conv_curve.h"
#include "agg/agg_conv_dash.h"
//#include "agg/agg_conv_gpc.h"
#include "agg/agg_conv_marker.h"
#include "agg/agg_conv_marker_adaptor.h"
#include "agg/agg_conv_segmentator.h"
#include "agg/agg_conv_shorten_path.h"
#include "agg/agg_conv_smooth_poly1.h"
#include "agg/agg_conv_stroke.h"
#include "agg/agg_conv_transform.h"
#include "agg/agg_conv_unclose_polygon.h"
#include "agg/agg_curves.h"
#include "agg/agg_dda_line.h"
#include "agg/agg_ellipse.h"
#include "agg/agg_ellipse_bresenham.h"
#include "agg/agg_embedded_raster_fonts.h"
#include "agg/agg_font_cache_manager.h"
#include "agg/agg_gamma_functions.h"
#include "agg/agg_gamma_lut.h"
#include "agg/agg_glyph_raster_bin.h"
#include "agg/agg_gradient_lut.h"
#include "agg/agg_gsv_text.h"
#include "agg/agg_image_accessors.h"
#include "agg/agg_image_filters.h"
#include "agg/agg_line_aa_basics.h"
#include "agg/agg_math.h"
#include "agg/agg_math_stroke.h"
#include "agg/agg_path_length.h"
#include "agg/agg_path_storage.h"
#include "agg/agg_path_storage_integer.h"
#include "agg/agg_pattern_filters_rgba.h"
#include "agg/agg_pixfmt_amask_adaptor.h"
#include "agg/agg_pixfmt_gray.h"
#include "agg/agg_pixfmt_rgb.h"
#include "agg/agg_pixfmt_rgba.h"
#include "agg/agg_pixfmt_rgb_packed.h"
#include "agg/agg_pixfmt_transposer.h"
#include "agg/agg_rasterizer_cells_aa.h"
#include "agg/agg_rasterizer_compound_aa.h"
#include "agg/agg_rasterizer_outline.h"
#include "agg/agg_rasterizer_outline_aa.h"
#include "agg/agg_rasterizer_scanline_aa.h"
#include "agg/agg_rasterizer_sl_clip.h"
#include "agg/agg_renderer_base.h"
#include "agg/agg_renderer_markers.h"        //画各种形状
#include "agg/agg_renderer_mclip.h"
#include "agg/agg_renderer_outline_aa.h"
#include "agg/agg_renderer_outline_image.h"
#include "agg/agg_renderer_primitives.h"        //高级绘图
#include "agg/agg_renderer_raster_text.h"
#include "agg/agg_renderer_scanline.h"
#include "agg/agg_rendering_buffer.h"
#include "agg/agg_rendering_buffer_dynarow.h"
#include "agg/agg_rounded_rect.h"
#include "agg/agg_scanline_bin.h"
#include "agg/agg_scanline_boolean_algebra.h"
#include "agg/agg_scanline_p.h"
#include "agg/agg_scanline_storage_aa.h"
#include "agg/agg_scanline_storage_bin.h"
#include "agg/agg_scanline_u.h"
#include "agg/agg_shorten_path.h"
#include "agg/agg_simul_eq.h"
#include "agg/agg_span_allocator.h"
#include "agg/agg_span_converter.h"
#include "agg/agg_span_gouraud.h"
#include "agg/agg_span_gouraud_gray.h"
#include "agg/agg_span_gouraud_rgba.h"
#include "agg/agg_span_gradient.h"
#include "agg/agg_span_gradient_alpha.h"
#include "agg/agg_span_image_filter.h"
#include "agg/agg_span_image_filter_gray.h"
#include "agg/agg_span_image_filter_rgb.h"
#include "agg/agg_span_image_filter_rgba.h"
#include "agg/agg_span_interpolator_adaptor.h"
#include "agg/agg_span_interpolator_linear.h"
#include "agg/agg_span_interpolator_persp.h"
#include "agg/agg_span_interpolator_trans.h"
#include "agg/agg_span_pattern_gray.h"
#include "agg/agg_span_pattern_rgb.h"
#include "agg/agg_span_pattern_rgba.h"
#include "agg/agg_span_solid.h"
#include "agg/agg_span_subdiv_adaptor.h"
#include "agg/agg_trans_affine.h"
#include "agg/agg_trans_bilinear.h"
#include "agg/agg_trans_double_path.h"
#include "agg/agg_trans_perspective.h"
#include "agg/agg_trans_single_path.h"
#include "agg/agg_trans_viewport.h"
#include "agg/agg_trans_warp_magnifier.h"
#include "agg/agg_vcgen_bspline.h"
#include "agg/agg_vcgen_contour.h"
#include "agg/agg_vcgen_dash.h"
#include "agg/agg_vcgen_markers_term.h"
#include "agg/agg_vcgen_smooth_poly1.h"
#include "agg/agg_vcgen_stroke.h"
#include "agg/agg_vcgen_vertex_sequence.h"
#include "agg/agg_vertex_sequence.h"
#include "agg/agg_vpgen_clip_polygon.h"
#include "agg/agg_vpgen_clip_polyline.h"
#include "agg/agg_vpgen_segmentator.h"
#include "agg/ctrl/agg_bezier_ctrl.h"
#include "agg/ctrl/agg_cbox_ctrl.h"
#include "agg/ctrl/agg_ctrl.h"
#include "agg/ctrl/agg_gamma_ctrl.h"
#include "agg/ctrl/agg_gamma_spline.h"
#include "agg/ctrl/agg_polygon_ctrl.h"
#include "agg/ctrl/agg_rbox_ctrl.h"
#include "agg/ctrl/agg_scale_ctrl.h"
#include "agg/ctrl/agg_slider_ctrl.h"
#include "agg/ctrl/agg_spline_ctrl.h"

//#include "agg/platform/agg_platform_support.h"
//#include "agg/platform/mac/agg_mac_pmap.h"
#ifdef CGL_PLATFORM_WINDOWS
#include "agg/platform/win32/agg_win32_bmp.h"
#endif

#include "agg/util/agg_color_conv.h"
#include "agg/util/agg_color_conv_rgb16.h"
#include "agg/util/agg_color_conv_rgb8.h"

//
// source
//

#include "agg/agg_arc.cpp"
#include "agg/agg_arrowhead.cpp"
#include "agg/agg_bezier_arc.cpp"
#include "agg/agg_bspline.cpp"
#include "agg/agg_curves.cpp"
#include "agg/agg_embedded_raster_fonts.cpp"
#include "agg/agg_gsv_text.cpp"
#include "agg/agg_image_filters.cpp"
#include "agg/agg_line_aa_basics.cpp"
#include "agg/agg_line_profile_aa.cpp"
#include "agg/agg_rounded_rect.cpp"
#include "agg/agg_sqrt_tables.cpp"
#include "agg/agg_trans_affine.cpp"
#include "agg/agg_trans_double_path.cpp"
#include "agg/agg_trans_single_path.cpp"
#include "agg/agg_trans_warp_magnifier.cpp"
#include "agg/agg_vcgen_bspline.cpp"
#include "agg/agg_vcgen_contour.cpp"
#include "agg/agg_vcgen_dash.cpp"
#include "agg/agg_vcgen_markers_term.cpp"
#include "agg/agg_vcgen_smooth_poly1.cpp"
#include "agg/agg_vcgen_stroke.cpp"
#include "agg/agg_vpgen_clip_polygon.cpp"
#include "agg/agg_vpgen_clip_polyline.cpp"
#include "agg/agg_vpgen_segmentator.cpp"
#include "agg/ctrl/agg_bezier_ctrl.cpp"
#include "agg/ctrl/agg_cbox_ctrl.cpp"
#include "agg/ctrl/agg_gamma_ctrl.cpp"
#include "agg/ctrl/agg_gamma_spline.cpp"
#include "agg/ctrl/agg_polygon_ctrl.cpp"
#include "agg/ctrl/agg_rbox_ctrl.cpp"
#include "agg/ctrl/agg_scale_ctrl.cpp"
#include "agg/ctrl/agg_slider_ctrl.cpp"
#include "agg/ctrl/agg_spline_ctrl.cpp"

//#include "agg/platform/AmigaOS/agg_platform_support.cpp"
//#include "agg/platform/BeOS/agg_platform_support.cpp"
//#include "agg/platform/mac/agg_mac_pmap.cpp"
//#include "agg/platform/mac/agg_platform_support.cpp"
//#include "agg/platform/sdl/agg_platform_support.cpp"
#if defined(CGL_PLATFORM_WINDOWS)
    //#include "agg/platform/win32/agg_platform_support.cpp"
    #include "agg/platform/win32/agg_win32_bmp.cpp"
#endif
//#include "agg/platform/X11/agg_platform_support.cpp"

#endif //AGG_HPP_20200118171008

#if 0
//agg object sample
class rectangle
{
private:
    double m_x1;
    double m_y1;
    double m_x2;
    double m_y2;
    int m_step;

public:
    rect() : m_x1(0.0), m_y1(0.0), m_x2(0), m_y2(0)
    {
        m_step = 0;
    }

    rect(double x1, double y1, double x2, double y2) : m_x1(x1), m_y1(y1), m_x2(x2), m_y2(y2)
    {
        m_step = 0;
    }

    void init(double x, double y, double rx, double ry)
    {

    }

    void approximation_scale(double scale)
    {
    }

    void rewind(unsigned path_id)
    {
        m_step = 0;
    }

    unsigned vertex(double* x, double* y)
    {
        ++m_step;
        switch(m_step){
        case 1:
            *x = m_x1;
            *y = m_y1;
            return agg::path_cmd_move_to;
        case 2:
            *x = m_x2;
            *y = m_y1;
            return agg::path_cmd_line_to;
        case 3:
            *x = m_x2;
            *y = m_y2;
            return agg::path_cmd_line_to;
        case 4:
            *x = m_x1;
            *y = m_y2;
            return agg::path_cmd_line_to;
        case 5:
            return agg::path_cmd_end_poly | agg::path_flags_close | agg::path_flags_ccw;
        default:
            return agg::path_cmd_stop;
        }
    }

};

#endif
