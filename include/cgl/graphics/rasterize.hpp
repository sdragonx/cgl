/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 rasterize.hpp

 2020-02-16 01:29:07

 从stb_truetype里面扣的软光栅化函数
 效率不怎么样

*/
#ifndef RASTERIZE_HPP_20200216012907
#define RASTERIZE_HPP_20200216012907

#include <cgl/public.h>

namespace cgl{



//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit1.h"
#include <stdlib.h>
#include <math.h>
#include <vector>
#include <cgl/vec.hpp>
#include <cgl/graphics/pixel.hpp>
#include <cgl/memory/pool.hpp>
#include <cgl/debug.hpp>
#include <cgl/math.hpp>
using namespace std;
using namespace cgl;

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;

template<typename T>
void draw_polygon(TCanvas* dc, const std::vector< vec2<T> >& ls)
{
    if(ls.empty()){
        return ;
    }
    MoveToEx(dc->Handle, ls[0].x, ls[0].y, NULL);
    for(int i=1; i<ls.size(); ++i){
        LineTo(dc->Handle, ls[i].x, ls[i].y);
    }
}
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormCreate(TObject *Sender)
{
    Image1->Picture->Bitmap->PixelFormat = pf32bit;
    Image1->Picture->Bitmap->Width = 1024;
    Image1->Picture->Bitmap->Height = 1024;
    //Image1->Picture->Bitmap->Palette
    show_console(true);
    printf("start\n");

}

//---------------------------------------------------------------------------

typedef graphics::scanline<vec4ub> scanline4ub;

typedef std::vector<vec2f> soft_path;
typedef std::vector<soft_path> soft_paths;

class soft_rasterize
{
public:
    struct Edge
    {
        float x0, y0, x1, y1;
        int invert;

        bool operator<(const Edge& e){
            return y0 < e.y0;
        }
    };

    struct ActiveEdge
    {
        float fx;
        float fdx;
        float fdy;
        float direction;
        float sy;
        float ey;

        void set(Edge *e, int off_x, float start_point)
        {
            float dxdy = (e->x1 - e->x0) / (e->y1 - e->y0);
            fdx = dxdy;
            fdy = dxdy != 0.0f ? (1.0f/dxdy) : 0.0f;
            fx = e->x0 + dxdy * (start_point - e->y0);
            fx -= off_x;
            direction = e->invert ? 1.0f : -1.0f;
            sy = e->y0;
            ey = e->y1;
        }
    };

    typedef std::vector<Edge> Edges;
    typedef std::vector<ActiveEdge*> ActiveEdges;


private:
    Edges m_edges;
    std::vector<float> scanline_buf;
    std::vector<float> fill_buf;
    //内存池
    pool< ActiveEdge, 1024 > allocator;

public:
    vec4ub color;

public:
    soft_rasterize() : m_edges()
    {

    }

    void render(
        scanline4ub& image,
        const soft_path& paths,
        float scale_x,
        float scale_y,
        float shift_x,
        float shift_y,
        int off_x,
        int off_y,
        bool invert);

private:
    void sort_edges(Edges& edges)
    {
        edges_quicksort(&edges[0], edges.size());
        edges_ins_sort(&edges[0], edges.size());
    }

    void clear_buffer()
    {
        std::fill(scanline_buf.begin(), scanline_buf.end(), 0);
        std::fill(fill_buf.begin(), fill_buf.end(), 0);
    }

    void edges_ins_sort(Edge* p, int n);
    void edges_quicksort(Edge* p, int n);
    void handle_clipped_edge(float *scanline, int x, ActiveEdge *e, float x0, float y0, float x1, float y1);
    void fill_active_edges(float *scanline, float *scanline_fill, int len, ActiveEdges& edges, float y_top);
    void rasterizeAA(scanline4ub& image, int vsubsample, int off_x, int off_y);
    void bitmap_update(scanline4ub& image, int y);
};


void soft_rasterize::edges_ins_sort(Edge* p, int n)
{
   int i,j;
   for (i=1; i < n; ++i) {
      Edge t = p[i], *a = &t;
      j = i;
      while (j > 0) {
         Edge *b = &p[j-1];
         int c = *a < *b;//ok
         if (!c) break;
         p[j] = p[j-1];
         --j;
      }
      if (i != j)
         p[j] = t;
   }
}

void soft_rasterize::edges_quicksort(Edge* p, int n)
{
   /* threshold for transitioning to insertion sort */
   while (n > 12) {
      Edge t;
      int c01,c12,c,m,i,j;

      /* compute median of three */
      m = n >> 1;
      c01 = p[0] < p[m];// STBTT__COMPARE(&p[0],&p[m]);
      c12 = p[m] < p[n-1];// STBTT__COMPARE(&p[m],&p[n-1]);
      /* if 0 >= mid >= end, or 0 < mid < end, then use mid */
      if (c01 != c12) {
         /* otherwise, we'll need to swap something else to middle */
         int z;
         c = p[0] < p[n-1];// STBTT__COMPARE(&p[0],&p[n-1]);
         /* 0>mid && mid<n:  0>n => n; 0<n => 0 */
         /* 0<mid && mid>n:  0>n => 0; 0<n => n */
         z = (c == c12) ? 0 : n-1;
         t = p[z];
         p[z] = p[m];
         p[m] = t;
      }
      /* now p[m] is the median-of-three */
      /* swap it to the beginning so it won't move around */
      t = p[0];
      p[0] = p[m];
      p[m] = t;

      /* partition loop */
      i=1;
      j=n-1;
      for(;;) {
         /* handling of equality is crucial here */
         /* for sentinels & efficiency with duplicates */
         for (;;++i) {
            //if (!STBTT__COMPARE(&p[i], &p[0])) break;
            if (!(p[i] < p[0])) break;
         }
         for (;;--j) {
            //if (!STBTT__COMPARE(&p[0], &p[j])) break;
            if (!(p[0] < p[j])) break;
         }
         /* make sure we haven't crossed */
         if (i >= j) break;
         //t = p[i];
         //p[i] = p[j];
         //p[j] = t;
         std::swap(p[i], p[j]);

         ++i;
         --j;
      }
      /* recurse on smaller side, iterate on larger */
      if (j < (n-i)) {
         edges_quicksort(p,j);
         p = p+i;
         n = n-i;
      } else {
         edges_quicksort(p+i, n-i);
         n = j;
      }
   }
}

void soft_rasterize::render(scanline4ub& image, const soft_path& path, float scale_x, float scale_y, float shift_x, float shift_y, int off_x, int off_y, bool invert)
{
    float y_scale_inv = invert ? -scale_y : scale_y;

    int vsubsample = 1;
    // vsubsample should divide 255 evenly; otherwise we won't reach full opacity

    m_edges.reserve(path.size() + 1);

    Edge e;
    vec2f a, b;
    //for(size_t i=0; i<paths.size(); ++i){//处理所有路径
    //    const soft_path& path = paths[i];
        a = path.back();
        for(size_t j=0; j<path.size(); ++j){
            b = path[j];
            if(a.y == b.y){// skip the edge if horizontal
                continue;
            }

            e.invert = 0;
            if(invert ? a.y > b.y : a.y < b.y){
                e.invert = 1;
                std::swap(a, b);
            }
            //缩放平移
            e.x0 = b.x * scale_x + shift_x;
            e.y0 = (b.y * y_scale_inv + shift_y) * vsubsample;
            e.x1 = a.x * scale_x + shift_x;
            e.y1 = (a.y * y_scale_inv + shift_y) * vsubsample;
            m_edges.push_back(e);
            a = path[j];
        }
    //}

    // now sort the edges by their highest point (should snap to integer, and then by x)
    //   STBTT_sort(e, n, sizeof(e[0]), sl_edge_compare);
    sort_edges(m_edges);
    //std::sort(e.begin(), e.end());

    /*
    TCanvas* dc = Form1->PaintBox1->Canvas;
    dc->FillRect(Form1->PaintBox1->ClientRect);

    for(size_t i=0; i<m_edges.size(); ++i){
        if(i & 1){
            //dc->Pen->Width = 3;
            dc->Pen->Color = clRed;
        }
        else{
            dc->Pen->Width = 1;
            dc->Pen->Color = clBlue;
        }
        dc->MoveTo(m_edges[i].x0, m_edges[i].y0);
        dc->LineTo(m_edges[i].x1, m_edges[i].y1);

        vec2i p(m_edges[i].x0, m_edges[i].y0);
        p = p.midpoint(m_edges[i].x1, m_edges[i].y1);

        dc->TextOut(p.x, p.y, i);
    }
    */

    //渲染
    // now, traverse the scanlines and find the intersections on each scanline, use xor winding rule
    rasterizeAA(image, vsubsample, off_x, off_y);

    m_edges.clear();
}

// directly AA rasterize edges w/o supersampling
void soft_rasterize::rasterizeAA(scanline4ub& image, int vsubsample, int off_x, int off_y)
{
    //(bitmap->w * 2 + 1)
//   float scanline[512];
//   float scanline2[513];
    scanline_buf.resize(image.width());
    fill_buf.resize(image.width() + 1);

    //STBTT__NOTUSED(vsubsample);

    //scanline = scanline_data;
    //scanline2 = scanline + image->w;

    Edge* edge = &m_edges.front();
    m_edges[m_edges.size()].y0 = (float) (off_y + image.height()) + 1;//底边

    ActiveEdges active;

    int y = off_y;
    for(int j = 0; j < image.height(); ++j, ++y) {
        // find center of pixel for this scanline
        const float scan_y_top    = y + 0.0f;
        const float scan_y_bottom = y + 1.0f;

        //清空行
        clear_buffer();

        // update all active edges;
        // remove all active edges that terminate before the top of this scanline
        //清理扫描过的边
        for(size_t i = active.size() - 1; i < active.size(); --i){
            if(active[i]->ey <= scan_y_top){
                active.erase(active.begin() + i);
            }
        }

        // insert all edges that start before the bottom of this scanline
        //添加新边
        while (edge->y0 <= scan_y_bottom) {
            if (edge->y0 != edge->y1) {
                ActiveEdge *z = allocator.allocate();
                z->set(edge, off_x, scan_y_top);
                CGL_ASSERT(z);
                if (j == 0 && off_y != 0) {
                    if (z->ey < scan_y_top) {
                        // this can happen due to subpixel positioning and some kind of fp rounding error i think
                        z->ey = scan_y_top;
                    }
                }
                CGL_ASSERT(z->ey >= scan_y_top); // if we get really unlucky a tiny bit of an edge can be out of bounds
                active.push_back(z);
            }
            ++edge;
        }

        // now process all active edges
        //渲染边
        if (!active.empty()){
            fill_active_edges(&scanline_buf[0], &fill_buf[1], image.width(), active, scan_y_top);
        }

        //写入数据
        bitmap_update(image, j);

        // advance all the edges
        //处理未扫描完毕的边
        for(size_t i=0; i<active.size(); ++i){
            active[i]->fx += active[i]->fdx;
        }
    }
    allocator.dispose();
}


// the edge passed in here does not cross the vertical line at x or the vertical line at x+1
// (i.e. it has already been clipped to those)
//此处传入的边不穿过x处的垂直线或x+1处的垂直线
//（也就是说，它已经被剪辑到那些）
void soft_rasterize::handle_clipped_edge(float *scanline, int x, ActiveEdge *e, float x0, float y0, float x1, float y1)
{
   if (y0 == y1) return;

//   Form1->Image1->Canvas->Pen->Color = clRed;
//   Form1->Image1->Canvas->Rectangle(x0, y0, x1, y1);
//   Sleep(100);
    //printf("%f %f %f %f\n", x0, y0, x1, y1);

   CGL_ASSERT(y0 < y1);
   CGL_ASSERT(e->sy <= e->ey);
   if (y0 > e->ey) return;
   if (y1 < e->sy) return;
   if (y0 < e->sy) {
      x0 += (x1-x0) * (e->sy - y0) / (y1-y0);
      y0 = e->sy;
   }
   if (y1 > e->ey) {
      x1 += (x1-x0) * (e->ey - y1) / (y1-y0);
      y1 = e->ey;
   }

   if (x0 == x)
      CGL_ASSERT(x1 <= x+1);
   else if (x0 == x+1)
      CGL_ASSERT(x1 >= x);
   else if (x0 <= x)
      CGL_ASSERT(x1 <= x);
   else if (x0 >= x+1)
      CGL_ASSERT(x1 >= x+1);
   else
      CGL_ASSERT(x1 >= x && x1 <= x+1);

   if (x0 <= x && x1 <= x)
      scanline[x] += e->direction * (y1-y0);
   else if (x0 >= x+1 && x1 >= x+1)
      ;
   else {
      CGL_ASSERT(x0 >= x && x0 <= x+1 && x1 >= x && x1 <= x+1);
      scanline[x] += e->direction * (y1-y0) * (1-((x0-x)+(x1-x))/2); // coverage = 1 - average x position
   }
}

//填充边数据（一行）
void soft_rasterize::fill_active_edges(float *scanline, float *scanline_fill, int len, ActiveEdges& edges, float y_top)
{
   float y_bottom = y_top + 1;

   //while (edge) {
   for(int i=0; i<edges.size(); ++i){
      // brute force every pixel
      ActiveEdge* edge = edges[i];

      // compute intersection points with top & bottom
      CGL_ASSERT(edge->ey >= y_top);

      if (edge->fdx == 0) {
        printf("edge->fdx = %f\n", edge->fdx);
         float x0 = edge->fx;
         if (x0 < len) {
            if (x0 >= 0) {
               handle_clipped_edge(scanline,(int) x0, edge, x0,y_top, x0,y_bottom);
               handle_clipped_edge(scanline_fill-1,(int) x0+1, edge, x0,y_top, x0,y_bottom);//这里调用位置-1
            } else {
               handle_clipped_edge(scanline_fill-1, 0, edge, x0,y_top, x0,y_bottom);
            }
         }
      }
      else {
        //printf("draw_line edge->fdx = %f\n", edge->fdx);
         float x0 = edge->fx;
         float dx = edge->fdx;
         float xb = x0 + dx;
         float x_top, x_bottom;
         float sy0,sy1;
         float dy = edge->fdy;
         CGL_ASSERT(edge->sy <= y_bottom && edge->ey >= y_top);

         // compute endpoints of line segment clipped to this scanline (if the
         // line segment starts on this scanline. x0 is the intersection of the
         // line with y_top, but that may be off the line segment.
         if (edge->sy > y_top) {
            x_top = x0 + dx * (edge->sy - y_top);
            sy0 = edge->sy;
         } else {
            x_top = x0;
            sy0 = y_top;
         }
         if (edge->ey < y_bottom) {
            x_bottom = x0 + dx * (edge->ey - y_top);
            sy1 = edge->ey;
         } else {
            x_bottom = xb;
            sy1 = y_bottom;
         }

         if (x_top >= 0 && x_bottom >= 0 && x_top < len && x_bottom < len) {//剪裁？？
            // from here on, we don't have to range check x values

            if ((int) x_top == (int) x_bottom) {
               float height;
               // simple case, only spans one pixel
               int x = (int) x_top;
               height = sy1 - sy0;
               CGL_ASSERT(x >= 0 && x < len);
               scanline[x] += edge->direction * (1-((x_top - x) + (x_bottom-x))/2)  * height;
               scanline_fill[x] += edge->direction * height; // everything right of this pixel is filled
            } else {
               int x,x1,x2;
               float y_crossing, step, sign, area;
               // covers 2+ pixels
               if (x_top > x_bottom) {
                  // flip scanline vertically; signed area is the same
                  float t;
                  sy0 = y_bottom - (sy0 - y_top);
                  sy1 = y_bottom - (sy1 - y_top);
                  t = sy0, sy0 = sy1, sy1 = t;
                  t = x_bottom, x_bottom = x_top, x_top = t;
                  dx = -dx;
                  dy = -dy;
                  t = x0, x0 = xb, xb = t;
               }

               x1 = (int) x_top;
               x2 = (int) x_bottom;
               // compute intersection with y axis at x1+1
               y_crossing = (x1+1 - x0) * dy + y_top;

               sign = edge->direction;
               // area of the rectangle covered from y0..y_crossing
               area = sign * (y_crossing-sy0);
               // area of the triangle (x_top,y0), (x+1,y0), (x+1,y_crossing)
               scanline[x1] += area * (1-((x_top - x1)+(x1+1-x1))/2);

               step = sign * dy;
               for (x = x1+1; x < x2; ++x) {
                  scanline[x] += area + step/2;
                  area += step;
               }
               y_crossing += dy * (x2 - (x1+1));

               CGL_ASSERT(fabs(area) <= 1.01f);

               scanline[x2] += area + sign * (1-((x2-x2)+(x_bottom-x2))/2) * (sy1-y_crossing);

               scanline_fill[x2] += sign * (sy1-sy0);
            }
         } else {
            printf("clip ？？ edge->fdx = %f\n", edge->fdx);
            // if edge goes outside of box we're drawing, we require
            // clipping logic. since this does not match the intended use
            // of this library, we use a different, very slow brute
            // force implementation
            int x;
            for (x=0; x < len; ++x) {
                //continue;//sd
               // cases:
               //
               // there can be up to two intersections with the pixel. any intersection
               // with left or right edges can be handled by splitting into two (or three)
               // regions. intersections with top & bottom do not necessitate case-wise logic.
               //
               // the old way of doing this found the intersections with the left & right edges,
               // then used some simple logic to produce up to three segments in sorted order
               // from top-to-bottom. however, this had a problem: if an x edge was epsilon
               // across the x border, then the corresponding y position might not be distinct
               // from the other y segment, and it might ignored as an empty segment. to avoid
               // that, we need to explicitly produce segments based on x positions.

               // rename variables to clearly-defined pairs
               float y0 = y_top;
               float x1 = (float) (x);
               float x2 = (float) (x+1);
               float x3 = xb;
               float y3 = y_bottom;

               // x = e->x + e->dx * (y-y_top)
               // (y-y_top) = (x - e->x) / e->dx
               // y = (x - e->x) / e->dx + y_top
               float y1 = (x - x0) / dx + y_top;
               float y2 = (x+1 - x0) / dx + y_top;

               if (x0 < x1 && x3 > x2) {         // three segments descending down-right
                  handle_clipped_edge(scanline,x,edge, x0,y0, x1,y1);
                  handle_clipped_edge(scanline,x,edge, x1,y1, x2,y2);
                  handle_clipped_edge(scanline,x,edge, x2,y2, x3,y3);
               } else if (x3 < x1 && x0 > x2) {  // three segments descending down-left
                  handle_clipped_edge(scanline,x,edge, x0,y0, x2,y2);
                  handle_clipped_edge(scanline,x,edge, x2,y2, x1,y1);
                  handle_clipped_edge(scanline,x,edge, x1,y1, x3,y3);
               } else if (x0 < x1 && x3 > x1) {  // two segments across x, down-right
                  handle_clipped_edge(scanline,x,edge, x0,y0, x1,y1);
                  handle_clipped_edge(scanline,x,edge, x1,y1, x3,y3);
               } else if (x3 < x1 && x0 > x1) {  // two segments across x, down-left
                  handle_clipped_edge(scanline,x,edge, x0,y0, x1,y1);
                  handle_clipped_edge(scanline,x,edge, x1,y1, x3,y3);
               } else if (x0 < x2 && x3 > x2) {  // two segments across x+1, down-right
                  handle_clipped_edge(scanline,x,edge, x0,y0, x2,y2);
                  handle_clipped_edge(scanline,x,edge, x2,y2, x3,y3);
               } else if (x3 < x2 && x0 > x2) {  // two segments across x+1, down-left
                  handle_clipped_edge(scanline,x,edge, x0,y0, x2,y2);
                  handle_clipped_edge(scanline,x,edge, x2,y2, x3,y3);
               } else {  // one segment
                  handle_clipped_edge(scanline,x,edge, x0,y0, x3,y3);
               }
            }
         }
      }
      //edge= edge->next;
   }
}

//更新缓冲区数据到图像
void soft_rasterize::bitmap_update(scanline4ub& image, int y)
{
    float sum = 0;
    float k;
    int m;
    vec4ub c(color.blue, color.green, color.red, color.alpha);
    for (int x = 0; x < image.width(); ++x) {//内存没有越界
        sum += fill_buf[x];
        k = scanline_buf[x] + sum;
        k = fabs(k) * c.alpha + 0.5f;//round
        m = f_toint(k);
        m = math::clamp(m, 0, 255);
//        image[y][x] = vec4ub(m, m, m, m);//黑白图像
        if(m)image[y][x] = graphics::blend32(image[y][x], c, m);
    }
}

vector<vec2f> points;

void __fastcall TForm1::Button1Click(TObject *Sender)
{
    scanline4ub scanl;
    scanl.assign(Image1->Picture->Bitmap);

    scanl.fill(vec4ub(0, 255, 0, 0));

    soft_rasterize r;
    r.color = vec4ub(255, 0, 0, 64);
    //soft_paths paths;
    //paths.push_back(points);
    r.render(scanl, points, 1, 1, 0, 0, 0, 0, 0);

    for(int y=0; y<512; ++y)
    for(int x=0; x<512; ++x){
//        int n = buf[y * 512 + x];
//        scanl[y][x] = RGB(n, n, n);
    }
    Image1->Repaint();
}
//---------------------------------------------------------------------------


void __fastcall TForm1::Image1MouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y)
{
    points.push_back(vec2f(X, Y));
    //draw_polygon(Image1->Canvas, points);

    Button1Click(0);
}
//---------------------------------------------------------------------------




}//end namespace cgl

#endif //RASTERIZE_HPP_20200216012907
