/* PhysX Object */

class KPPObject {
	protected:
		glm::vec3 velocity();
		void set_velocity(vec3 vel);
		glm::vec3 position();
		void set_position(vec3 p);
		glm::vec3 rotation();
		void set_rotation(vec3 r);
		glm::vec3 scale();
		void set_scale(vec3 s);
		bound* bBox();
		void bBox(bound*);
		//Bounding Box?
		//Bounding sphere?
		/* Getters and Setters */
		/* Velocity - vec3
		 * Position - vec3
		 * Rotation - vec3
		 * Scale - vec3
		 * Bounding Box*
		 * Bounding Sphere*
		 */
};

class KPPDrawnObject : public KPPObject {
	protected:
	         GLuint meshIndex();
	         void set_meshIndex(GLunint i);
	         GLuint textureIndex();
	         void set_textureIndex(GLuint i);
	         glm::vec3 diffColor();
	         void set_diffColor(glm::vec3 c);
	         glm::vec3 specColor();
	         void set_specColor(glm::vec3 c);
	         float specularity();
	         void set_specularity(float f);
	         void draw();
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
	        glm::vec3 lookAt();
	        void set_lookAt(glm::vec3 l);
		/* Getters and Setters */
		/* LookAt - vec3 */	
};

class KPPLight : public KPPObject {
	protected:
	        glm::vec3 color();
	        void set_color(glm::vec3 c);
	        glm::vec3 intensity();
	        void set_intensity(glm::vec3 i);
		/* Getters and Setters */
		/* Color - vec3 
		 * Intensity - float */	
};
