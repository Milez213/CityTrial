
/* PhysX Object */

class KPPObject {
	protected:
		vec3 velocity();
		void set_velocity(vec3 vel);
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
		/* Getters and Setters */
		/* MeshIndex - GLuint
		 * TextureIndex - GLuint
		 * DiffColor - vec3
		 * SpecColor - vec3
		 * Specularity - float
		 * proporties - struct
		 */
};

class KPPCamera : public KPPObject {
	protected:
		/* Getters and Setters */
		/* LookAt - vec3 */	
};

class KPPLight : public KPPObject {
	protected:
		/* Getters and Setters */
		/* Color - vec3 
		 * Intensity - float */	
};