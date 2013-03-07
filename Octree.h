//
//  Octree.h
//  
//
//  Created by Eric Johnson on 3/5/13.
//
//

#ifndef ____Octree__
#define ____Octree__

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
      void getCollisionsFor(std::set<T> *rtn, T val);
      void getSubsetInFrustum(std::set<T> *rtn, bool (*isInFrustum)(bound));
      
      
   private:
      void divide();
      //void undivide();
   };
   
   struct LeafNode {
      std::set<SubDivision *> parents;
      T val;
      const bound &boundingBox;
      
      LeafNode(T val);
      ~LeafNode();
      void removeFromParents();
   };
   
   
   typedef typename std::tr1::unordered_map<T, LeafNode *>::iterator wrappedIterator;
   class iterator : std::iterator_traits<typename std::set<T>::iterator> {
      wrappedIterator mapItr;
      
   public:
      iterator(wrappedIterator mapItr) : mapItr(mapItr) {}
      
      const T& operator*() const { return mapItr->first; }
      const T* operator->() const { return &mapItr->first; }
      iterator& operator++() { ++mapItr; return *this; }
      iterator operator++(int) { return iterator(mapItr++); }
   };
   
   unsigned int minPerSub;
   unsigned int maxPerSub;
   SubDivision *head;
   std::tr1::unordered_map<T, LeafNode *> leafMap;
      
public:   
   Octree(bound &boundingBox, unsigned int maxPerSub = 8);
   ~Octree();
   void insert(T val);
   void update(T val);
   void erase(T val);
   iterator begin() { return iterator(leafMap.begin()); }
   iterator end() { return iterator(leafMap.end()); }
   iterator erase(iterator position);
   std::set<T > getCollisionsFor(T val);
   std::set<T > getSubsetInFrustum(bool (*isInFrustum)(bound));
};



template<class T>
inline bool operator==(typename Octree<T>::iterator const& lhs, typename Octree<T>::iterator const& rhs) {
   return lhs.mapItr == rhs.mapItr;
}

#include "Octree.cpp"

#endif /* defined(____Octree__) */
