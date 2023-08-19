#ifndef PLANET_H_I
#define PLANET_H_I

class Planet {
public:
	float radius, distance, orbit, orbitSpeed, axisTilt, axisAni;
	Planet(float _radius, float _distance, float _orbit, float _orbitSpeed, float _axisTilt, float _axisAni);

	void drawSmallOrbit(void);

	void drawMoon(void);

};

#endif