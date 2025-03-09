module;

#include <SFML/Graphics.hpp>

export module Entity;

export class Entity : public sf::Drawable, public sf::Transformable  {

protected:
    sf::Sprite sprite;
    sf::Texture texture;
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        // apply the entity's transform -- combine it with the one that was passed by the caller
        states.transform *= getTransform(); // getTransform() is defined by sf::Transformable

        // apply the texture
        states.texture = &texture;

        // draw the sprite
        target.draw(sprite, states);
    }

public:
    Entity() = default;
    ~Entity() = default;

    Entity(const std::string& texture_name, float posx, float posy, float scalex, float scaley) {
        if (!set_sprite(texture_name))
            return;
        sprite.setPosition(posx, posy);
        sprite.setScale(scalex, scaley);
    }
    Entity(const std::string& texture_name, float posx, float posy) {
        if (!set_sprite(texture_name))
            return;
        sprite.setPosition(posx, posy);
    }
    Entity(const std::string& texture_name, float scale) {
        if (!set_sprite(texture_name))
            return;
        sprite.setScale(scale, scale);
    }
    Entity(const std::string& texture_name) {
        if (!set_sprite(texture_name))
            return;
    }

    bool set_sprite(const std::string& texture_name) {
        if (!texture.loadFromFile(texture_name))
            return false;
        texture.setSmooth(true);
        sprite.setTexture(texture);
        return true;
    }
    sf::Texture& getTexture() { return this->texture; }
    sf::Sprite& getSprite() { return this->sprite; }
};