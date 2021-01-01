/*
 * SGI FREE SOFTWARE LICENSE B (Version 2.0, Sept. 18, 2008)
 * Copyright (C) 1991-2000 Silicon Graphics, Inc. All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice including the dates of first publication and
 * either this permission notice or a reference to
 * http://oss.sgi.com/projects/FreeB/
 * shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * SILICON GRAPHICS, INC. BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
 * OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * Except as contained in this notice, the name of Silicon Graphics, Inc.
 * shall not be used in advertising or otherwise to promote the sale, use or
 * other dealings in this Software without prior written authorization from
 * Silicon Graphics, Inc.
 */
/*
** Author: Eric Veach, July 1994.
**
*/

#ifndef LIBTESS_GEOMETRY_HPP
#define LIBTESS_GEOMETRY_HPP

#include "mesh.hpp"

namespace libtess{

inline int VertexIsCCW( Vertex *u, Vertex *v, Vertex *w )
{
    /* For almost-degenerate situations, the results are not reliable.
     * Unless the floating-point arithmetic can be performed without
     * rounding errors, *any* implementation will give incorrect results
     * on some degenerate inputs, so the client must have some way to
     * handle this situation.
     */
    return (u->s*(v->t - w->t) + v->s*(w->t - u->t) + w->s*(u->t - v->t)) >= 0;
}

inline bool VertexEqual(Vertex* u, Vertex* v)
{
//    return ((u)->s == (v)->s && (u)->t == (v)->t);
    return IsEqual(u->s, v->s) && IsEqual(u->t, v->t);
}

inline bool VertexLessEqual(Vertex* u, Vertex* v)
{
//    return ( u->s < v->s ) || //((u)->s == (v)->s && (u)->t <= (v)->t));
//        ( IsEqual( u->s, v->s ) && ( u->t < v->t || IsEqual( u->t, v->t ) ) );

    return ( u->s < v->s ) || //((u)->s == (v)->s && (u)->t <= (v)->t));
        ( IsEqual( u->s, v->s ) && ( u->t < v->t || IsEqual( u->t, v->t ) ) );

}

int CountFaceVertices( Face* face )
{
    HalfEdge *e = face->edge;
    int n = 0;
    do{
        n++;
        e = e->Lnext;
    }while (e != face->edge);
    return n;
}

inline bool EdgeGoesLeft(HalfEdge* e)
{
    return VertexLessEqual( (e)->mirror->vertex, (e)->vertex );
}

inline bool EdgeGoesRight(HalfEdge* e)
{
    return VertexLessEqual( (e)->vertex, (e)->mirror->vertex );
}

inline bool EdgeIsInternal(HalfEdge* e)
{
    return e->mirror->Lface && e->mirror->Lface->inside;
}

/* Versions of VertLeq, EdgeSign, EdgeEval with s and t transposed. */
inline bool VertexTransLEQ(Vertex* u, Vertex* v)
{
     return (((u)->t < (v)->t) || ((u)->t == (v)->t && (u)->s <= (v)->s));
}

//Manhattan
inline Float VertexDistance(Vertex* u, Vertex* v)
{
    return fabs(u->s - v->s) + fabs(u->t - v->t);
}

Float EdgeEval( Vertex *u, Vertex *v, Vertex *w )
{
    /* Given three vertices u,v,w such that VertLeq(u,v) && VertLeq(v,w),
     * evaluates the t-coord of the edge uw at the s-coord of the vertex v.
     * Returns v->t - (uw)(v->s), ie. the signed distance from uw to v.
     * If uw is vertical (and thus passes thru v), the result is zero.
     *
     * The calculation is extremely accurate and stable, even when v
     * is very close to u or w.  In particular if we set v->t = 0 and
     * let r be the negated result (this evaluates (uw)(v->s)), then
     * r is guaranteed to satisfy MIN(u->t,w->t) <= r <= MAX(u->t,w->t).

     * ������������u��v��w��ʹ VertLeq(u,v) && VertLeq(v,w)
     * �ڶ���v��s���괦�����uw��t���ꡣ
     * ����v->t - (uw)(v->s)������uw��v���з��ž��롣
     * ������Ǵ�ֱ�ģ����ͨ��v����������㡣
     *
     * ��ʹ��v�ǳ��ӽ�u��wʱ������Ҳ�ǳ���ȷ���ȶ���
     * �ر��������������v->t=0����rΪ�󷴽��������㣨uw����v->s������
     * ��r��֤���� MIN��u->t��w->t��<= r <= MAX��u->t��w->t����
     */
    Float gapL, gapR;

    assert( VertexLessEqual( u, v ) && VertexLessEqual( v, w ));

    gapL = v->s - u->s;
    gapR = w->s - v->s;

    if( gapL + gapR > 0 ) {
        if( gapL < gapR ) {
            return (v->t - u->t) + (u->t - w->t) * (gapL / (gapL + gapR));
        } else {
            return (v->t - w->t) + (w->t - u->t) * (gapR / (gapL + gapR));
        }
    }
    /* vertical line */
    return 0;
}

Float EdgeSign( Vertex *u, Vertex *v, Vertex *w )
{
    /* Returns a number whose sign matches EdgeEval(u,v,w) but which
     * is cheaper to evaluate.  Returns > 0, == 0 , or < 0
     * as v is above, on, or below the edge uw.
     *
     * ����һ���������EdgeEval(u,v,w)ƥ�䵫����ɱ��ϵ͵����֡�
     * ��v�ڱ�Եuw���Ϸ����Ϸ����·�ʱ������>0��==0��<0��
     */
    Float gapL, gapR;

    assert( VertexLessEqual( u, v ) && VertexLessEqual( v, w ) );

    gapL = v->s - u->s;
    gapR = w->s - v->s;

    if( gapL + gapR > 0 ) {
        return (v->t - w->t) * gapL + (v->t - u->t) * gapR;
    }
    /* vertical line */
    return 0;
}


/***********************************************************************
 * Define versions of EdgeSign, EdgeEval with s and t transposed.
 */

Float EdgeTransEval( Vertex *u, Vertex *v, Vertex *w )
{
    /* Given three vertices u,v,w such that VertexTransLEQ(u,v) && VertexTransLEQ(v,w),
     * evaluates the t-coord of the edge uw at the s-coord of the vertex v.
     * Returns v->s - (uw)(v->t), ie. the signed distance from uw to v.
     * If uw is vertical (and thus passes thru v), the result is zero.
     *
     * The calculation is extremely accurate and stable, even when v
     * is very close to u or w.  In particular if we set v->s = 0 and
     * let r be the negated result (this evaluates (uw)(v->t)), then
     * r is guaranteed to satisfy MIN(u->s,w->s) <= r <= MAX(u->s,w->s).
     */
    Float gapL, gapR;

    assert( VertexTransLEQ( u, v ) && VertexTransLEQ( v, w ));

    gapL = v->t - u->t;
    gapR = w->t - v->t;

    if( gapL + gapR > 0 ) {
        if( gapL < gapR ) {
            return (v->s - u->s) + (u->s - w->s) * (gapL / (gapL + gapR));
        } else {
            return (v->s - w->s) + (w->s - u->s) * (gapR / (gapL + gapR));
        }
    }
    /* vertical line */
    return 0;
}

Float EdgeTransSign( Vertex *u, Vertex *v, Vertex *w )
{
    /* Returns a number whose sign matches TransEval(u,v,w) but which
     * is cheaper to evaluate.  Returns > 0, == 0 , or < 0
     * as v is above, on, or below the edge uw.
     */
    Float gapL, gapR;

    assert( VertexTransLEQ( u, v ) && VertexTransLEQ( v, w ));

    gapL = v->t - u->t;
    gapR = w->t - v->t;

    if( gapL + gapR > 0 ) {
        return (v->s - w->s) * gapL + (v->s - u->s) * gapR;
    }
    /* vertical line */
    return 0;
}

/* Given parameters a,x,b,y returns the value (b*x+a*y)/(a+b),
 * or (x+y)/2 if a==b==0.  It requires that a,b >= 0, and enforces
 * this in the rare case that one argument is slightly negative.
 * The implementation is extremely stable numerically.
 * In particular it guarantees that the result r satisfies
 * MIN(x,y) <= r <= MAX(x,y), and the results are very accurate
 * even when a and b differ greatly in magnitude.
 *
 * �������� a��x��b��y
 * ����(b*x+a*y)/(a+b)�����a==b==0���򷵻�(x+y)/2��
 * ��Ҫ��a��b>=0������һ��������΢Ϊ���ĺ��������ǿ��ִ�С�
 * ����ֵ��ʵ���Ƿǳ��ȶ��ġ�
 * �ر�أ�����֤�˽��r����MIN(x,y)<=r<=MAX(x,y)��
 * ���Ҽ�ʹa��b�������������ܴ󣬽��Ҳ�ǳ���ȷ��
 */

//#define RealInterpolate(a,x,b,y)
//    (a = (a < 0) ? 0 : a, b = (b < 0) ? 0 : b,
//    ((a <= b) ? ((b == 0) ? ((x+y) / 2)
//    : (x + (y-x) * (a/(a+b))))
//    : (y + (x-y) * (b/(a+b)))))

inline Float Interpolate( Float a, Float x, Float b, Float y )
{
    a = (a < 0) ? 0 : a;
    b = (b < 0) ? 0 : b;
    return (a <= b) ? ((b == 0) ? ((x+y) / 2) : (x + (y-x) * (a/(a+b)))) : (y + (x-y) * (b/(a+b)));
}

/* Given edges (o1,d1) and (o2,d2), compute their point of intersection.
 * The computed point is guaranteed to lie in the intersection of the
 * bounding rectangles defined by each edge.
 * ������ (o1,d1) �� (o2,d2)���������ǵĽ��㡣
 * ����㱣֤λ����ÿ���߶���ı߽���εĽ��㴦��
 */
void EdgeIntersect( Vertex *o1, Vertex *d1, Vertex *o2, Vertex *d2, Vertex *v )
{
    Float z1, z2;

    /* This is certainly not the most efficient way to find the intersection
     * of two line segments, but it is very numerically stable.
     *
     * Strategy: find the two middle vertices in the VertLeq ordering,
     * and interpolate the intersection s-value from these.  Then repeat
     * using the VertexTransLEQ ordering to find the intersection t-value.
     *
     * �⵱Ȼ�����ҵ������߶��ཻ������Ч��������������ֵ�Ϸǳ��ȶ���
     * ���ԣ��ڶ����������ҵ������м䶥�㣬
     * �����в��뽻��sֵ��Ȼ���ظ�ʹ��VertexTransLEQ˳�������ҽ���tֵ��
     */

    if( ! VertexLessEqual( o1, d1 )) { std::swap( o1, d1 ); }
    if( ! VertexLessEqual( o2, d2 )) { std::swap( o2, d2 ); }
    if( ! VertexLessEqual( o1, o2 )) { std::swap( o1, o2 ); std::swap( d1, d2 ); }

    if( ! VertexLessEqual( o2, d1 )) {
        /* Technically, no intersection -- do our best */
        v->s = (o2->s + d1->s) / 2;
    } else if( VertexLessEqual( d1, d2 )) {
        /* Interpolate between o2 and d1 */
        z1 = EdgeEval( o1, o2, d1 );
        z2 = EdgeEval( o2, d1, d2 );
        if( z1+z2 < 0 ) { z1 = -z1; z2 = -z2; }
        v->s = Interpolate( z1, o2->s, z2, d1->s );
    } else {
        /* Interpolate between o2 and d2 */
        z1 = EdgeSign( o1, o2, d1 );
        z2 = -EdgeSign( o1, d2, d1 );
        if( z1+z2 < 0 ) { z1 = -z1; z2 = -z2; }
        v->s = Interpolate( z1, o2->s, z2, d2->s );
    }

    /* Now repeat the process for t */

    if( ! VertexTransLEQ( o1, d1 )) { std::swap( o1, d1 ); }
    if( ! VertexTransLEQ( o2, d2 )) { std::swap( o2, d2 ); }
    if( ! VertexTransLEQ( o1, o2 )) { std::swap( o1, o2 ); std::swap( d1, d2 ); }

    if( ! VertexTransLEQ( o2, d1 )) {
        /* Technically, no intersection -- do our best */
        v->t = (o2->t + d1->t) / 2;
    } else if( VertexTransLEQ( d1, d2 )) {
        /* Interpolate between o2 and d1 */
        z1 = EdgeTransEval( o1, o2, d1 );
        z2 = EdgeTransEval( o2, d1, d2 );
        if( z1+z2 < 0 ) { z1 = -z1; z2 = -z2; }
        v->t = Interpolate( z1, o2->t, z2, d1->t );
    } else {
        /* Interpolate between o2 and d2 */
        z1 = EdgeTransSign( o1, o2, d1 );
        z2 = -EdgeTransSign( o1, d2, d1 );
        if( z1+z2 < 0 ) { z1 = -z1; z2 = -z2; }
        v->t = Interpolate( z1, o2->t, z2, d2->t );
    }
}

//libtess2
Float inCircle( Vertex *v, Vertex *v0, Vertex *v1, Vertex *v2 )
{
    Float adx, ady, bdx, bdy, cdx, cdy;
    Float abdet, bcdet, cadet;
    Float alift, blift, clift;

    adx = v0->s - v->s;
    ady = v0->t - v->t;
    bdx = v1->s - v->s;
    bdy = v1->t - v->t;
    cdx = v2->s - v->s;
    cdy = v2->t - v->t;

    abdet = adx * bdy - bdx * ady;
    bcdet = bdx * cdy - cdx * bdy;
    cadet = cdx * ady - adx * cdy;

    alift = adx * adx + ady * ady;
    blift = bdx * bdx + bdy * bdy;
    clift = cdx * cdx + cdy * cdy;

    return alift * bcdet + blift * cadet + clift * abdet;
}

/*
    Returns 1 is edge is locally delaunay
 */
int EdgeIsLocallyDelaunay( HalfEdge *e )
{
    return inCircle(e->mirror->Lnext->Lnext->vertex, e->Lnext->vertex, e->Lnext->Lnext->vertex, e->vertex) < 0;
}

}// end namespace libtess

#endif// LIBTESS_GEOMETRY_HPP