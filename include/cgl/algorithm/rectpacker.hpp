/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 maxrects.hpp

 2017-02-04 18:55:44

 thanks for @author Jukka Jyl鋘ki

 class rectpages 自动添加分页

 2018/7/29 11:18 模板化

 2018-1-13 还没有加入纹理大小过滤，对于大于一定尺寸的矩形进行筛选，剔除
 添加纹理图块的时候，矩形大小选择+1或者+2的边界，然后再添加矩形

 pagerect.hpp

*/
#ifndef MAXRECTS_HPP_20170204185544
#define MAXRECTS_HPP_20170204185544

#pragma once

#include <cgl/public.h>
#include <cgl/vec.hpp>
#include <vector>

namespace cgl{
namespace math{

template<typename T>
class maxrects
{
public:
    // Specifies the different heuristic rules that can be used when deciding where to place a new rectangle.
    enum METHOD
    {
        BEST_SHORT_SIDE_FIT,    //< -BSSF: Positions the rectangle against the short side of a free rectangle into which it fits the best.
        BEST_LONG_SIDE_FIT,     //< -BLSF: Positions the rectangle against the long side of a free rectangle into which it fits the best.
        BEST_AREA_FIT,          //< -BAF: Positions the rectangle into the smallest free rect into which it fits.
        BOTTOM_LEFT,            //< -BL: Does the Tetris placement.
        CONTACT_POINT           //< -CP: Choosest the placement where the rectangle touches other rects as much as possible.
    };
    const static size_t npos = -1;
private:
    T binWidth;
    T binHeight;

    bool allowRotate;
    std::vector< vec4<T> > usedRectangles;
    std::vector< vec4<T> > freeRectangles;

public:
    maxrects() : binWidth( 0 ), binHeight( 0 ), allowRotate( false ) { /*void*/ }
    maxrects( T width, T height, bool rotate = false )
    {
        init( width, height, rotate );
    }

    //(Re)initializes the packer to an empty bin of width x height units. Call whenever
    //you need to restart with a new bin.
    void init( T width, T height, bool rotate = false );

    T width()  const { return binWidth; }
    T height() const { return binHeight; }

    /// Computes the placement score for placing the given rectangle with the given method.
    /// @param score1 [out] The primary placement score will be outputted here.
    /// @param score2 [out] The secondary placement score will be outputted here. This isu sed to break ties.
    /// @return This struct identifies where the rectangle would be placed if it were placed.
    vec4<T> score_rect(T width, T height, METHOD method, int &score1, int &score2) const;

    // insert a new rectangle, if successed return index of usedRectangles
    // failed return npos
    size_t insert( T width, T height, METHOD method );

    // Computes the ratio of used surface area to the total bin area.
    float occupancy() const;

    const vec4<T>& operator[] (size_t index) const
    {
        return usedRectangles[index];
    }

    size_t size() const
    {
        return usedRectangles.size();
    }
private:
    vec4<T> findBottomLeft( T width, T height, int &bestY, int &bestX ) const;
    vec4<T> findBestShortSideFit( T width, T height, int &bestShortSideFit, int &bestLongSideFit ) const;
    vec4<T> findBestLongSideFit( T width, T height, int &bestShortSideFit, int &bestLongSideFit ) const;
    vec4<T> findBestAreaFit( T width, T height, int &bestAreaFit, int &bestShortSideFit ) const;
    vec4<T> findContactPoint( T width, T height, int &contactScore ) const;

    /// Returns 0 if the two intervals i1 and i2 are disjoint, or the length of their overlap otherwise.
    int CommonIntervalLength(int i1start, int i1end, int i2start, int i2end)const
    {
        if (i1end < i2start || i2end < i1start)
            return 0;
        return std::min(i1end, i2end) - std::max(i1start, i2start);
    }

    /// Computes the placement score for the -CP variant.
    int ContactPointScoreNode(T x, T y, T width, T height) const;

    /// @return True if the free node was split.
    bool SplitFreeNode(vec4<T> freeNode, const vec4<T> &usedNode);

    /// Goes through the free rectangle list and removes any redundant entries.
    void PruneFreeList();

    bool IsContainedIn(const vec4<T>& a, const vec4<T>& b)
    {
        return a.x >= b.x && a.y >= b.y
            && a.x+a.width <= b.x+b.width 
            && a.y+a.height <= b.y+b.height;
    }
};

template<typename T>
void maxrects<T>::init( T width, T height, bool rotate )
{
    binWidth    = width;
    binHeight    = height;
    allowRotate    = rotate;

    usedRectangles.clear();
    freeRectangles.clear();

    freeRectangles.push_back(vec4<T>( 0, 0, width, height ));
}

template<typename T>
float maxrects<T>::occupancy() const
{
    float packed = 0;
    for ( size_t i = 0; i < usedRectangles.size(); ++i ){
        packed += float(usedRectangles[i].width) * float(usedRectangles[i].height);
    }

    return packed / (float(binWidth) * float(binHeight));
}

template<typename T>
vec4<T> maxrects<T>::score_rect(T width, T height, METHOD method, int &score1, int &score2) const
{
    vec4<T> node;
    score1 = std::numeric_limits<int>::max();
    score2 = std::numeric_limits<int>::max();
    switch(method)
    {
    case BEST_SHORT_SIDE_FIT:
        node = findBestShortSideFit( width, height, score1, score2 );
        break;
    case BEST_LONG_SIDE_FIT:
        node = findBestLongSideFit( width, height, score2, score1 );
        break;
    case BEST_AREA_FIT:
        node = findBestAreaFit( width, height, score1, score2 );
        break;
    case BOTTOM_LEFT:
        node = findBottomLeft( width, height, score1, score2 );
        break;
    case CONTACT_POINT:
        node = findContactPoint( width, height, score1 );
        // Reverse since we are minimizing, but for contact point score bigger is better.
        score1 = -score1;
        break;
    default:
        break;//unpacked
    }

    // Cannot fit the current rectangle.
    if (node.height == 0)    {
        score1 = score2 = std::numeric_limits<int>::max();
    }

    return node;
}

template<typename T>
size_t maxrects<T>::insert(T width, T height, METHOD method)
{
    vec4<T> node;

    // Unused in this function. We don't need to know the score after finding the position.
    int score1 =  std::numeric_limits<T>::max();
    int score2 =  std::numeric_limits<T>::max();
    switch(method){
    case BEST_SHORT_SIDE_FIT:
        node = findBestShortSideFit( width, height, score1, score2 );
        break;
    case BEST_LONG_SIDE_FIT:
        node = findBestLongSideFit( width, height, score2, score1 );
        break;
    case BEST_AREA_FIT:
        node = findBestAreaFit( width, height, score1, score2 );
        break;
    case BOTTOM_LEFT:
        node = findBottomLeft( width, height, score1, score2 );
        break;
    case CONTACT_POINT:
        node = findContactPoint( width, height, score1 );
        break;
    default:
        break;//unpacked
    }
        
    if (node.height == 0){
        return npos;//unpacked
    }

    size_t numRectanglesToProcess = freeRectangles.size();
    for(size_t i = 0; i < numRectanglesToProcess; ++i){
        if (SplitFreeNode(freeRectangles[i], node)){
            freeRectangles.erase(freeRectangles.begin() + i);
            --i;
            --numRectanglesToProcess;
        }
    }

    PruneFreeList();

    usedRectangles.push_back(node);
    return usedRectangles.size() - 1;
}

template<typename T>
vec4<T> maxrects<T>::findBottomLeft(T width, T height, int &bestY, int &bestX) const
{
    vec4<T> bestNode;

    bestY =  std::numeric_limits<T>::max();
    bestX =  std::numeric_limits<T>::max();

    for(size_t i = 0; i < freeRectangles.size(); ++i)
    {
        // Try to place the rectangle in upright (non-flipped) orientation.
        if (freeRectangles[i].width >= width && freeRectangles[i].height >= height)
        {
            int topSideY = freeRectangles[i].y + height;
            if (topSideY < bestY || (topSideY == bestY && freeRectangles[i].x < bestX))
            {
                bestNode.x = freeRectangles[i].x;
                bestNode.y = freeRectangles[i].y;
                bestNode.width = width;
                bestNode.height = height;
                bestY = topSideY;
                bestX = freeRectangles[i].x;
            }
        }
        if (allowRotate && freeRectangles[i].width >= height && freeRectangles[i].height >= width)
        {
            int topSideY = freeRectangles[i].y + width;
            if (topSideY < bestY || (topSideY == bestY && freeRectangles[i].x < bestX))
            {
                bestNode.x = freeRectangles[i].x;
                bestNode.y = freeRectangles[i].y;
                bestNode.width = height;
                bestNode.height = width;
                bestY = topSideY;
                bestX = freeRectangles[i].x;
            }
        }
    }
    return bestNode;
}

template<typename T>
vec4<T> maxrects<T>::findBestShortSideFit(T width, T height, int &bestShortSideFit, int &bestLongSideFit) const
{
    vec4<T> bestNode;

    bestShortSideFit =  std::numeric_limits<T>::max();
    bestLongSideFit =  std::numeric_limits<T>::max();

    for(size_t i = 0; i < freeRectangles.size(); ++i)
    {
        // Try to place the rectangle in upright (non-flipped) orientation.
        if (freeRectangles[i].width >= width && freeRectangles[i].height >= height)
        {
            int leftoverHoriz = abs(freeRectangles[i].width - width);
            int leftoverVert = abs(freeRectangles[i].height - height);
            int shortSideFit = std::min(leftoverHoriz, leftoverVert);
            int longSideFit = std::max(leftoverHoriz, leftoverVert);

            if (shortSideFit < bestShortSideFit || (shortSideFit == bestShortSideFit && longSideFit < bestLongSideFit))
            {
                bestNode.x = freeRectangles[i].x;
                bestNode.y = freeRectangles[i].y;
                bestNode.width = width;
                bestNode.height = height;
                bestShortSideFit = shortSideFit;
                bestLongSideFit = longSideFit;
            }
        }

        if (allowRotate && freeRectangles[i].width >= height && freeRectangles[i].height >= width)
        {
            int flippedLeftoverHoriz = abs(freeRectangles[i].width - height);
            int flippedLeftoverVert = abs(freeRectangles[i].height - width);
            int flippedShortSideFit = std::min(flippedLeftoverHoriz, flippedLeftoverVert);
            int flippedLongSideFit = std::max(flippedLeftoverHoriz, flippedLeftoverVert);

            if (flippedShortSideFit < bestShortSideFit || (flippedShortSideFit == bestShortSideFit && flippedLongSideFit < bestLongSideFit))
            {
                bestNode.x = freeRectangles[i].x;
                bestNode.y = freeRectangles[i].y;
                bestNode.width = height;
                bestNode.height = width;
                bestShortSideFit = flippedShortSideFit;
                bestLongSideFit = flippedLongSideFit;
            }
        }
    }
    return bestNode;
}

template<typename T>
vec4<T> maxrects<T>::findBestLongSideFit(T width, T height,    int &bestShortSideFit, int &bestLongSideFit) const
{
    vec4<T> bestNode;

    bestShortSideFit =  std::numeric_limits<T>::max();
    bestLongSideFit =  std::numeric_limits<T>::max();

    for(size_t i = 0; i < freeRectangles.size(); ++i)
    {
        // Try to place the rectangle in upright (non-flipped) orientation.
        if (freeRectangles[i].width >= width && freeRectangles[i].height >= height)
        {
            int leftoverHoriz = abs(freeRectangles[i].width - width);
            int leftoverVert = abs(freeRectangles[i].height - height);
            int shortSideFit = std::min(leftoverHoriz, leftoverVert);
            int longSideFit = std::max(leftoverHoriz, leftoverVert);

            if (longSideFit < bestLongSideFit || (longSideFit == bestLongSideFit && shortSideFit < bestShortSideFit))
            {
                bestNode.x = freeRectangles[i].x;
                bestNode.y = freeRectangles[i].y;
                bestNode.width = width;
                bestNode.height = height;
                bestShortSideFit = shortSideFit;
                bestLongSideFit = longSideFit;
            }
        }

        if (allowRotate && freeRectangles[i].width >= height && freeRectangles[i].height >= width)
        {
            int leftoverHoriz = abs(freeRectangles[i].width - height);
            int leftoverVert = abs(freeRectangles[i].height - width);
            int shortSideFit = std::min(leftoverHoriz, leftoverVert);
            int longSideFit = std::max(leftoverHoriz, leftoverVert);

            if (longSideFit < bestLongSideFit || (longSideFit == bestLongSideFit && shortSideFit < bestShortSideFit))
            {
                bestNode.x = freeRectangles[i].x;
                bestNode.y = freeRectangles[i].y;
                bestNode.width = height;
                bestNode.height = width;
                bestShortSideFit = shortSideFit;
                bestLongSideFit = longSideFit;
            }
        }
    }
    return bestNode;
}

template<typename T>
vec4<T> maxrects<T>::findBestAreaFit(T width, T height, int &bestAreaFit, int &bestShortSideFit) const
{
    vec4<T> bestNode;

    bestAreaFit =  std::numeric_limits<T>::max();
    bestShortSideFit =  std::numeric_limits<T>::max();

    for(size_t i = 0; i < freeRectangles.size(); ++i)
    {
        int areaFit = freeRectangles[i].width * freeRectangles[i].height - width * height;

        // Try to place the rectangle in upright (non-flipped) orientation.
        if (freeRectangles[i].width >= width && freeRectangles[i].height >= height)
        {
            int leftoverHoriz = abs(freeRectangles[i].width - width);
            int leftoverVert = abs(freeRectangles[i].height - height);
            int shortSideFit = std::min(leftoverHoriz, leftoverVert);

            if (areaFit < bestAreaFit || (areaFit == bestAreaFit && shortSideFit < bestShortSideFit))
            {
                bestNode.x = freeRectangles[i].x;
                bestNode.y = freeRectangles[i].y;
                bestNode.width = width;
                bestNode.height = height;
                bestShortSideFit = shortSideFit;
                bestAreaFit = areaFit;
            }
        }

        if (allowRotate && freeRectangles[i].width >= height && freeRectangles[i].height >= width)
        {
            int leftoverHoriz = abs(freeRectangles[i].width - height);
            int leftoverVert = abs(freeRectangles[i].height - width);
            int shortSideFit = std::min(leftoverHoriz, leftoverVert);

            if (areaFit < bestAreaFit || (areaFit == bestAreaFit && shortSideFit < bestShortSideFit))
            {
                bestNode.x = freeRectangles[i].x;
                bestNode.y = freeRectangles[i].y;
                bestNode.width = height;
                bestNode.height = width;
                bestShortSideFit = shortSideFit;
                bestAreaFit = areaFit;
            }
        }
    }
    return bestNode;
}

template<typename T>
int maxrects<T>::ContactPointScoreNode(T x, T y, T width, T height) const
{
    int score = 0;

    if (x == 0 || x + width == binWidth)
        score += height;
    if (y == 0 || y + height == binHeight)
        score += width;

    for(size_t i = 0; i < usedRectangles.size(); ++i)
    {
        if (usedRectangles[i].x == x + width || usedRectangles[i].x + usedRectangles[i].width == x)
            score += CommonIntervalLength(usedRectangles[i].y, usedRectangles[i].y + usedRectangles[i].height, y, y + height);
        if (usedRectangles[i].y == y + height || usedRectangles[i].y + usedRectangles[i].height == y)
            score += CommonIntervalLength(usedRectangles[i].x, usedRectangles[i].x + usedRectangles[i].width, x, x + width);
    }
    return score;
}

template<typename T>
vec4<T> maxrects<T>::findContactPoint(T width, T height, int &bestContactScore) const
{
    vec4<T> bestNode;

    bestContactScore = -1;

    for(size_t i = 0; i < freeRectangles.size(); ++i)
    {
        // Try to place the rectangle in upright (non-flipped) orientation.
        if (freeRectangles[i].width >= width && freeRectangles[i].height >= height)
        {
            int score = ContactPointScoreNode(freeRectangles[i].x, freeRectangles[i].y, width, height);
            if (score > bestContactScore)
            {
                bestNode.x = freeRectangles[i].x;
                bestNode.y = freeRectangles[i].y;
                bestNode.width = width;
                bestNode.height = height;
                bestContactScore = score;
            }
        }
        if (freeRectangles[i].width >= height && freeRectangles[i].height >= width)
        {
            int score = ContactPointScoreNode(freeRectangles[i].x, freeRectangles[i].y, height, width);
            if (score > bestContactScore)
            {
                bestNode.x = freeRectangles[i].x;
                bestNode.y = freeRectangles[i].y;
                bestNode.width = height;
                bestNode.height = width;
                bestContactScore = score;
            }
        }
    }
    return bestNode;
}

template<typename T>
bool maxrects<T>::SplitFreeNode(vec4<T> freeNode, const vec4<T> &usedNode)
{
    // Test with SAT if the rectangles even intersect.
    if (usedNode.x >= freeNode.x + freeNode.width || usedNode.x + usedNode.width <= freeNode.x ||
        usedNode.y >= freeNode.y + freeNode.height || usedNode.y + usedNode.height <= freeNode.y){

        return false;
    }

    if (usedNode.x < freeNode.x + freeNode.width && usedNode.x + usedNode.width > freeNode.x){

        // New node at the top side of the used node.

        if (usedNode.y > freeNode.y && usedNode.y < freeNode.y + freeNode.height)

        {
            vec4<T> newNode = freeNode;

                newNode.height = usedNode.y - newNode.y;
                freeRectangles.push_back(newNode);

        }
        // New node at the bottom side of the used node.

        if (usedNode.y + usedNode.height < freeNode.y + freeNode.height)
{

            vec4<T> newNode = freeNode;

            newNode.y = usedNode.y + usedNode.height;

            newNode.height = freeNode.y + freeNode.height - (usedNode.y + usedNode.height);
            freeRectangles.push_back(newNode);

        }
    }

    
    if (usedNode.y < freeNode.y + freeNode.height && usedNode.y + usedNode.height > freeNode.y)
    {

        // New node at the left side of the used node.

        if (usedNode.x > freeNode.x && usedNode.x < freeNode.x + freeNode.width)
{
            vec4<T> newNode = freeNode;

            newNode.width = usedNode.x - newNode.x;
            freeRectangles.push_back(newNode);

        }


        // New node at the right side of the used node.
        if (usedNode.x + usedNode.width < freeNode.x + freeNode.width)

        {
            vec4<T> newNode = freeNode;
            newNode.x = usedNode.x + usedNode.width;

            newNode.width = freeNode.x + freeNode.width - (usedNode.x + usedNode.width);

            freeRectangles.push_back(newNode);

        }

    }


    return true;
}

template<typename T>
void maxrects<T>::PruneFreeList()
{
    /* 
    ///  Would be nice to do something like this, to avoid a Theta(n^2) loop through each pair.
    ///  But unfortunately it doesn't quite cut it, since we also want to detect containment. 
    ///  Perhaps there's another way to do this faster than Theta(n^2).

    if (freeRectangles.size() > 0)
        clb::sort::QuickSort(&freeRectangles[0], freeRectangles.size(), NodeSortCmp);

    for(size_t i = 0; i < freeRectangles.size()-1; ++i)
        if (freeRectangles[i].x == freeRectangles[i+1].x &&
            freeRectangles[i].y == freeRectangles[i+1].y &&
            freeRectangles[i].width == freeRectangles[i+1].width &&
            freeRectangles[i].height == freeRectangles[i+1].height)
        {
            freeRectangles.erase(freeRectangles.begin() + i);
            --i;
        }
    */

    /// Go through each pair and remove any rectangle that is redundant.
    for(size_t i = 0; i < freeRectangles.size(); ++i){
        for(size_t j = i+1; j < freeRectangles.size(); ++j){
            if (IsContainedIn(freeRectangles[i], freeRectangles[j])){
                freeRectangles.erase(freeRectangles.begin()+i);
                --i;
                break;
            }
            if (IsContainedIn(freeRectangles[j], freeRectangles[i])){
                freeRectangles.erase(freeRectangles.begin()+j);
                --j;
            }
        }
    }//end for
}


//---------------------------------------------------------------------------
//
// rectspacker
//

template<typename T>
class rectspaker
{
public:
    typedef rectspaker this_type;
    typedef maxrects<T> binpaker_type;
    
    const static int npos = binpaker_type::npos;
private:
    std::vector<binpaker_type> packedPages;
    int     pageWidth;
    int     pageHeight;
    bool allowRotate;
public:
    rectspaker() : packedPages(), pageWidth(), pageHeight(), allowRotate( false )
    {
    }

    rectspaker( T width, T height, bool rotate = false ) : packedPages(), pageWidth(), pageHeight(), allowRotate( false )
    {
        init( width, height, rotate );
    }

    void init( T width, T height, bool rotate = false )
    {
        pageWidth    = width;
        pageHeight    = height;
        allowRotate    = rotate;
        packedPages.clear();
        //packedPages.push_back(maxrects(pageWidth, pageHeight, allowRotate));
    }

    //insert rectangle
    //return vec2ui(page, index)
    vec2ui insert(T width, T height, TYPENAME binpaker_type::METHOD method)
    {
        vec2ui page( npos, npos );

        if ( pageWidth < width || pageHeight < height )
        {
            if ( allowRotate ){
                if ( pageWidth < height || pageHeight < width ){
                    return page; //unpacked
                }
            }
            else{
                return page; //unpacked
            }
        }

        for ( size_t i = 0; i < packedPages.size(); ++i )
        {
            page.y = packedPages[i].insert( width, height, method );
            if ( page.y != npos ){
                page.x = i;
                break;
            }
        }

        // insert new page
        if ( page.y == npos )
        {
            packedPages.push_back( binpaker_type( pageWidth, pageHeight, allowRotate ) );
            page.x    = packedPages.size() - 1;
            page.y    = packedPages.back().insert( width, height, method );
        }
        return page;
    }

    const binpaker_type & operator[] (size_t index) const
    {
        return packedPages[index];
    }

    size_t size() const
    {
        return packedPages.size();
    }

    //int load();
    //int save();
};

}//end namespace math
}//end namespace cgl

#endif /* MAXRECTS_HPP_20170204185544 */
