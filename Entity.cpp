#include "SFML/Graphics.hpp"
#include <iostream>

class Entity
{
	sf::CircleShape shape{50};

  public:
  	float radius;
  	float mass;
  	float position[2];
	float velocity[2];
	float acceleration[2];
	Entity(float inPos[2]){
		position[0]=inPos[0];
		position[1]=inPos[1];
		resetMotion();	

		radius = 10;
		mass = 1;
		shape.setRadius(radius);
		shape.setPosition(sf::Vector2f(position[0],position[1]));
	}
	
	sf::CircleShape renderShape(){
		shape.setRadius(radius);
		shape.setPosition(sf::Vector2f(position[0],position[1]));
		shape.setFillColor(sf::Color(100,250,250));
		return shape;
	}

	float* getPosition(){
		return position;
	}
	void applyGravity(Entity entity){
		float xDist = (entity.position[0]-position[0])/10;
		float yDist = (entity.position[1]-position[1])/10;
		float dist = sqrt(yDist*yDist + xDist*xDist);
		float multiplier = mass*entity.mass*1;
		float gravMag = multiplier/(dist*dist);
		float gravVector[2] = {xDist*gravMag/dist, yDist*gravMag/dist};
		if(radius!=25){
			std::cout<<dist<<"\n";
			std::cout<<radius<<": "<<gravVector[0]<<" "<<gravVector[1]<<"\n";
		}
		setForce(gravVector);
	}

	void applyForce(float force[2]){
		acceleration[0] += force[0]/mass;
		acceleration[1] += force[1]/mass;
	}

	void setForce(float force[2]){
		acceleration[0] = force[0]/mass;
		acceleration[1] = force[1]/mass;
	}

	void resetMotion(){
		resetVelocity();
		resetAcceleration();	
	}

	void resetAcceleration(){
		acceleration[0]=0;
		acceleration[1]=0;
	}

	void resetVelocity(){
		velocity[0] = 0;
		velocity[1] = 0;
	}

	void updatePosition(double dt){
		velocity[0] = velocity[0]+acceleration[0]*dt;
		velocity[1] = velocity[1]+acceleration[1]*dt;
		position[0] += velocity[0]*dt+acceleration[0]*dt*dt/2.0;
		position[1] += velocity[1]*dt+acceleration[1]/2.0*dt*dt;
	}

};