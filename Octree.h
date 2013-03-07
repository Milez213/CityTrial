//
//  Octree.h
//  
//
//  Created by Eric Johnson on 3/5/13.
//
//

#ifndef ____Octree__
#define ____Octree__

#include <tr1/unordered_set>
#include <tr1/unordered_map>
#include <set>
#include <iostream>
#include "ModelManager.h"

template <class T>
class Octree {
   struct LeafNode;
   struct SubDivision {
      bound boundingBox;
      SubDivision *subDivisions[8];
      std::set<LeafNode *> leaves;
      
      SubDivision(bound boundingBox);
      ~SubDivision();
      void add(LeafNode *leaf);
      bound makeSubBound(bool xPositive, bool yPositive, bool zPositive);
      SubDivision *getSubDivision(bool xPositive, bool yPositive, bool zPositive);
      
   private:
      void divide();
      //void undivide();
   };
   
   struct LeafNode {
      std::set<SubDivision *> parents;
      T &val;
      const bound &boundingBox;
      
      LeafNode(T &val);
      ~LeafNode();
      void removeFromParents();
   };
   
   unsigned int minPerSub;
   unsigned int maxPerSub;
   SubDivision *head;
   std::tr1::unordered_map<T &, LeafNode *> leafMap;
      
public:   
   Octree(bound &boundingBox, unsigned int maxPerSub = 8);
   ~Octree();
   void add(T &val);
   void update(T &val);
   void remove(T &val);
   std::set<T &> getCollisionsFor(T &val);
};

#include "Octree.cpp"

#endif /* defined(____Octree__) */
