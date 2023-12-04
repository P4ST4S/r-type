/*
** EPITECH PROJECT, 2023
** B-CPP-500-PAR-5-1-rtype-ethan.nguyen
** File description:
** TextureSystem
*/

#include "TextureSystem.hpp"

TextureSystem::TextureSystem(std::shared_ptr<RGraphic::IGraphic> lib) : _lib(lib)
{
}

TextureSystem::~TextureSystem()
{
}

void TextureSystem::operator()(Registry &registry, SparseArray<PositionComponent> &positionArray, SparseArray<TextureComponent> &TextureArray)
{
    Zipper<SparseArray<PositionComponent>, SparseArray<TextureComponent>> zipper(positionArray, TextureArray);

    for (auto it = zipper.begin(); it != zipper.end(); it++) {
        TextureComponent &texture = std::get<TextureComponent&>(*it);
        if (texture.hadTexture()){
            RGraphic::RRectangle_t src = texture.getSourceRect();
            double scale = texture.getScale();
            Vector2D<double> position = std::get<PositionComponent&>(*it).getPosition();
            double width = static_cast<double>(src.width) * scale;
            double height = static_cast<double>(src.height) * scale;
            _lib->drawTexture(texture.getTexture(), src, {position._x - width / 2, position._y - height / 2, width, height}, 0);
            texture.nextFrame();
        }
    }
}
