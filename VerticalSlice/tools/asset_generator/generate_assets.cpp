#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <cmath>

// Define our gem types and their corresponding colors and filenames
struct GemAsset {
    std::string filename;
    sf::Color color;
};

// Using the full list from the GDD
std::map<std::string, GemAsset> gemAssets = {
    {"gem_raw",      {"gem_raw.png",      sf::Color(128, 128, 128)}},
    {"gem_air",      {"gem_air.png",      sf::Color(173, 216, 230)}},
    {"gem_fire",     {"gem_fire.png",     sf::Color(255, 69, 0)}},
    {"gem_earth",    {"gem_earth.png",    sf::Color(139, 69, 19)}},
    {"gem_water",    {"gem_water.png",    sf::Color(0, 119, 190)}},
    {"gem_umbral",   {"gem_umbral.png",   sf::Color(75, 0, 130)}},
    {"gem_light",    {"gem_light.png",    sf::Color(255, 255, 0)}},
    {"gem_life",     {"gem_life.png",     sf::Color(0, 200, 0)}},
    {"gem_death",    {"gem_death.png",    sf::Color(107, 142, 35)}},
    {"gem_mental",   {"gem_mental.png",   sf::Color(255, 0, 255)}},
    {"gem_perception",{"gem_perception.png",sf::Color(0, 255, 255)}},
    {"gem_transference",{"gem_transference.png",sf::Color(240, 230, 140)}},
    {"gem_enhancement",{"gem_enhancement.png",sf::Color(192, 192, 192)}},
    {"gem_skull",    {"gem_skull.png",    sf::Color::White}},
    {"gem_coin",     {"gem_coin.png",     sf::Color(255, 215, 0)}}
};

void drawSymbol(sf::RenderTexture& texture, const std::string& type, unsigned int size) {
    float center = size / 2.f;
    float innerSize = size * 0.4f;

    if (type == "gem_fire") {
        float reducedInnerSize = innerSize * 0.8f;
        float y_offset = reducedInnerSize * -0.15f; // Move up
        sf::ConvexShape triangle;
        triangle.setPointCount(3);
        triangle.setPoint(0, {center, center - reducedInnerSize + y_offset});
        triangle.setPoint(1, {center + reducedInnerSize, center + reducedInnerSize + y_offset});
        triangle.setPoint(2, {center - reducedInnerSize, center + reducedInnerSize + y_offset});
        triangle.setFillColor(sf::Color::White);
        texture.draw(triangle);
    } else if (type == "gem_water") {
        float reducedInnerSize = innerSize * 0.8f;
        float y_offset = reducedInnerSize * 0.15f; // Move down
        sf::ConvexShape triangle;
        triangle.setPointCount(3);
        triangle.setPoint(0, {center, center + reducedInnerSize + y_offset});
        triangle.setPoint(1, {center + reducedInnerSize, center - reducedInnerSize + y_offset});
        triangle.setPoint(2, {center - reducedInnerSize, center - reducedInnerSize + y_offset});
        triangle.setFillColor(sf::Color::White);
        texture.draw(triangle);
    } else if (type == "gem_earth") {
        sf::RectangleShape square({innerSize * 1.44f, innerSize * 1.44f}); // 1.8 * 0.8 = 1.44
        square.setOrigin({square.getSize().x / 2.f, square.getSize().y / 2.f});
        square.setPosition({center, center});
        square.setFillColor(sf::Color::White);
        texture.draw(square);
    } else if (type == "gem_air") {
        sf::VertexArray spiral(sf::PrimitiveType::LineStrip, 10);
        for (int i = 0; i < 10; ++i) {
            float angle = i * 0.8f;
            float radius = i * (innerSize / 10.f);
            spiral[i].position = sf::Vector2f(center + cos(angle) * radius, center + sin(angle) * radius);
            spiral[i].color = sf::Color::White;
        }
        texture.draw(spiral);
    } else if (type == "gem_light") {
        sf::CircleShape inner(innerSize * 0.8f);
        inner.setOrigin({inner.getRadius(), inner.getRadius()});
        inner.setPosition({center, center});
        inner.setFillColor(sf::Color::White);
        texture.draw(inner);
    } else if (type == "gem_umbral") {
        sf::ConvexShape crescent;
        crescent.setPointCount(6);
        crescent.setPoint(0, {center - innerSize, center - innerSize * 0.5f});
        crescent.setPoint(1, {center, center - innerSize});
        crescent.setPoint(2, {center + innerSize, center});
        crescent.setPoint(3, {center, center + innerSize});
        crescent.setPoint(4, {center - innerSize, center + innerSize * 0.5f});
        crescent.setPoint(5, {center - innerSize * 0.5f, center});
        crescent.setFillColor(sf::Color::White);
        texture.draw(crescent);
    } else if (type == "gem_life") {
        sf::ConvexShape leaf;
        leaf.setPointCount(4);
        leaf.setPoint(0, {center, center - innerSize});
        leaf.setPoint(1, {center + innerSize, center});
        leaf.setPoint(2, {center, center + innerSize});
        leaf.setPoint(3, {center - innerSize, center});
        leaf.setFillColor(sf::Color::White);
        texture.draw(leaf);
    } else if (type == "gem_death") {
        sf::RectangleShape line1({innerSize * 2.f, 4.f});
        line1.setOrigin({line1.getSize().x / 2.f, line1.getSize().y / 2.f});
        line1.setPosition({center, center});
        line1.setRotation(sf::degrees(45));
        line1.setFillColor(sf::Color::White);
        sf::RectangleShape line2 = line1;
        line2.setRotation(sf::degrees(-45));
        texture.draw(line1);
        texture.draw(line2);
    } else if (type == "gem_mental") {
        sf::CircleShape inner(innerSize * 0.8f);
        inner.setOrigin({inner.getRadius(), inner.getRadius()});
        inner.setPosition({center, center});
        inner.setFillColor(sf::Color::White);
        texture.draw(inner);
    } else if (type == "gem_perception") {
        sf::ConvexShape eye;
        eye.setPointCount(4);
        eye.setPoint(0, {center - innerSize, center});
        eye.setPoint(1, {center, center - innerSize * 0.7f});
        eye.setPoint(2, {center + innerSize, center});
        eye.setPoint(3, {center, center + innerSize * 0.7f});
        eye.setFillColor(sf::Color::White);
        texture.draw(eye);
    } else if (type == "gem_transference") {
        sf::ConvexShape arrow1;
        arrow1.setPointCount(3);
        arrow1.setPoint(0, {center - innerSize, center - innerSize * 0.5f});
        arrow1.setPoint(1, {center, center - innerSize * 0.5f});
        arrow1.setPoint(2, {center, center});
        arrow1.setFillColor(sf::Color::White);
        sf::ConvexShape arrow2;
        arrow2.setPointCount(3);
        arrow2.setPoint(0, {center + innerSize, center + innerSize * 0.5f});
        arrow2.setPoint(1, {center, center + innerSize * 0.5f});
        arrow2.setPoint(2, {center, center});
        arrow2.setFillColor(sf::Color::White);
        texture.draw(arrow1);
        texture.draw(arrow2);
    } else if (type == "gem_enhancement") {
        sf::RectangleShape line1({innerSize * 1.8f, 4.f});
        line1.setOrigin({line1.getSize().x / 2.f, line1.getSize().y / 2.f});
        line1.setPosition({center, center});
        line1.setFillColor(sf::Color::White);
        sf::RectangleShape line2 = line1;
        line2.setRotation(sf::degrees(90));
        texture.draw(line1);
        texture.draw(line2);
    } else if (type == "gem_coin") {
        sf::RectangleShape bar({innerSize * 1.5f, innerSize * 0.8f});
        bar.setOrigin({bar.getSize().x / 2.f, bar.getSize().y / 2.f});
        bar.setPosition({center, center});
        bar.setFillColor(sf::Color::White);
        texture.draw(bar);
    } else if (type == "gem_skull") {
        // Draw a simple dagger
        sf::ConvexShape blade;
        blade.setPointCount(4);
        blade.setPoint(0, {center, center - innerSize * 1.2f});
        blade.setPoint(1, {center + innerSize * 0.3f, center});
        blade.setPoint(2, {center, center + innerSize * 0.2f});
        blade.setPoint(3, {center - innerSize * 0.3f, center});
        blade.setFillColor(sf::Color(200, 200, 200)); // Silver blade
        blade.setOutlineColor(sf::Color::Black);
        blade.setOutlineThickness(1.f);

        sf::RectangleShape hilt({innerSize * 0.8f, innerSize * 0.2f});
        hilt.setOrigin({hilt.getSize().x / 2.f, hilt.getSize().y / 2.f});
        hilt.setPosition({center, center + innerSize * 0.25f});
        hilt.setFillColor(sf::Color(139, 69, 19)); // Brown hilt
        
        texture.draw(blade);
        texture.draw(hilt);
    }
}

void generateIcon(const std::string& type, const std::string& filename, const sf::Color& color, unsigned int size) {
    sf::RenderTexture renderTexture({size, size});
    renderTexture.clear(sf::Color::Transparent);

    sf::CircleShape backgroundCircle(size / 2.f - 2.f);
    backgroundCircle.setFillColor(color);
    backgroundCircle.setPosition({2.f, 2.f});
    renderTexture.draw(backgroundCircle);

    drawSymbol(renderTexture, type, size);
    
    renderTexture.display();

    sf::Texture texture = renderTexture.getTexture();
    sf::Image image = texture.copyToImage();
    
    if (!image.saveToFile("../../assets/" + filename)) {
        std::cerr << "Failed to save " << filename << std::endl;
    } else {
        std::cout << "Successfully generated " << filename << std::endl;
    }
}

int main() {
    for (const auto& pair : gemAssets) {
        generateIcon(pair.first, pair.second.filename, pair.second.color, 64);
    }
    return 0;
}