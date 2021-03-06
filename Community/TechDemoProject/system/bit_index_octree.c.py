##//-------------------------------------------------------------------------------
##// Copyright 2002 Mitsubishi Electric Research Laboratories.
##// All Rights Reserved.
##//
##// Permission to use, copy, modify and distribute this software and its
##// documentation for educational, research and non-profit purposes, without fee,
##// and without a written agreement is hereby granted, provided that the above
##// copyright notice and the following three paragraphs appear in all copies.
##//
##// To request permission to incorporate this software into commercial products
##// contact MERL - Mitsubishi Electric Research Laboratories, 201 Broadway,
##// Cambridge, MA 02139.
##//
##// IN NO EVENT SHALL MERL BE LIABLE TO ANY PARTY FOR DIRECT, INDIRECT, SPECIAL,
##// INCIDENTAL, OR CONSEQUENTIAL DAMAGES, INCLUDING LOST PROFITS, ARISING OUT OF
##// THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF MERL HAS BEEN ADVISED
##// OF THE POSSIBILITY OF SUCH DAMAGES.
##8
##//
##// MERL SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
##// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
##// THE SOFTWARE PROVIDED HEREUNDER IS ON AN "AS IS" BASIS, AND MERL HAS NO
##// OBLIGATIONS TO PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR
##// MODIFICATIONS.
##//-------------------------------------------------------------------------------
##//-------------------------------------------------------------------------------
##// Generic quadtree cell. Note that the locational codes and the cell level are
##// only used in neighbor searching; they are not necessary for point or region
##// location.
##//-------------------------------------------------------------------------------
class _qtCell:
    def __init__(self, xLocCode, yLocCode, level, parent, children, data=None):
        self.xLocCode = xLocCode
        self.yLocCode = yLocCode
        self.level = level
        self.parent = parent
        self.children = children
        self.data = data
##    unsigned int xLocCode; // X locational code
##    unsigned int yLocCode; // Y locational code
##    unsigned int level; // Cell level in hierarchy (smallest cell has level 0)
##    struct _qtCell *parent; // Pointer to parent cell
##    struct _qtCell *children; // Pointer to first of 4 contiguous child cells
##    void *data; // Application specific cell data
##    } qtCell;
##//-------------------------------------------------------------------------------
##// Maximum quadtree depth and related constants
##//-------------------------------------------------------------------------------
QT_N_LEVELS = 16 # Number of possible levels in the quadtree
QT_ROOT_LEVEL = 15 # Level of root cell (QT_N_LEVELS - 1)
QT_MAX_VAL = 32768.0 #For converting positions to locational codes
##// (QT_MAX_VAL = 2^QT_ROOT_LEVEL)
##//-------------------------------------------------------------------------------
##// Macro to traverse a quadtree from a specified cell (typically the root cell)
##// to a leaf cell by following the x and y locational codes, xLocCode and
##// yLocCode. Upon entering, cell is the specified cell and nextLevel is one less
##// than the level of the specified cell. Upon termination, cell is the leaf cell
##// and nextLevel is one less than the level of the leaf cell.
##//-------------------------------------------------------------------------------
##def QT_TRAVERSE(cell,nextLevel,xLocCode,yLocCode):
##    while cell.children:
##        childBranchBit = 1 << nextLevel
##        childIndex = ((xLocCode & childBranchBit) >> nextLevel) + ((yLocCode & childBranchBit) >> nextLevel-1)
##        cell = &(((cell)->children)[childIndex]); \
        
def QT_TRAVERSE(cell,nextLevel,xLocCode,yLocCode):
    for c in cell.children:
        childBranchBit = 1 << nextLevel
        childIndex = (((xLocCode & childBranchBit) >> nextLevel) \
            + ((yLocCode & childBranchBit) >> nextLevel-1))
        cell = cell.children[childIndex]
    return cell
    

##} \
##}
##//-------------------------------------------------------------------------------
##// Macro to traverse a quadtree from a specified cell to an offspring cell by
##// following the x and y locational codes, xLocCode and yLocCode. The offpring
##// cell is either at a specified level or is a leaf cell if a leaf cell is
##// reached before the specified level. Upon entering, cell is the specified
##// cell and nextLevel is one less than the level of the specified cell. Upon
##// termination, cell is the offspring cell and nextLevel is one less than the
##// level of the offspring cell.
##//-------------------------------------------------------------------------------
def QT_TRAVERSE_TO_LEVEL(cell,nextLevel,xLocCode,yLocCode,level):
    n = nextLevel - level + 1
    while n:
        n-=1
        childBranchBit = 1 << nextLevel
        childIndex = (((xLocCode & childBranchBit) >> nextLevel) \
            + ((yLocCode & childBranchBit) >> nextLevel-1))
        cell = cell.children[childIndex]
        if not cell.children: break
    return cell

##//-------------------------------------------------------------------------------
##// Macro for traversing a quadtree to a common ancestor of a specified cell
##// and its neighbor, whose x or y locational code differs from the cell's
##// corresponding x or y locational code by binaryDiff (determined by XOR'ing the
##// appropriate pair of x or y locational codes). Upon entering, cell is the
##// specified cell and cellLevel is the cell's level. Upon termination, cell is
##// the common ancestor and cellLevel is the common ancestor's level.
##//-------------------------------------------------------------------------------
def QT_GET_COMMON_ANCESTOR(cell,cellLevel,binaryDiff):
    while (binaryDiff & (1 << cellLevel)):
        cell = cell.parent
        cellLevel+=1

##//-------------------------------------------------------------------------------
##// Locate the leaf cell containing the specified point p, where p lies in
##// [0,1)x[0,1).
##//-------------------------------------------------------------------------------
def qtLocateCell(root, pos):
##//----Determine the x and y locational codes of the point's position. Refer
##//----to [King2001] for more efficient methods for converting floating point
##//----numbers to integers.
    xLocCode = int(pos[0] * QT_MAX_VAL)
    yLocCode = int(pos[1] * QT_MAX_VAL)
    zLocCode = int(pos[2] * QT_MAX_VAL)
##//----Follow the branching patterns of the locational codes from the root cell
##//----to locate the leaf cell containing p
    cell = root
    nextLevel = QT_ROOT_LEVEL - 1
    found = QT_TRAVERSE(cell,nextLevel,xLocCode,yLocCode);
    return(found)

##//-------------------------------------------------------------------------------
##// Locate the smallest cell that entirely contains a rectangular region defined
##// by its bottom-left vertex v0 and its top-right vertex v1, where v0 and v1
##// lie in [0,1)x[0,1).
##//-------------------------------------------------------------------------------
def qtLocateRegion (root, v0, v1):
##//----Determine the x and y locational codes of the region boundaries. Refer
##//----to [King2001] for more efficient methods for converting floating point
##//----numbers to integers.
    x0LocCode = v0[0] * QT_MAX_VAL
    y0LocCode = v0[1] * QT_MAX_VAL
    x1LocCode = v1[0] * QT_MAX_VAL
    y1LocCode = v1[1] * QT_MAX_VAL
##//----Determine the XOR'ed pairs of locational codes of the region boundaries
    xDiff = x0LocCode ^ x1LocCode
    yDiff = y0LocCode ^ y1LocCode
##//----Determine the level of the smallest possible cell entirely containing
##//----the region
    cell = root
    level = QT_ROOT_LEVEL
    minLevel = QT_ROOT_LEVEL
    while not (xDiff & (1 << level)) & level: 
        level-=1
    while not (yDiff & (1 << minLevel)) & (minLevel > level):
         minLevel -= 1
    minLevel+=1
##//----Follow the branching patterns of the locational codes of v0 from the
##//----root cell to the smallest cell entirely containing the region
    level = QT_ROOT_LEVEL - 1;
    QT_TRAVERSE_TO_LEVEL(cell,level,x0LocCode,y0LocCode,minLevel);
    return cell

##//-------------------------------------------------------------------------------
##// Locate the left edge neighbor of the same size or larger than a specified
##// cell. A null pointer is returned if no such neighbor exists.
##//-------------------------------------------------------------------------------
def qtLocateLeftNeighbor(cell):
##//----No left neighbor if this is the left side of the quadtree
    if cell.xLocCode == 0: return 0
    else:
##//----Get cell's x and y locational codes and the x locational code of the
##//----cell's smallest possible left neighbor
        xLocCode = cell.xLocCode
        yLocCode = cell.yLocCode
        xLeftLocCode = xLocCode - 0x00000001
##//----Determine the smallest common ancestor of the cell and the cell's
##//----smallest possible left neighbor
        cellLevel = nextLevel
        diff = xLocCode ^ xLeftLocCode
        pCell = cell
        cellLevel = nextLevel = cell.level
        QT_GET_COMMON_ANCESTOR(pCell,nextLevel,diff)
##//----Start from the smallest common ancestor and follow the branching
##//----patterns of the locational codes downward to the smallest left
##//----neighbor of size greater than or equal to cell
        nextLevel-=1
        QT_TRAVERSE_TO_LEVEL(pCell,nextLevel,xLeftLocCode,yLocCode,cellLevel)
        return pCell 
    
##//-------------------------------------------------------------------------------
##// Locate the right edge neighbor of the same size or larger than a specified
##// cell. A null pointer is returned if no such neighbor exists.
##//-------------------------------------------------------------------------------
def qtLocateRightNeighbor(cell):
##//----No right neighbor if this is the right side of the quadtree
    binaryCellSize = 1 << cell.level
    if ((cell.xLocCode + binaryCellSize) >= (1 << QT_ROOT_LEVEL)):
        return 0
    else:
##//----Get cell's x and y locational codes and the x locational code of the
##//----cell's right neighbors
        xLocCode = cell.xLocCode
        yLocCode = cell.yLocCode
        xRightLocCode = xLocCode + binaryCellSize
##//----Determine the smallest common ancestor of the cell and the cell's
##//----right neighbors
        cellLevel = nextLevel
        diff = xLocCode ^ xRightLocCode
        pCell = cell
        cellLevel = nextLevel = cell.level
        QT_GET_COMMON_ANCESTOR(pCell,nextLevel,diff)
##//----Start from the smallest common ancestor and follow the branching
##//----patterns of the locational codes downward to the smallest right
##//----neighbor of size greater than or equal to cell
        nextLevel-=1
        QT_TRAVERSE_TO_LEVEL(pCell,nextLevel,xRightLocCode,yLocCode,cellLevel)
        return(pCell)
        
##//-------------------------------------------------------------------------------
##// Locate the three leaf cell vertex neighbors touching the right-bottom vertex
##// of a specified cell. bVtxNbr, rVtxNbr, and rbVtxNbr are set to null if the
##// corresponding neighbor does not exist.
##//-------------------------------------------------------------------------------
def qtLocateRBVertexNeighbors (qtCell *cell, qtCell **bVtxNbr, qtCell **rVtxNbr, qtCell **rbVtxNbr)
##//----There are no right neighbors if this is the right side of the quadtree and
##//----no bottom neighbors if this is the bottom of the quadtree
    binCellSize = 1 << cell.level
    noRight = ((cell.xLocCode + binCellSize) >= (1 << QT_ROOT_LEVEL)) ? 1 : 0
unsigned int noBottom = (cell->yLocCode == 0) ? 1 : 0;
//----Get cell's x and y locational codes and the x and y locational codes of
//----the cell's right and bottom vertex neighbors
unsigned int xRightLocCode = cell->xLocCode + binCellSize;
unsigned int xLocCode = xRightLocCode - 0x00000001;
unsigned int yLocCode = cell->yLocCode;
unsigned int yBottomLocCode = yLocCode - 0x00000001;
unsigned int rightLevel, bottomLevel;
unsigned int diff;
qtCell *commonRight, *commonBottom;
//----Determine the right leaf cell vertex neighbor
if (noRight) *rVtxNbr = 0;
else {
//----Determine the smallest common ancestor of the cell and the cell's
//----right neighbor. Save this right common ancestor and its level for
//----determining the right-bottom vertex.
unsigned int level = cell->level;
diff = xLocCode ^ xRightLocCode;
commonRight = cell;
QT_GET_COMMON_ANCESTOR(commonRight,level,diff);
rightLevel = level;
//----Follow the branching patterns of the locational codes downward from
//----the smallest common ancestor to the right leaf cell vertex neighbor
*rVtxNbr = commonRight;
11
level--;
QT_TRAVERSE_TO_LEVEL(*rVtxNbr,level,xRightLocCode,cell->yLocCode,0);
}
//----Determine the bottom leaf cell vertex neighbor
if (noBottom) *bVtxNbr = 0;
else {
//----Determine the smallest common ancestor of the cell and the cell's
//----bottom neighbor. Save this bottom common ancestor and its level for
//----determining the right-bottom vertex.
unsigned int level = cell->level;
diff = yLocCode ^ yBottomLocCode;
commonBottom = cell;
QT_GET_COMMON_ANCESTOR(commonBottom,level,diff);
bottomLevel = level;
//----Follow the branching patterns of the locational codes downward from
//----the smallest common ancestor to the bottom leaf cell vertex neighbor
*bVtxNbr = commonBottom;
level--;
QT_TRAVERSE_TO_LEVEL(*bVtxNbr,level,xLocCode,yBottomLocCode,0);
}
//----Determine the right-bottom leaf cell vertex neighbor
if (noRight || noBottom) *rbVtxNbr = 0;
else {
//----Follow the branching patterns of the locational codes downward from
//----the smallest common ancestor (the larger of the right common ancestor
//----and the bottom common ancestor) to the right-bottom leaf cell vertex
//----neighbor
if (rightLevel >= bottomLevel) {
*rbVtxNbr = commonRight;
rightLevel--;
QT_TRAVERSE_TO_LEVEL(*rbVtxNbr,rightLevel,xRightLocCode,yBottomLocCode,0);
} else {
*rbVtxNbr = commonBottom;
bottomLevel--;
QT_TRAVERSE_TO_LEVEL(*rbVtxNbr,bottomLevel,xRightLocCode,yBottomLocCode,0);
}
}
}