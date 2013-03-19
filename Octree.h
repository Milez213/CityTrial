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
#include "GameDrawableObject.h"
#include "GameKartObject.h"
#include "calcFrustum.h"

class Octree {
public:
   class Filter {
   public:
      virtual bool operator() (bound subDivision) = 0;
   };
   
protected:
   struct LeafNode;
   
   struct SubDivision {
      static GameDrawableObject *drawableCube;
      
      bound boundingBox;
      SubDivision *subDivisions[8];
      std::set<LeafNode *> leaves;
      
      SubDivision(bound boundingBox);
      ~SubDivision();
      void add(LeafNode *leaf);
      bound makeSubBound(bool xPositive, bool yPositive, bool zPositive);
      SubDivision *getSubDivision(bool xPositive, bool yPositive, bool zPositive);
      //void getCollisionsFor(std::set<GameDrawableObject *> *rtn, GameDrawableObject *val);
      //void getSubsetInFrustum(std::set<GameDrawableObject *> *rtn, bool (*isInFrustum)(bound));
      void getFilteredSubset(std::set<GameDrawableObject *> *rtn, Filter &filter);
      void draw(PhongShader *meshShader, RenderingHelper modelViewMatrix);
      
   private:
      void divide();
      //void undivide();
   };
   
   struct LeafNode {
      std::set<SubDivision *> parents;
      GameDrawableObject *val;
      bound boundingBox;
      
      LeafNode(GameDrawableObject *val);
      void removeFromParents();
   };
   
   
   typedef std::tr1::unordered_map<GameDrawableObject *, LeafNode *>::iterator wrappedIterator;
   
   unsigned int maxPerSub;
   unsigned int minPerSub;
   SubDivision *head;
   std::tr1::unordered_map<GameDrawableObject *, LeafNode *> leafMap;
      
public:
   class iterator : std::iterator_traits<std::set<GameDrawableObject *>::iterator> {
      wrappedIterator mapItr;
      
   public:
      friend class Octree;
      friend bool operator==(Octree::iterator const& lhs, Octree::iterator const& rhs);
      friend bool operator!=(Octree::iterator const& lhs, Octree::iterator const& rhs);
      
      iterator() {}
      iterator(wrappedIterator mapItr) : mapItr(mapItr) {}
      
      GameDrawableObject * const operator*() const { return mapItr->first; }
      //GameDrawableObject * const* operator->() const { return &mapItr->first; }
      iterator& operator++() { ++mapItr; return *this; }
      iterator operator++(int) { return iterator(mapItr++); }
   };
   
   Octree(bound boundingBox = bound(), unsigned int maxPerSub = 8);
   ~Octree();
   void insert(GameDrawableObject *val);
   void update(GameDrawableObject *val);
   void erase(iterator position);
   iterator begin() { return iterator(leafMap.begin()); }
   iterator end() { return iterator(leafMap.end()); }
   std::set<GameDrawableObject *> getFilteredSubset(Filter &filter);
   void draw(PhongShader *meshShader, RenderingHelper modelViewMatrix);
   //std::set<GameDrawableObject *> getCollisionsWith(GameDrawableObject *val);
   //std::set<GameDrawableObject *> getSubsetInFrustum(bool (*isInFrustum)(bound));
};




inline bool operator==(Octree::iterator const& lhs, Octree::iterator const& rhs) {
   return lhs.mapItr == rhs.mapItr;
}


inline bool operator!=(Octree::iterator const& lhs, Octree::iterator const& rhs) {
   return !(lhs == rhs);
}



class ViewFrustumFilter : public Octree::Filter {
public:
   virtual bool operator() (bound subDivision) {
      return isBoundInFrustum(subDivision);
   }
};

class KartCollisionFilter : public Octree::Filter {
   GameKartObject *kart;
public:
   KartCollisionFilter(GameKartObject *kart) : kart(kart) {}
   virtual bool operator() (bound subDivision) {
      return ModelManager::boxOnBox(kart->getBoundingInfo(), subDivision);
   }
};

#endif /* defined(____Octree__) */
