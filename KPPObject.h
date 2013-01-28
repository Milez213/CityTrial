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


