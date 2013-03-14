//
//  Octree.cpp
//  
//
//  Created by Eric Johnson on 3/5/13.
//
//

#include "Octree.h"

Octree::Octree(bound boundingBox, unsigned int maxPerSub) :
      maxPerSub(maxPerSub), minPerSub(maxPerSub/2) {
         
   //boundingBox.center = glm::vec3(0);
   //boundingBox.dimension = glm::vec3(100);
   //boundingBox.radius = 100;
   this->head = new SubDivision(boundingBox);
}

Octree::~Octree() {
   delete head;
}

void Octree::insert(GameDrawableObject *val) {
   LeafNode *leaf = new LeafNode(val);
   
   leafMap.insert(std::make_pair<GameDrawableObject *, LeafNode *>(val, leaf));
   
   head->add(leaf);
}

void Octree::update(GameDrawableObject *val) {
   wrappedIterator mapItr = leafMap.find(val);
   
   if (mapItr == leafMap.end())
      return;
   
   LeafNode *leaf = mapItr->second;
   
   leaf->removeFromParents();
   leaf->boundingBox = val->getBoundingInfo();
   head->add(leaf);
}

void Octree::erase(Octree::iterator position) {
   LeafNode *leaf = position.mapItr->second;
   
   leaf->removeFromParents();
   leafMap.erase(position.mapItr);
   
   delete leaf;
   //printf("deleted %s\n", position.mapItr->first->getName());
}

std::set<GameDrawableObject *> Octree::getFilteredSubset(Filter &filter) {
   std::set<GameDrawableObject *> rtn;
   head->getFilteredSubset(&rtn, filter);
   return rtn;
}



Octree::SubDivision::SubDivision(bound boundingBox) :
      boundingBox(boundingBox), subDivisions() {
}

Octree::SubDivision::~SubDivision() {
   for (int i = 0; i < 8; i++)
      if (subDivisions[i])
         delete subDivisions[i];
}

void Octree::SubDivision::add(LeafNode *leaf) {
   
   if (leaf->boundingBox.dimension.x > boundingBox.dimension.x/2) {
      leaves.insert(leaf);
      leaf->parents.insert(this);
   }
   else {
      if (leaf->boundingBox.center.y + leaf->boundingBox.dimension.y > boundingBox.center.y) {
         if (leaf->boundingBox.center.x + leaf->boundingBox.dimension.x > boundingBox.center.x) {
            if (leaf->boundingBox.center.z + leaf->boundingBox.dimension.z > boundingBox.center.z) {
               getSubDivision(true, true, true)->add(leaf);
            }
            if (leaf->boundingBox.center.z - leaf->boundingBox.dimension.z < boundingBox.center.z) {
               getSubDivision(true, true, false)->add(leaf);
            }
         }
         if (leaf->boundingBox.center.x - leaf->boundingBox.dimension.x < boundingBox.center.x) {
            if (leaf->boundingBox.center.z + leaf->boundingBox.dimension.z > boundingBox.center.z) {
               getSubDivision(false, true, true)->add(leaf);
            }
            if (leaf->boundingBox.center.z - leaf->boundingBox.dimension.z < boundingBox.center.z) {
               getSubDivision(false, true, false)->add(leaf);
            }
         }
      }
      if (leaf->boundingBox.center.y - leaf->boundingBox.dimension.y < boundingBox.center.y) {
         if (leaf->boundingBox.center.x + leaf->boundingBox.dimension.x > boundingBox.center.x) {
            if (leaf->boundingBox.center.z + leaf->boundingBox.dimension.z > boundingBox.center.z) {
               getSubDivision(true, false, true)->add(leaf);
            }
            if (leaf->boundingBox.center.z - leaf->boundingBox.dimension.z < boundingBox.center.z) {
               getSubDivision(true, false, false)->add(leaf);
            }
         }
         if (leaf->boundingBox.center.x - leaf->boundingBox.dimension.x < boundingBox.center.x) {
            if (leaf->boundingBox.center.z + leaf->boundingBox.dimension.z > boundingBox.center.z) {
               getSubDivision(false, false, true)->add(leaf);
            }
            if (leaf->boundingBox.center.z - leaf->boundingBox.dimension.z < boundingBox.center.z) {
               getSubDivision(false, false, false)->add(leaf);
            }
         }
      }
   }
}

Octree::SubDivision *Octree::SubDivision::getSubDivision(
      bool xPositive, bool yPositive, bool zPositive)
{
   if (!subDivisions[(xPositive ? 4 : 0)+(yPositive ? 2 : 0)+(zPositive ? 1 : 0)])
      subDivisions[(xPositive ? 4 : 0)+(yPositive ? 2 : 0)+(zPositive ? 1 : 0)] =
            new SubDivision(makeSubBound(xPositive, yPositive, zPositive));
      
   return subDivisions[(xPositive ? 4 : 0)+(yPositive ? 2 : 0)+(zPositive ? 1 : 0)];
}

bound Octree::SubDivision::makeSubBound(bool xPositive, bool yPositive, bool zPositive) {
   bound rtn;
   
   rtn.dimension.x = boundingBox.dimension.x/2;
   rtn.dimension.y = boundingBox.dimension.y/2;
   rtn.dimension.z = boundingBox.dimension.z/2;
   rtn.radius = boundingBox.radius/2;
   rtn.center = boundingBox.center;
   
   if (xPositive)
      rtn.center.x += rtn.dimension.x;
   else
      rtn.center.x -= rtn.dimension.x;
   if (yPositive)
      rtn.center.y += rtn.dimension.y;
   else
      rtn.center.y -= rtn.dimension.y;
   if (zPositive)
      rtn.center.z += rtn.dimension.z;
   else
      rtn.center.z -= rtn.dimension.z;
      
   
   return rtn;
}

void Octree::SubDivision::getFilteredSubset(std::set<GameDrawableObject *> *rtn, Octree::Filter &filter) {
   std::set<LeafNode *>::iterator itr;
   for (itr = leaves.begin(); itr != leaves.end(); itr++) {
      if (filter((*itr)->boundingBox)) {
         rtn->insert((*itr)->val);
      }
   }
   
   for (int i = 0; i < 8; i++) {
      if (subDivisions[i] && filter(subDivisions[i]->boundingBox)) {
         subDivisions[i]->getFilteredSubset(rtn, filter);
      }
   }
}




Octree::LeafNode::LeafNode(GameDrawableObject *val) : val(val), boundingBox(val->getBoundingInfo()) {}


void Octree::LeafNode::removeFromParents() {
   std::set<SubDivision *>::iterator itr;
   for (itr = parents.begin(); itr != parents.end();) {
      (*itr)->leaves.erase(this);
      parents.erase(itr++);
   }
}