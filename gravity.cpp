#include "SFML/graphics.hpp"
#include <cmath>

using sf::Event;
using sf::Vector2f;
using sf::Vector2i;

const int WIN_HEIGHT = 480;
const int WIN_WIDTH = 640;
const float ZOOM_FACTOR = 1.2;
const int SCROLL_AMOUNT = 5;
const float KEY_ZOOM = 1.0;

int graphicsLoop(const terrain* map) {
    image.create(map->h_map[0].size(), map->h_map.size());:
    sf::Texture texture;
    texture.loadFromImage(image);
    sf::Sprite sprite(texture);
    Event event;
    bool drag = false;
    Vector2f refCrsrCoords(-1, -1);
    
    image = renderMap(map, image);
    texture.loadFromImage(image);
    
    while(window.isOpen()){
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();
            
            if (event.type == Event::KeyReleased && event.key.code == sf::Keyboard::Key::Q)
                window.close();
            
            keyMove(event);
            mouseMove(event, drag, refCrsrCoords);                 
        }
        window.clear(sf::Color::Black);
        window.draw(sprite);
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

void keyMove(Event event){
    float windowScale = (float)(window.getSize().x);
    if (event.type == Event::KeyPressed && event.key.shift) {
        if (event.key.code == sf::Keyboard::Key::Up)
            zoomToCenter(window, KEY_ZOOM);
        else if (event.key.code == sf::Keyboard::Key::Down)
            zoomToCenter(window, -KEY_ZOOM);
    } else if (event.type == Event::KeyPressed) {
        // TODO scale scroll based on current zoom level
        float windowShift = SCROLL_AMOUNT;//SCROLL_AMOUNT*windowScale/480;
        if (event.key.code == sf::Keyboard::Key::Left)
            moveWindowByVector(window, Vector2f{-windowShift, 0});
        if (event.key.code == sf::Keyboard::Key::Right)
            moveWindowByVector(window, Vector2f{windowShift, 0});
        if (event.key.code == sf::Keyboard::Key::Up)
            moveWindowByVector(window, Vector2f{0, -windowShift});
        if (event.key.code == sf::Keyboard::Key::Down)
            moveWindowByVector(window, Vector2f{0, windowShift});
    }
}

void mouseMove(Event event, bool& drag, Vector2f& refCrsrCoords){ //TODO: Fix badly done parameters. 
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
        moveWindowByVector(window, Vector2f{x, y});
    }
}  



