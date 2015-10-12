
#include "SFML/Graphics.hpp"
#ifndef GRAVITY_H_INCLUDED
#define GRAVITY_H_INCLUDED

using sf::Event;
using sf::Vector2f;


int graphicsLoop();
//window operations
void zoomToMouse(sf::RenderWindow& window, Event event);
void zoomToCenter(sf::RenderWindow &window, const float &amount);
void moveWindowByVector(sf::RenderWindow& window, const sf::Vector2f offset);
void keyMove(sf::RenderWindow& window, Event event);
void mouseMove(sf::RenderWindow& window, Event event, bool& drag, Vector2f& refCrsrCoords);

#endif
