//
//  quadtree.hpp
//  DelaunayVoronoi
//
//  Created by wingyiu on 2/23/16.
//  Copyright © 2016 wingyiu. All rights reserved.
//

#ifndef quadtree_h
#define quadtree_h

#include <cstdio>

class QuadtreeNode
{
public:
private:
    int m_dimension;
    QuadtreeNode* parent;
    QuadtreeNode** children;
};

#endif /* quadtree_hpp */
