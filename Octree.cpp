//
//  Octree.cpp
//  
//
//  Created by Eric Johnson on 3/5/13.
//
//

template <class T>
Octree<T>::Octree(bound &boundingBox, unsigned int maxPerSub) :
      maxPerSub(maxPerSub), minPerSub(maxPerSub/2) {
   this->head = new SubDivision(boundingBox);
}

template <class T>
Octree<T>::~Octree() {
   delete head;
}

template <class T>
void Octree<T>::insert(T &val) {
   LeafNode *leaf = new LeafNode(val);
   
   head->add(leaf);
}

template <class T>
void Octree<T>::update(T &val) {
   LeafNode *leaf = leafMap[val];
   
   leaf->removeFromParents();
   head->add(leaf);
}

template <class T>
void Octree<T>::erase(T &val) {
   LeafNode *leaf = leafMap[val];
   
   leaf->removeFromParents();
   leafMap.erase(leaf.val);
   
   delete leaf;
}

template <class T>
typename Octree<T>::iterator Octree<T>::erase(typename Octree<T>::iterator position) {
   wrappedIterator rtn;
   LeafNode *leaf = position.mapItr->second;
   
   leaf->removeFromParents();
   rtn = leafMap.erase(position.mapItr);
   
   delete leaf;
   
   return iterator(rtn);
}

template <class T>
typename std::set<T &> Octree<T>::getCollisionsFor(T &val) {
   std::set<T &> rtn;
   head->getCollisionsFor(&rtn, val);
   return rtn;
}

template <class T>
typename std::set<T &> Octree<T>::getSubsetInFrustum(bool (*isInFrustum)(bound)) {
   std::set<T &> rtn;
   head->getSubsetInFrustum(&rtn, isInFrustum);
   return rtn;
}



template <class T>
Octree<T>::SubDivision::SubDivision(bound boundingBox) :
      boundingBox(boundingBox), subDivisions() {
}

template <class T>
Octree<T>::SubDivision::~SubDivision() {
   for (int i = 0; i < 8; i++)
      if (subDivisions[i])
         delete subDivisions[i];
}

template <class T>
void Octree<T>::SubDivision::add(LeafNode *leaf) {
   
   if (leaf->bound.dimension.x > boundingBox.dimension.x/2) {
      leaves.insert(leaf);
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

template <class T>
typename Octree<T>::SubDivision *Octree<T>::SubDivision::getSubDivision(
      bool xPositive, bool yPositive, bool zPositive)
{
   if (!subDivisions[(xPositive ? 4 : 0)+(yPositive ? 2 : 0)+(zPositive ? 1 : 0)])
      subDivisions[(xPositive ? 4 : 0)+(yPositive ? 2 : 0)+(zPositive ? 1 : 0)] =
            new SubDivision(makeSubBound(xPositive, yPositive, zPositive));
      
   return subDivisions[(xPositive ? 4 : 0)+(yPositive ? 2 : 0)+(zPositive ? 1 : 0)];
}

template <class T>
bound Octree<T>::SubDivision::makeSubBound(bool xPositive, bool yPositive, bool zPositive) {
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

template <class T>
void Octree<T>::SubDivision::getCollisionsFor(std::set<T &> *rtn, T &val) {
   typename std::set<LeafNode *>::iterator itr;
   for (itr = leaves.begin(); itr != leaves.end; itr++) {
      rtn->insert(itr->val);
   }
   
   for (int i = 0; i < 8; i++) {
      if (subDivisions[i] && ModelManager::boxOnBox(val.getBoundingInfo(), subDivisions[i]->boundingBox)) {
         subDivisions[i]->getCollisionsFor(rtn, val);
      }
   }
}

template <class T>
void Octree<T>::SubDivision::getSubsetInFrustum(std::set<T &> *rtn, bool (*isInFrustum)(bound)) {
   typename std::set<LeafNode *>::iterator itr;
   for (itr = leaves.begin(); itr != leaves.end; itr++) {
      rtn->insert(itr->val);
   }
   
   for (int i = 0; i < 8; i++) {
      if (subDivisions[i] && (*isInFrustum)(subDivisions[i]->boundingBox)) {
         subDivisions[i]->getSubsetInFrustum(rtn, isInFrustum);
      }
   }
}



template <class T>
Octree<T>::LeafNode::LeafNode(T &val) : val(val), bound(val.getBoundingInfo()) {
   leafMap.add(std::make_pair<T &, LeafNode *>(val, this));
}

template <class T>
void Octree<T>::LeafNode::removeFromParents() {
   typename std::set<LeafNode *>::iterator itr;
   for (itr = parents.begin(); itr != parents.end();) {
      itr->leaves.erase(this);
      itr = parents.erase(itr);
   }
}