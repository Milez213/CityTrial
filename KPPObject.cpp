class KPPObject {
protected:

glm::vec3 velocity;
glm::vec3 position;
glm::vec3 rotation;
glm::vec3 scale;
bound* bBox;

glm::vec3 velocity(){
   return velocity;
}
void set_velocity(glm::vec3 vel){
   velocity = vel;
}
glm::vec3 position(){
   return position;
}
void set_position(glm::vec3 p){
   position = p;
}
glm::vec3 rotation(){
   return rotation;
}
void set_rotation(glm::vec3 r){
   rotation = r;
}
glm::vec3 scale(){
   return scale;
}
void set_scale(glm::vec3 s){
   scale = s;
}
bound* bBox()
{
   return bBox:
}

void bBox(bound* b){
   bBox = b;
}

//Bounding Box?
//Bounding sphere?
/* Getters and Setters */
/* Velocity - vec3
* Position - vec3
* Rotation - vec3
* Scale - vec3
* Bounding Box*
* Bounding Sphere*
* MakeSure
*/
};

class KPPDrawnObject : public KPPObject {
protected:

GLuint meshIndex;
GLunint textureIndex;
glm::vec3 diffColor;
glm::vec3 specColor;
float specularity;


GLuint meshIndex(){
   return meshIndex;
}
void set_meshIndex(GLunint i){
   meshIndex = i;
}
GLuint textureIndex(){
   return textureIndex;
}
void set_textureIndex(GLuint i){
   textureIndex = i;
}
glm::vec3 diffColor(){
   return diffColor;
}
void set_diffColor(glm::vec3 c){
   diffColor = c;
}
glm::vec3 specColor(){
   return specColor;
}
void set_specColor(glm::vec3 c){
   specColor = c;
}
float specularity(){
   return specularity;
}
void set_specularity(float f){
   specularity = f;
}

void draw(){
}

//Properties??
/* Getters and Setters */
/* MeshIndex - GLuint
* TextureIndex - GLuint
* DiffColor - vec3
* SpecColor - vec3
* Specularity - float
* properties - struct
*/
};

class KPPCamera : public KPPObject {
protected:
glm::vec3 lookAt;

glm::vec3 lookAt(){
   return lookAt;
}
void set_lookAt(glm::vec3 l){
   lookAt = l;
}
/* Getters and Setters */
/* LookAt - vec3 */
};

class KPPLight : public KPPObject {
protected:
glm::vec3 color;
glm::vec3 intensity;

glm::vec3 color(){
   return color;
}
void set_color(glm::vec3 c){
   color = c;
}
glm::vec3 intensity(){
   return intensity;
}
void set_intensity(glm::vec3 i){
   intensity = i;
}
/* Getters and Setters */
/* Color - vec3
* Intensity - float */
};
