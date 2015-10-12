#include "gravity.hpp"
#include "SFML/Graphics.hpp"
#include <cmath>

using sf::Vector2i;

const int WIN_HEIGHT = 480;
const int WIN_WIDTH = 640;
const float ZOOM_FACTOR = 1.2;
const int SCROLL_AMOUNT = 5;
const float KEY_ZOOM = 1.0;

int main() {
    Event event;
    bool drag = false;
    Vector2f refCrsrCoords(-1, -1);
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::RenderWindow window(sf::VideoMode(640, 480), "PhysSym", sf::Style::Default, settings);
    sf::CircleShape shape(50);

    while(window.isOpen()){
        window.clear(sf::Color::Black);
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();
            
            if (event.type == Event::KeyReleased && event.key.code == sf::Keyboard::Q)
                window.close();
            
            keyMove(window, event);
            mouseMove(window, event, drag, refCrsrCoords);                 
        }

        shape.setFillColor(sf::Color(100,250,50));
        shape.setPosition(100,100);
        window.draw(shape);
        window.display();
    }
    
    return 0;
}

///Zooms given window to the mouse position.
void zoomToMouse( sf::RenderWindow& window, Event event){
    int x = event.mouseWheel.x;
    int y = event.mouseWheel.y;
    float delta = event.mouseWheel.delta;
    
    sf::View view = window.getView();
    Vector2i crsrPixel(x,y);
    Vector2f crsrCoords = window.mapPixelToCoords(crsrPixel);
    
    float zoom;
    if (delta < 0)
        zoom = (int)(delta/abs(delta))*ZOOM_FACTOR*-1;
    else
        zoom = 1/((int)(delta/abs(delta))*ZOOM_FACTOR);
    
    view.setCenter(crsrCoords);
    view.zoom(zoom);
    
    Vector2f crsrCoordsNew = window.mapPixelToCoords(crsrPixel, view);//Finds new mouse coordinates
    view.move( crsrCoords.x-crsrCoordsNew.x, crsrCoords.y-crsrCoordsNew.y); //Moves view based on new mouse coords.
    window.setView(view);
}

//Zooms to center of given window.
void zoomToCenter(sf::RenderWindow &window, const float &amount) {
    sf::View view = window.getView();

    if (amount < 0)
        view.zoom(abs(amount) * ZOOM_FACTOR);
    else if (amount > 0)
        view.zoom(1 / (amount * ZOOM_FACTOR));

    window.setView(view);
}

void moveWindowByVector(sf::RenderWindow& window, const Vector2f offset) {
    sf::View view = window.getView();
    view.move(offset.x, offset.y);
    window.setView(view);
}

void keyMove(sf::RenderWindow& window, Event event){
    float windowScale = (float)(window.getSize().x);
    if (event.type == Event::KeyPressed && event.key.shift) {
        if (event.key.code == sf::Keyboard::Up)
            zoomToCenter(window, KEY_ZOOM);
        else if (event.key.code == sf::Keyboard::Down)
            zoomToCenter(window, -KEY_ZOOM);
    } else if (event.type == Event::KeyPressed) {
        // TODO scale scroll based on current zoom level
        float windowShift = SCROLL_AMOUNT;//SCROLL_AMOUNT*windowScale/480;
        if (event.key.code == sf::Keyboard::Left)
            moveWindowByVector(window, Vector2f(-windowShift, 0));
        if (event.key.code == sf::Keyboard::Right)
            moveWindowByVector(window, Vector2f(windowShift, 0));
        if (event.key.code == sf::Keyboard::Up)
            moveWindowByVector(window, Vector2f(0, -windowShift));
        if (event.key.code == sf::Keyboard::Down)
            moveWindowByVector(window, Vector2f(0, windowShift));
    }
}

void mouseMove(sf::RenderWindow& window, Event event, bool& drag, Vector2f& refCrsrCoords){ //TODO: Fix badly done parameters. 
    if (event.type == Event::MouseWheelMoved) 
        zoomToMouse(window, event);
        
    if (event.type == Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Middle){
        if (!drag){
            Vector2i crsrPixel(sf::Mouse::getPosition(window));
            refCrsrCoords = window.mapPixelToCoords(crsrPixel);
        }
        drag = true;
    }
    
    if (event.type == Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Middle)
        drag = false;

    if (event.type == Event::MouseMoved && drag){
        Vector2i crsrPixel( sf::Mouse::getPosition(window));
        Vector2f crsrCoords = window.mapPixelToCoords(crsrPixel);
        float x = (refCrsrCoords.x - crsrCoords.x);
        float y = (refCrsrCoords.y - crsrCoords.y);
        moveWindowByVector(window, Vector2f(x, y));
    }
}  



