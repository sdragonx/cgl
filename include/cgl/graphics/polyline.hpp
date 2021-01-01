/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 polyline.hpp

 2019-12-23 08:04:25

*/
#ifndef POLYLINE_HPP_20191223080425
#define POLYLINE_HPP_20191223080425

#include <vector>
#include <iterator>
#include <cassert>
#include <cgl/vec.hpp>
#include <cgl/math.hpp>

namespace cgl{
namespace model{
//namespace {

//The threshold for mitered joints.
//If the joint's angle is smaller than this angle,
//the joint will be drawn beveled instead.
static float miterMinAngle = 0.349066; // ~20 degrees

//The minimum angle of a round joint's triangles.
static float roundMinAngle = 0.174533; // ~10 degrees

template<typename T>
bool intersection1(const line<T>& a, const line<T>& b, bool allowOverlap, vec2<T>& cross)
{
    if(math::line_intersect(a.a, b.b, b.a, b.b, cross)){
        if(allowOverlap){
            return math::is_on_line(a.a, a.b, cross) && math::is_on_line(b.a, b.b, cross);
        }
        else{
            return true;
        }
    }
    return false;
}

vec2f direction(const vec2f& a, const vec2f& b, bool normalized = true)
{
    vec2f vec = b - a;
    return normalized ? vec.normalize() : vec;
}

template<typename T>
bool intersection(const line<T> &a, const line<T> &b, bool infiniteLines, vec2<T>& cross)
{
    // calculate un-normalized direction vectors
    vec2f r = direction(a.a, a.b, false);
    vec2f s = direction(b.a, b.b, false);

    vec2f originDist = b.a - a.a;

    float uNumerator = math::cross(originDist, r);
    float denominator = math::cross(r, s);

    if (is_zero(denominator)) {
        // The lines are parallel
        return false;
    }

    // solve the intersection positions
    float u = uNumerator / denominator;
    float t = math::cross(originDist, s) / denominator;

    if (!infiniteLines && (t < 0 || t > 1 || u < 0 || u > 1)) {
        // the intersection lies outside of the line segments
        return false;
    }

    // calculate the intersection point
    // a.a + r * t;
    //return Vec2Maths::add(a.a, Vec2Maths::multiply(r, t));
    cross = a.a + r * t;
    return true;
}

template<typename T>
class polyline
{
public:
    typedef polyline<T> this_type;
    typedef T value_type;
    typedef vec2<T> point_type;
    typedef line<T> line_type;
    typedef std::vector<point_type> list_type;

    enum JointStyle {
        /**
         * Corners are drawn with sharp joints.
         * If the joint's outer angle is too large,
         * the joint is drawn as beveled instead,
         * to avoid the miter extending too far out.
         */
        //直角
        JointStyle_MITER,
        /**
         * Corners are flattened.
         */
        //拐角
        JointStyle_BEVEL,
        /**
         * Corners are rounded off.
         */
        //圆角
        JointStyle_ROUND
    };

    enum EndCapStyle {
        /**
         * Path ends are drawn flat,
         * and don't exceed the actual end point.
         */
        EndCapStyle_BUTT, // lol
        /**
         * Path ends are drawn flat,
         * but extended beyond the end point
         * by half the line thickness.
         */
        EndCapStyle_SQUARE,//bevel，没实现啊~~~
        /**
         * Path ends are rounded off.
         */
        EndCapStyle_ROUND,
        /**
         * Path ends are connected according to the JointStyle.
         * When using this EndCapStyle, don't specify the common start/end point twice,
         * as Polyline2D connects the first and last input point itself.
         */
        EndCapStyle_JOINT   //close_polygon
    };

    struct PolySegment
    {
        line_type center, edge1, edge2;

        PolySegment(const line_type& center, float thickness) : center(center)
        {
            // calculate the segment's outer edges by offsetting
            // the central line by the normal vector
            // multiplied with the thickness
            // center + center.normal() * thickness
            point_type v = center.b - center.a;
            v.normalize();
            v = point_type(-v.y, v.x);
            v *= thickness;
            edge1 = center + v;
            edge2 = center - v;
        }
    };

    typedef PolySegment segment_type;

    /**
     * Creates a vector of vertices describing a solid path through the input points.
     * @param points The points of the path.
     * @param thickness The path's thickness.
     * @param jointStyle The path's joint style.
     * @param endCapStyle The path's end cap style.
     * @param allowOverlap Whether to allow overlapping vertices.
     *                        This yields better results when dealing with paths
     *                        whose points have a distance smaller than the thickness,
     *                        but may introduce overlapping vertices,
     *                        which is undesirable when rendering transparent paths.
     * @return The vertices describing the path.
     * @tparam Vec2 The vector type to use for the vertices.
     *              Must have public non-const float fields "x" and "y".
     *              Must have a two-args constructor taking x and y values.
     *              See crushedpixel::Vec2 for a type that satisfies these requirements.
     * @tparam InputCollection The collection type of the input points.
     *                         Must contain elements of type Vec2.
     *                         Must expose size() and operator[] functions.
     */

    int create(
        list_type& ls,              //output triangles
        const list_type& points,    //input line strip
        float pen_width,            //pen width
        JointStyle jointStyle = JointStyle_MITER,
        EndCapStyle endCapStyle = EndCapStyle_BUTT,
        bool allowOverlap = false)
    {
        // operate on half the thickness to make our lives easier
        pen_width /= 2;

        if(points.size() == 1){
            switch(jointStyle){
            case JointStyle_MITER:
                pushFlatDot(ls, points[0], pen_width);
                break;
            case JointStyle_BEVEL:
                pushFlatDot(ls, points[0], pen_width);
                break;
            case JointStyle_ROUND:
                pushRoundDot(ls, points[0], pen_width);
                break;
            }
            return 0;
        }

        // create poly segments from the points
        std::vector<segment_type> segments;
        for (size_t i = 0; i + 1 < points.size(); i++) {
            const point_type &point1 = points[i];
            const point_type &point2 = points[i + 1];

            // to avoid division-by-zero errors,
            // only create a line segment for non-identical points
            if (point1 != point2) {
                //segments.emplace_back(LineSegment<Vec2>(point1, point2), thickness);
                segments.push_back(segment_type(line_type(point1, point2), pen_width));
            }
        }

        if(endCapStyle == EndCapStyle_JOINT) {//fixed joint style
            if(points.size() == 2){
                if(jointStyle == JointStyle_ROUND){
                    endCapStyle = EndCapStyle_SQUARE;
                }
                else{
                    endCapStyle = EndCapStyle_ROUND;
                }
            }
        }

        if(endCapStyle == EndCapStyle_JOINT) {
            // create a connecting segment from the last to the first point
            const point_type &point1 = points[points.size() - 1];
            const point_type &point2 = points[0];

            // to avoid division-by-zero errors,
            // only create a line segment for non-identical points
            if(point1 != point2) {
                //segments.emplace_back(LineSegment<point_type>(point1, point2), thickness);
                segments.push_back(segment_type(line_type(point1, point2), pen_width));
            }
        }

        if (segments.empty()) {
            // handle the case of insufficient input points
            //return vertices;
            return ls.size();
        }

        point_type nextStart1;
        point_type nextStart2;
        point_type start1;
        point_type start2;
        point_type end1;
        point_type end2;

        // calculate the path's global start and end points
        segment_type &firstSegment = segments[0];
        segment_type &lastSegment = segments[segments.size() - 1];

        point_type pathStart1 = firstSegment.edge1.a;
        point_type pathStart2 = firstSegment.edge2.a;
        point_type pathEnd1 = lastSegment.edge1.b;
        point_type pathEnd2 = lastSegment.edge2.b;

        // handle different end cap styles
        if (endCapStyle == EndCapStyle_SQUARE) {
            // extend the start/end points by half the thickness
            pathStart1 = pathStart1 - firstSegment.edge1.direction() * pen_width;
            pathStart2 = pathStart2 - firstSegment.edge2.direction() * pen_width;
            pathEnd1 = pathEnd1 + lastSegment.edge1.direction() * pen_width;
            pathEnd2 = pathEnd2 + lastSegment.edge2.direction() * pen_width;

        } else if (endCapStyle == EndCapStyle_ROUND) {
            // draw half circle end caps
            createTriangleFan(ls, firstSegment.center.a, firstSegment.center.a,
                              firstSegment.edge1.a, firstSegment.edge2.a, false);
            createTriangleFan(ls, lastSegment.center.b, lastSegment.center.b,
                              lastSegment.edge1.b, lastSegment.edge2.b, true);

        } else if (endCapStyle == EndCapStyle_JOINT) {
            // join the last (connecting) segment and the first segment
            createJoint(ls, lastSegment, firstSegment, jointStyle,
                        pathEnd1, pathEnd2, pathStart1, pathStart2, allowOverlap);
        }

        // generate mesh data for path segments
        for (size_t i = 0; i < segments.size(); i++) {
            segment_type &segment = segments[i];

            // calculate start
            if (i == 0) {
                // this is the first segment
                start1 = pathStart1;
                start2 = pathStart2;
            }

            if (i + 1 == segments.size()) {
                // this is the last segment
                end1 = pathEnd1;
                end2 = pathEnd2;

            } else {
                createJoint(ls, segment, segments[i + 1], jointStyle,
                            end1, end2, nextStart1, nextStart2, allowOverlap);
            }

            //线体
            // emit vertices
            ls.push_back(start1);
            ls.push_back(start2);
            ls.push_back(end1);

            ls.push_back(end1);
            ls.push_back(start2);
            ls.push_back(end2);

            start1 = nextStart1;
            start2 = nextStart2;
        }

        return 0;
    }

private:
    int createJoint(list_type& ls,
        const segment_type &segment1, const segment_type& segment2,
        JointStyle jointStyle,
        point_type &end1, point_type &end2,
        point_type &nextStart1, point_type &nextStart2,
        bool allowOverlap)
    {
        // calculate the angle between the two line segments
        point_type dir1 = segment1.center.direction();
        point_type dir2 = segment2.center.direction();

        float angle = get_angle(dir1, dir2);

        // wrap the angle around the 180° mark if it exceeds 90°
        // for minimum angle detection
        float wrappedAngle = angle;
        if (wrappedAngle > M_PI / 2) {
            wrappedAngle = M_PI - wrappedAngle;
        }

        if (jointStyle == JointStyle_MITER && wrappedAngle < miterMinAngle) {
            // the minimum angle for mitered joints wasn't exceeded.
            // to avoid the intersection point being extremely far out,
            // thus producing an enormous joint like a rasta on 4/20,
            // we render the joint beveled instead.
            jointStyle = JointStyle_BEVEL;
        }

        if (jointStyle == JointStyle_MITER) {
            // calculate each edge's intersection point
            // with the next segment's central line
            point_type cross1, cross2;
            bool sec1 = intersection(segment1.edge1, segment2.edge1, true, cross1);
            bool sec2 = intersection(segment1.edge2, segment2.edge2, true, cross2);

            end1 = sec1 ? cross1 : segment1.edge1.b;
            end2 = sec2 ? cross2 : segment1.edge2.b;

            nextStart1 = end1;
            nextStart2 = end2;

        } else {
            // joint style is either BEVEL or ROUND

            // find out which are the inner edges for this joint
            float x1 = dir1.x;
            float x2 = dir2.x;
            float y1 = dir1.y;
            float y2 = dir2.y;

            bool clockwise = x1 * y2 - x2 * y1 < 0;

            const line_type *inner1, *inner2, *outer1, *outer2;

            // as the normal vector is rotated counter-clockwise,
            // the first edge lies to the left
            // from the central line's perspective,
            // and the second one to the right.
            if (clockwise) {
                outer1 = &segment1.edge1;
                outer2 = &segment2.edge1;
                inner1 = &segment1.edge2;
                inner2 = &segment2.edge2;
            } else {
                outer1 = &segment1.edge2;
                outer2 = &segment2.edge2;
                inner1 = &segment1.edge1;
                inner2 = &segment2.edge1;
            }

            // calculate the intersection point of the inner edges
            point_type cross;
            bool innerSecOpt = intersection(*inner1, *inner2, allowOverlap, cross);

            point_type innerSec = innerSecOpt
                            ? cross ///*innerSecOpt
                            // for parallel lines, simply connect them directly
                            : inner1->b;

            // if there's no inner intersection, flip
            // the next start position for near-180° turns
            point_type innerStart;
            if (innerSecOpt) {
                innerStart = innerSec;
            } else if (angle > M_PI / 2) {
                innerStart = outer1->b;
            } else {
                innerStart = inner1->b;
            }

            if (clockwise) {
                end1 = outer1->b;
                end2 = innerSec;

                nextStart1 = outer2->a;
                nextStart2 = innerStart;

            } else {
                end1 = innerSec;
                end2 = outer1->b;

                nextStart1 = innerStart;
                nextStart2 = outer2->a;
            }

            // connect the intersection points according to the joint style

            if (jointStyle == JointStyle_BEVEL) {
                // simply connect the intersection points
                ls.push_back(outer1->b);
                ls.push_back(outer2->a);
                ls.push_back(innerSec);


            } else if (jointStyle == JointStyle_ROUND) {
                // draw a circle between the ends of the outer edges,
                // centered at the actual point
                // with half the line thickness as the radius
                createTriangleFan(ls, innerSec, segment1.center.b, outer1->b, outer2->a, clockwise);
            } else {
                assert(false);
            }
        }

        return ls.size();
    }

    /**
     * Creates a partial circle between two points.
     * The points must be equally far away from the origin.
     * @param vertices The vector to add vertices to.
     * @param connectTo The position to connect the triangles to.
     * @param origin The circle's origin.
     * @param start The circle's starting point.
     * @param end The circle's ending point.
     * @param clockwise Whether the circle's rotation is clockwise.
     */
    int createTriangleFan(std::vector< point_type >& ls, point_type connectTo, point_type origin,
                                            point_type start, point_type end, bool clockwise)
    {

        point_type point1 = start - origin;
        point_type point2 = end - origin;

        // calculate the angle between the two points
        float angle1 = std::atan2(point1.y, point1.x);
        float angle2 = std::atan2(point2.y, point2.x);

        // ensure the outer angle is calculated
        if (clockwise) {
            if (angle2 > angle1) {
                angle2 = angle2 - 2 * M_PI;
            }
        } else {
            if (angle1 > angle2) {
                angle1 = angle1 - 2 * M_PI;
            }
        }

        float jointAngle = angle2 - angle1;

        // calculate the amount of triangles to use for the joint
        //int numTriangles = std::max(1, (int) std::floor(std::abs(jointAngle) / roundMinAngle));
        int numTriangles = std::max(1, (int) std::floor(math::abs(jointAngle) / roundMinAngle));

        // calculate the angle of each triangle
        float triAngle = jointAngle / numTriangles;

        point_type startPoint = start;
        point_type endPoint;
        for (int t = 0; t < numTriangles; t++) {
            if (t + 1 == numTriangles) {
                // it's the last triangle - ensure it perfectly
                // connects to the next line
                endPoint = end;
            } else {
                float rot = (t + 1) * triAngle;

                // rotate the original point around the origin
                endPoint.x = std::cos(rot) * point1.x - std::sin(rot) * point1.y;
                endPoint.y = std::sin(rot) * point1.x + std::cos(rot) * point1.y;
//
//                endPoint.x = std::cos(rot) * point1.x - std::sin(rot) * point1.y;
//                endPoint.y = std::sin(rot) * point1.x + std::cos(rot) * point1.y;
//                endPoint = point1 - origin;
//                endPoint.rotate(rot * M_INV_RD);
//                endPoint = vec2f(10, 0);
//                endPoint.rotate(rot * M_INV_RD);


                // re-add the rotation origin to the target point
                endPoint = endPoint + origin;//Vec2Maths::add(endPoint, origin);
            }

            // emit the triangle
            push_triangle(ls, startPoint, endPoint, connectTo);

            startPoint = endPoint;
        }

        return 0;
    }

    int pushFlatDot(list_type& ls, point_type center, float pen_width)
    {
        point_type a(center.x - pen_width, center.y - pen_width);
        point_type b(center.x + pen_width, center.y - pen_width);
        point_type c(center.x + pen_width, center.y + pen_width);
        point_type d(center.x - pen_width, center.y + pen_width);
        push_triangle(ls, a, b, c);
        push_triangle(ls, a, c, d);
    }

    int pushRoundDot(list_type& ls, point_type center, float pen_width)
    {
        int numTriangles = M_PI * 2 / roundMinAngle;

        point_type first(pen_width, 0.0f);
        point_type second;
        first += center;
        for (int t = 1; t <= numTriangles + 1; t++) {
            float rot = t * roundMinAngle;
            second.x = std::cos(rot) * pen_width;
            second.y = std::sin(rot) * pen_width;
            second += center;
            push_triangle(ls, first, second, center);
            first = second;
        }

        return 0;
    }

private:
    //calculates the angle between two vectors.
    template<typename T>
    float get_angle(const vec2<T>& a, const vec2<T>& b)
    {
        return std::acos(a.dot(b) / (a.length() * b.length()));
//        return math::get_angle(a.x - b.x, a.y - b.y);
    }

    inline void push_triangle(list_type& ls, const point_type& v1, const point_type& v2, const point_type& v3)
    {
        ls.push_back(v1);
        ls.push_back(v2);
        ls.push_back(v3);
    }
};

//}//end namespace
}//end namespace model
}//end namespace cgl

#endif //POLYLINE_HPP_20191223080425

